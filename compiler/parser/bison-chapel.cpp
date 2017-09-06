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
    const char*               pch;

    Vec<const char*>*         vpch;
    RetTag                    retTag;
    AggregateTag              aggrTag;
    bool                      b;
    IntentTag                 pt;
    Expr*                     pexpr;
    DefExpr*                  pdefexpr;
    CallExpr*                 pcallexpr;
    BlockStmt*                pblockstmt;
    Type*                     ptype;
    EnumType*                 penumtype;
    FnSymbol*                 pfnsymbol;
    Flag                      flag;
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    IntentExpr                pIntentExpr;
    ForallIntents*            pForallIntents;
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif


/* Line 288 of yacc.c  */
#line 136 "chapel.ypp"

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
#line 158 "chapel.ypp"

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
#line 240 "bison-chapel.cpp"

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
     TCATCH = 271,
     TCLASS = 272,
     TCOBEGIN = 273,
     TCOFORALL = 274,
     TCONFIG = 275,
     TCONST = 276,
     TCONTINUE = 277,
     TDEFER = 278,
     TDELETE = 279,
     TDMAPPED = 280,
     TDO = 281,
     TDOMAIN = 282,
     TELSE = 283,
     TENUM = 284,
     TEXCEPT = 285,
     TEXPORT = 286,
     TEXTERN = 287,
     TFOR = 288,
     TFORALL = 289,
     TFORWARDING = 290,
     TIF = 291,
     TIN = 292,
     TINDEX = 293,
     TINLINE = 294,
     TINOUT = 295,
     TITER = 296,
     TLABEL = 297,
     TLAMBDA = 298,
     TLET = 299,
     TLOCAL = 300,
     TMINUSMINUS = 301,
     TMODULE = 302,
     TNEW = 303,
     TNIL = 304,
     TNOINIT = 305,
     TON = 306,
     TONLY = 307,
     TOTHERWISE = 308,
     TOUT = 309,
     TPARAM = 310,
     TPLUSPLUS = 311,
     TPRAGMA = 312,
     TPRIMITIVE = 313,
     TPRIVATE = 314,
     TPROC = 315,
     TPUBLIC = 316,
     TRECORD = 317,
     TREDUCE = 318,
     TREF = 319,
     TREQUIRE = 320,
     TRETURN = 321,
     TSCAN = 322,
     TSELECT = 323,
     TSERIAL = 324,
     TSINGLE = 325,
     TSPARSE = 326,
     TSUBDOMAIN = 327,
     TSYNC = 328,
     TTHEN = 329,
     TTHROW = 330,
     TTHROWS = 331,
     TTRY = 332,
     TTRYBANG = 333,
     TTYPE = 334,
     TUNDERSCORE = 335,
     TUNION = 336,
     TUSE = 337,
     TVAR = 338,
     TWHEN = 339,
     TWHERE = 340,
     TWHILE = 341,
     TWITH = 342,
     TYIELD = 343,
     TZIP = 344,
     TALIAS = 345,
     TAND = 346,
     TASSIGN = 347,
     TASSIGNBAND = 348,
     TASSIGNBOR = 349,
     TASSIGNBXOR = 350,
     TASSIGNDIVIDE = 351,
     TASSIGNEXP = 352,
     TASSIGNLAND = 353,
     TASSIGNLOR = 354,
     TASSIGNMINUS = 355,
     TASSIGNMOD = 356,
     TASSIGNMULTIPLY = 357,
     TASSIGNPLUS = 358,
     TASSIGNSL = 359,
     TASSIGNSR = 360,
     TBAND = 361,
     TBNOT = 362,
     TBOR = 363,
     TBXOR = 364,
     TCOLON = 365,
     TCOMMA = 366,
     TDIVIDE = 367,
     TDOT = 368,
     TDOTDOT = 369,
     TDOTDOTDOT = 370,
     TEQUAL = 371,
     TEXP = 372,
     TGREATER = 373,
     TGREATEREQUAL = 374,
     THASH = 375,
     TLESS = 376,
     TLESSEQUAL = 377,
     TMINUS = 378,
     TMOD = 379,
     TNOT = 380,
     TNOTEQUAL = 381,
     TOR = 382,
     TPLUS = 383,
     TQUESTION = 384,
     TSEMI = 385,
     TSHIFTLEFT = 386,
     TSHIFTRIGHT = 387,
     TSTAR = 388,
     TSWAP = 389,
     TASSIGNREDUCE = 390,
     TIO = 391,
     TLCBR = 392,
     TRCBR = 393,
     TLP = 394,
     TRP = 395,
     TLSBR = 396,
     TRSBR = 397,
     TNOELSE = 398,
     TUMINUS = 399,
     TUPLUS = 400
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
#line 189 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 340 of yacc.c  */
#line 453 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 459 "bison-chapel.cpp"
/* Unqualified %code blocks.  */

/* Line 344 of yacc.c  */
#line 38 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;


/* Line 344 of yacc.c  */
#line 197 "chapel.ypp"

  #include "build.h"
  #include "CatchStmt.h"
  #include "DeferStmt.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForallStmt.h"
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
#line 538 "bison-chapel.cpp"

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
#define YYLAST   14136

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  146
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  121
/* YYNRULES -- Number of rules.  */
#define YYNRULES  480
/* YYNRULES -- Number of states.  */
#define YYNSTATES  911

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   400

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
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    17,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    45,    47,    49,    52,    55,    59,    63,    67,    71,
      75,    79,    83,    86,    90,    94,    97,   100,   104,   106,
     112,   119,   120,   122,   124,   127,   131,   133,   136,   138,
     142,   146,   152,   153,   155,   157,   159,   163,   169,   175,
     179,   184,   189,   194,   199,   204,   209,   214,   219,   224,
     229,   234,   239,   244,   249,   254,   259,   260,   262,   264,
     266,   268,   271,   273,   276,   280,   282,   284,   287,   290,
     292,   294,   296,   298,   300,   302,   306,   312,   318,   321,
     324,   330,   334,   341,   348,   353,   359,   365,   369,   373,
     380,   386,   393,   399,   406,   410,   415,   422,   430,   437,
     445,   450,   456,   461,   466,   470,   477,   483,   486,   490,
     494,   497,   500,   504,   508,   509,   512,   515,   519,   525,
     527,   531,   535,   541,   542,   545,   549,   552,   556,   563,
     572,   574,   576,   578,   579,   582,   583,   586,   590,   596,
     598,   601,   605,   607,   611,   612,   613,   622,   623,   625,
     628,   631,   632,   633,   644,   648,   652,   658,   664,   666,
     670,   672,   675,   677,   679,   681,   683,   685,   687,   689,
     691,   693,   695,   697,   699,   701,   703,   705,   707,   709,
     711,   713,   715,   717,   719,   721,   723,   725,   727,   729,
     731,   733,   735,   737,   739,   741,   743,   745,   747,   748,
     752,   756,   757,   759,   763,   768,   773,   780,   787,   788,
     790,   792,   794,   796,   798,   801,   804,   806,   808,   810,
     811,   813,   815,   818,   820,   822,   824,   826,   827,   829,
     832,   834,   836,   838,   839,   841,   843,   845,   847,   849,
     852,   854,   855,   857,   860,   863,   864,   867,   871,   876,
     881,   884,   889,   890,   893,   896,   901,   906,   911,   917,
     922,   923,   925,   927,   929,   933,   937,   942,   948,   950,
     952,   956,   958,   961,   965,   966,   969,   972,   973,   978,
     979,   982,   985,   987,   992,   997,  1004,  1006,  1007,  1009,
    1011,  1015,  1020,  1024,  1029,  1036,  1037,  1040,  1043,  1046,
    1049,  1052,  1055,  1057,  1059,  1063,  1067,  1069,  1071,  1073,
    1077,  1081,  1082,  1084,  1086,  1090,  1094,  1098,  1100,  1102,
    1104,  1106,  1108,  1110,  1112,  1114,  1117,  1122,  1127,  1132,
    1138,  1141,  1144,  1151,  1158,  1163,  1173,  1183,  1191,  1198,
    1205,  1210,  1220,  1230,  1238,  1243,  1250,  1257,  1267,  1277,
    1284,  1286,  1288,  1290,  1292,  1294,  1296,  1298,  1300,  1304,
    1305,  1307,  1312,  1314,  1318,  1323,  1325,  1329,  1332,  1336,
    1340,  1342,  1346,  1349,  1354,  1356,  1358,  1360,  1362,  1364,
    1366,  1368,  1370,  1375,  1379,  1383,  1386,  1389,  1391,  1393,
    1395,  1397,  1399,  1401,  1403,  1408,  1413,  1418,  1422,  1426,
    1430,  1434,  1439,  1443,  1448,  1450,  1452,  1454,  1456,  1458,
    1462,  1467,  1471,  1476,  1480,  1485,  1489,  1495,  1499,  1503,
    1507,  1511,  1515,  1519,  1523,  1527,  1531,  1535,  1539,  1543,
    1547,  1551,  1555,  1559,  1563,  1567,  1571,  1575,  1579,  1583,
    1587,  1590,  1593,  1596,  1599,  1602,  1605,  1609,  1613,  1617,
    1621,  1625,  1629,  1633,  1637,  1639,  1641,  1643,  1645,  1647,
    1649
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     147,     0,    -1,   148,    -1,    -1,   148,   149,    -1,   151,
      -1,   150,   151,    -1,    57,     7,    -1,   150,    57,     7,
      -1,   152,    -1,   154,    -1,   159,    -1,   160,    -1,   167,
      -1,   161,    -1,   170,    -1,   173,    -1,   171,    -1,   180,
      -1,   174,    -1,   175,    -1,   179,    -1,   166,    -1,   244,
     130,    -1,    12,   151,    -1,    13,   245,   151,    -1,    14,
     162,   130,    -1,    18,   245,   154,    -1,    22,   162,   130,
      -1,    24,   254,   130,    -1,    42,   163,   151,    -1,    45,
     254,   165,    -1,    45,   165,    -1,    51,   254,   165,    -1,
      69,   254,   165,    -1,    69,   165,    -1,    73,   151,    -1,
      88,   254,   130,    -1,     1,    -1,   153,    47,   163,   137,
     138,    -1,   153,    47,   163,   137,   155,   138,    -1,    -1,
      61,    -1,    59,    -1,   137,   138,    -1,   137,   155,   138,
      -1,   149,    -1,   155,   149,    -1,   254,    -1,   254,    11,
     254,    -1,   156,   111,   254,    -1,   156,   111,   254,    11,
     254,    -1,    -1,   156,    -1,   133,    -1,   156,    -1,    82,
     233,   130,    -1,    82,   254,    30,   158,   130,    -1,    82,
     254,    52,   157,   130,    -1,    65,   233,   130,    -1,   255,
      92,   254,   130,    -1,   255,   103,   254,   130,    -1,   255,
     100,   254,   130,    -1,   255,   102,   254,   130,    -1,   255,
      96,   254,   130,    -1,   255,   101,   254,   130,    -1,   255,
      97,   254,   130,    -1,   255,    93,   254,   130,    -1,   255,
      94,   254,   130,    -1,   255,    95,   254,   130,    -1,   255,
     105,   254,   130,    -1,   255,   104,   254,   130,    -1,   255,
     134,   254,   130,    -1,   255,   135,   254,   130,    -1,   255,
      98,   254,   130,    -1,   255,    99,   254,   130,    -1,    -1,
     163,    -1,     3,    -1,   162,    -1,     7,    -1,    26,   151,
      -1,   154,    -1,    66,   130,    -1,    66,   254,   130,    -1,
     130,    -1,   168,    -1,    61,   168,    -1,    59,   168,    -1,
     187,    -1,   217,    -1,   183,    -1,   169,    -1,   194,    -1,
     220,    -1,    35,   254,   130,    -1,    35,   254,    30,   158,
     130,    -1,    35,   254,    52,   157,   130,    -1,    35,   220,
      -1,    32,     9,    -1,    26,   151,    86,   254,   130,    -1,
      86,   254,   165,    -1,    19,   254,    37,   254,   245,   165,
      -1,    19,   254,    37,   172,   245,   165,    -1,    19,   254,
     245,   165,    -1,    33,   254,    37,   254,   165,    -1,    33,
     254,    37,   172,   165,    -1,    33,   254,   165,    -1,    33,
     172,   165,    -1,    33,    55,   163,    37,   254,   165,    -1,
      34,   254,    37,   254,   165,    -1,    34,   254,    37,   254,
     248,   165,    -1,    34,   254,    37,   172,   165,    -1,    34,
     254,    37,   172,   248,   165,    -1,    34,   254,   165,    -1,
      34,   254,   248,   165,    -1,   141,   233,    37,   254,   142,
     151,    -1,   141,   233,    37,   254,   248,   142,   151,    -1,
     141,   233,    37,   172,   142,   151,    -1,   141,   233,    37,
     172,   248,   142,   151,    -1,   141,   233,   142,   151,    -1,
     141,   233,   248,   142,   151,    -1,    89,   139,   233,   140,
      -1,    36,   254,    74,   151,    -1,    36,   254,   154,    -1,
      36,   254,    74,   151,    28,   151,    -1,    36,   254,   154,
      28,   151,    -1,    23,   151,    -1,    77,   254,   130,    -1,
      78,   254,   130,    -1,    77,   161,    -1,    78,   161,    -1,
      77,   154,   176,    -1,    78,   154,   176,    -1,    -1,   176,
     177,    -1,    16,   154,    -1,    16,   178,   154,    -1,    16,
     139,   178,   140,   154,    -1,   163,    -1,   163,   110,   254,
      -1,    75,   254,   130,    -1,    68,   254,   137,   181,   138,
      -1,    -1,   181,   182,    -1,    84,   233,   165,    -1,    53,
     151,    -1,    53,    26,   151,    -1,   184,   163,   185,   137,
     186,   138,    -1,    32,   164,   184,   163,   185,   137,   186,
     138,    -1,    17,    -1,    62,    -1,    81,    -1,    -1,   110,
     233,    -1,    -1,   186,   167,    -1,   186,   150,   167,    -1,
      29,   163,   137,   188,   138,    -1,   189,    -1,   188,   111,
      -1,   188,   111,   189,    -1,   163,    -1,   163,    92,   254,
      -1,    -1,    -1,    43,   191,   202,   192,   209,   228,   216,
     212,    -1,    -1,    39,    -1,    31,   164,    -1,    32,   164,
      -1,    -1,    -1,   193,   208,   195,   197,   196,   209,   228,
     210,   216,   211,    -1,   207,   199,   201,    -1,   207,   200,
     201,    -1,   207,   198,   113,   199,   201,    -1,   207,   198,
     113,   200,   201,    -1,   239,    -1,   139,   254,   140,    -1,
     163,    -1,   107,   163,    -1,   106,    -1,   108,    -1,   109,
      -1,   107,    -1,   116,    -1,   126,    -1,   122,    -1,   119,
      -1,   121,    -1,   118,    -1,   128,    -1,   123,    -1,   133,
      -1,   112,    -1,   131,    -1,   132,    -1,   124,    -1,   117,
      -1,   125,    -1,    15,    -1,   120,    -1,    10,    -1,   134,
      -1,   136,    -1,    92,    -1,   103,    -1,   100,    -1,   102,
      -1,    96,    -1,   101,    -1,    97,    -1,    93,    -1,    94,
      -1,    95,    -1,   105,    -1,   104,    -1,    -1,   139,   203,
     140,    -1,   139,   203,   140,    -1,    -1,   204,    -1,   203,
     111,   204,    -1,   205,   163,   232,   226,    -1,   205,   163,
     232,   215,    -1,   205,   139,   225,   140,   232,   226,    -1,
     205,   139,   225,   140,   232,   215,    -1,    -1,   206,    -1,
      37,    -1,    40,    -1,    54,    -1,    21,    -1,    21,    37,
      -1,    21,    64,    -1,    55,    -1,    64,    -1,    79,    -1,
      -1,    55,    -1,    64,    -1,    21,    64,    -1,    21,    -1,
      79,    -1,    60,    -1,    41,    -1,    -1,    21,    -1,    21,
      64,    -1,    64,    -1,    55,    -1,    79,    -1,    -1,    76,
      -1,   130,    -1,   212,    -1,   154,    -1,   166,    -1,   129,
     163,    -1,   129,    -1,    -1,   213,    -1,   115,   254,    -1,
     115,   214,    -1,    -1,    85,   254,    -1,    79,   218,   130,
      -1,    20,    79,   218,   130,    -1,    32,    79,   218,   130,
      -1,   163,   219,    -1,   163,   219,   111,   218,    -1,    -1,
      92,   240,    -1,    92,   229,    -1,   221,    55,   222,   130,
      -1,   221,    21,   222,   130,    -1,   221,    64,   222,   130,
      -1,   221,    21,    64,   222,   130,    -1,   221,    83,   222,
     130,    -1,    -1,    20,    -1,    32,    -1,   223,    -1,   222,
     111,   223,    -1,   163,   228,   226,    -1,   163,   227,    90,
     254,    -1,   139,   225,   140,   228,   226,    -1,    80,    -1,
     163,    -1,   139,   225,   140,    -1,   224,    -1,   224,   111,
      -1,   224,   111,   225,    -1,    -1,    92,    50,    -1,    92,
     254,    -1,    -1,   110,   141,   233,   142,    -1,    -1,   110,
     240,    -1,   110,   229,    -1,     1,    -1,   141,   233,   142,
     240,    -1,   141,   233,   142,   229,    -1,   141,   233,    37,
     254,   142,   240,    -1,     1,    -1,    -1,   240,    -1,   213,
      -1,   141,   142,   230,    -1,   141,   233,   142,   230,    -1,
     141,   142,   231,    -1,   141,   233,   142,   231,    -1,   141,
     233,    37,   254,   142,   230,    -1,    -1,   110,   240,    -1,
     110,   213,    -1,   110,    27,    -1,   110,    70,    -1,   110,
      73,    -1,   110,   231,    -1,   254,    -1,   213,    -1,   233,
     111,   254,    -1,   233,   111,   213,    -1,    80,    -1,   254,
      -1,   213,    -1,   234,   111,   234,    -1,   235,   111,   234,
      -1,    -1,   237,    -1,   238,    -1,   237,   111,   238,    -1,
     163,    92,   213,    -1,   163,    92,   254,    -1,   213,    -1,
     254,    -1,   163,    -1,   243,    -1,   255,    -1,   242,    -1,
     263,    -1,   262,    -1,    70,   254,    -1,    38,   139,   236,
     140,    -1,    27,   139,   236,   140,    -1,    72,   139,   236,
     140,    -1,    71,    72,   139,   236,   140,    -1,    12,   254,
      -1,    73,   254,    -1,    33,   254,    37,   254,    26,   254,
      -1,    33,   254,    37,   172,    26,   254,    -1,    33,   254,
      26,   254,    -1,    33,   254,    37,   254,    26,    36,   254,
      74,   254,    -1,    33,   254,    37,   172,    26,    36,   254,
      74,   254,    -1,    33,   254,    26,    36,   254,    74,   254,
      -1,    34,   254,    37,   254,    26,   254,    -1,    34,   254,
      37,   172,    26,   254,    -1,    34,   254,    26,   254,    -1,
      34,   254,    37,   254,    26,    36,   254,    74,   254,    -1,
      34,   254,    37,   172,    26,    36,   254,    74,   254,    -1,
      34,   254,    26,    36,   254,    74,   254,    -1,   141,   233,
     142,   254,    -1,   141,   233,    37,   254,   142,   254,    -1,
     141,   233,    37,   172,   142,   254,    -1,   141,   233,    37,
     254,   142,    36,   254,    74,   254,    -1,   141,   233,    37,
     172,   142,    36,   254,    74,   254,    -1,    36,   254,    74,
     254,    28,   254,    -1,    49,    -1,   243,    -1,   239,    -1,
     258,    -1,   257,    -1,   190,    -1,   252,    -1,   253,    -1,
     251,   136,   254,    -1,    -1,   246,    -1,    87,   139,   247,
     140,    -1,   250,    -1,   247,   111,   250,    -1,    87,   139,
     249,   140,    -1,   250,    -1,   249,   111,   250,    -1,   206,
     239,    -1,   254,    63,   239,    -1,   266,    63,   239,    -1,
     255,    -1,   251,   136,   254,    -1,    48,   254,    -1,    44,
     222,    37,   254,    -1,   260,    -1,   240,    -1,   241,    -1,
     264,    -1,   265,    -1,   190,    -1,   252,    -1,   253,    -1,
     139,   115,   254,   140,    -1,   254,   110,   254,    -1,   254,
     114,   254,    -1,   254,   114,    -1,   114,   254,    -1,   114,
      -1,   239,    -1,   257,    -1,   258,    -1,   259,    -1,   255,
      -1,   190,    -1,   256,   139,   236,   140,    -1,   256,   141,
     236,   142,    -1,    58,   139,   236,   140,    -1,   254,   113,
     163,    -1,   254,   113,    79,    -1,   254,   113,    27,    -1,
     139,   234,   140,    -1,   139,   234,   111,   140,    -1,   139,
     235,   140,    -1,   139,   235,   111,   140,    -1,     4,    -1,
       5,    -1,     6,    -1,     7,    -1,     8,    -1,   137,   233,
     138,    -1,   137,   233,   111,   138,    -1,   141,   233,   142,
      -1,   141,   233,   111,   142,    -1,   141,   261,   142,    -1,
     141,   261,   111,   142,    -1,   254,    90,   254,    -1,   261,
     111,   254,    90,   254,    -1,   254,   128,   254,    -1,   254,
     123,   254,    -1,   254,   133,   254,    -1,   254,   112,   254,
      -1,   254,   131,   254,    -1,   254,   132,   254,    -1,   254,
     124,   254,    -1,   254,   116,   254,    -1,   254,   126,   254,
      -1,   254,   122,   254,    -1,   254,   119,   254,    -1,   254,
     121,   254,    -1,   254,   118,   254,    -1,   254,   106,   254,
      -1,   254,   108,   254,    -1,   254,   109,   254,    -1,   254,
      91,   254,    -1,   254,   127,   254,    -1,   254,   117,   254,
      -1,   254,    15,   254,    -1,   254,    10,   254,    -1,   254,
     120,   254,    -1,   254,    25,   254,    -1,   128,   254,    -1,
     123,   254,    -1,    46,   254,    -1,    56,   254,    -1,   125,
     254,    -1,   107,   254,    -1,   254,    63,   254,    -1,   254,
      63,   172,    -1,   266,    63,   254,    -1,   266,    63,   172,
      -1,   254,    67,   254,    -1,   254,    67,   172,    -1,   266,
      67,   254,    -1,   266,    67,   172,    -1,   128,    -1,   133,
      -1,    91,    -1,   127,    -1,   106,    -1,   108,    -1,   109,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   438,   438,   443,   444,   450,   451,   456,   457,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   495,
     497,   502,   503,   504,   519,   520,   525,   526,   531,   536,
     541,   545,   552,   557,   561,   566,   570,   571,   572,   576,
     580,   581,   582,   583,   584,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   599,   600,   604,   608,
     609,   613,   614,   618,   619,   623,   624,   625,   626,   627,
     628,   629,   630,   634,   635,   639,   640,   641,   642,   646,
     661,   662,   663,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   683,   689,   695,
     701,   708,   718,   722,   723,   724,   725,   729,   732,   733,
     734,   735,   736,   737,   741,   742,   746,   747,   748,   752,
     753,   757,   760,   765,   766,   770,   772,   774,   781,   791,
     804,   809,   814,   822,   823,   828,   829,   831,   836,   846,
     855,   859,   867,   868,   873,   878,   872,   903,   909,   916,
     924,   936,   942,   935,   970,   974,   979,   983,   991,   992,
     996,   997,   998,   999,  1000,  1001,  1002,  1003,  1004,  1005,
    1006,  1007,  1008,  1009,  1010,  1011,  1012,  1013,  1014,  1015,
    1016,  1017,  1018,  1019,  1020,  1021,  1025,  1026,  1027,  1028,
    1029,  1030,  1031,  1032,  1033,  1034,  1035,  1036,  1040,  1041,
    1045,  1049,  1050,  1051,  1055,  1057,  1059,  1061,  1066,  1067,
    1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1083,
    1084,  1085,  1086,  1087,  1088,  1092,  1093,  1097,  1098,  1099,
    1100,  1101,  1102,  1106,  1107,  1110,  1111,  1115,  1116,  1120,
    1122,  1127,  1128,  1132,  1133,  1137,  1138,  1142,  1144,  1146,
    1151,  1164,  1181,  1182,  1184,  1189,  1197,  1205,  1213,  1222,
    1232,  1233,  1234,  1238,  1239,  1247,  1249,  1256,  1261,  1263,
    1265,  1270,  1272,  1274,  1281,  1282,  1283,  1287,  1288,  1293,
    1294,  1295,  1296,  1316,  1320,  1324,  1332,  1336,  1337,  1338,
    1342,  1344,  1350,  1352,  1354,  1359,  1360,  1361,  1362,  1363,
    1364,  1365,  1371,  1372,  1373,  1374,  1378,  1379,  1380,  1384,
    1385,  1389,  1390,  1394,  1395,  1399,  1400,  1401,  1402,  1406,
    1417,  1418,  1419,  1420,  1421,  1422,  1424,  1426,  1428,  1430,
    1432,  1434,  1439,  1441,  1443,  1445,  1447,  1449,  1451,  1453,
    1455,  1457,  1459,  1461,  1463,  1470,  1476,  1482,  1488,  1497,
    1505,  1513,  1514,  1515,  1516,  1517,  1518,  1519,  1520,  1525,
    1526,  1530,  1535,  1538,  1543,  1548,  1551,  1556,  1560,  1561,
    1565,  1566,  1571,  1576,  1584,  1585,  1586,  1587,  1588,  1589,
    1590,  1591,  1592,  1594,  1596,  1598,  1600,  1602,  1607,  1608,
    1609,  1610,  1621,  1622,  1626,  1627,  1628,  1632,  1633,  1634,
    1642,  1643,  1644,  1645,  1649,  1650,  1651,  1652,  1653,  1654,
    1655,  1656,  1657,  1658,  1662,  1670,  1671,  1675,  1676,  1677,
    1678,  1679,  1680,  1681,  1682,  1683,  1684,  1685,  1686,  1687,
    1688,  1689,  1690,  1691,  1692,  1693,  1694,  1695,  1696,  1697,
    1701,  1702,  1703,  1704,  1705,  1706,  1710,  1711,  1712,  1713,
    1717,  1718,  1719,  1720,  1725,  1726,  1727,  1728,  1729,  1730,
    1731
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "CSTRINGLITERAL", "EXTERNCODE", "TALIGN",
  "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBY", "TCATCH", "TCLASS",
  "TCOBEGIN", "TCOFORALL", "TCONFIG", "TCONST", "TCONTINUE", "TDEFER",
  "TDELETE", "TDMAPPED", "TDO", "TDOMAIN", "TELSE", "TENUM", "TEXCEPT",
  "TEXPORT", "TEXTERN", "TFOR", "TFORALL", "TFORWARDING", "TIF", "TIN",
  "TINDEX", "TINLINE", "TINOUT", "TITER", "TLABEL", "TLAMBDA", "TLET",
  "TLOCAL", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TON",
  "TONLY", "TOTHERWISE", "TOUT", "TPARAM", "TPLUSPLUS", "TPRAGMA",
  "TPRIMITIVE", "TPRIVATE", "TPROC", "TPUBLIC", "TRECORD", "TREDUCE",
  "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSINGLE",
  "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHROW", "TTHROWS", "TTRY",
  "TTRYBANG", "TTYPE", "TUNDERSCORE", "TUNION", "TUSE", "TVAR", "TWHEN",
  "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND",
  "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
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
  "if_stmt", "defer_stmt", "try_stmt", "catch_stmt_ls", "catch_stmt",
  "catch_expr", "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
  "class_decl_stmt", "class_tag", "opt_inherit", "class_level_stmt_ls",
  "enum_decl_stmt", "enum_ls", "enum_item", "lambda_decl_expr", "$@1",
  "$@2", "linkage_spec", "fn_decl_stmt", "$@3", "$@4",
  "fn_decl_stmt_inner", "fn_decl_receiver_expr", "fn_ident",
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
     395,   396,   397,   398,   399,   400
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   146,   147,   148,   148,   149,   149,   150,   150,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   152,
     152,   153,   153,   153,   154,   154,   155,   155,   156,   156,
     156,   156,   157,   157,   158,   158,   159,   159,   159,   160,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   162,   162,   163,   164,
     164,   165,   165,   166,   166,   167,   167,   167,   167,   167,
     167,   167,   167,   168,   168,   169,   169,   169,   169,   170,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   172,   173,   173,   173,   173,   174,   175,   175,
     175,   175,   175,   175,   176,   176,   177,   177,   177,   178,
     178,   179,   180,   181,   181,   182,   182,   182,   183,   183,
     184,   184,   184,   185,   185,   186,   186,   186,   187,   188,
     188,   188,   189,   189,   191,   192,   190,   193,   193,   193,
     193,   195,   196,   194,   197,   197,   197,   197,   198,   198,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   201,   201,
     202,   203,   203,   203,   204,   204,   204,   204,   205,   205,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   207,
     207,   207,   207,   207,   207,   208,   208,   209,   209,   209,
     209,   209,   209,   210,   210,   211,   211,   212,   212,   213,
     213,   214,   214,   215,   215,   216,   216,   217,   217,   217,
     218,   218,   219,   219,   219,   220,   220,   220,   220,   220,
     221,   221,   221,   222,   222,   223,   223,   223,   224,   224,
     224,   225,   225,   225,   226,   226,   226,   227,   227,   228,
     228,   228,   228,   229,   229,   229,   229,   230,   230,   230,
     231,   231,   231,   231,   231,   232,   232,   232,   232,   232,
     232,   232,   233,   233,   233,   233,   234,   234,   234,   235,
     235,   236,   236,   237,   237,   238,   238,   238,   238,   239,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   242,
     243,   244,   244,   244,   244,   244,   244,   244,   244,   245,
     245,   246,   247,   247,   248,   249,   249,   250,   250,   250,
     251,   251,   252,   253,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   255,   255,
     255,   255,   256,   256,   257,   257,   257,   258,   258,   258,
     259,   259,   259,   259,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   261,   261,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     263,   263,   263,   263,   263,   263,   264,   264,   264,   264,
     265,   265,   265,   265,   266,   266,   266,   266,   266,   266,
     266
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     2,     2,     3,     1,     5,
       6,     0,     1,     1,     2,     3,     1,     2,     1,     3,
       3,     5,     0,     1,     1,     1,     3,     5,     5,     3,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     0,     1,     1,     1,
       1,     2,     1,     2,     3,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     3,     5,     5,     2,     2,
       5,     3,     6,     6,     4,     5,     5,     3,     3,     6,
       5,     6,     5,     6,     3,     4,     6,     7,     6,     7,
       4,     5,     4,     4,     3,     6,     5,     2,     3,     3,
       2,     2,     3,     3,     0,     2,     2,     3,     5,     1,
       3,     3,     5,     0,     2,     3,     2,     3,     6,     8,
       1,     1,     1,     0,     2,     0,     2,     3,     5,     1,
       2,     3,     1,     3,     0,     0,     8,     0,     1,     2,
       2,     0,     0,    10,     3,     3,     5,     5,     1,     3,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     3,
       3,     0,     1,     3,     4,     4,     6,     6,     0,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     0,
       1,     1,     2,     1,     1,     1,     1,     0,     1,     2,
       1,     1,     1,     0,     1,     1,     1,     1,     1,     2,
       1,     0,     1,     2,     2,     0,     2,     3,     4,     4,
       2,     4,     0,     2,     2,     4,     4,     4,     5,     4,
       0,     1,     1,     1,     3,     3,     4,     5,     1,     1,
       3,     1,     2,     3,     0,     2,     2,     0,     4,     0,
       2,     2,     1,     4,     4,     6,     1,     0,     1,     1,
       3,     4,     3,     4,     6,     0,     2,     2,     2,     2,
       2,     2,     1,     1,     3,     3,     1,     1,     1,     3,
       3,     0,     1,     1,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     4,     4,     4,     5,
       2,     2,     6,     6,     4,     9,     9,     7,     6,     6,
       4,     9,     9,     7,     4,     6,     6,     9,     9,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     4,     1,     3,     4,     1,     3,     2,     3,     3,
       1,     3,     2,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     3,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     4,     3,     3,     3,
       3,     4,     3,     4,     1,     1,     1,     1,     1,     3,
       4,     3,     4,     3,     4,     3,     5,     3,     3,     3,
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
       3,     0,     0,     1,    38,    78,   424,   425,   426,   427,
     428,     0,   379,    76,   150,   379,     0,   281,    76,     0,
       0,     0,     0,     0,    76,    76,     0,     0,   280,     0,
       0,   168,     0,   164,     0,     0,     0,     0,   370,     0,
       0,     0,     0,   280,   280,   151,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   152,     0,
       0,     0,   476,   478,     0,   479,   480,   407,     0,     0,
     477,   474,    85,   475,     0,     0,     0,     4,     0,     5,
       9,     0,    10,    11,    12,    14,   339,    22,    13,    86,
      92,    15,    17,    16,    19,    20,    21,    18,    91,     0,
      89,   399,     0,    93,    90,    94,     0,   408,   395,   396,
     342,   340,     0,     0,   400,   401,     0,   341,     0,   409,
     410,   411,   394,   344,   343,   397,   398,     0,    24,   350,
       0,     0,   380,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   408,   340,   400,   401,   379,   341,
     409,   410,     0,     0,   127,     0,     0,   331,     0,    80,
      79,   169,    99,     0,   170,     0,     0,     0,     0,     0,
     281,   282,    98,     0,     0,   331,     0,     0,     0,     0,
       0,   283,     0,    82,    32,     0,   462,   392,     0,   463,
       7,   331,   282,    88,    87,   260,   323,     0,   322,    83,
       0,     0,    35,     0,   345,     0,   331,    36,   351,     0,
     134,   130,     0,   341,   134,   131,     0,   272,     0,     0,
     322,     0,     0,   465,   406,   461,   464,   460,    44,    46,
       0,     0,   326,     0,   328,     0,     0,   327,     0,   322,
       0,     0,     6,     0,   153,   246,   245,   171,     0,     0,
       0,     0,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   405,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   331,   331,
       0,     0,     0,    25,    26,     0,    27,     0,     0,     0,
       0,     0,     0,     0,    28,    29,     0,   339,   337,     0,
     332,   333,   338,     0,     0,     0,     0,     0,   108,     0,
       0,   107,     0,     0,     0,   114,     0,     0,    52,    95,
       0,   124,     0,    30,   221,   165,   288,     0,   289,   291,
       0,   302,     0,     0,   294,     0,     0,    81,    31,    33,
       0,   170,   259,     0,    59,    84,   143,    34,   331,     0,
     141,   132,   128,   133,   129,     0,   270,   267,    56,     0,
      52,   101,    37,    45,    47,     0,   429,     0,     0,   420,
       0,   422,     0,     0,     0,     0,     0,     0,   433,     8,
       0,     0,     0,   239,     0,     0,     0,     0,     0,   378,
     457,   456,   459,   467,   466,   471,   470,   453,   450,   451,
     452,   403,   440,   419,   418,   417,   404,   444,   455,   449,
     447,   458,   448,   446,   438,   443,   445,   454,   437,   441,
     442,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     469,   468,   473,   472,   233,   230,   231,   232,   236,   237,
     238,     0,     0,   382,     0,     0,     0,     0,     0,     0,
       0,     0,   431,   379,   379,   104,   268,     0,     0,   347,
       0,   162,     0,   159,   269,   153,     0,     0,     0,   354,
       0,     0,     0,   360,     0,     0,     0,   115,   475,    55,
       0,    48,    53,     0,   123,     0,     0,   346,     0,   222,
       0,   229,   247,     0,   292,     0,   306,     0,   301,   395,
       0,     0,   285,   393,   284,   416,   325,   324,     0,     0,
     348,     0,   135,     0,   274,   395,     0,     0,     0,   430,
     402,   421,   329,   423,   330,     0,     0,   432,   120,   364,
       0,   435,   434,     0,     0,   154,   155,   243,   240,   241,
     244,   172,     0,     0,   276,   275,   277,   279,    60,    67,
      68,    69,    64,    66,    74,    75,    62,    65,    63,    61,
      71,    70,    72,    73,   414,   415,   234,   235,   387,     0,
     381,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,   335,   336,   334,     0,   160,   158,
       0,     0,   122,     0,     0,   106,     0,   105,     0,     0,
     112,     0,     0,   110,     0,     0,   385,     0,    96,     0,
      97,     0,     0,   126,   228,   220,     0,   315,   248,   251,
     250,   252,     0,   290,   293,     0,   294,     0,   286,   295,
     296,     0,     0,   142,   144,   349,     0,   136,   139,     0,
       0,   271,    57,    58,     0,     0,     0,     0,   121,     0,
      39,     0,   280,   242,   247,   203,   201,   206,   213,   214,
     215,   210,   212,   208,   211,   209,   207,   217,   216,   182,
     185,   183,   184,   195,   186,   199,   191,   189,   202,   190,
     188,   193,   198,   200,   187,   192,   196,   197,   194,   204,
     205,     0,   180,     0,   218,   218,   178,   278,   383,   408,
     408,     0,     0,     0,     0,     0,     0,     0,     0,   103,
     102,   163,   161,   155,   109,     0,     0,   353,     0,   352,
       0,     0,   359,   113,     0,   358,   111,     0,   384,    50,
      49,   125,   369,   223,     0,     0,   294,   249,   265,   287,
       0,     0,     0,   146,     0,     0,     0,   137,     0,     0,
     118,   366,     0,     0,   116,   365,     0,   436,    40,    76,
     280,   280,   148,   280,   156,     0,   181,     0,     0,   221,
     174,   175,     0,     0,     0,     0,     0,     0,     0,     0,
     280,   357,     0,     0,   363,     0,     0,   386,     0,   315,
     318,   319,   320,     0,   317,   321,   395,   261,   225,   224,
       0,     0,     0,   304,   395,   147,   145,     0,   140,     0,
     119,     0,   117,   157,   253,   179,   180,   218,   218,     0,
       0,     0,     0,     0,     0,     0,   149,     0,     0,     0,
       0,    51,   294,   307,     0,   262,   264,   263,   266,   257,
     258,   166,     0,   138,     0,     0,   254,   265,   176,   177,
     219,     0,     0,     0,     0,     0,     0,   356,   355,   362,
     361,   227,   226,   309,   310,   312,   395,     0,   431,   395,
     368,   367,     0,     0,   311,   313,   255,   173,   256,   307,
     314
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   229,    78,   514,    80,    81,    82,   230,
     509,   513,   510,    83,    84,    85,   160,    86,   164,   184,
      87,    88,    89,    90,    91,    92,   609,    93,    94,    95,
     371,   542,   669,    96,    97,   538,   664,    98,    99,   402,
     682,   100,   492,   493,   143,   177,   522,   102,   103,   403,
     684,   571,   723,   724,   725,   800,   345,   518,   519,   520,
     471,   572,   247,   652,   877,   907,   871,   196,   866,   828,
     831,   104,   218,   376,   105,   106,   180,   181,   349,   350,
     532,   353,   354,   528,   894,   825,   766,   231,   235,   236,
     319,   320,   321,   144,   108,   109,   110,   145,   112,   131,
     132,   472,   336,   635,   473,   113,   146,   147,   116,   149,
     118,   150,   151,   121,   122,   240,   123,   124,   125,   126,
     127
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -761
static const yytype_int16 yypact[] =
{
    -761,    95,  2570,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
    -761,  3791,    17,   116,  -761,    17,  8103,    80,   116,  3791,
    8103,  3791,    73,   116,   155,   490,  6574,  8103,  6713,  8103,
      87,  -761,   116,  -761,    58,  6991,  8103,  8103,  -761,  8103,
    8103,   165,   178,   402,   743,  -761,  7130,  7269,  8103,  6991,
    8103,   147,   181,  3791,  8103,  8242,  8242,   116,  -761,  7130,
    8103,  8103,  -761,  -761,  8103,  -761,  -761,  1815,  8103,  8103,
    -761,  8103,  -761,  -761,  2957,  6157,  7130,  -761,  3652,  -761,
    -761,   284,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
    -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,   116,
    -761,   301,    46,  -761,  -761,  -761,   277,   224,  -761,  -761,
    -761,   229,   234,   267,   309,   322, 13831,  1157,   168,   331,
     342,  -761,  -761,  -761,  -761,  -761,  -761,   304,  -761, 13831,
     268,  3791,  -761,   343,  -761,   287,  8103,  8103,  8103,  8103,
    8103,  7130,  7130,   248,  -761,  -761,  -761,  -761, 11087,   314,
    -761,  -761,   116,   344,  -761, 11346,   328,  7130,   341,  -761,
    -761,  -761,  -761,   116,   154,   116,   345,    48, 10075, 10008,
    -761,  -761,  -761, 11025, 10204,  7130,  3791,   347,    30,    83,
      56,  -761,  3791,  -761,  -761, 10266,   431,   367, 10266,   431,
    -761,  7130,   358,  -761,  -761,   116,  -761,   -17, 13831,  -761,
   11408, 10886,  -761, 10266, 13831,   350,  7130,  -761, 13831, 11470,
    -761,  -761, 11532,  1231,  -761,  -761, 11594,   398,   366,   -15,
   11160, 10266, 11656,   313,  1665,   431,   313,   431,  -761,  -761,
    3096,    68,  -761,  8103,  -761,    34,    93, 13831,    54, 11718,
     -25,   474,  -761,   116,   390,  -761,  -761,  -761,    37,    58,
      58,    58,  -761,  8103,  8103,  8103,  8103,  7408,  7408,  8103,
    8103,  8103,  8103,  8103,  8103,    43,  1815,  8103,  8103,  8103,
    8103,  8103,  8103,  8103,  8103,  8103,  8103,  8103,  8103,  8103,
    8103,  8103,  8103,  8103,  8103,  8103,  8103,  8103,  8103,  8103,
    8103,  8103,  8103,  8103,  8103,  8103,  8103,  8103,  7130,  7130,
    7408,  7408,  6018,  -761,  -761,  3235,  -761, 11222, 11284, 11780,
      96,  7408,    48,   373,  -761,  -761,  8103,   412,  -761,   365,
     395,  -761, 13831,   116,   378,   116,   473,  7130,  -761,  3930,
    7408,  -761,  4069,  7408,   374,  -761,    48,  8381,  8103,  -761,
    3791,   484,   375,  -761,   102,  -761,  -761,    30,  -761,   410,
     383,  -761,  1991,   435,   434,  8103,    58,  -761,  -761,  -761,
     388,  -761,  -761,  7130,  -761,  -761,  -761,  -761,  7130,   399,
    -761,   513,  -761,   513,  -761,  5459,   419,  -761,  -761,  8381,
    8103,  -761,  -761,  -761,  -761,  6852,  -761,  9875,  6296,  -761,
    6435,  -761,  7408,  5598,  2812,   404,  8103,  5878,  -761,  -761,
     403,  7130,   413,   294,    58,   212,   306,   316,   317, 10948,
   13941, 13941,   108,  -761,   108,  -761,   108, 14003,   995,   425,
     924,   367,   313,  -761,  -761,  -761,  1665,  2159,   108,   288,
     288, 13941,   288,   288,   891,   313,  2159, 13970,   891,   431,
     431,   313, 11842, 11904, 11966, 12028, 12090, 12152, 12214, 12276,
   12338, 12400, 12462, 12524, 12586, 12648, 12710, 12772,   407,   409,
    -761,   108,  -761,   108,   163,  -761,  -761,  -761,  -761,  -761,
    -761,   116,   184,  -761, 13893,   325,  8520,  7408,  8659,  7408,
    8103,  7408,  9679,    17, 12834,  -761,  -761, 12901,  7130,  -761,
    7130,   461,    94,  -761,  -761,   390,  8103,   188,  8103, 13831,
      52, 10328,  8103, 13831,    66, 10137,  6018,  -761,   429,   451,
     436, 12963,   451,   437,   540, 13025,  3791,  -761,   199,  -761,
      63,  -761,   329,   430,    30,    99,  -761,  7130,  -761,   372,
    8103,  7547,  -761, 13831,  -761,  -761,  -761, 13831,     6,   439,
    -761,    49,  -761,  7130,  -761,   321,   116,   441,   446,  -761,
    -761,  -761,  -761,  -761,  -761,   -18,  1058,  -761,  -761, 13831,
    3791, 13831,  -761, 13087,  3374,   472,  -761,   516,  -761,  -761,
    -761,  -761,  2654,   324,  -761,  -761,  -761,  -761,  -761,  -761,
    -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
    -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  6018,
    -761,  7408,  7408,  8103,   559, 13149,  8103,   560, 13211,   445,
    9716,    48,    48,  -761,  -761, 13831,  -761,  8103,   116,  -761,
     452, 10266,  -761, 10390,  4208,  -761,  4347,  -761, 10452,  4486,
    -761,    48,  4625,  -761,    48,   200,  -761,  8103,  -761,  8103,
    -761,  3791,  8103,  -761,   556,  -761,    30,   487,   530,  -761,
    -761,  -761,    50,  -761,  -761,  5459,   434,   112, 13831,  -761,
   13831,  4764,  7130,  -761,  -761,  -761,   116,  -761,   488,   287,
     183,  -761,  -761,  -761,  4903,   459,  5042,   465,  -761,  8103,
    -761,  3513,   543,  -761,   329,  -761,  -761,  -761,  -761,  -761,
    -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
     116,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
    -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
    -761,  8103,   496,   499,   475,   475,  -761,  -761,  -761,   203,
     223, 13273,  8798,  8937, 13335,  9076,  9215,  9354,  9493,  -761,
    -761, 13831,  -761,  -761,  -761,  3791,  8103, 13831,  8103, 13831,
    3791,  8103, 13831,  -761,  8103, 13831,  -761,  6018,  -761, 13397,
   13831,  -761, 13831,  -761,   476,  7686,   122,  -761,   532,  -761,
    7408,  5181,  3791,  -761,    41,   479,  8103,  -761,  5320,  8103,
    -761, 13831,  3791,  8103,  -761, 13831,  3791, 13831,  -761,   697,
     794,   794,  -761,  1371,  -761,    33,  -761,  9946,  2380,   102,
    -761,  -761,  8103,  8103,  8103,  8103,  8103,  8103,  8103,  8103,
     860, 13025, 10514, 10576, 13025, 10638, 10700,  -761,  8103,   487,
      73,  8103,  8103,  5738,  -761,  -761,    -3,  7130,  -761,  -761,
    8103,   -39,  9760,  -761,   627,   328,  -761,   287, 13831, 10762,
    -761, 10824,  -761,  -761,   547,  -761,  -761,   475,   475,   226,
   13459, 13521, 13583, 13645, 13707, 13769,  -761,  3791,  3791,  3791,
    3791, 13831,   122,  7825,   193,  -761,  -761, 13831, 13831,  -761,
    -761,  -761,  9493,  -761,  3791,  3791,  -761,   532,  -761,  -761,
    -761,  8103,  8103,  8103,  8103,  8103,  8103, 13025, 13025, 13025,
   13025,  -761,  -761,  -761,  -761,  -761,   100,  7408,  9632,   751,
   13025, 13025,    31,  9831,  -761,  -761,  -761,  -761,  -761,  7964,
    -761
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -761,  -761,  -761,     4,  -663,     1,  -761,  -761,  1473,    65,
    -266,   252,   260,  -761,  -761,   389,    89,  2196,   -10,   469,
    -754,  -609,   -42,  -761,  -761,  -761,    45,  -761,  -761,  -761,
     426,  -761,   -23,  -761,  -761,  -761,  -761,  -761,   480,   153,
     -94,  -761,  -761,    35,   677,  -761,  -761,  -761,  -761,  -761,
    -761,  -761,  -761,  -146,  -143,  -649,  -761,  -140,    22,  -761,
    -336,  -761,  -761,   -14,  -761,  -761,  -241,   854,  -761,  -195,
    -203,  -761,  -131,  -761,   641,  -761,  -168,   320,  -761,  -334,
    -645,  -761,  -502,  -365,  -670,  -760,  -141,     9,    70,  -761,
    -144,  -761,   194,   351,  -264,  -761,  -761,   883,  -761,    -8,
    -761,  -761,  -208,  -761,  -465,  -761,  1042,  1286,   -11,   639,
    -761,  1551,  1574,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
    -298
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -432
static const yytype_int16 yytable[] =
{
     129,   193,   194,    79,   475,   148,    77,   135,   521,   155,
     544,   769,   128,   523,   161,   168,   169,   173,   174,   793,
     154,   313,   156,   656,   185,   186,   187,    47,   188,   189,
     395,   342,   324,     5,   351,   198,   200,   201,   203,   204,
       5,   636,   208,   209,   212,   216,     5,   360,   220,   221,
     222,   351,     5,   223,   207,   197,   224,   225,   226,   661,
     227,     5,   369,   198,   237,   239,     5,   182,   219,   334,
     423,   167,   512,   794,   182,    79,   801,   870,   624,   242,
     405,   406,   407,   408,   351,   238,   397,   245,   529,  -316,
     662,   392,   629,   355,   363,     3,   363,    47,   305,  -299,
     351,   404,   133,   895,   130,  -228,   246,   153,  -316,  -299,
     346,   545,  -316,   364,   512,   378,  -299,   398,  -299,     5,
    -299,   829,   424,   464,   674,   129,   307,   308,   309,   208,
     198,   239,   303,   481,   728,  -299,  -299,  -316,   905,   465,
     312,   334,   466,   655,   663,   388,   322,   793,   870,   770,
     768,   310,   363,   334,   458,   459,   467,   468,     5,   152,
     655,   906,   159,  -299,   322,   393,   469,   356,   305,   347,
    -299,    14,   190,  -297,   389,  -299,   178,   343,   305,   385,
     322,   470,   361,   357,   843,   305,   305,  -299,   666,   305,
     654,  -299,  -308,   352,  -299,   322,   394,   178,   878,   879,
     596,   794,   646,   305,   390,   618,   386,   393,   475,   655,
    -299,  -308,   157,  -299,   531,  -308,    45,   892,   263,   205,
     770,   265,   387,   393,   539,   268,   175,   597,   904,  -299,
     897,    79,   619,   391,   384,    58,   573,   827,   482,   910,
    -308,  -228,   409,   410,   411,   412,   414,   416,   417,   418,
     419,   420,   421,   422,   771,   426,   427,   428,   429,   430,
     431,   432,   433,   434,   435,   436,   437,   438,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   322,   322,   461,
     463,   474,   817,   844,   393,   599,   631,   634,   248,   363,
     484,   475,   413,   415,   393,   487,    79,   298,   521,   299,
     644,   757,   764,   256,  -388,   567,   198,   191,   499,   501,
     206,   503,   505,   356,   600,   778,   511,   511,   622,   515,
     357,   243,   249,   357,  -389,   898,   497,   644,   256,   645,
     758,   250,   574,  -388,   533,   460,   462,   675,   677,   568,
     648,   257,   537,   107,  -372,   258,   483,   322,   569,  -371,
     251,     5,   107,  -389,   252,   159,   880,   300,   511,   511,
     107,   301,   107,   570,   537,   500,   257,   237,   504,   237,
     258,   556,   537,   559,   649,   561,   563,  -413,   602,  -413,
     198,   529,   301,   650,   260,   558,   261,   262,   263,   -76,
     264,   265,   266,   253,   107,   268,   833,   302,   651,  -300,
     565,   274,   275,   833,   316,   671,   278,   356,   -76,   279,
     280,   281,   170,   263,   305,   107,   265,   356,   356,   107,
     268,  -375,  -273,    24,   192,   356,   575,   555,  -300,  -376,
    -413,    31,  -413,  -167,   211,   215,   576,   577,  -300,   -43,
     256,  -273,  -377,  -412,   727,  -412,   256,  -300,   552,   475,
     554,  -374,  -167,   521,  -300,   499,   605,   503,   608,   515,
     610,   559,  -373,   304,   314,   611,   612,   615,   323,   322,
     265,   399,   107,  -300,   327,   621,   344,   623,   257,   368,
     375,   628,   258,     5,   257,   474,   377,   159,   258,   162,
     401,   826,  -300,   486,   488,   489,   490,   834,   494,  -300,
     496,  -282,   516,   506,   834,   517,   239,   643,   202,   658,
     660,   524,   604,   525,   607,   530,   531,   107,   535,   541,
     546,   260,   239,   107,   262,   263,   657,   264,   265,   540,
     564,   263,   268,   264,   265,  -282,   560,   594,   268,   275,
     566,   595,   670,   617,  -282,   275,   279,   280,   281,   -54,
      14,   678,   637,    17,   281,    79,   638,   640,   641,   163,
     653,   672,    23,  -282,    24,   789,   673,   464,    28,   665,
     683,   107,    31,   363,  -167,   732,   735,   737,   474,   743,
     414,   461,   731,   465,   767,   734,   466,   765,   776,   896,
      41,   782,   790,  -167,   791,    45,   741,   786,   899,  -339,
     467,   468,   798,   747,   799,   749,   819,   830,   752,   837,
     469,   755,    57,   876,    58,   357,   759,   357,   760,   681,
     357,   762,   548,   357,   896,   470,   328,   331,   335,   547,
     373,   117,   761,   775,   325,   896,   413,   460,   620,   810,
     117,   198,   847,   742,   358,   848,   107,   359,   117,   849,
     117,   908,   773,   781,  -303,   785,   763,   891,   787,   172,
     795,   774,   367,    72,   902,   780,   534,   784,   862,   101,
     107,   792,    79,   107,   616,   384,     0,     0,   101,     0,
     381,   107,   117,  -303,   213,   213,   101,     0,   101,     0,
       5,     0,     0,  -303,   159,     0,     0,     0,     0,     0,
     797,     0,  -303,   117,     0,     0,     0,   117,  -282,  -303,
       0,   747,   749,     0,   752,   755,   781,   785,     0,     0,
     101,     0,     0,     0,   811,   812,     0,   813,  -303,   814,
     815,     0,     0,   816,     0,   107,   474,     0,   193,   194,
       0,   101,  -282,     0,     0,   101,     0,  -303,     0,   832,
     559,  -282,     0,   170,  -303,   838,     0,   559,   839,     0,
     117,     0,   841,   835,    24,   192,   163,     0,     0,     0,
    -282,   485,    31,   840,  -167,     0,     0,   842,  -305,     0,
     -42,   811,   850,   851,   814,   852,   853,   854,   855,     0,
       0,     0,     0,  -167,     0,   507,     0,   861,   101,     0,
     204,   208,   239,     0,   170,   117,   867,  -305,     0,   868,
       0,   117,   598,     0,     0,    24,   192,  -305,     0,     0,
       0,     0,   864,    31,     0,  -167,  -305,     0,     0,     0,
       0,     0,     0,  -305,     0,     0,   887,   888,   889,   890,
       0,     0,     0,   101,  -167,     0,     0,     0,     0,   101,
       0,   785,  -305,   900,   901,     0,     0,   107,     0,   117,
     887,   888,   889,   890,   900,   901,     0,    14,     0,     0,
      17,  -305,     0,     0,     0,   111,   903,   559,  -305,    23,
       0,    24,   789,     0,   111,    28,     0,     0,   785,    31,
       0,  -167,   111,     0,   111,     0,     0,   101,     0,     0,
       0,   107,     0,     0,     0,   107,   256,    41,     0,   790,
    -167,   791,    45,   726,     0,     0,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,   111,     0,     0,    57,
       0,    58,     0,     0,   117,     0,     0,     0,     0,   256,
       0,     0,   729,   730,   257,     0,     0,   111,   258,     0,
       0,   111,     0,     0,     0,     0,     0,     0,   117,   625,
     627,   117,     0,   630,   633,   107,     0,   107,     0,   117,
     107,     0,   101,   107,     0,     0,     0,   257,     0,     0,
      72,   258,   107,     0,     0,     0,     0,   260,   856,   261,
     262,   263,     0,   264,   265,     0,   101,     0,   268,   101,
       0,   318,   107,     0,   111,   275,     0,   101,     0,     0,
     256,     0,   279,   280,   281,   107,     0,   107,     0,   318,
     260,     0,   107,   117,   263,     0,   264,   265,     0,     0,
       0,   268,     0,     0,   114,   318,     0,     0,   275,     0,
       0,     0,     0,   114,     0,   279,   280,   281,   257,   111,
     318,   114,   258,   114,     0,   111,     0,     0,   254,     0,
       0,   101,     0,   255,     0,     0,     0,     0,     0,     0,
     739,   740,     0,   256,     0,     0,     0,     0,     0,     0,
     744,     0,     0,     0,     0,   114,   107,     0,     0,     0,
     753,   107,     0,   756,     0,   263,     0,   264,   265,     0,
       0,     0,   268,   111,     0,     0,   114,     0,     0,   275,
     114,   257,     0,   107,     0,   258,   279,   280,   281,     0,
       0,     0,     0,   107,     0,     0,     0,   107,     0,     0,
       0,     0,     0,     0,     0,   334,     0,     0,     0,   259,
       0,     0,   318,   318,     0,   117,     0,     0,     0,     0,
       0,     0,     0,     0,   260,     0,   261,   262,   263,     0,
     264,   265,   266,   114,   267,   268,   269,   270,   271,   272,
     273,   274,   275,     0,   276,   277,   278,     0,   111,   279,
     280,   281,     0,   101,     0,     0,     0,     0,     0,   117,
     676,     0,     0,   117,     0,     0,     0,     0,   107,   107,
     107,   107,   111,     0,     0,   111,     0,   536,   114,     0,
       0,     0,   318,   111,   114,   107,   107,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,   536,
       0,   101,   234,   836,   234,     0,     0,   536,     0,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   117,     0,   117,     0,     0,   117,     0,
       0,   117,   114,     0,     0,     0,     0,   111,     0,     0,
     117,     0,     0,     0,     0,     0,     0,     0,   115,     0,
       0,   296,   297,  -390,     0,     0,  -412,   115,  -412,     0,
     117,   101,     0,   101,     0,   115,   101,   115,     0,   101,
       0,     0,     0,   117,     0,   117,     0,     0,   101,     0,
     117,     0,     0,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,     0,   101,   115,
       0,     0,   614,     0,   318,     0,     0,   114,     0,     0,
       0,   101,     0,   101,     0,     0,     0,     0,   101,     0,
     115,     0,     0,     0,   115,   296,   297,     0,     0,     0,
    -412,   114,  -412,     0,   114,     0,     0,     0,     0,     0,
       0,     0,   114,     0,   117,     0,     0,     0,    14,   117,
       0,    17,     0,     0,     0,     0,     0,     0,     0,   111,
      23,     0,    24,   789,     0,     0,    28,     0,     0,     0,
      31,   117,  -167,     0,     0,     0,     0,   115,     0,     0,
       0,   117,   101,     0,     0,   117,     0,   101,   241,     0,
     790,  -167,   791,    45,     0,     0,   114,     0,     0,     0,
       0,     0,     0,   111,     0,     0,     0,   111,     0,   101,
      57,     0,    58,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   115,   101,     0,     0,     0,     0,   115,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   117,   117,   117,   117,
       0,    72,     0,     0,     0,     0,     0,   111,   183,   111,
       0,     0,   111,   117,   117,   111,   115,     0,     0,     0,
       0,     0,   183,     0,   111,     0,     0,     0,   210,   214,
       0,     0,     0,     0,   101,   101,   101,   101,     0,     0,
       0,     0,     0,     0,   111,     0,     0,     0,     0,     0,
       0,   101,   101,   119,     0,     0,     0,   111,   114,   111,
       0,     0,   119,     0,   111,     0,     0,     0,     0,     0,
     119,     0,   119,     0,     0,     0,   120,     0,     0,     0,
       0,     0,     0,     0,     0,   120,     0,     0,     0,     0,
       0,   115,     0,   120,     0,   120,     0,     0,     0,     0,
       0,     0,   114,     0,   119,     0,   114,     0,   306,     0,
       0,     0,     0,     0,     0,   115,     0,     0,   115,   824,
       0,     0,     0,     0,     0,   119,   115,   120,   111,   119,
       0,     0,     0,   111,     0,     0,     0,     0,     0,     0,
     183,   183,   183,     0,     0,     0,     0,   341,   120,     0,
       0,     0,   120,     0,     0,   111,     0,     0,   183,     0,
       0,   183,     0,     0,     0,   111,   114,     0,   114,   111,
       0,   114,     0,     0,   114,     0,   183,     0,     0,     0,
     115,   865,   119,   114,     0,     0,     0,     0,     0,     0,
     256,     0,     0,     0,   183,     0,     0,     0,     0,     0,
       0,     0,     0,   114,     0,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   114,   893,   114,     0,
       0,     0,     0,   114,     0,     0,     0,   119,   257,     0,
       0,     0,   258,   119,     0,     0,     0,     0,     0,     0,
     111,   111,   111,   111,     0,     0,     0,     0,     0,     0,
     120,     0,   893,     0,     0,     0,   120,   111,   111,     0,
       0,     0,     0,   893,     0,     0,     0,     0,     0,     0,
       0,   260,     0,   261,   262,   263,     0,   264,   265,     0,
       0,   119,   268,     0,     0,   183,     0,   114,   274,   275,
       0,     0,   114,   278,     0,     0,   279,   280,   281,     0,
       0,     0,   115,     0,   120,     0,     0,     0,     0,   183,
       0,     0,     0,     0,   114,     0,     0,     0,     5,     6,
       7,     8,     9,    10,   114,     0,     0,   136,   114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,   115,     0,   137,   138,
     115,   139,     0,    30,     0,     0,   119,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
       0,    40,     0,    42,     0,     0,     0,     0,     0,   120,
     119,     0,     0,   119,     0,    50,    51,    52,   140,     0,
       0,   119,     0,     0,     0,     0,     0,     0,     0,   114,
     114,   114,   114,   120,     0,     0,   120,     0,     0,     0,
     115,     0,   115,     0,   120,   115,   114,   114,   115,     0,
       0,    63,    64,    65,    66,     0,     0,   115,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,     0,    71,     0,   119,     0,   115,    73,     0,
       0,     0,   141,     0,    75,     0,   142,     0,     0,     0,
     115,     0,   115,     0,     0,     0,     0,   115,   120,     0,
       0,     0,     0,   183,   183,     0,     0,   183,   183,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   526,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   667,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,   115,     0,     0,    33,    34,   115,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,   115,     0,
       0,    50,    51,    52,   140,     0,     0,   119,   115,     0,
       0,     0,   115,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,   183,   183,     0,     0,     0,     0,
     120,     0,     0,     0,   183,     0,   341,    63,    64,    65,
      66,   341,     0,     0,   183,    67,     0,   183,     0,     0,
       0,   119,     0,     0,    68,   119,    69,     0,    70,    71,
       0,     0,     0,     0,    73,     0,     0,     0,   141,     0,
      75,     0,   527,     0,   120,     0,     0,     0,   120,     0,
       0,     0,   777,   115,   115,   115,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   119,     0,   119,     0,     0,
     119,     0,     0,   119,   256,     0,     0,     0,     0,     0,
       0,     0,   119,     0,     0,     0,     0,     0,   120,     0,
     120,     0,     0,   120,     0,     0,   120,     0,     0,   134,
       0,     0,   119,     0,   134,   120,     0,     0,     0,   158,
     134,   134,   257,     0,     0,   119,   258,   119,   176,     0,
     179,     0,   119,     0,     0,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,   120,     0,
     120,     0,     0,   217,     0,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,     0,   261,   262,   263,
       0,   264,   265,   266,     0,     0,   268,   269,   270,     0,
     272,   273,   274,   275,     0,   341,   341,   278,   341,   341,
     279,   280,   281,     0,     0,   244,   119,     0,     0,     0,
       0,   119,     0,     0,   869,     0,     0,     0,     0,     0,
     873,     0,   341,     0,   341,     0,     0,     0,     0,   120,
       0,     0,     0,   119,   120,     0,     0,     0,     0,     0,
       0,     0,     0,   119,     0,     0,     0,   119,     0,     0,
       0,     0,     0,     0,     0,     0,   120,     0,   217,     0,
       0,     0,     0,   317,     0,     0,   120,     0,     0,   217,
     120,   326,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   317,     0,     0,   348,   869,     0,     0,     0,     0,
       0,     0,     0,     5,     0,     0,     0,   317,   134,     0,
     685,   362,     0,     0,     0,   686,     0,     0,     0,     0,
       0,     0,   317,     0,     0,     0,     0,     0,   119,   119,
     119,   119,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   119,   119,     0,     0,     0,
       0,   120,   120,   120,   120,     0,     0,     0,     0,   400,
       0,     0,     0,     0,   179,   179,   179,   179,   120,   120,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   425,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   687,   688,   689,   690,   691,   692,     0,     0,
     693,   694,   695,   696,   697,   698,   699,   700,   701,   702,
       0,     0,   703,     0,   317,   317,   704,   705,   706,   707,
     708,   709,   710,   711,   712,   713,   714,     0,   715,     0,
       0,   716,   717,   718,   719,     0,   720,     0,     0,   491,
       0,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,     0,     0,     0,     0,     0,     0,
       0,     0,   179,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   317,     0,     0,     0,     0,     0,
      -2,     4,     0,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -280,    18,    19,    20,     0,    21,    22,     0,    23,
     179,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -167,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -280,    40,    41,    42,    43,
    -167,    44,    45,     0,  -280,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -280,     0,     0,    60,     5,    61,     0,
       0,    62,     0,     0,   685,     0,     0,     0,     0,   686,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,   317,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       0,    76,     0,     0,     0,     0,   647,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   668,     0,     0,
       0,     0,   217,     0,     0,     0,   687,   688,   689,   690,
     691,   692,     0,     0,   693,   694,   695,   696,   697,   698,
     699,   700,   701,   702,     0,     0,   703,     0,   722,     0,
     704,   705,   706,   707,   708,   709,   710,   711,   712,   713,
     714,     0,   715,     0,     0,   716,   717,   718,   719,     0,
     720,     0,     0,   721,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,   491,     5,     6,     7,     8,     9,
      10,     0,  -431,     0,    11,    12,    13,  -431,     0,    14,
      15,    16,    17,  -280,    18,    19,    20,  -431,    21,    22,
    -431,    23,   348,    24,    25,    26,    27,    28,    29,     0,
      30,    31,     0,  -167,    32,    33,    34,    35,    36,   -41,
      37,    38,   668,    39,     0,     0,     0,  -280,    40,     0,
      42,    43,  -167,    44,    45,  -431,  -280,    46,    47,  -431,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -280,   796,     0,    60,     0,
      61,     0,     0,  -431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -431,    64,
    -431,  -431,  -431,  -431,  -431,  -431,  -431,     0,  -431,  -431,
    -431,  -431,  -431,  -431,  -431,  -431,  -431,    69,  -431,  -431,
    -431,     0,    72,  -431,  -431,  -431,     0,     0,     0,    74,
    -431,    75,     0,    76,     0,     0,     0,     0,     4,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -280,    18,
      19,    20,     0,    21,    22,   134,    23,     0,    24,    25,
      26,    27,    28,    29,   846,    30,    31,     0,  -167,    32,
      33,    34,    35,    36,   -41,    37,    38,     0,    39,     0,
       0,     0,  -280,    40,    41,    42,    43,  -167,    44,    45,
       0,  -280,    46,    47,     0,    48,    49,    50,    51,    52,
      53,     0,    54,     0,    55,    56,    57,     0,    58,    59,
    -280,     0,     0,    60,     0,    61,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    69,     0,    70,    71,   195,    72,     0,     0,
      73,     0,     0,     0,    74,   228,    75,     4,    76,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,     0,    14,    15,    16,    17,  -280,    18,    19,
      20,     0,    21,    22,     0,    23,     0,    24,    25,    26,
      27,    28,    29,     0,    30,    31,     0,  -167,    32,    33,
      34,    35,    36,   -41,    37,    38,     0,    39,     0,     0,
       0,  -280,    40,    41,    42,    43,  -167,    44,    45,     0,
    -280,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -280,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,   383,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -280,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -167,    32,    33,    34,
      35,    36,   -41,    37,    38,     0,    39,     0,     0,     0,
    -280,    40,    41,    42,    43,  -167,    44,    45,     0,  -280,
      46,    47,     0,    48,    49,    50,    51,    52,    53,     0,
      54,     0,    55,    56,    57,     0,    58,    59,  -280,     0,
       0,    60,     0,    61,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,     0,    72,     0,     0,    73,     0,
       0,     0,    74,   228,    75,     4,    76,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
       0,    14,    15,    16,    17,  -280,    18,    19,    20,     0,
      21,    22,     0,    23,     0,    24,    25,    26,    27,    28,
      29,     0,    30,    31,     0,  -167,    32,    33,    34,    35,
      36,   -41,    37,    38,     0,    39,     0,     0,     0,  -280,
      40,    41,    42,    43,  -167,    44,    45,     0,  -280,    46,
      47,     0,    48,    49,    50,    51,    52,    53,     0,    54,
       0,    55,    56,    57,     0,    58,    59,  -280,     0,     0,
      60,     0,    61,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,    72,     0,     0,    73,     0,     0,
       0,    74,   680,    75,     4,    76,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,     0,
      14,    15,    16,    17,  -280,    18,    19,    20,     0,    21,
      22,     0,    23,     0,    24,    25,    26,    27,    28,    29,
       0,    30,    31,     0,  -167,    32,    33,    34,    35,    36,
     -41,    37,    38,     0,    39,     0,     0,     0,  -280,    40,
      41,    42,    43,  -167,    44,    45,     0,  -280,    46,    47,
       0,    48,    49,    50,    51,    52,    53,     0,    54,     0,
      55,    56,    57,     0,    58,    59,  -280,     0,     0,    60,
       0,    61,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,     0,     0,     0,     0,    67,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,    69,     0,
      70,    71,     0,    72,     0,     0,    73,     0,     0,     0,
      74,   788,    75,     4,    76,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,     0,    14,
      15,    16,    17,  -280,    18,    19,    20,     0,    21,    22,
       0,    23,     0,    24,    25,    26,    27,    28,    29,     0,
      30,    31,     0,  -167,    32,    33,    34,    35,    36,   -41,
      37,    38,     0,    39,     0,     0,     0,  -280,    40,   241,
      42,    43,  -167,    44,    45,     0,  -280,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -280,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -280,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -167,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -280,    40,     0,    42,
      43,  -167,    44,    45,     0,  -280,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -280,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -280,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,   498,     0,    30,    31,
       0,  -167,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -280,    40,     0,    42,    43,
    -167,    44,    45,     0,  -280,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -280,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -280,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,   502,     0,    30,    31,     0,
    -167,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -280,    40,     0,    42,    43,  -167,
      44,    45,     0,  -280,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -280,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -280,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,   746,     0,    30,    31,     0,  -167,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -280,    40,     0,    42,    43,  -167,    44,
      45,     0,  -280,    46,    47,     0,    48,    49,    50,    51,
      52,    53,     0,    54,     0,    55,    56,    57,     0,    58,
      59,  -280,     0,     0,    60,     0,    61,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,    70,    71,     0,    72,     0,
       0,    73,     0,     0,     0,    74,     0,    75,     4,    76,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -280,    18,
      19,    20,     0,    21,    22,     0,    23,     0,    24,    25,
      26,    27,    28,   748,     0,    30,    31,     0,  -167,    32,
      33,    34,    35,    36,   -41,    37,    38,     0,    39,     0,
       0,     0,  -280,    40,     0,    42,    43,  -167,    44,    45,
       0,  -280,    46,    47,     0,    48,    49,    50,    51,    52,
      53,     0,    54,     0,    55,    56,    57,     0,    58,    59,
    -280,     0,     0,    60,     0,    61,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    69,     0,    70,    71,     0,    72,     0,     0,
      73,     0,     0,     0,    74,     0,    75,     4,    76,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,     0,    14,    15,    16,    17,  -280,    18,    19,
      20,     0,    21,    22,     0,    23,     0,    24,    25,    26,
      27,    28,   751,     0,    30,    31,     0,  -167,    32,    33,
      34,    35,    36,   -41,    37,    38,     0,    39,     0,     0,
       0,  -280,    40,     0,    42,    43,  -167,    44,    45,     0,
    -280,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -280,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -280,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   754,     0,    30,    31,     0,  -167,    32,    33,    34,
      35,    36,   -41,    37,    38,     0,    39,     0,     0,     0,
    -280,    40,     0,    42,    43,  -167,    44,    45,     0,  -280,
      46,    47,     0,    48,    49,    50,    51,    52,    53,     0,
      54,     0,    55,    56,    57,     0,    58,    59,  -280,     0,
       0,    60,     0,    61,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,     0,    72,     0,     0,    73,     0,
       0,     0,    74,     0,    75,     4,    76,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
       0,    14,    15,    16,    17,  -280,    18,    19,    20,     0,
     772,    22,     0,    23,     0,    24,    25,    26,    27,    28,
      29,     0,    30,    31,     0,  -167,    32,    33,    34,    35,
      36,   -41,    37,    38,     0,    39,     0,     0,     0,  -280,
      40,     0,    42,    43,  -167,    44,    45,     0,  -280,    46,
      47,     0,    48,    49,    50,    51,    52,    53,     0,    54,
       0,    55,    56,    57,     0,    58,    59,  -280,     0,     0,
      60,     0,    61,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,    72,     0,     0,    73,     0,     0,
       0,    74,     0,    75,     4,    76,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,     0,
      14,    15,    16,    17,  -280,    18,    19,    20,     0,    21,
      22,     0,    23,     0,    24,    25,    26,    27,    28,   779,
       0,    30,    31,     0,  -167,    32,    33,    34,    35,    36,
     -41,    37,    38,     0,    39,     0,     0,     0,  -280,    40,
       0,    42,    43,  -167,    44,    45,     0,  -280,    46,    47,
       0,    48,    49,    50,    51,    52,    53,     0,    54,     0,
      55,    56,    57,     0,    58,    59,  -280,     0,     0,    60,
       0,    61,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,     0,     0,     0,     0,    67,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,    69,     0,
      70,    71,     0,    72,     0,     0,    73,     0,     0,     0,
      74,     0,    75,     4,    76,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,     0,    14,
      15,    16,    17,  -280,    18,    19,    20,     0,    21,    22,
       0,    23,     0,    24,    25,    26,    27,    28,   783,     0,
      30,    31,     0,  -167,    32,    33,    34,    35,    36,   -41,
      37,    38,     0,    39,     0,     0,     0,  -280,    40,     0,
      42,    43,  -167,    44,    45,     0,  -280,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -280,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,   526,    76,     5,     6,     7,     8,     9,    10,
       0,  -431,     0,   136,     0,     0,  -431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -431,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,  -431,     0,     0,     0,  -431,     0,
       0,    50,    51,    52,   140,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -298,  -431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -431,    64,  -431,
    -431,  -431,     0,  -431,  -431,  -431,     0,  -431,  -431,  -431,
    -431,  -431,  -431,  -431,  -431,  -431,    69,  -431,  -431,  -431,
       0,     0,  -431,  -431,  -431,     0,     0,     0,   141,     0,
      75,   526,   543,     5,     6,     7,     8,     9,    10,     0,
    -431,     0,   136,     0,     0,  -431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -431,     0,    22,     0,     0,
       0,     0,     0,   137,   138,     0,   139,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,     0,    40,     0,    42,     0,
       0,     0,     0,  -431,     0,     0,     0,  -431,     0,     0,
      50,    51,    52,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -431,    64,  -431,  -431,
    -431,     0,  -431,  -431,  -431,     0,  -431,  -431,  -431,  -431,
    -431,  -431,  -431,  -431,  -431,    69,  -431,  -431,  -431,     0,
       0,  -431,  -431,  -431,     0,     0,     0,   141,     0,    75,
     526,   543,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   137,   138,     0,   139,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,     0,    40,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   141,     0,    75,     0,
     543,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   137,   138,     0,   139,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,    70,    71,   195,     0,     0,
       0,    73,     0,     0,     0,   141,     0,    75,     0,   142,
     557,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   137,   138,     0,   139,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,    70,    71,   195,     0,     0,
       0,    73,     0,     0,     0,   141,     0,    75,     0,   142,
     863,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   137,   138,     0,   139,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,    70,    71,     0,     0,     0,
       0,    73,     0,     0,     0,   141,     0,    75,     0,   142,
     562,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,   464,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   137,   138,     0,   139,   465,    30,     0,   466,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,   467,   468,    40,     0,    42,     0,     0,     0,
       0,     0,   469,     0,     0,     0,     0,     0,    50,    51,
      52,   140,     0,     0,     0,     0,     0,   470,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,    70,    71,     0,     0,     0,
       0,    73,     0,     0,     0,   141,     0,    75,     0,   142,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     137,   138,     0,   139,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,     0,    40,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,    52,
     140,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,     0,     0,     0,
       0,    67,   233,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    69,     0,    70,    71,   195,     0,     0,     0,
      73,     0,     0,     0,   141,     0,    75,     0,   142,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   136,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   137,
     138,     0,   139,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,     0,    40,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    52,   140,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,   195,     0,     0,     0,    73,
       0,     0,     0,   141,     0,    75,   551,   142,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   137,   138,
       0,   139,     0,    30,     0,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
       0,    40,     0,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   195,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,   553,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,   165,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   166,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,   170,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,   171,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,    51,    52,   140,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,     0,     0,     0,     0,    67,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,    69,     0,
      70,    71,     0,     0,     0,     0,    73,     0,     0,     0,
     141,     0,    75,     0,   142,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   136,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   137,   138,     0,   139,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,     0,    40,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   195,     0,     0,     0,    73,     0,     0,     0,   141,
     549,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   182,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,    52,   140,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,     0,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     0,   142,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   137,   138,     0,   139,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,     0,    40,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,    52,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,   195,
       0,     0,     0,    73,     0,     0,     0,   141,     0,    75,
       0,   142,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   137,   138,     0,   139,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,     0,    40,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,   199,
       0,     0,    73,     0,     0,     0,   141,     0,    75,     0,
     142,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   137,   138,     0,   139,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   166,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,    70,    71,     0,     0,     0,
       0,    73,     0,     0,     0,   141,     0,    75,     0,   142,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     137,   138,     0,   139,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,   659,     0,     0,
       0,     0,     0,    40,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,    52,
     140,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    69,     0,    70,    71,     0,     0,     0,     0,
      73,     0,     0,     0,   141,     0,    75,     0,   142,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   136,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   820,     0,     0,     0,     0,     0,   137,
     138,     0,   139,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,     0,    40,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   821,    51,    52,   822,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,   195,     0,     0,     0,    73,
       0,     0,     0,   141,     0,    75,     0,   823,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   137,   138,
       0,   139,     0,    30,     0,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
       0,    40,     0,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   195,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   823,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     809,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,   195,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,    51,    52,   140,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,     0,     0,     0,     0,    67,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,    69,     0,
      70,    71,     0,     0,     0,     0,    73,     0,     0,     0,
     141,     0,    75,     0,   142,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   136,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   137,   138,     0,   139,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,     0,    40,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,    52,   140,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,     0,     0,     0,   508,     0,     0,     0,   141,     0,
      75,     0,   142,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   137,   138,     0,   603,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,     0,    40,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,    52,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
       0,     0,     0,    73,     0,     0,     0,   141,     0,    75,
       0,   142,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   137,   138,     0,   606,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,     0,    40,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   141,     0,    75,     0,
     142,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   137,   138,     0,   803,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,    70,    71,     0,     0,     0,
       0,    73,     0,     0,     0,   141,     0,    75,     0,   142,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     137,   138,     0,   804,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,     0,    40,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,    52,
     140,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    69,     0,    70,    71,     0,     0,     0,     0,
      73,     0,     0,     0,   141,     0,    75,     0,   142,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   136,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   137,
     138,     0,   806,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,     0,    40,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    52,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,     0,     0,     0,    73,
       0,     0,     0,   141,     0,    75,     0,   142,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   137,   138,
       0,   807,     0,    30,     0,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
       0,    40,     0,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,     0,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     808,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   809,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,    51,    52,   140,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,     0,     0,     0,     0,    67,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,    69,     0,
      70,    71,     0,     0,     0,     0,    73,     0,     0,     0,
     141,     0,    75,     0,   142,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   136,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   137,   138,     0,   139,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     5,     6,     7,     8,     9,    10,    40,     0,
      42,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,    22,     0,     0,     0,
       0,     0,   137,   138,     0,   139,     0,    30,     0,     0,
       0,     0,    33,    34,  -307,    36,   254,    37,    38,     0,
       0,   255,     0,     0,     0,    40,     0,    42,     0,    64,
       0,   256,     0,  -307,     0,     0,     0,  -307,     0,    50,
      51,    52,   140,     0,     0,     0,     0,    69,     0,     0,
       0,   195,     0,     0,     0,     0,     0,     0,     0,   141,
     254,    75,  -307,   823,     0,   255,     0,     0,     0,   257,
       0,     0,     0,   258,     0,   256,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,     0,
     142,     0,   260,   257,   261,   262,   263,   258,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   254,   276,   277,   278,     0,   255,   279,   280,   281,
       0,   259,     0,     0,     0,     0,   256,     0,   738,     0,
       0,     0,     0,     0,     0,     0,   260,     0,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   254,   276,   277,   278,     0,
     255,   279,   280,   281,   257,     0,     0,     0,   258,     0,
     256,     0,   872,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,   257,   261,
     262,   263,   258,   264,   265,   266,     0,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   254,   276,   277,   278,
       0,   255,   279,   280,   281,     0,   259,     0,     0,     0,
       0,   256,     0,   909,     0,     0,     0,     0,     0,     0,
       0,   260,     0,   261,   262,   263,     0,   264,   265,   266,
       0,   267,   268,   269,   270,   271,   272,   273,   274,   275,
       0,   276,   277,   278,     0,     0,   279,   280,   281,   257,
       0,     0,     0,   258,     0,   550,     0,     0,   254,     0,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,   332,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,   333,     0,     0,     0,     0,
       0,     0,   260,     0,   261,   262,   263,     0,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   257,   276,   277,   278,   258,     0,   279,   280,   281,
       0,     0,     0,     0,     0,   254,   845,     0,     0,     0,
     255,     0,     0,     0,     0,   334,     0,     0,     0,   259,
     256,   329,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   330,     0,   260,     0,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,   274,   275,     0,   276,   277,   278,     0,   257,   279,
     280,   281,   258,     0,     0,   305,     0,   254,     0,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,   632,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   260,     0,   261,   262,   263,     0,   264,   265,   266,
       0,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     257,   276,   277,   278,   258,     0,   279,   280,   281,     0,
       0,     0,   305,     0,   254,     0,     0,     0,     0,   255,
       0,     0,     0,     0,   334,     0,     0,     0,   259,   256,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,     0,   261,   262,   263,     0,   264,
     265,   266,     0,   267,   268,   269,   270,   271,   272,   273,
     274,   275,     0,   276,   277,   278,     0,   257,   279,   280,
     281,   258,     0,     0,   305,     0,   254,     0,   340,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,   182,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,   261,   262,   263,     0,   264,   265,   266,     0,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   257,
     276,   277,   278,   258,     0,   279,   280,   281,   254,     0,
       0,   305,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,   626,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,   261,   262,   263,     0,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   257,   276,   277,   278,   258,     0,   279,   280,   281,
     254,     0,     0,   305,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,     0,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   257,   276,   277,   278,   258,     0,   279,
     280,   281,   254,     0,   745,   305,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   257,   276,   277,   278,   258,
       0,   279,   280,   281,   254,     0,   750,   305,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   257,   276,   277,
     278,   258,     0,   279,   280,   281,   254,     0,   857,   305,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,   261,   262,   263,     0,   264,   265,   266,     0,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   257,
     276,   277,   278,   258,     0,   279,   280,   281,   254,     0,
     858,   305,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,   261,   262,   263,     0,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   257,   276,   277,   278,   258,     0,   279,   280,   281,
     254,     0,   859,   305,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,     0,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   257,   276,   277,   278,   258,     0,   279,
     280,   281,   254,     0,   860,   305,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   257,   276,   277,   278,   258,
       0,   279,   280,   281,   254,     0,   874,   305,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   257,   276,   277,
     278,   258,     0,   279,   280,   281,   254,     0,   875,   305,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,   261,   262,   263,     0,   264,   265,   266,     0,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   257,
     276,   277,   278,   258,     0,   279,   280,   281,   254,     0,
       0,   305,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,   261,   262,   263,     0,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   257,   276,   277,   278,   258,     0,   279,   280,   281,
       0,     0,     0,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   254,     0,     0,     0,   259,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,     0,     0,   260,   337,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,   274,   275,     0,   276,   277,   278,   338,     0,   279,
     280,   281,     0,     0,  -391,     0,     0,     0,   257,     0,
       0,     0,   258,     0,     0,     0,     0,   254,     0,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,   311,     0,     0,     0,     0,     0,
       0,   260,     0,   261,   262,   263,     0,   264,   265,   266,
       0,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     257,   276,   277,   278,   258,   339,   279,   280,   281,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     254,     0,     0,     0,   130,   255,     0,     0,   259,     0,
       0,     0,     0,     0,     0,   256,     0,     0,     0,     0,
     379,     0,     0,   260,     0,   261,   262,   263,     0,   264,
     265,   266,     0,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   380,   276,   277,   278,     0,     0,   279,   280,
     281,     0,     0,   257,     0,     0,     0,   258,     0,     0,
       0,     0,   254,     0,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,   476,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,   477,
       0,     0,     0,     0,     0,     0,   260,     0,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   257,   276,   277,   278,   258,
       0,   279,   280,   281,   254,     0,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
     478,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,   479,     0,     0,     0,     0,     0,     0,   260,     0,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   257,   276,   277,
     278,   258,     0,   279,   280,   281,   254,     0,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,   261,   262,   263,     0,   264,   265,   266,     0,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   257,
     276,   277,   278,   258,     0,   279,   280,   281,   254,     0,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,   261,   262,   263,     0,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   257,   276,   277,   278,   258,   315,   279,   280,   281,
     254,     0,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,     0,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   257,   276,   277,   278,   258,   365,   279,
     280,   281,   254,     0,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   257,   276,   277,   278,   258,
     370,   279,   280,   281,   254,     0,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   257,   276,   277,
     278,   258,   372,   279,   280,   281,   254,     0,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,   261,   262,   263,     0,   264,   265,   266,     0,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   257,
     276,   277,   278,   258,   374,   279,   280,   281,   254,     0,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,   261,   262,   263,     0,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   257,   276,   277,   278,   258,   382,   279,   280,   281,
     254,     0,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,     0,   396,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,     0,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   257,   276,   277,   278,   258,     0,   279,
     280,   281,   254,     0,   480,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   257,   276,   277,   278,   258,
       0,   279,   280,   281,   254,     0,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   257,   276,   277,
     278,   258,   578,   279,   280,   281,   254,     0,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,   261,   262,   263,     0,   264,   265,   266,     0,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   257,
     276,   277,   278,   258,   579,   279,   280,   281,   254,     0,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,   261,   262,   263,     0,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   257,   276,   277,   278,   258,   580,   279,   280,   281,
     254,     0,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,     0,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   257,   276,   277,   278,   258,   581,   279,
     280,   281,   254,     0,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   257,   276,   277,   278,   258,
     582,   279,   280,   281,   254,     0,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   257,   276,   277,
     278,   258,   583,   279,   280,   281,   254,     0,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,   261,   262,   263,     0,   264,   265,   266,     0,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   257,
     276,   277,   278,   258,   584,   279,   280,   281,   254,     0,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,   261,   262,   263,     0,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   257,   276,   277,   278,   258,   585,   279,   280,   281,
     254,     0,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,     0,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   257,   276,   277,   278,   258,   586,   279,
     280,   281,   254,     0,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   257,   276,   277,   278,   258,
     587,   279,   280,   281,   254,     0,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   257,   276,   277,
     278,   258,   588,   279,   280,   281,   254,     0,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,   261,   262,   263,     0,   264,   265,   266,     0,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   257,
     276,   277,   278,   258,   589,   279,   280,   281,   254,     0,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,   261,   262,   263,     0,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   257,   276,   277,   278,   258,   590,   279,   280,   281,
     254,     0,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,     0,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   257,   276,   277,   278,   258,   591,   279,
     280,   281,   254,     0,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   257,   276,   277,   278,   258,
     592,   279,   280,   281,   254,     0,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   257,   276,   277,
     278,   258,   593,   279,   280,   281,     0,     0,     0,     0,
       0,   254,     0,     0,     0,     0,   255,     0,     0,     0,
       0,   130,     0,     0,     0,   259,   256,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,   261,   262,   263,     0,   264,   265,   266,     0,
     267,   268,   269,   270,   271,   272,   273,   274,   275,     0,
     276,   277,   278,     0,   257,   279,   280,   281,   258,     0,
       0,     0,     0,   254,   639,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
       0,     0,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,     0,   261,
     262,   263,     0,   264,   265,   266,     0,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   257,   276,   277,   278,
     258,   613,   279,   280,   281,   254,     0,     0,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,     0,   642,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
       0,   261,   262,   263,     0,   264,   265,   266,     0,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   257,   276,
     277,   278,   258,     0,   279,   280,   281,   254,     0,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   260,     0,   261,   262,   263,     0,   264,   265,   266,
       0,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     257,   276,   277,   278,   258,     0,   279,   280,   281,   254,
       0,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,   733,     0,   679,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,     0,   261,   262,   263,     0,   264,
     265,   266,     0,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   257,   276,   277,   278,   258,     0,   279,   280,
     281,   254,     0,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,   736,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,     0,   261,   262,   263,
       0,   264,   265,   266,     0,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   257,   276,   277,   278,   258,     0,
     279,   280,   281,   254,     0,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
       0,     0,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,     0,   261,
     262,   263,     0,   264,   265,   266,     0,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   257,   276,   277,   278,
     258,     0,   279,   280,   281,   254,     0,   802,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
       0,   261,   262,   263,     0,   264,   265,   266,     0,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   257,   276,
     277,   278,   258,     0,   279,   280,   281,   254,   818,   805,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   260,     0,   261,   262,   263,     0,   264,   265,   266,
       0,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     257,   276,   277,   278,   258,     0,   279,   280,   281,   254,
       0,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,     0,     0,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,     0,   261,   262,   263,     0,   264,
     265,   266,     0,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   257,   276,   277,   278,   258,     0,   279,   280,
     281,   254,     0,   881,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,     0,   261,   262,   263,
       0,   264,   265,   266,     0,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   257,   276,   277,   278,   258,     0,
     279,   280,   281,   254,     0,   882,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
       0,     0,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,     0,   261,
     262,   263,     0,   264,   265,   266,     0,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   257,   276,   277,   278,
     258,     0,   279,   280,   281,   254,     0,   883,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
       0,   261,   262,   263,     0,   264,   265,   266,     0,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   257,   276,
     277,   278,   258,     0,   279,   280,   281,   254,     0,   884,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   260,     0,   261,   262,   263,     0,   264,   265,   266,
       0,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     257,   276,   277,   278,   258,     0,   279,   280,   281,   254,
       0,   885,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,     0,     0,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,     0,   261,   262,   263,     0,   264,
     265,   266,     0,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   257,   276,   277,   278,   258,     0,   279,   280,
     281,   254,     0,   886,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,     0,   261,   262,   263,
       0,   264,   265,   266,     0,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   257,   276,   277,   278,   258,     0,
     279,   280,   281,   254,     0,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
       0,     0,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,     0,   261,
     262,   263,     0,   264,   265,   266,     0,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   601,   276,   277,   278,
     258,     0,   279,   280,   281,     0,   256,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,     0,     0,   260,
       0,   261,   262,   263,   257,   264,   265,   266,   258,   267,
     268,   269,   270,   271,   272,   273,   274,   275,     0,   276,
     277,   278,     0,     0,   279,   280,   281,     0,   256,     0,
       0,     0,   259,   257,     0,     0,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,     0,   261,
     262,   263,     0,   264,   265,   266,     0,   267,   268,   269,
     270,   259,   272,   273,   274,   275,   257,   276,   277,   278,
     258,     0,   279,   280,   281,     0,   260,     0,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
       0,   272,   273,   274,   275,     0,   276,     0,   278,     0,
       0,   279,   280,   281,     0,     0,     0,     0,     0,   260,
       0,   261,   262,   263,     0,   264,   265,   266,     0,   267,
     268,   269,   270,     0,   272,   273,   274,   275,     0,   276,
       0,   278,     0,     0,   279,   280,   281
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-761))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,    43,    44,     2,   302,    16,     2,    15,   344,    20,
     375,   656,    11,   347,    24,    26,    27,    28,    29,   682,
      19,   152,    21,   525,    35,    36,    37,    66,    39,    40,
     238,   175,   163,     3,     1,    46,    47,    48,    49,    50,
       3,   506,    53,    54,    55,    56,     3,   191,    59,    60,
      61,     1,     3,    64,    53,    46,    67,    68,    69,    53,
      71,     3,   206,    74,    75,    76,     3,    26,    59,    87,
      27,    26,   338,   682,    26,    74,   725,   831,    26,    78,
     248,   249,   250,   251,     1,    76,   111,    41,   352,    92,
      84,    37,    26,    37,   111,     0,   111,    66,   137,    66,
       1,    64,    13,   863,    87,     3,    60,    18,   111,    76,
      80,   375,   115,   130,   380,   130,    66,   142,    85,     3,
      37,   766,    79,    21,   142,   136,   137,   138,   139,   140,
     141,   142,   131,    37,   599,    85,    37,   140,   898,    37,
     148,    87,    40,   110,   138,   111,   157,   810,   902,    37,
     652,   142,   111,    87,   298,   299,    54,    55,     3,    79,
     110,   130,     7,   130,   175,   111,    64,   111,   137,   139,
     137,    17,     7,    90,   140,    92,   139,   176,   137,   111,
     191,    79,   192,   182,   793,   137,   137,   137,   139,   137,
     524,    92,    92,   110,   111,   206,   142,   139,   847,   848,
      37,   810,   139,   137,   111,   111,   138,   111,   506,   110,
     111,   111,   139,   130,    92,   115,    62,   862,   110,    72,
      37,   113,   233,   111,   368,   117,   139,    64,   898,   130,
      37,   230,   138,   140,   230,    81,   404,   115,   142,   909,
     140,   139,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   142,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   757,   795,   111,   111,   504,   505,    21,   111,
     311,   599,   257,   258,   111,   316,   305,   139,   644,   141,
     111,   111,   646,    25,   111,    21,   327,   139,   329,   330,
     139,   332,   333,   111,   140,   142,   337,   338,   140,   340,
     329,    47,    55,   332,   111,   142,   327,   111,    25,   140,
     140,    64,   130,   140,   355,   300,   301,   555,   556,    55,
      21,    63,   363,     2,   130,    67,   311,   368,    64,   130,
      83,     3,    11,   140,   130,     7,   140,    63,   379,   380,
      19,    67,    21,    79,   385,   330,    63,   388,   333,   390,
      67,   392,   393,   394,    55,   396,   397,   139,    63,   141,
     401,   655,    67,    64,   106,   394,   108,   109,   110,    41,
     112,   113,   114,   136,    53,   117,   771,   139,    79,    37,
     401,   123,   124,   778,    86,   546,   128,   111,    60,   131,
     132,   133,    20,   110,   137,    74,   113,   111,   111,    78,
     117,   130,   111,    31,    32,   111,   130,   392,    66,   130,
     139,    39,   141,    41,    55,    56,   130,   130,    76,    47,
      25,   130,   130,   139,   130,   141,    25,    85,   388,   757,
     390,   130,    60,   799,    92,   476,   477,   478,   479,   480,
     481,   482,   130,   130,   130,   483,   484,   488,   137,   490,
     113,     7,   131,   111,   139,   496,   139,   498,    63,   139,
      92,   502,    67,     3,    63,   506,   130,     7,    67,     9,
     110,   765,   130,   130,    92,   140,   111,   771,   130,   137,
      37,    21,    28,   139,   778,   140,   527,   516,    49,   530,
     531,   111,   477,   140,   479,    90,    92,   176,   140,    16,
     111,   106,   543,   182,   109,   110,   527,   112,   113,   140,
     137,   110,   117,   112,   113,    55,   142,   140,   117,   124,
     137,   142,   543,    92,    64,   124,   131,   132,   133,   130,
      17,   560,   111,    20,   133,   564,   130,   130,    28,    79,
     140,   130,    29,    83,    31,    32,   130,    21,    35,   140,
      64,   230,    39,   111,    41,    26,    26,   142,   599,   137,
     601,   602,   603,    37,    64,   606,    40,   110,   110,   863,
      57,   142,    59,    60,    61,    62,   617,   142,   872,   113,
      54,    55,   113,   624,   139,   626,   140,    85,   629,   140,
      64,   632,    79,    76,    81,   624,   637,   626,   639,   564,
     629,   642,   380,   632,   898,    79,   167,   168,   169,   379,
     214,     2,   641,   666,   164,   909,   601,   602,   495,   743,
      11,   662,   798,   618,   185,   798,   305,   188,    19,   799,
      21,   902,   661,   674,    37,   676,   644,   862,   679,    28,
     684,   662,   203,   130,   877,   674,   356,   676,   819,     2,
     329,   138,   681,   332,   490,   681,    -1,    -1,    11,    -1,
     221,   340,    53,    66,    55,    56,    19,    -1,    21,    -1,
       3,    -1,    -1,    76,     7,    -1,    -1,    -1,    -1,    -1,
     721,    -1,    85,    74,    -1,    -1,    -1,    78,    21,    92,
      -1,   732,   733,    -1,   735,   736,   737,   738,    -1,    -1,
      53,    -1,    -1,    -1,   745,   746,    -1,   748,   111,   750,
     751,    -1,    -1,   754,    -1,   394,   757,    -1,   790,   791,
      -1,    74,    55,    -1,    -1,    78,    -1,   130,    -1,   770,
     771,    64,    -1,    20,   137,   776,    -1,   778,   779,    -1,
     131,    -1,   783,   772,    31,    32,    79,    -1,    -1,    -1,
      83,   312,    39,   782,    41,    -1,    -1,   786,    37,    -1,
      47,   802,   803,   804,   805,   806,   807,   808,   809,    -1,
      -1,    -1,    -1,    60,    -1,   336,    -1,   818,   131,    -1,
     821,   822,   823,    -1,    20,   176,   827,    66,    -1,   830,
      -1,   182,   471,    -1,    -1,    31,    32,    76,    -1,    -1,
      -1,    -1,   823,    39,    -1,    41,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,   857,   858,   859,   860,
      -1,    -1,    -1,   176,    60,    -1,    -1,    -1,    -1,   182,
      -1,   872,   111,   874,   875,    -1,    -1,   516,    -1,   230,
     881,   882,   883,   884,   885,   886,    -1,    17,    -1,    -1,
      20,   130,    -1,    -1,    -1,     2,   897,   898,   137,    29,
      -1,    31,    32,    -1,    11,    35,    -1,    -1,   909,    39,
      -1,    41,    19,    -1,    21,    -1,    -1,   230,    -1,    -1,
      -1,   560,    -1,    -1,    -1,   564,    25,    57,    -1,    59,
      60,    61,    62,   572,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    79,
      -1,    81,    -1,    -1,   305,    -1,    -1,    -1,    -1,    25,
      -1,    -1,   601,   602,    63,    -1,    -1,    74,    67,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,   329,   500,
     501,   332,    -1,   504,   505,   624,    -1,   626,    -1,   340,
     629,    -1,   305,   632,    -1,    -1,    -1,    63,    -1,    -1,
     130,    67,   641,    -1,    -1,    -1,    -1,   106,   138,   108,
     109,   110,    -1,   112,   113,    -1,   329,    -1,   117,   332,
      -1,   157,   661,    -1,   131,   124,    -1,   340,    -1,    -1,
      25,    -1,   131,   132,   133,   674,    -1,   676,    -1,   175,
     106,    -1,   681,   394,   110,    -1,   112,   113,    -1,    -1,
      -1,   117,    -1,    -1,     2,   191,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    11,    -1,   131,   132,   133,    63,   176,
     206,    19,    67,    21,    -1,   182,    -1,    -1,    10,    -1,
      -1,   394,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
     611,   612,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
     621,    -1,    -1,    -1,    -1,    53,   745,    -1,    -1,    -1,
     631,   750,    -1,   634,    -1,   110,    -1,   112,   113,    -1,
      -1,    -1,   117,   230,    -1,    -1,    74,    -1,    -1,   124,
      78,    63,    -1,   772,    -1,    67,   131,   132,   133,    -1,
      -1,    -1,    -1,   782,    -1,    -1,    -1,   786,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      -1,    -1,   298,   299,    -1,   516,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,   131,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    -1,   126,   127,   128,    -1,   305,   131,
     132,   133,    -1,   516,    -1,    -1,    -1,    -1,    -1,   560,
     142,    -1,    -1,   564,    -1,    -1,    -1,    -1,   857,   858,
     859,   860,   329,    -1,    -1,   332,    -1,   363,   176,    -1,
      -1,    -1,   368,   340,   182,   874,   875,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   560,    -1,   385,
      -1,   564,   388,   774,   390,    -1,    -1,   393,    -1,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   624,    -1,   626,    -1,    -1,   629,    -1,
      -1,   632,   230,    -1,    -1,    -1,    -1,   394,    -1,    -1,
     641,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,
      -1,   134,   135,   136,    -1,    -1,   139,    11,   141,    -1,
     661,   624,    -1,   626,    -1,    19,   629,    21,    -1,   632,
      -1,    -1,    -1,   674,    -1,   676,    -1,    -1,   641,    -1,
     681,    -1,    -1,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,   661,    53,
      -1,    -1,   488,    -1,   490,    -1,    -1,   305,    -1,    -1,
      -1,   674,    -1,   676,    -1,    -1,    -1,    -1,   681,    -1,
      74,    -1,    -1,    -1,    78,   134,   135,    -1,    -1,    -1,
     139,   329,   141,    -1,   332,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   340,    -1,   745,    -1,    -1,    -1,    17,   750,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   516,
      29,    -1,    31,    32,    -1,    -1,    35,    -1,    -1,    -1,
      39,   772,    41,    -1,    -1,    -1,    -1,   131,    -1,    -1,
      -1,   782,   745,    -1,    -1,   786,    -1,   750,    57,    -1,
      59,    60,    61,    62,    -1,    -1,   394,    -1,    -1,    -1,
      -1,    -1,    -1,   560,    -1,    -1,    -1,   564,    -1,   772,
      79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,   782,
      -1,    -1,   176,   786,    -1,    -1,    -1,    -1,   182,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   857,   858,   859,   860,
      -1,   130,    -1,    -1,    -1,    -1,    -1,   624,    35,   626,
      -1,    -1,   629,   874,   875,   632,   230,    -1,    -1,    -1,
      -1,    -1,    49,    -1,   641,    -1,    -1,    -1,    55,    56,
      -1,    -1,    -1,    -1,   857,   858,   859,   860,    -1,    -1,
      -1,    -1,    -1,    -1,   661,    -1,    -1,    -1,    -1,    -1,
      -1,   874,   875,     2,    -1,    -1,    -1,   674,   516,   676,
      -1,    -1,    11,    -1,   681,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    21,    -1,    -1,    -1,     2,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,   305,    -1,    19,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   560,    -1,    53,    -1,   564,    -1,   135,    -1,
      -1,    -1,    -1,    -1,    -1,   329,    -1,    -1,   332,   765,
      -1,    -1,    -1,    -1,    -1,    74,   340,    53,   745,    78,
      -1,    -1,    -1,   750,    -1,    -1,    -1,    -1,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,   174,    74,    -1,
      -1,    -1,    78,    -1,    -1,   772,    -1,    -1,   185,    -1,
      -1,   188,    -1,    -1,    -1,   782,   624,    -1,   626,   786,
      -1,   629,    -1,    -1,   632,    -1,   203,    -1,    -1,    -1,
     394,   827,   131,   641,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   661,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   674,   863,   676,    -1,
      -1,    -1,    -1,   681,    -1,    -1,    -1,   176,    63,    -1,
      -1,    -1,    67,   182,    -1,    -1,    -1,    -1,    -1,    -1,
     857,   858,   859,   860,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    -1,   898,    -1,    -1,    -1,   182,   874,   875,    -1,
      -1,    -1,    -1,   909,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,    -1,
      -1,   230,   117,    -1,    -1,   312,    -1,   745,   123,   124,
      -1,    -1,   750,   128,    -1,    -1,   131,   132,   133,    -1,
      -1,    -1,   516,    -1,   230,    -1,    -1,    -1,    -1,   336,
      -1,    -1,    -1,    -1,   772,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   782,    -1,    -1,    12,   786,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,   560,    -1,    33,    34,
     564,    36,    -1,    38,    -1,    -1,   305,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,   305,
     329,    -1,    -1,   332,    -1,    70,    71,    72,    73,    -1,
      -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   857,
     858,   859,   860,   329,    -1,    -1,   332,    -1,    -1,    -1,
     624,    -1,   626,    -1,   340,   629,   874,   875,   632,    -1,
      -1,   106,   107,   108,   109,    -1,    -1,   641,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
     125,    -1,    -1,   128,    -1,   394,    -1,   661,   133,    -1,
      -1,    -1,   137,    -1,   139,    -1,   141,    -1,    -1,    -1,
     674,    -1,   676,    -1,    -1,    -1,    -1,   681,   394,    -1,
      -1,    -1,    -1,   500,   501,    -1,    -1,   504,   505,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   541,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,   745,    -1,    -1,    43,    44,   750,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   772,    -1,
      -1,    70,    71,    72,    73,    -1,    -1,   516,   782,    -1,
      -1,    -1,   786,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,   611,   612,    -1,    -1,    -1,    -1,
     516,    -1,    -1,    -1,   621,    -1,   623,   106,   107,   108,
     109,   628,    -1,    -1,   631,   114,    -1,   634,    -1,    -1,
      -1,   560,    -1,    -1,   123,   564,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,
     139,    -1,   141,    -1,   560,    -1,    -1,    -1,   564,    -1,
      -1,    -1,   669,   857,   858,   859,   860,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     874,   875,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   624,    -1,   626,    -1,    -1,
     629,    -1,    -1,   632,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   641,    -1,    -1,    -1,    -1,    -1,   624,    -1,
     626,    -1,    -1,   629,    -1,    -1,   632,    -1,    -1,    13,
      -1,    -1,   661,    -1,    18,   641,    -1,    -1,    -1,    23,
      24,    25,    63,    -1,    -1,   674,    67,   676,    32,    -1,
      34,    -1,   681,    -1,    -1,   661,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   774,   674,    -1,
     676,    -1,    -1,    57,    -1,   681,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,    -1,   117,   118,   119,    -1,
     121,   122,   123,   124,    -1,   812,   813,   128,   815,   816,
     131,   132,   133,    -1,    -1,    99,   745,    -1,    -1,    -1,
      -1,   750,    -1,    -1,   831,    -1,    -1,    -1,    -1,    -1,
     837,    -1,   839,    -1,   841,    -1,    -1,    -1,    -1,   745,
      -1,    -1,    -1,   772,   750,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   782,    -1,    -1,    -1,   786,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   772,    -1,   152,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   782,    -1,    -1,   163,
     786,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,   178,   902,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    -1,   191,   192,    -1,
      10,   195,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,   857,   858,
     859,   860,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   874,   875,    -1,    -1,    -1,
      -1,   857,   858,   859,   860,    -1,    -1,    -1,    -1,   243,
      -1,    -1,    -1,    -1,   248,   249,   250,   251,   874,   875,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   265,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    -1,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
      -1,    -1,   112,    -1,   298,   299,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,    -1,   128,    -1,
      -1,   131,   132,   133,   134,    -1,   136,    -1,    -1,   323,
      -1,   325,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   347,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,    -1,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
     404,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    -1,    75,    -1,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,     3,    88,    -1,
      -1,    91,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
      -1,    -1,    -1,    -1,   114,    -1,   490,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,    -1,   127,   128,    -1,
     130,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,   139,
      -1,   141,    -1,    -1,    -1,    -1,   520,    -1,    -1,    -1,
     524,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   541,    -1,    -1,
      -1,    -1,   546,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    -1,    -1,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,    -1,    -1,   112,    -1,   572,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,    -1,   128,    -1,    -1,   131,   132,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,   618,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    13,    14,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,   646,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,   666,    51,    -1,    -1,    -1,    55,    56,    -1,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    75,    -1,    77,
      78,    79,    -1,    81,    82,    83,   700,    -1,    86,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,    -1,    -1,    -1,   137,
     138,   139,    -1,   141,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    26,    27,   789,    29,    -1,    31,    32,
      33,    34,    35,    36,   798,    38,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    -1,    51,    -1,
      -1,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      -1,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    -1,    75,    -1,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,   125,    -1,   127,   128,   129,   130,    -1,    -1,
     133,    -1,    -1,    -1,   137,   138,   139,     1,   141,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    -1,    26,    27,    -1,    29,    -1,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      -1,    75,    -1,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,    -1,   127,   128,    -1,   130,    -1,    -1,   133,
      -1,    -1,    -1,   137,   138,   139,     1,   141,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    -1,
      75,    -1,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
     125,    -1,   127,   128,    -1,   130,    -1,    -1,   133,    -1,
      -1,    -1,   137,   138,   139,     1,   141,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      26,    27,    -1,    29,    -1,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    -1,    75,
      -1,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
      -1,   127,   128,    -1,   130,    -1,    -1,   133,    -1,    -1,
      -1,   137,   138,   139,     1,   141,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    -1,    26,
      27,    -1,    29,    -1,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    56,
      57,    58,    59,    60,    61,    62,    -1,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    -1,    75,    -1,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,
     127,   128,    -1,   130,    -1,    -1,   133,    -1,    -1,    -1,
     137,   138,   139,     1,   141,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    -1,    26,    27,
      -1,    29,    -1,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    -1,    75,    -1,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,
     128,    -1,   130,    -1,    -1,   133,    -1,    -1,    -1,   137,
      -1,   139,     1,   141,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    -1,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    -1,    75,    -1,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    -1,    88,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,   128,
      -1,   130,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,
     139,     1,   141,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    -1,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    -1,    75,    -1,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    -1,    88,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,    -1,   127,   128,    -1,
     130,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,   139,
       1,   141,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    -1,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    -1,    75,    -1,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    -1,    88,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,   125,    -1,   127,   128,    -1,   130,
      -1,    -1,   133,    -1,    -1,    -1,   137,    -1,   139,     1,
     141,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    -1,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    -1,    75,    -1,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    -1,    88,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,    -1,   127,   128,    -1,   130,    -1,
      -1,   133,    -1,    -1,    -1,   137,    -1,   139,     1,   141,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    26,    27,    -1,    29,    -1,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    58,    59,    60,    61,    62,
      -1,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    -1,    75,    -1,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,   125,    -1,   127,   128,    -1,   130,    -1,    -1,
     133,    -1,    -1,    -1,   137,    -1,   139,     1,   141,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    -1,    26,    27,    -1,    29,    -1,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    58,    59,    60,    61,    62,    -1,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      -1,    75,    -1,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,    -1,   127,   128,    -1,   130,    -1,    -1,   133,
      -1,    -1,    -1,   137,    -1,   139,     1,   141,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    -1,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    -1,
      75,    -1,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
     125,    -1,   127,   128,    -1,   130,    -1,    -1,   133,    -1,
      -1,    -1,   137,    -1,   139,     1,   141,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      26,    27,    -1,    29,    -1,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    58,    59,    60,    61,    62,    -1,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    -1,    75,
      -1,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
      -1,   127,   128,    -1,   130,    -1,    -1,   133,    -1,    -1,
      -1,   137,    -1,   139,     1,   141,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    -1,    26,
      27,    -1,    29,    -1,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    56,
      -1,    58,    59,    60,    61,    62,    -1,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    -1,    75,    -1,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,
     127,   128,    -1,   130,    -1,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,     1,   141,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    -1,    26,    27,
      -1,    29,    -1,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,
      58,    59,    60,    61,    62,    -1,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    -1,    75,    -1,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,
     128,    -1,   130,    -1,    -1,   133,    -1,    -1,    -1,   137,
      -1,   139,     1,   141,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    67,    -1,
      -1,    70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,    -1,   131,   132,   133,    -1,    -1,    -1,   137,    -1,
     139,     1,   141,     3,     4,     5,     6,     7,     8,    -1,
      10,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    67,    -1,    -1,
      70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,   131,   132,   133,    -1,    -1,    -1,   137,    -1,   139,
       1,   141,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,
     141,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,    -1,   127,   128,   129,    -1,    -1,
      -1,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,   141,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,    -1,   127,   128,   129,    -1,    -1,
      -1,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,   141,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,   141,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    37,    38,    -1,    40,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    54,    55,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,   141,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,    -1,
      -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,   125,    -1,   127,   128,   129,    -1,    -1,    -1,
     133,    -1,    -1,    -1,   137,    -1,   139,    -1,   141,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,    -1,   127,   128,   129,    -1,    -1,    -1,   133,
      -1,    -1,    -1,   137,    -1,   139,   140,   141,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
     125,    -1,   127,   128,   129,    -1,    -1,    -1,   133,    -1,
      -1,    -1,   137,    -1,   139,   140,   141,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,   137,    -1,   139,    -1,   141,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    32,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    72,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,    -1,   141,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,
     128,   129,    -1,    -1,    -1,   133,    -1,    -1,    -1,   137,
     138,   139,    -1,   141,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,
     139,    -1,   141,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,    -1,   127,   128,   129,
      -1,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,   139,
      -1,   141,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,   125,    -1,   127,   128,    -1,   130,
      -1,    -1,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,
     141,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,   141,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    50,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,   137,    -1,   139,    -1,   141,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,    -1,   127,   128,   129,    -1,    -1,    -1,   133,
      -1,    -1,    -1,   137,    -1,   139,    -1,   141,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
     125,    -1,   127,   128,   129,    -1,    -1,    -1,   133,    -1,
      -1,    -1,   137,    -1,   139,    -1,   141,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
      -1,   127,   128,   129,    -1,    -1,    -1,   133,    -1,    -1,
      -1,   137,    -1,   139,    -1,   141,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    72,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,    -1,   141,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,   137,
      -1,   139,    -1,   141,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,
     139,    -1,   141,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,   139,
      -1,   141,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,
     141,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,   141,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,   137,    -1,   139,    -1,   141,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,   137,    -1,   139,    -1,   141,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,   137,    -1,   139,    -1,   141,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,   137,    -1,   139,    -1,   141,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    72,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,    -1,   141,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,     3,     4,     5,     6,     7,     8,    56,    -1,
      58,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    73,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    92,    46,    10,    48,    49,    -1,
      -1,    15,    -1,    -1,    -1,    56,    -1,    58,    -1,   107,
      -1,    25,    -1,   111,    -1,    -1,    -1,   115,    -1,    70,
      71,    72,    73,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
      10,   139,   140,   141,    -1,    15,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    67,    -1,    25,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
     141,    -1,   106,    63,   108,   109,   110,    67,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    10,   126,   127,   128,    -1,    15,   131,   132,   133,
      -1,    91,    -1,    -1,    -1,    -1,    25,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    10,   126,   127,   128,    -1,
      15,   131,   132,   133,    63,    -1,    -1,    -1,    67,    -1,
      25,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    63,   108,
     109,   110,    67,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    10,   126,   127,   128,
      -1,    15,   131,   132,   133,    -1,    91,    -1,    -1,    -1,
      -1,    25,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,   126,   127,   128,    -1,    -1,   131,   132,   133,    63,
      -1,    -1,    -1,    67,    -1,   140,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    10,   140,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    -1,   126,   127,   128,    -1,    63,   131,
     132,   133,    67,    -1,    -1,   137,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    26,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,    -1,   131,   132,   133,    -1,
      -1,    -1,   137,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    -1,   126,   127,   128,    -1,    63,   131,   132,
     133,    67,    -1,    -1,   137,    -1,    10,    -1,    74,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,    -1,   131,   132,   133,    10,    -1,
      -1,   137,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,    -1,   131,   132,   133,
      10,    -1,    -1,   137,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    63,   126,   127,   128,    67,    -1,   131,
     132,   133,    10,    -1,    74,   137,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    63,   126,   127,   128,    67,
      -1,   131,   132,   133,    10,    -1,    74,   137,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,    -1,   131,   132,   133,    10,    -1,    74,   137,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,    -1,   131,   132,   133,    10,    -1,
      74,   137,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,    -1,   131,   132,   133,
      10,    -1,    74,   137,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    63,   126,   127,   128,    67,    -1,   131,
     132,   133,    10,    -1,    74,   137,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    63,   126,   127,   128,    67,
      -1,   131,   132,   133,    10,    -1,    74,   137,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,    -1,   131,   132,   133,    10,    -1,    74,   137,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,    -1,   131,   132,   133,    10,    -1,
      -1,   137,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,    -1,   131,   132,   133,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    91,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,   106,    30,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    -1,   126,   127,   128,    52,    -1,   131,
     132,   133,    -1,    -1,   136,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    87,    15,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      30,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    52,   126,   127,   128,    -1,    -1,   131,   132,
     133,    -1,    -1,    63,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    63,   126,   127,   128,    67,
      -1,   131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,    -1,   131,   132,   133,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,    -1,   131,   132,   133,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,   130,   131,   132,   133,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    63,   126,   127,   128,    67,   130,   131,
     132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    63,   126,   127,   128,    67,
     130,   131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,   130,   131,   132,   133,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,   130,   131,   132,   133,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,   130,   131,   132,   133,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    63,   126,   127,   128,    67,    -1,   131,
     132,   133,    10,    -1,    74,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    63,   126,   127,   128,    67,
      -1,   131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,   130,   131,   132,   133,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,   130,   131,   132,   133,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,   130,   131,   132,   133,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    63,   126,   127,   128,    67,   130,   131,
     132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    63,   126,   127,   128,    67,
     130,   131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,   130,   131,   132,   133,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,   130,   131,   132,   133,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,   130,   131,   132,   133,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    63,   126,   127,   128,    67,   130,   131,
     132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    63,   126,   127,   128,    67,
     130,   131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,   130,   131,   132,   133,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,   130,   131,   132,   133,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,   130,   131,   132,   133,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    63,   126,   127,   128,    67,   130,   131,
     132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    63,   126,   127,   128,    67,
     130,   131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    -1,
     126,   127,   128,    -1,    63,   131,   132,   133,    67,    -1,
      -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,   130,   131,   132,   133,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    28,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,    -1,   131,   132,   133,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,    -1,   131,   132,   133,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    63,   126,   127,   128,    67,    -1,   131,   132,
     133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    63,   126,   127,   128,    67,    -1,
     131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,    -1,   131,   132,   133,    10,    -1,    74,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,    -1,   131,   132,   133,    10,    11,    74,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,    -1,   131,   132,   133,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    63,   126,   127,   128,    67,    -1,   131,   132,
     133,    10,    -1,    74,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    63,   126,   127,   128,    67,    -1,
     131,   132,   133,    10,    -1,    74,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,    -1,   131,   132,   133,    10,    -1,    74,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,    -1,   131,   132,   133,    10,    -1,    74,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,    -1,   131,   132,   133,    10,
      -1,    74,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    63,   126,   127,   128,    67,    -1,   131,   132,
     133,    10,    -1,    74,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    63,   126,   127,   128,    67,    -1,
     131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,    -1,   131,   132,   133,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    63,   112,   113,   114,    67,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    -1,   126,
     127,   128,    -1,    -1,   131,   132,   133,    -1,    25,    -1,
      -1,    -1,    91,    63,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,    91,   121,   122,   123,   124,    63,   126,   127,   128,
      67,    -1,   131,   132,   133,    -1,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,   126,    -1,   128,    -1,
      -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,   121,   122,   123,   124,    -1,   126,
      -1,   128,    -1,    -1,   131,   132,   133
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   147,   148,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    17,    18,    19,    20,    22,    23,
      24,    26,    27,    29,    31,    32,    33,    34,    35,    36,
      38,    39,    42,    43,    44,    45,    46,    48,    49,    51,
      56,    57,    58,    59,    61,    62,    65,    66,    68,    69,
      70,    71,    72,    73,    75,    77,    78,    79,    81,    82,
      86,    88,    91,   106,   107,   108,   109,   114,   123,   125,
     127,   128,   130,   133,   137,   139,   141,   149,   150,   151,
     152,   153,   154,   159,   160,   161,   163,   166,   167,   168,
     169,   170,   171,   173,   174,   175,   179,   180,   183,   184,
     187,   190,   193,   194,   217,   220,   221,   239,   240,   241,
     242,   243,   244,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   262,   263,   264,   265,   266,   151,   254,
      87,   245,   246,   162,   163,   245,    12,    33,    34,    36,
      73,   137,   141,   190,   239,   243,   252,   253,   254,   255,
     257,   258,    79,   162,   151,   254,   151,   139,   163,     7,
     162,   164,     9,    79,   164,    55,    89,   172,   254,   254,
      20,    32,   220,   254,   254,   139,   163,   191,   139,   163,
     222,   223,    26,   154,   165,   254,   254,   254,   254,   254,
       7,   139,    32,   168,   168,   129,   213,   233,   254,   130,
     254,   254,   165,   254,   254,    72,   139,   151,   254,   254,
     154,   161,   254,   255,   154,   161,   254,   163,   218,   233,
     254,   254,   254,   254,   254,   254,   254,   254,   138,   149,
     155,   233,    80,   115,   213,   234,   235,   254,   233,   254,
     261,    57,   151,    47,   163,    41,    60,   208,    21,    55,
      64,    83,   130,   136,    10,    15,    25,    63,    67,    91,
     106,   108,   109,   110,   112,   113,   114,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   126,   127,   128,   131,
     132,   133,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   134,   135,   139,   141,
      63,    67,   139,   151,   130,   137,   154,   254,   254,   254,
     233,    37,   245,   218,   130,   130,    86,   163,   213,   236,
     237,   238,   254,   137,   218,   184,   163,   139,   165,    26,
      37,   165,    26,    37,    87,   165,   248,    30,    52,   130,
      74,   154,   236,   151,   139,   202,    80,   139,   163,   224,
     225,     1,   110,   227,   228,    37,   111,   151,   165,   165,
     236,   164,   163,   111,   130,   130,   137,   165,   139,   236,
     130,   176,   130,   176,   130,    92,   219,   130,   130,    30,
      52,   165,   130,   138,   149,   111,   138,   254,   111,   140,
     111,   140,    37,   111,   142,   248,    90,   111,   142,     7,
     163,   110,   185,   195,    64,   222,   222,   222,   222,   254,
     254,   254,   254,   172,   254,   172,   254,   254,   254,   254,
     254,   254,   254,    27,    79,   163,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   236,   236,
     172,   254,   172,   254,    21,    37,    40,    54,    55,    64,
      79,   206,   247,   250,   254,   266,    26,    37,    26,    37,
      74,    37,   142,   172,   254,   165,   130,   254,    92,   140,
     111,   163,   188,   189,   130,   163,    37,   233,    36,   254,
     172,   254,    36,   254,   172,   254,   139,   165,   133,   156,
     158,   254,   156,   157,   151,   254,    28,   140,   203,   204,
     205,   206,   192,   225,   111,   140,     1,   141,   229,   240,
      90,    92,   226,   254,   223,   140,   213,   254,   181,   236,
     140,    16,   177,   141,   229,   240,   111,   158,   157,   138,
     140,   140,   234,   140,   234,   172,   254,   142,   151,   254,
     142,   254,   142,   254,   137,   233,   137,    21,    55,    64,
      79,   197,   207,   222,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   140,   142,    37,    64,   239,   111,
     140,    63,    63,    36,   172,   254,    36,   172,   254,   172,
     254,   245,   245,   130,   213,   254,   238,    92,   111,   138,
     185,   254,   140,   254,    26,   165,    26,   165,   254,    26,
     165,   248,    26,   165,   248,   249,   250,   111,   130,    11,
     130,    28,    28,   151,   111,   140,   139,   163,    21,    55,
      64,    79,   209,   140,   225,   110,   228,   233,   254,    50,
     254,    53,    84,   138,   182,   140,   139,   154,   163,   178,
     233,   218,   130,   130,   142,   248,   142,   248,   151,    90,
     138,   155,   186,    64,   196,    10,    15,    92,    93,    94,
      95,    96,    97,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   112,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   128,   131,   132,   133,   134,
     136,   139,   163,   198,   199,   200,   239,   130,   250,   239,
     239,   254,    26,    26,   254,    26,    26,   142,   142,   165,
     165,   254,   189,   137,   165,    74,    36,   254,    36,   254,
      74,    36,   254,   165,    36,   254,   165,   111,   140,   254,
     254,   151,   254,   204,   225,   110,   232,    64,   228,   226,
      37,   142,    26,   151,   233,   178,   110,   154,   142,    36,
     151,   254,   142,    36,   151,   254,   142,   254,   138,    32,
      59,    61,   138,   150,   167,   209,   163,   254,   113,   139,
     201,   201,    74,    36,    36,    74,    36,    36,    36,    36,
     186,   254,   254,   254,   254,   254,   254,   250,    11,   140,
      27,    70,    73,   141,   213,   231,   240,   115,   215,   226,
      85,   216,   254,   229,   240,   151,   165,   140,   254,   254,
     151,   254,   151,   167,   228,   140,   163,   199,   200,   203,
     254,   254,   254,   254,   254,   254,   138,    74,    74,    74,
      74,   254,   232,   142,   233,   213,   214,   254,   254,   154,
     166,   212,   142,   154,    74,    74,    76,   210,   201,   201,
     140,    74,    74,    74,    74,    74,    74,   254,   254,   254,
     254,   215,   226,   213,   230,   231,   240,    37,   142,   240,
     254,   254,   216,   254,   230,   231,   130,   211,   212,   142,
     230
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
#line 438 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 443 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 444 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 451 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 456 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 457 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 476 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 477 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 478 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 479 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 483 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 489 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 491 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), currentModuleType, new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 498 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), currentModuleType, (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 519 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 520 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 525 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 526 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 541 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 545 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 552 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 570 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 589 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 590 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 591 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 592 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 593 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 594 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 595 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 599 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 609 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 613 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 618 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 619 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 623 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 625 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 626 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 647 "chapel.ypp"
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

  case 100:

/* Line 1806 of yacc.c  */
#line 661 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 662 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 663 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 664 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 665 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 666 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 667 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 668 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 669 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 670 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 671 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pForallIntents),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 673 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pForallIntents),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 675 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 676 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pForallIntents),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 678 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 684 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pForallIntents),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 690 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 696 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pForallIntents),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 702 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 709 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pForallIntents),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pForallIntents),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 718 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 722 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 723 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 724 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 725 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 761 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 765 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 775 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 782 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch),
                                             NULL,
                                             (yyvsp[(1) - (6)].aggrTag),
                                             (yyvsp[(3) - (6)].pcallexpr),
                                             (yyvsp[(5) - (6)].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 792 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch),
                                             (yyvsp[(2) - (8)].pch),
                                             (yyvsp[(3) - (8)].aggrTag),
                                             (yyvsp[(5) - (8)].pcallexpr),
                                             (yyvsp[(7) - (8)].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 804 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 809 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 814 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 822 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 823 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 828 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 830 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 832 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 837 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 847 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[(1) - (1)].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[(1) - (1)].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[(1) - (1)].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 856 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 860 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 873 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 878 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 883 "chapel.ypp"
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

  case 167:

/* Line 1806 of yacc.c  */
#line 903 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 909 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 916 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 924 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 936 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 942 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 949 "chapel.ypp"
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

  case 174:

/* Line 1806 of yacc.c  */
#line 971 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 975 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 980 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 984 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 992 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 997 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 998 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 999 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 1000 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 1001 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 1002 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 1003 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 1004 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 1005 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 1006 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 1007 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 1008 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 1010 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 1011 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 1012 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1035 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1036 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1041 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1045 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1049 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1050 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1051 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1056 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1058 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1060 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1062 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1066 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1067 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1071 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1072 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1073 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1074 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1075 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1076 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1077 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1078 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1083 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1085 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1101 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1102 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1123 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1143 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1147 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1152 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1165 "chapel.ypp"
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

  case 272:

/* Line 1806 of yacc.c  */
#line 1181 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1183 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1185 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1190 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1198 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1206 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1214 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1223 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1232 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1240 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1248 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1250 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      USR_WARN(var, "support for '=>' to alias arrays is deprecated as of chpl version 1.15.  Use a 'ref' declaration instead.");
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1257 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1262 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1266 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1296 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1321 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1325 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1332 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1336 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1337 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1389 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1394 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 1395 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1399 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1400 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1406 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1423 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1425 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1427 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1429 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 1442 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 354:

/* Line 1806 of yacc.c  */
#line 1444 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1446 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1448 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1452 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1454 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1456 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1458 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1460 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1464 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1471 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1477 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1483 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1489 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1539 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pForallIntents) = (yyvsp[(3) - (4)].pForallIntents); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pForallIntents) = new ForallIntents();
      addForallIntent((yyval.pForallIntents), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1552 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pForallIntents), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1557 "chapel.ypp"
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[(1) - (2)].pt); (yyval.pIntentExpr).iVar = (yyvsp[(2) - (2)].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1572 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1577 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1593 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1626 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1627 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1642 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1643 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1644 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1649 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1651 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1652 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1653 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1654 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1659 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1663 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1670 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1671 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1676 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 1680 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 1682 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 1685 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 1686 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 1687 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 1688 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 452:

/* Line 1806 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 453:

/* Line 1806 of yacc.c  */
#line 1691 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 454:

/* Line 1806 of yacc.c  */
#line 1692 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 455:

/* Line 1806 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 456:

/* Line 1806 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:

/* Line 1806 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 458:

/* Line 1806 of yacc.c  */
#line 1696 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 459:

/* Line 1806 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 460:

/* Line 1806 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 461:

/* Line 1806 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 462:

/* Line 1806 of yacc.c  */
#line 1703 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 463:

/* Line 1806 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 464:

/* Line 1806 of yacc.c  */
#line 1705 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 465:

/* Line 1806 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 466:

/* Line 1806 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 467:

/* Line 1806 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 468:

/* Line 1806 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 469:

/* Line 1806 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 470:

/* Line 1806 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 471:

/* Line 1806 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 472:

/* Line 1806 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 473:

/* Line 1806 of yacc.c  */
#line 1720 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 474:

/* Line 1806 of yacc.c  */
#line 1725 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 475:

/* Line 1806 of yacc.c  */
#line 1726 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 476:

/* Line 1806 of yacc.c  */
#line 1727 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 477:

/* Line 1806 of yacc.c  */
#line 1728 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 478:

/* Line 1806 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 479:

/* Line 1806 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 480:

/* Line 1806 of yacc.c  */
#line 1731 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 8599 "bison-chapel.cpp"
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



