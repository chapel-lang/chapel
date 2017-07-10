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
#line 537 "bison-chapel.cpp"

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
#define YYLAST   14001

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  146
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  121
/* YYNRULES -- Number of rules.  */
#define YYNRULES  479
/* YYNRULES -- Number of states.  */
#define YYNSTATES  909

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
      75,    79,    82,    86,    90,    93,    96,   100,   102,   108,
     115,   116,   118,   120,   123,   127,   129,   132,   134,   138,
     142,   148,   149,   151,   153,   155,   159,   165,   171,   175,
     180,   185,   190,   195,   200,   205,   210,   215,   220,   225,
     230,   235,   240,   245,   250,   255,   256,   258,   260,   262,
     264,   267,   269,   272,   276,   278,   280,   283,   286,   288,
     290,   292,   294,   296,   298,   302,   308,   314,   317,   320,
     326,   330,   337,   344,   349,   355,   361,   365,   369,   376,
     382,   389,   395,   402,   406,   411,   418,   426,   433,   441,
     446,   452,   457,   462,   466,   473,   479,   482,   486,   490,
     493,   496,   500,   504,   505,   508,   511,   515,   521,   523,
     527,   531,   537,   538,   541,   545,   548,   552,   559,   568,
     570,   572,   574,   575,   578,   579,   582,   586,   592,   594,
     597,   601,   603,   607,   608,   609,   618,   619,   621,   624,
     627,   628,   629,   640,   644,   648,   654,   660,   662,   666,
     668,   671,   673,   675,   677,   679,   681,   683,   685,   687,
     689,   691,   693,   695,   697,   699,   701,   703,   705,   707,
     709,   711,   713,   715,   717,   719,   721,   723,   725,   727,
     729,   731,   733,   735,   737,   739,   741,   743,   744,   748,
     752,   753,   755,   759,   764,   769,   776,   783,   784,   786,
     788,   790,   792,   794,   797,   800,   802,   804,   806,   807,
     809,   811,   814,   816,   818,   820,   822,   823,   825,   828,
     830,   832,   834,   835,   837,   839,   841,   843,   845,   848,
     850,   851,   853,   856,   859,   860,   863,   867,   872,   877,
     880,   885,   886,   889,   892,   897,   902,   907,   913,   918,
     919,   921,   923,   925,   929,   933,   938,   944,   946,   948,
     952,   954,   957,   961,   962,   965,   968,   969,   974,   975,
     978,   981,   983,   988,   993,  1000,  1002,  1003,  1005,  1007,
    1011,  1016,  1020,  1025,  1032,  1033,  1036,  1039,  1042,  1045,
    1048,  1051,  1053,  1055,  1059,  1063,  1065,  1067,  1069,  1073,
    1077,  1078,  1080,  1082,  1086,  1090,  1094,  1096,  1098,  1100,
    1102,  1104,  1106,  1108,  1110,  1113,  1118,  1123,  1128,  1134,
    1137,  1140,  1147,  1154,  1159,  1169,  1179,  1187,  1194,  1201,
    1206,  1216,  1226,  1234,  1239,  1246,  1253,  1263,  1273,  1280,
    1282,  1284,  1286,  1288,  1290,  1292,  1294,  1296,  1300,  1301,
    1303,  1308,  1310,  1314,  1319,  1321,  1325,  1328,  1332,  1336,
    1338,  1342,  1345,  1350,  1352,  1354,  1356,  1358,  1360,  1362,
    1364,  1366,  1371,  1375,  1379,  1382,  1385,  1387,  1389,  1391,
    1393,  1395,  1397,  1399,  1404,  1409,  1414,  1418,  1422,  1426,
    1430,  1435,  1439,  1444,  1446,  1448,  1450,  1452,  1454,  1458,
    1463,  1467,  1472,  1476,  1481,  1485,  1491,  1495,  1499,  1503,
    1507,  1511,  1515,  1519,  1523,  1527,  1531,  1535,  1539,  1543,
    1547,  1551,  1555,  1559,  1563,  1567,  1571,  1575,  1579,  1583,
    1586,  1589,  1592,  1595,  1598,  1601,  1605,  1609,  1613,  1617,
    1621,  1625,  1629,  1633,  1635,  1637,  1639,  1641,  1643,  1645
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
     151,    -1,    51,   254,   165,    -1,    69,   254,   165,    -1,
      69,   165,    -1,    73,   151,    -1,    88,   254,   130,    -1,
       1,    -1,   153,    47,   163,   137,   138,    -1,   153,    47,
     163,   137,   155,   138,    -1,    -1,    61,    -1,    59,    -1,
     137,   138,    -1,   137,   155,   138,    -1,   149,    -1,   155,
     149,    -1,   254,    -1,   254,    11,   254,    -1,   156,   111,
     254,    -1,   156,   111,   254,    11,   254,    -1,    -1,   156,
      -1,   133,    -1,   156,    -1,    82,   233,   130,    -1,    82,
     254,    30,   158,   130,    -1,    82,   254,    52,   157,   130,
      -1,    65,   233,   130,    -1,   255,    92,   254,   130,    -1,
     255,   103,   254,   130,    -1,   255,   100,   254,   130,    -1,
     255,   102,   254,   130,    -1,   255,    96,   254,   130,    -1,
     255,   101,   254,   130,    -1,   255,    97,   254,   130,    -1,
     255,    93,   254,   130,    -1,   255,    94,   254,   130,    -1,
     255,    95,   254,   130,    -1,   255,   105,   254,   130,    -1,
     255,   104,   254,   130,    -1,   255,   134,   254,   130,    -1,
     255,   135,   254,   130,    -1,   255,    98,   254,   130,    -1,
     255,    99,   254,   130,    -1,    -1,   163,    -1,     3,    -1,
     162,    -1,     7,    -1,    26,   151,    -1,   154,    -1,    66,
     130,    -1,    66,   254,   130,    -1,   130,    -1,   168,    -1,
      61,   168,    -1,    59,   168,    -1,   187,    -1,   217,    -1,
     183,    -1,   169,    -1,   194,    -1,   220,    -1,    35,   254,
     130,    -1,    35,   254,    30,   158,   130,    -1,    35,   254,
      52,   157,   130,    -1,    35,   220,    -1,    32,     9,    -1,
      26,   151,    86,   254,   130,    -1,    86,   254,   165,    -1,
      19,   254,    37,   254,   245,   165,    -1,    19,   254,    37,
     172,   245,   165,    -1,    19,   254,   245,   165,    -1,    33,
     254,    37,   254,   165,    -1,    33,   254,    37,   172,   165,
      -1,    33,   254,   165,    -1,    33,   172,   165,    -1,    33,
      55,   163,    37,   254,   165,    -1,    34,   254,    37,   254,
     165,    -1,    34,   254,    37,   254,   248,   165,    -1,    34,
     254,    37,   172,   165,    -1,    34,   254,    37,   172,   248,
     165,    -1,    34,   254,   165,    -1,    34,   254,   248,   165,
      -1,   141,   233,    37,   254,   142,   151,    -1,   141,   233,
      37,   254,   248,   142,   151,    -1,   141,   233,    37,   172,
     142,   151,    -1,   141,   233,    37,   172,   248,   142,   151,
      -1,   141,   233,   142,   151,    -1,   141,   233,   248,   142,
     151,    -1,    89,   139,   233,   140,    -1,    36,   254,    74,
     151,    -1,    36,   254,   154,    -1,    36,   254,    74,   151,
      28,   151,    -1,    36,   254,   154,    28,   151,    -1,    23,
     151,    -1,    77,   254,   130,    -1,    78,   254,   130,    -1,
      77,   161,    -1,    78,   161,    -1,    77,   154,   176,    -1,
      78,   154,   176,    -1,    -1,   176,   177,    -1,    16,   154,
      -1,    16,   178,   154,    -1,    16,   139,   178,   140,   154,
      -1,   163,    -1,   163,   110,   254,    -1,    75,   254,   130,
      -1,    68,   254,   137,   181,   138,    -1,    -1,   181,   182,
      -1,    84,   233,   165,    -1,    53,   151,    -1,    53,    26,
     151,    -1,   184,   163,   185,   137,   186,   138,    -1,    32,
     164,   184,   163,   185,   137,   186,   138,    -1,    17,    -1,
      62,    -1,    81,    -1,    -1,   110,   233,    -1,    -1,   186,
     167,    -1,   186,   150,   167,    -1,    29,   163,   137,   188,
     138,    -1,   189,    -1,   188,   111,    -1,   188,   111,   189,
      -1,   163,    -1,   163,    92,   254,    -1,    -1,    -1,    43,
     191,   202,   192,   209,   228,   216,   212,    -1,    -1,    39,
      -1,    31,   164,    -1,    32,   164,    -1,    -1,    -1,   193,
     208,   195,   197,   196,   209,   228,   210,   216,   211,    -1,
     207,   199,   201,    -1,   207,   200,   201,    -1,   207,   198,
     113,   199,   201,    -1,   207,   198,   113,   200,   201,    -1,
     239,    -1,   139,   254,   140,    -1,   163,    -1,   107,   163,
      -1,   106,    -1,   108,    -1,   109,    -1,   107,    -1,   116,
      -1,   126,    -1,   122,    -1,   119,    -1,   121,    -1,   118,
      -1,   128,    -1,   123,    -1,   133,    -1,   112,    -1,   131,
      -1,   132,    -1,   124,    -1,   117,    -1,   125,    -1,    15,
      -1,   120,    -1,    10,    -1,   134,    -1,   136,    -1,    92,
      -1,   103,    -1,   100,    -1,   102,    -1,    96,    -1,   101,
      -1,    97,    -1,    93,    -1,    94,    -1,    95,    -1,   105,
      -1,   104,    -1,    -1,   139,   203,   140,    -1,   139,   203,
     140,    -1,    -1,   204,    -1,   203,   111,   204,    -1,   205,
     163,   232,   226,    -1,   205,   163,   232,   215,    -1,   205,
     139,   225,   140,   232,   226,    -1,   205,   139,   225,   140,
     232,   215,    -1,    -1,   206,    -1,    37,    -1,    40,    -1,
      54,    -1,    21,    -1,    21,    37,    -1,    21,    64,    -1,
      55,    -1,    64,    -1,    79,    -1,    -1,    55,    -1,    64,
      -1,    21,    64,    -1,    21,    -1,    79,    -1,    60,    -1,
      41,    -1,    -1,    21,    -1,    21,    64,    -1,    64,    -1,
      55,    -1,    79,    -1,    -1,    76,    -1,   130,    -1,   212,
      -1,   154,    -1,   166,    -1,   129,   163,    -1,   129,    -1,
      -1,   213,    -1,   115,   254,    -1,   115,   214,    -1,    -1,
      85,   254,    -1,    79,   218,   130,    -1,    20,    79,   218,
     130,    -1,    32,    79,   218,   130,    -1,   163,   219,    -1,
     163,   219,   111,   218,    -1,    -1,    92,   240,    -1,    92,
     229,    -1,   221,    55,   222,   130,    -1,   221,    21,   222,
     130,    -1,   221,    64,   222,   130,    -1,   221,    21,    64,
     222,   130,    -1,   221,    83,   222,   130,    -1,    -1,    20,
      -1,    32,    -1,   223,    -1,   222,   111,   223,    -1,   163,
     228,   226,    -1,   163,   227,    90,   254,    -1,   139,   225,
     140,   228,   226,    -1,    80,    -1,   163,    -1,   139,   225,
     140,    -1,   224,    -1,   224,   111,    -1,   224,   111,   225,
      -1,    -1,    92,    50,    -1,    92,   254,    -1,    -1,   110,
     141,   233,   142,    -1,    -1,   110,   240,    -1,   110,   229,
      -1,     1,    -1,   141,   233,   142,   240,    -1,   141,   233,
     142,   229,    -1,   141,   233,    37,   254,   142,   240,    -1,
       1,    -1,    -1,   240,    -1,   213,    -1,   141,   142,   230,
      -1,   141,   233,   142,   230,    -1,   141,   142,   231,    -1,
     141,   233,   142,   231,    -1,   141,   233,    37,   254,   142,
     230,    -1,    -1,   110,   240,    -1,   110,   213,    -1,   110,
      27,    -1,   110,    70,    -1,   110,    73,    -1,   110,   231,
      -1,   254,    -1,   213,    -1,   233,   111,   254,    -1,   233,
     111,   213,    -1,    80,    -1,   254,    -1,   213,    -1,   234,
     111,   234,    -1,   235,   111,   234,    -1,    -1,   237,    -1,
     238,    -1,   237,   111,   238,    -1,   163,    92,   213,    -1,
     163,    92,   254,    -1,   213,    -1,   254,    -1,   163,    -1,
     243,    -1,   255,    -1,   242,    -1,   263,    -1,   262,    -1,
      70,   254,    -1,    38,   139,   236,   140,    -1,    27,   139,
     236,   140,    -1,    72,   139,   236,   140,    -1,    71,    72,
     139,   236,   140,    -1,    12,   254,    -1,    73,   254,    -1,
      33,   254,    37,   254,    26,   254,    -1,    33,   254,    37,
     172,    26,   254,    -1,    33,   254,    26,   254,    -1,    33,
     254,    37,   254,    26,    36,   254,    74,   254,    -1,    33,
     254,    37,   172,    26,    36,   254,    74,   254,    -1,    33,
     254,    26,    36,   254,    74,   254,    -1,    34,   254,    37,
     254,    26,   254,    -1,    34,   254,    37,   172,    26,   254,
      -1,    34,   254,    26,   254,    -1,    34,   254,    37,   254,
      26,    36,   254,    74,   254,    -1,    34,   254,    37,   172,
      26,    36,   254,    74,   254,    -1,    34,   254,    26,    36,
     254,    74,   254,    -1,   141,   233,   142,   254,    -1,   141,
     233,    37,   254,   142,   254,    -1,   141,   233,    37,   172,
     142,   254,    -1,   141,   233,    37,   254,   142,    36,   254,
      74,   254,    -1,   141,   233,    37,   172,   142,    36,   254,
      74,   254,    -1,    36,   254,    74,   254,    28,   254,    -1,
      49,    -1,   243,    -1,   239,    -1,   258,    -1,   257,    -1,
     190,    -1,   252,    -1,   253,    -1,   251,   136,   254,    -1,
      -1,   246,    -1,    87,   139,   247,   140,    -1,   250,    -1,
     247,   111,   250,    -1,    87,   139,   249,   140,    -1,   250,
      -1,   249,   111,   250,    -1,   206,   239,    -1,   254,    63,
     239,    -1,   266,    63,   239,    -1,   255,    -1,   251,   136,
     254,    -1,    48,   254,    -1,    44,   222,    37,   254,    -1,
     260,    -1,   240,    -1,   241,    -1,   264,    -1,   265,    -1,
     190,    -1,   252,    -1,   253,    -1,   139,   115,   254,   140,
      -1,   254,   110,   254,    -1,   254,   114,   254,    -1,   254,
     114,    -1,   114,   254,    -1,   114,    -1,   239,    -1,   257,
      -1,   258,    -1,   259,    -1,   255,    -1,   190,    -1,   256,
     139,   236,   140,    -1,   256,   141,   236,   142,    -1,    58,
     139,   236,   140,    -1,   254,   113,   163,    -1,   254,   113,
      79,    -1,   254,   113,    27,    -1,   139,   234,   140,    -1,
     139,   234,   111,   140,    -1,   139,   235,   140,    -1,   139,
     235,   111,   140,    -1,     4,    -1,     5,    -1,     6,    -1,
       7,    -1,     8,    -1,   137,   233,   138,    -1,   137,   233,
     111,   138,    -1,   141,   233,   142,    -1,   141,   233,   111,
     142,    -1,   141,   261,   142,    -1,   141,   261,   111,   142,
      -1,   254,    90,   254,    -1,   261,   111,   254,    90,   254,
      -1,   254,   128,   254,    -1,   254,   123,   254,    -1,   254,
     133,   254,    -1,   254,   112,   254,    -1,   254,   131,   254,
      -1,   254,   132,   254,    -1,   254,   124,   254,    -1,   254,
     116,   254,    -1,   254,   126,   254,    -1,   254,   122,   254,
      -1,   254,   119,   254,    -1,   254,   121,   254,    -1,   254,
     118,   254,    -1,   254,   106,   254,    -1,   254,   108,   254,
      -1,   254,   109,   254,    -1,   254,    91,   254,    -1,   254,
     127,   254,    -1,   254,   117,   254,    -1,   254,    15,   254,
      -1,   254,    10,   254,    -1,   254,   120,   254,    -1,   254,
      25,   254,    -1,   128,   254,    -1,   123,   254,    -1,    46,
     254,    -1,    56,   254,    -1,   125,   254,    -1,   107,   254,
      -1,   254,    63,   254,    -1,   254,    63,   172,    -1,   266,
      63,   254,    -1,   266,    63,   172,    -1,   254,    67,   254,
      -1,   254,    67,   172,    -1,   266,    67,   254,    -1,   266,
      67,   172,    -1,   128,    -1,   133,    -1,    91,    -1,   127,
      -1,   106,    -1,   108,    -1,   109,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   437,   437,   442,   443,   449,   450,   455,   456,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   493,   495,
     500,   501,   502,   517,   518,   523,   524,   529,   534,   539,
     543,   550,   555,   559,   564,   568,   569,   570,   574,   578,
     579,   580,   581,   582,   583,   584,   585,   586,   587,   588,
     589,   590,   591,   592,   593,   597,   598,   602,   606,   607,
     611,   612,   616,   617,   621,   622,   623,   624,   625,   626,
     627,   628,   632,   633,   637,   638,   639,   640,   644,   659,
     660,   661,   662,   663,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   681,   687,   693,   699,
     706,   716,   720,   721,   722,   723,   727,   730,   731,   732,
     733,   734,   735,   739,   740,   744,   745,   746,   750,   751,
     755,   758,   763,   764,   768,   770,   772,   779,   789,   802,
     807,   812,   820,   821,   826,   827,   829,   834,   844,   853,
     857,   865,   866,   871,   876,   870,   901,   907,   914,   922,
     934,   940,   933,   968,   972,   977,   981,   989,   990,   994,
     995,   996,   997,   998,   999,  1000,  1001,  1002,  1003,  1004,
    1005,  1006,  1007,  1008,  1009,  1010,  1011,  1012,  1013,  1014,
    1015,  1016,  1017,  1018,  1019,  1023,  1024,  1025,  1026,  1027,
    1028,  1029,  1030,  1031,  1032,  1033,  1034,  1038,  1039,  1043,
    1047,  1048,  1049,  1053,  1055,  1057,  1059,  1064,  1065,  1069,
    1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,  1081,  1082,
    1083,  1084,  1085,  1086,  1090,  1091,  1095,  1096,  1097,  1098,
    1099,  1100,  1104,  1105,  1108,  1109,  1113,  1114,  1118,  1120,
    1125,  1126,  1130,  1131,  1135,  1136,  1140,  1142,  1144,  1149,
    1162,  1179,  1180,  1182,  1187,  1195,  1203,  1211,  1220,  1230,
    1231,  1232,  1236,  1237,  1245,  1247,  1254,  1259,  1261,  1263,
    1268,  1270,  1272,  1279,  1280,  1281,  1285,  1286,  1291,  1292,
    1293,  1294,  1314,  1318,  1322,  1330,  1334,  1335,  1336,  1340,
    1342,  1348,  1350,  1352,  1357,  1358,  1359,  1360,  1361,  1362,
    1363,  1369,  1370,  1371,  1372,  1376,  1377,  1378,  1382,  1383,
    1387,  1388,  1392,  1393,  1397,  1398,  1399,  1400,  1404,  1415,
    1416,  1417,  1418,  1419,  1420,  1422,  1424,  1426,  1428,  1430,
    1432,  1437,  1439,  1441,  1443,  1445,  1447,  1449,  1451,  1453,
    1455,  1457,  1459,  1461,  1468,  1474,  1480,  1486,  1495,  1503,
    1511,  1512,  1513,  1514,  1515,  1516,  1517,  1518,  1523,  1524,
    1528,  1533,  1536,  1541,  1546,  1549,  1554,  1558,  1559,  1563,
    1564,  1569,  1574,  1582,  1583,  1584,  1585,  1586,  1587,  1588,
    1589,  1590,  1592,  1594,  1596,  1598,  1600,  1605,  1606,  1607,
    1608,  1619,  1620,  1624,  1625,  1626,  1630,  1631,  1632,  1640,
    1641,  1642,  1643,  1647,  1648,  1649,  1650,  1651,  1652,  1653,
    1654,  1655,  1656,  1660,  1668,  1669,  1673,  1674,  1675,  1676,
    1677,  1678,  1679,  1680,  1681,  1682,  1683,  1684,  1685,  1686,
    1687,  1688,  1689,  1690,  1691,  1692,  1693,  1694,  1695,  1699,
    1700,  1701,  1702,  1703,  1704,  1708,  1709,  1710,  1711,  1715,
    1716,  1717,  1718,  1723,  1724,  1725,  1726,  1727,  1728,  1729
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
     151,   151,   151,   151,   151,   151,   151,   151,   152,   152,
     153,   153,   153,   154,   154,   155,   155,   156,   156,   156,
     156,   157,   157,   158,   158,   159,   159,   159,   160,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   162,   162,   163,   164,   164,
     165,   165,   166,   166,   167,   167,   167,   167,   167,   167,
     167,   167,   168,   168,   169,   169,   169,   169,   170,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   172,   173,   173,   173,   173,   174,   175,   175,   175,
     175,   175,   175,   176,   176,   177,   177,   177,   178,   178,
     179,   180,   181,   181,   182,   182,   182,   183,   183,   184,
     184,   184,   185,   185,   186,   186,   186,   187,   188,   188,
     188,   189,   189,   191,   192,   190,   193,   193,   193,   193,
     195,   196,   194,   197,   197,   197,   197,   198,   198,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   201,   201,   202,
     203,   203,   203,   204,   204,   204,   204,   205,   205,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   207,   207,
     207,   207,   207,   207,   208,   208,   209,   209,   209,   209,
     209,   209,   210,   210,   211,   211,   212,   212,   213,   213,
     214,   214,   215,   215,   216,   216,   217,   217,   217,   218,
     218,   219,   219,   219,   220,   220,   220,   220,   220,   221,
     221,   221,   222,   222,   223,   223,   223,   224,   224,   224,
     225,   225,   225,   226,   226,   226,   227,   227,   228,   228,
     228,   228,   229,   229,   229,   229,   230,   230,   230,   231,
     231,   231,   231,   231,   232,   232,   232,   232,   232,   232,
     232,   233,   233,   233,   233,   234,   234,   234,   235,   235,
     236,   236,   237,   237,   238,   238,   238,   238,   239,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   242,   243,
     244,   244,   244,   244,   244,   244,   244,   244,   245,   245,
     246,   247,   247,   248,   249,   249,   250,   250,   250,   251,
     251,   252,   253,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   255,   255,   255,
     255,   256,   256,   257,   257,   257,   258,   258,   258,   259,
     259,   259,   259,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   261,   261,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   263,
     263,   263,   263,   263,   263,   264,   264,   264,   264,   265,
     265,   265,   265,   266,   266,   266,   266,   266,   266,   266
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     2,     3,     3,     2,     2,     3,     1,     5,     6,
       0,     1,     1,     2,     3,     1,     2,     1,     3,     3,
       5,     0,     1,     1,     1,     3,     5,     5,     3,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     0,     1,     1,     1,     1,
       2,     1,     2,     3,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     3,     5,     5,     2,     2,     5,
       3,     6,     6,     4,     5,     5,     3,     3,     6,     5,
       6,     5,     6,     3,     4,     6,     7,     6,     7,     4,
       5,     4,     4,     3,     6,     5,     2,     3,     3,     2,
       2,     3,     3,     0,     2,     2,     3,     5,     1,     3,
       3,     5,     0,     2,     3,     2,     3,     6,     8,     1,
       1,     1,     0,     2,     0,     2,     3,     5,     1,     2,
       3,     1,     3,     0,     0,     8,     0,     1,     2,     2,
       0,     0,    10,     3,     3,     5,     5,     1,     3,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     3,
       0,     1,     3,     4,     4,     6,     6,     0,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     0,     1,
       1,     2,     1,     1,     1,     1,     0,     1,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     2,     1,
       0,     1,     2,     2,     0,     2,     3,     4,     4,     2,
       4,     0,     2,     2,     4,     4,     4,     5,     4,     0,
       1,     1,     1,     3,     3,     4,     5,     1,     1,     3,
       1,     2,     3,     0,     2,     2,     0,     4,     0,     2,
       2,     1,     4,     4,     6,     1,     0,     1,     1,     3,
       4,     3,     4,     6,     0,     2,     2,     2,     2,     2,
       2,     1,     1,     3,     3,     1,     1,     1,     3,     3,
       0,     1,     1,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     4,     4,     5,     2,
       2,     6,     6,     4,     9,     9,     7,     6,     6,     4,
       9,     9,     7,     4,     6,     6,     9,     9,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       4,     1,     3,     4,     1,     3,     2,     3,     3,     1,
       3,     2,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     3,     3,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     4,     3,     3,     3,     3,
       4,     3,     4,     1,     1,     1,     1,     1,     3,     4,
       3,     4,     3,     4,     3,     5,     3,     3,     3,     3,
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
       3,     0,     0,     1,    37,    77,   423,   424,   425,   426,
     427,     0,   378,    75,   149,   378,     0,   280,    75,     0,
       0,     0,     0,     0,    75,    75,     0,     0,   279,     0,
       0,   167,     0,   163,     0,     0,     0,     0,   369,     0,
       0,     0,     0,   279,   279,   150,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   151,     0,
       0,     0,   475,   477,     0,   478,   479,   406,     0,     0,
     476,   473,    84,   474,     0,     0,     0,     4,     0,     5,
       9,     0,    10,    11,    12,    14,   338,    22,    13,    85,
      91,    15,    17,    16,    19,    20,    21,    18,    90,     0,
      88,   398,     0,    92,    89,    93,     0,   407,   394,   395,
     341,   339,     0,     0,   399,   400,     0,   340,     0,   408,
     409,   410,   393,   343,   342,   396,   397,     0,    24,   349,
       0,     0,   379,     0,    76,     0,     0,     0,     0,     0,
       0,     0,     0,   398,   407,   339,   399,   400,   378,   340,
     408,   409,     0,     0,   126,     0,     0,   330,     0,    79,
      78,   168,    98,     0,   169,     0,     0,     0,     0,     0,
     280,   281,    97,     0,     0,   330,     0,     0,     0,     0,
       0,   282,    31,   461,   391,     0,   462,     7,   330,   281,
      87,    86,   259,   322,     0,   321,    82,     0,     0,     0,
      81,    34,     0,   344,     0,   330,    35,   350,     0,   133,
     129,     0,   340,   133,   130,     0,   271,     0,     0,   321,
       0,     0,   464,   405,   460,   463,   459,    43,    45,     0,
       0,   325,     0,   327,     0,     0,   326,     0,   321,     0,
       0,     6,     0,   152,   245,   244,   170,     0,     0,     0,
       0,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   330,   330,     0,
       0,     0,    25,    26,     0,    27,     0,     0,     0,     0,
       0,     0,     0,    28,    29,     0,   338,   336,     0,   331,
     332,   337,     0,     0,     0,     0,     0,   107,     0,     0,
     106,     0,     0,     0,   113,     0,     0,    51,    94,     0,
     123,     0,    30,   220,   164,   287,     0,   288,   290,     0,
     301,     0,     0,   293,     0,     0,    32,     0,   169,   258,
       0,    58,    83,   142,    80,    33,   330,     0,   140,   131,
     127,   132,   128,     0,   269,   266,    55,     0,    51,   100,
      36,    44,    46,     0,   428,     0,     0,   419,     0,   421,
       0,     0,     0,     0,     0,     0,   432,     8,     0,     0,
       0,   238,     0,     0,     0,     0,     0,   377,   456,   455,
     458,   466,   465,   470,   469,   452,   449,   450,   451,   402,
     439,   418,   417,   416,   403,   443,   454,   448,   446,   457,
     447,   445,   437,   442,   444,   453,   436,   440,   441,   438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   467,
     472,   471,   232,   229,   230,   231,   235,   236,   237,     0,
       0,   381,     0,     0,     0,     0,     0,     0,     0,     0,
     430,   378,   378,   103,   267,     0,     0,   346,     0,   161,
       0,   158,   268,   152,     0,     0,     0,   353,     0,     0,
       0,   359,     0,     0,     0,   114,   474,    54,     0,    47,
      52,     0,   122,     0,     0,   345,     0,   221,     0,   228,
     246,     0,   291,     0,   305,     0,   300,   394,     0,     0,
     284,   392,   283,   415,   324,   323,     0,     0,   347,     0,
     134,     0,   273,   394,     0,     0,     0,   429,   401,   420,
     328,   422,   329,     0,     0,   431,   119,   363,     0,   434,
     433,     0,     0,   153,   154,   242,   239,   240,   243,   171,
       0,     0,   275,   274,   276,   278,    59,    66,    67,    68,
      63,    65,    73,    74,    61,    64,    62,    60,    70,    69,
      71,    72,   413,   414,   233,   234,   386,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,   334,   335,   333,     0,   159,   157,     0,     0,
     121,     0,     0,   105,     0,   104,     0,     0,   111,     0,
       0,   109,     0,     0,   384,     0,    95,     0,    96,     0,
       0,   125,   227,   219,     0,   314,   247,   250,   249,   251,
       0,   289,   292,     0,   293,     0,   285,   294,   295,     0,
       0,   141,   143,   348,     0,   135,   138,     0,     0,   270,
      56,    57,     0,     0,     0,     0,   120,     0,    38,     0,
     279,   241,   246,   202,   200,   205,   212,   213,   214,   209,
     211,   207,   210,   208,   206,   216,   215,   181,   184,   182,
     183,   194,   185,   198,   190,   188,   201,   189,   187,   192,
     197,   199,   186,   191,   195,   196,   193,   203,   204,     0,
     179,     0,   217,   217,   177,   277,   382,   407,   407,     0,
       0,     0,     0,     0,     0,     0,     0,   102,   101,   162,
     160,   154,   108,     0,     0,   352,     0,   351,     0,     0,
     358,   112,     0,   357,   110,     0,   383,    49,    48,   124,
     368,   222,     0,     0,   293,   248,   264,   286,     0,     0,
       0,   145,     0,     0,     0,   136,     0,     0,   117,   365,
       0,     0,   115,   364,     0,   435,    39,    75,   279,   279,
     147,   279,   155,     0,   180,     0,     0,   220,   173,   174,
       0,     0,     0,     0,     0,     0,     0,     0,   279,   356,
       0,     0,   362,     0,     0,   385,     0,   314,   317,   318,
     319,     0,   316,   320,   394,   260,   224,   223,     0,     0,
       0,   303,   394,   146,   144,     0,   139,     0,   118,     0,
     116,   156,   252,   178,   179,   217,   217,     0,     0,     0,
       0,     0,     0,     0,   148,     0,     0,     0,     0,    50,
     293,   306,     0,   261,   263,   262,   265,   256,   257,   165,
       0,   137,     0,     0,   253,   264,   175,   176,   218,     0,
       0,     0,     0,     0,     0,   355,   354,   361,   360,   226,
     225,   308,   309,   311,   394,     0,   430,   394,   367,   366,
       0,     0,   310,   312,   254,   172,   255,   306,   313
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   228,    78,   512,    80,    81,    82,   229,
     507,   511,   508,    83,    84,    85,   160,    86,   164,   201,
      87,    88,    89,    90,    91,    92,   607,    93,    94,    95,
     369,   540,   667,    96,    97,   536,   662,    98,    99,   400,
     680,   100,   490,   491,   143,   177,   520,   102,   103,   401,
     682,   569,   721,   722,   723,   798,   344,   516,   517,   518,
     469,   570,   246,   650,   875,   905,   869,   193,   864,   826,
     829,   104,   217,   374,   105,   106,   180,   181,   348,   349,
     530,   352,   353,   526,   892,   823,   764,   230,   234,   235,
     318,   319,   320,   144,   108,   109,   110,   145,   112,   131,
     132,   470,   335,   633,   471,   113,   146,   147,   116,   149,
     118,   150,   151,   121,   122,   239,   123,   124,   125,   126,
     127
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -768
static const yytype_int16 yypact[] =
{
    -768,   107,  2617,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -768,  3761,    63,   150,  -768,    63,  7933,   104,   150,  3761,
    7933,  3761,    84,   150,   433,   627,  6404,  7933,  6543,  7933,
      87,  -768,   150,  -768,    72,  3761,  7933,  7933,  -768,  7933,
    7933,   213,    96,   780,   835,  -768,  6821,  6960,  7933,  7099,
    7933,   240,   198,  3761,  7933,  8072,  8072,   150,  -768,  6821,
    7933,  7933,  -768,  -768,  7933,  -768,  -768,  9462,  7933,  7933,
    -768,  7933,  -768,  -768,  2927,  5987,  6821,  -768,  3622,  -768,
    -768,   282,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,   150,
    -768,   -44,    42,  -768,  -768,  -768,   337,   230,  -768,  -768,
    -768,   243,   266,   216,   301,   302, 13758,  2331,   220,   311,
     318,  -768,  -768,  -768,  -768,  -768,  -768,   376,  -768, 13758,
     284,  3761,  -768,   356,  -768,   322,  7933,  7933,  7933,  7933,
    7933,  6821,  6821,   254,  -768,  -768,  -768,  -768, 11014,   343,
    -768,  -768,   150,   360,  -768, 11273,   426,  6821,   372,  -768,
    -768,  -768,  -768,   150,   135,   150,   384,   -13, 10002,  9935,
    -768,  -768,  -768, 10952, 10131,  6821,  3761,   387,    24,    51,
      43,  -768,  -768,   485,   402, 10193,   485,  -768,  6821,   293,
    -768,  -768,   150,  -768,   -24, 13758,  -768, 11335, 10813,  3761,
    -768,  -768, 10193, 13758,   393,  6821,  -768, 13758, 11397,  -768,
    -768, 11459,  5412,  -768,  -768, 11521,   424,   395,   106, 11087,
   10193, 11583,   466,  1657,   485,   466,   485,  -768,  -768,  3066,
       6,  -768,  7933,  -768,    59,   222, 13758,   276, 11645,   -22,
     527,  -768,   150,   434,  -768,  -768,  -768,    20,    72,    72,
      72,  -768,  7933,  7933,  7933,  7933,  7238,  7238,  7933,  7933,
    7933,  7933,  7933,  7933,   292,  9462,  7933,  7933,  7933,  7933,
    7933,  7933,  7933,  7933,  7933,  7933,  7933,  7933,  7933,  7933,
    7933,  7933,  7933,  7933,  7933,  7933,  7933,  7933,  7933,  7933,
    7933,  7933,  7933,  7933,  7933,  7933,  7933,  6821,  6821,  7238,
    7238,  5848,  -768,  -768,  3205,  -768, 11149, 11211, 11707,    44,
    7238,   -13,   406,  -768,  -768,  7933,   453,  -768,   409,   440,
    -768, 13758,   150,   420,   150,   522,  6821,  -768,  3900,  7238,
    -768,  4039,  7238,   425,  -768,   -13,  8211,  7933,  -768,  3761,
     541,   430,  -768,   290,  -768,  -768,    24,  -768,   460,   442,
    -768,    65,   483,   482,  7933,    72,  -768,   444,  -768,  -768,
    6821,  -768,  -768,  -768,  -768,  -768,  6821,   451,  -768,   564,
    -768,   564,  -768,  5429,   481,  -768,  -768,  8211,  7933,  -768,
    -768,  -768,  -768,  6682,  -768,  1208,  6126,  -768,  6265,  -768,
    7238,  2348,  2788,   452,  7933,  5708,  -768,  -768,   459,  6821,
     463,   174,    72,   180,   205,   298,   315, 10875,  2655,  2655,
     115,  -768,   115,  -768,   115,  9512,   993,   429,  1697,   402,
     466,  -768,  -768,  -768,  1657, 13868,   115,  1524,  1524,  2655,
    1524,  1524,  1407,   466, 13868,  9724,  1407,   485,   485,   466,
   11769, 11831, 11893, 11955, 12017, 12079, 12141, 12203, 12265, 12327,
   12389, 12451, 12513, 12575, 12637, 12699,   461,   468,  -768,   115,
    -768,   115,   175,  -768,  -768,  -768,  -768,  -768,  -768,   150,
     225,  -768, 13820,   386,  8350,  7238,  8489,  7238,  7933,  7238,
    1874,    63, 12761,  -768,  -768, 12828,  6821,  -768,  6821,   520,
     186,  -768,  -768,   434,  7933,   257,  7933, 13758,    52, 10255,
    7933, 13758,    28, 10064,  5848,  -768,   487,   504,   490, 12890,
     504,   493,   603, 12952,  3761,  -768,   265,  -768,    76,  -768,
     383,   488,    24,   341,  -768,  6821,  -768,   577,  7933,  7377,
    -768, 13758,  -768,  -768,  -768, 13758,   156,   492,  -768,    21,
    -768,  6821,  -768,   370,   150,   505,   508,  -768,  -768,  -768,
    -768,  -768,  -768,   -32,  2106,  -768,  -768, 13758,  3761, 13758,
    -768, 13014,  3344,   528,  -768,   578,  -768,  -768,  -768,  -768,
    9590,   377,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -768,  -768,  -768,  -768,  -768,  -768,  -768,  5848,  -768,  7238,
    7238,  7933,   615, 13076,  7933,   618, 13138,   503,  2443,   -13,
     -13,  -768,  -768, 13758,  -768,  7933,   150,  -768,   510, 10193,
    -768, 10317,  4178,  -768,  4317,  -768, 10379,  4456,  -768,   -13,
    4595,  -768,   -13,   275,  -768,  7933,  -768,  7933,  -768,  3761,
    7933,  -768,   600,  -768,    24,   540,   587,  -768,  -768,  -768,
      81,  -768,  -768,  5429,   482,   190, 13758,  -768, 13758,  4734,
    6821,  -768,  -768,  -768,   150,  -768,   548,   322,   196,  -768,
    -768,  -768,  4873,   517,  5012,   523,  -768,  7933,  -768,  3483,
     546,  -768,   383,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,   150,  -768,
    -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  7933,
     554,   558,   533,   533,  -768,  -768,  -768,   295,   310, 13200,
    8628,  8767, 13262,  8906,  9045,  9184,  9323,  -768,  -768, 13758,
    -768,  -768,  -768,  3761,  7933, 13758,  7933, 13758,  3761,  7933,
   13758,  -768,  7933, 13758,  -768,  5848,  -768, 13324, 13758,  -768,
   13758,  -768,   534,  7516,     4,  -768,   588,  -768,  7238,  5151,
    3761,  -768,    64,   537,  7933,  -768,  5290,  7933,  -768, 13758,
    3761,  7933,  -768, 13758,  3761, 13758,  -768,   391,   467,   467,
    -768,  1113,  -768,    66,  -768,  9873,  9638,   290,  -768,  -768,
    7933,  7933,  7933,  7933,  7933,  7933,  7933,  7933,   982, 12952,
   10441, 10503, 12952, 10565, 10627,  -768,  7933,   540,    84,  7933,
    7933,  5568,  -768,  -768,   259,  6821,  -768,  -768,  7933,   -15,
    9758,  -768,   619,   426,  -768,   322, 13758, 10689,  -768, 10751,
    -768,  -768,   602,  -768,  -768,   533,   533,   316, 13386, 13448,
   13510, 13572, 13634, 13696,  -768,  3761,  3761,  3761,  3761, 13758,
       4,  7655,   197,  -768,  -768, 13758, 13758,  -768,  -768,  -768,
    9323,  -768,  3761,  3761,  -768,   588,  -768,  -768,  -768,  7933,
    7933,  7933,  7933,  7933,  7933, 12952, 12952, 12952, 12952,  -768,
    -768,  -768,  -768,  -768,   432,  7238,  9540,   676, 12952, 12952,
     -25,  9802,  -768,  -768,  -768,  -768,  -768,  7794,  -768
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -768,  -768,  -768,     2,  -660,  1821,  -768,  -768,  1509,   118,
    -244,   305,   309,  -768,  -768,   439,   206,  2003,    -5,   335,
    -767,  -586,   -42,  -768,  -768,  -768,    90,  -768,  -768,  -768,
     474,  -768,    29,  -768,  -768,  -768,  -768,  -768,   530,   199,
     -45,  -768,  -768,    82,   748,  -768,  -768,  -768,  -768,  -768,
    -768,  -768,  -768,   -99,   -97,  -638,  -768,   -96,    58,  -768,
    -340,  -768,  -768,    23,  -768,  -768,  -198,    25,  -768,  -157,
    -166,  -768,  -142,  -768,   684,  -768,  -217,   361,  -768,  -335,
    -646,  -768,  -501,  -359,  -729,  -716,  -102,    15,   133,  -768,
    -129,  -768,   229,   329,  -339,  -768,  -768,   830,  -768,    -9,
    -768,  -768,  -197,  -768,  -457,  -768,   920,  1069,   -11,   423,
    -768,  1331,  1414,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -294
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -431
static const yytype_int16 yytable[] =
{
     129,   190,   191,   519,    77,   148,   135,   473,   767,   155,
     312,   521,   527,   199,   542,   168,   169,   173,   174,   161,
     791,   323,   654,     5,     5,   183,   184,     5,   185,   186,
     403,   404,   405,   406,   543,   195,   197,   198,   202,   203,
     393,    47,   207,   208,   211,   215,   341,   634,   219,   220,
     221,    47,   350,   222,   627,   333,   223,   224,   225,   357,
     226,   194,   868,   195,   236,   238,   524,   350,     5,     6,
       7,     8,     9,    10,   218,     5,   367,   136,   622,     5,
     354,   479,   350,   244,   402,   799,  -374,   360,  -298,   395,
     199,   237,    22,   510,   792,  -412,   529,  -412,   137,   138,
     233,   139,   245,    30,   345,   904,   361,     3,    33,    34,
     672,    36,   304,    37,    38,   333,   167,   383,   827,   825,
     396,    40,   304,    42,   304,   129,   306,   307,   308,   207,
     195,   238,  -298,   868,   510,    50,    51,    52,   140,   311,
     726,  -296,  -298,  -298,   384,   893,   321,  -298,   791,   766,
     130,  -298,    14,     5,   355,   391,    62,   309,   304,   178,
     664,   351,  -298,   346,   321,   304,  -298,   902,   456,   457,
     386,    63,    64,    65,    66,   360,   653,   321,   908,    67,
     903,  -298,   317,   152,   358,   571,   480,   652,    68,   304,
      69,   653,    70,    71,   321,   565,  -298,    45,    73,   387,
     317,   304,   141,  -298,    75,   841,   525,   876,   877,   659,
     473,   178,   594,   317,   890,   644,    58,   360,  -298,   133,
     187,   385,   792,   157,   153,   262,   175,   768,   264,   566,
     317,   382,   267,   768,   895,   188,   376,   537,   567,   595,
     660,   407,   408,   409,   410,   412,   414,   415,   416,   417,
     418,   419,   420,   568,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   321,   321,   459,   461,
     472,   355,   842,  -227,   661,     5,     5,   616,   815,   482,
     159,   391,   519,   473,   485,   629,   632,   391,   391,   762,
     572,   462,   204,   390,   527,   195,   355,   497,   499,   421,
     501,   503,   317,   317,   617,   509,   509,   463,   513,   242,
     464,   107,   769,   388,   -75,   573,   597,   205,   776,   896,
     107,   495,   350,   531,   465,   466,   411,   413,   107,   535,
     107,  -315,   252,   -75,   467,   321,   673,   675,   247,   297,
    -371,   298,   389,   333,   107,   598,   509,   509,   360,   468,
    -315,   422,   535,  -370,  -315,   236,   642,   236,  -298,   554,
     535,   557,   107,   559,   561,   534,   755,   391,   195,   458,
     460,   317,   248,  -412,     5,  -412,   251,   620,   159,  -315,
     481,   249,   669,   107,   646,   643,  -387,   107,   534,   355,
     831,   233,  -281,   233,   563,   756,   534,   831,   392,   498,
     250,  -388,   502,   301,   824,   117,   355,   642,   574,  -227,
     832,  -375,  -376,  -298,   117,  -387,     5,   832,   647,   299,
     159,  -373,   117,   300,   117,   575,  -281,   648,  -372,   600,
    -388,   653,  -298,   300,   255,  -281,   878,   519,   117,   304,
     107,   473,   649,   497,   603,   501,   606,   513,   608,   557,
     163,  -298,   609,   610,  -281,   613,   117,   321,   212,   212,
     553,  -272,  -411,   619,  -411,   621,   303,   170,   355,   626,
     313,   255,   256,   472,   210,   214,   257,   117,    24,   189,
    -272,   117,   327,   330,   334,   107,    31,   725,  -166,   322,
     255,   612,   315,   317,   238,   264,   373,   656,   658,   550,
     356,   552,   894,   326,  -307,   375,   343,  -166,   107,   256,
     238,   897,   366,   257,   397,   259,   484,   365,   261,   262,
     655,   263,   264,  -307,   399,   486,   267,  -307,   256,   487,
     492,   488,   257,   274,   117,   379,   668,   894,   107,   494,
     278,   279,   280,    14,   504,   602,    17,   605,   894,   514,
     515,   522,  -307,   528,   529,    23,   262,    24,   787,   264,
     539,    28,   523,   267,   533,    31,   472,  -166,   412,   459,
     729,   538,   544,   732,   558,   262,   562,   263,   264,   117,
     564,   592,   267,    41,   739,   788,  -166,   789,    45,   274,
     593,   745,   615,   747,  -299,   635,   750,   -53,   280,   753,
     636,   462,   117,   638,   757,    57,   758,    58,   651,   760,
       5,   639,   663,   107,   159,   670,   162,   463,   671,   360,
     464,   730,   681,  -299,   733,   735,   483,   741,  -281,   195,
     763,   765,   117,  -299,   465,   466,  -302,   107,   774,   780,
     107,   779,  -299,   783,   467,   784,   785,  -338,   107,  -299,
     505,   796,   797,   828,   817,   772,    72,   835,   874,   468,
     679,   382,  -281,   546,   790,  -302,   545,   371,  -299,   411,
     458,  -281,   618,   773,   324,  -302,   808,   845,   740,   846,
     761,   847,   906,   889,  -302,   793,   163,  -299,   795,   900,
    -281,  -302,   172,  -304,  -299,   860,   532,   614,     0,   745,
     747,   107,   750,   753,   779,   783,     0,   117,     0,     0,
    -302,     0,   809,   810,     0,   811,     0,   812,   813,     0,
       0,   814,  -304,     0,   472,     0,   190,   191,     0,  -302,
     101,   117,  -304,     0,   117,     0,  -302,   830,   557,   101,
       0,  -304,   117,   836,     0,   557,   837,   101,  -304,   101,
     839,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,     0,  -304,   822,   809,
     848,   849,   812,   850,   851,   852,   853,     0,   596,     0,
     170,   101,     0,     0,     0,   859,  -304,     0,   203,   207,
     238,    24,   189,  -304,   865,   117,     0,   866,     0,    31,
       0,  -166,   101,     0,     0,     0,   101,   -42,     0,     0,
       0,     0,   111,   623,   625,     0,   862,   628,   631,     0,
    -166,   111,     0,   107,   885,   886,   887,   888,     0,   111,
     863,   111,     0,     0,     0,   170,     0,     0,     0,   783,
       0,   898,   899,     0,     0,   111,    24,   189,   885,   886,
     887,   888,   898,   899,    31,     0,  -166,     0,     0,   101,
       0,     0,   -41,   111,   901,   557,   891,   107,     0,     0,
       0,   107,     0,     0,     0,  -166,   783,     0,     0,   724,
       0,     0,     0,     0,   111,     0,     0,     0,   111,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   891,   114,     0,   101,     0,     0,     0,   727,   728,
       0,   114,   891,     0,     0,     0,     0,   117,     0,   114,
       0,   114,     0,     0,   737,   738,     0,   101,     0,     0,
       0,   107,     0,   107,   742,   114,   107,     0,     0,   107,
       0,   111,     0,     0,   751,     0,     0,   754,   107,     0,
       0,     0,     0,   114,     0,     0,     0,   101,     0,     0,
       0,   117,     0,     0,     0,   117,     0,     0,   107,     0,
       0,     0,     0,     0,   114,     0,     0,     0,   114,    14,
       0,   107,    17,   107,     0,     0,   111,     0,   107,     0,
       0,    23,     0,    24,   787,     0,     0,    28,   255,     0,
       0,    31,     0,  -166,     0,     0,     0,     0,     0,   111,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    41,
       0,   788,  -166,   789,    45,   117,     0,   117,     0,     0,
     117,   114,   101,   117,     0,     0,   256,     0,     0,   111,
     257,    57,   117,    58,     0,     0,     0,     0,     0,     0,
       0,   115,   107,     0,     0,     0,   101,   107,     0,   101,
     115,     0,   117,     0,     0,     0,     0,   101,   115,     0,
     115,     0,     0,     0,     0,   117,   114,   117,     0,   107,
       0,     0,   117,   262,   115,   263,   264,   834,     0,   107,
     267,     0,    72,   107,     0,     0,     0,   274,     0,   114,
     854,     0,   115,     0,   278,   279,   280,     0,     0,     0,
      14,     0,     0,    17,   111,     0,     0,     0,     0,     0,
     101,     0,    23,   115,    24,   787,     0,   115,    28,   114,
       0,     0,    31,     0,  -166,     0,     0,     0,   111,     0,
       0,   111,     0,     0,     0,     0,   117,     0,     0,   111,
     240,   117,   788,  -166,   789,    45,     0,     0,     0,     0,
       0,     0,     0,     0,   107,   107,   107,   107,     0,     0,
       0,     0,    57,   117,    58,     0,     0,     0,     0,     0,
     115,   107,   107,   117,     0,     0,     0,   117,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   253,     0,
       0,     0,   111,   254,   114,     0,     0,     0,     0,     0,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    72,     0,   115,     0,     0,   114,     0,
       0,   114,     0,     0,     0,     0,     0,     0,     0,   114,
       0,     0,   101,     0,     0,     0,     0,     0,   115,     0,
       0,   256,     0,     0,     0,   257,     0,     0,   117,   117,
     117,   117,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   117,   117,     0,   115,   258,
       0,     0,     0,     0,     0,     0,   101,     0,     0,     0,
     101,     0,   114,     0,   259,     0,   260,   261,   262,     0,
     263,   264,   265,     0,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   119,   275,   276,   277,     0,     0,   278,
     279,   280,   119,     0,   111,     0,     0,     0,   548,     0,
     119,     0,   119,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   119,     0,     0,     0,
     101,     0,   101,   115,     0,   101,     0,     0,   101,     0,
       0,     0,     0,     0,   119,     0,     0,   101,   111,     0,
       0,     0,   111,     0,     0,     0,     0,   115,     0,     0,
     115,     0,     0,     0,     0,   119,     0,   101,   115,   119,
       0,     0,     0,     0,     0,     0,   120,     0,     0,     0,
     101,     0,   101,     0,     0,   120,     0,   101,     0,     0,
       0,     0,   255,   120,   114,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   120,
       0,     0,   111,     0,   111,     0,     0,   111,     0,     0,
     111,   115,   119,     0,     0,     0,     0,   120,     0,   111,
     256,     0,     0,     0,   257,     0,     0,     0,   114,     0,
       0,     0,   114,     0,     0,     0,     0,     0,   120,   111,
       0,   101,   120,     0,     0,     0,   101,     0,     0,     0,
       0,     0,   111,     0,   111,     0,     0,   119,     0,   111,
       0,     0,     0,   259,     0,   260,   261,   262,   101,   263,
     264,     0,     0,     0,   267,     0,     0,     0,   101,     0,
     119,   274,   101,     0,     0,     0,     0,     0,   278,   279,
     280,     0,   114,     0,   114,   120,     0,   114,     0,   255,
     114,     0,     0,     0,     0,     0,     0,     0,   200,   114,
     119,     0,     0,     0,   209,   213,     0,     0,     0,     0,
       0,     0,     0,   111,     0,     0,     0,     0,   111,   114,
       0,     0,     0,   115,     0,     0,     0,   256,     0,     0,
     120,   257,   114,     0,   114,     0,     0,     0,     0,   114,
     111,     0,     0,   101,   101,   101,   101,     0,     0,     0,
     111,     0,     0,   120,   111,     0,     0,     0,     0,     0,
     101,   101,     0,     0,     0,     0,     0,   115,     0,     0,
     259,   115,   260,   261,   262,   119,   263,   264,   265,     0,
       0,   267,     0,   120,   305,     0,     0,   273,   274,     0,
       0,     0,   277,     0,     0,   278,   279,   280,     0,   119,
       0,     0,   119,   114,     0,     0,     0,     0,   114,     0,
     119,     0,     0,     0,     0,     0,   200,   200,   200,     0,
       0,     0,   255,   340,     0,   111,   111,   111,   111,     0,
     114,   115,     0,   115,   200,     0,   115,     0,     0,   115,
     114,     0,   111,   111,   114,     0,     0,     0,   115,     0,
       0,   200,     0,     0,     0,     0,     0,     0,   120,     0,
     256,     0,   255,   119,   257,     0,     0,     0,   115,   200,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   115,   120,   115,     0,   120,     0,     0,   115,     0,
       0,     0,     0,   120,     0,     0,     0,     0,     0,     0,
     256,     0,     0,   259,   257,   260,   261,   262,     0,   263,
     264,     0,     0,     0,   267,   114,   114,   114,   114,     0,
     273,   274,     0,     0,     0,   277,     0,     0,   278,   279,
     280,     0,   114,   114,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,     0,   120,   262,     0,   263,
     264,     0,   115,     0,   267,     0,     0,   115,     0,     0,
     200,   274,     0,    79,     0,     0,     0,     0,   278,   279,
     280,     0,   128,     0,     0,     0,     0,     0,     0,   115,
     154,     0,   156,     0,   200,   119,     0,     0,     0,   115,
       0,     0,     0,   115,     0,     0,   182,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   206,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,   119,
       0,     0,     0,   119,     0,    79,     0,     0,     0,   241,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,   115,   115,   115,   115,   120,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,   115,   115,     0,    50,    51,    52,   140,     0,     0,
       0,     0,   302,   119,     0,   119,     0,     0,   119,     0,
       0,   119,     0,     0,     0,     0,     0,     0,     0,     0,
     119,     0,   120,     0,     0,     0,   120,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
     119,     0,     0,     0,     0,     0,     0,   342,     0,    69,
       0,     0,     0,   119,     0,   119,     0,   200,   200,     0,
     119,   200,   200,    75,     0,   142,   134,     0,     0,     0,
     364,   134,     0,     0,     0,     0,   158,   134,   134,     0,
       0,     0,     0,     0,     0,   176,   120,   179,   120,     0,
       0,   120,     0,     0,   120,     0,     0,     0,   665,     0,
      79,     0,     0,   120,     0,     0,     0,     0,     0,     0,
     216,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   120,   119,     0,     0,     0,     0,   119,
       0,     0,     0,     0,     0,     0,   120,     0,   120,     0,
       0,     0,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   119,   243,     0,     0,     0,     0,     0,     0,     0,
       0,   119,     0,     0,     0,   119,   253,     0,   200,   200,
       0,   254,     0,     0,     0,    79,     0,     0,   200,     0,
     340,   255,     0,     0,     0,   340,     0,     0,   200,     0,
       0,   200,     0,     0,     0,     0,     0,     0,     0,   364,
       0,     0,   364,     0,     0,   216,     0,   120,     0,     0,
     316,     0,   120,     0,     0,     0,   216,     0,   325,   256,
       0,     0,     0,   257,     0,     0,   775,     0,   316,     0,
       0,   347,     0,     0,   120,     0,   119,   119,   119,   119,
       0,   316,   134,   333,   120,   359,     0,   258,   120,     0,
       0,     0,     0,   119,   119,     0,     0,     0,   316,     0,
       0,     0,   259,   556,   260,   261,   262,     0,   263,   264,
     265,     0,   266,   267,   268,   269,   270,   271,   272,   273,
     274,     0,   275,   276,   277,     0,     0,   278,   279,   280,
       0,     0,     0,     0,     0,   398,     0,     0,   674,     0,
     179,   179,   179,   179,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   423,     0,   120,
     120,   120,   120,     0,     0,     0,     0,     0,     0,     0,
       0,   200,     0,     0,     0,     0,   120,   120,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     316,   316,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   340,
     340,     0,   340,   340,     0,   489,     0,   493,     0,     0,
       0,     0,     0,     0,     0,   641,     0,     0,   867,     0,
       0,     0,     0,     0,   871,     0,   340,     0,   340,   347,
       0,     5,     6,     7,     8,     9,    10,     0,   179,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,   316,
       0,     0,     0,     0,     0,    22,     0,     0,     0,   676,
       0,   137,   138,    79,   139,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,   179,    42,     0,     0,   867,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,   140,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,     0,     0,    62,
       0,     0,     0,   364,     0,   364,     0,     0,   364,     0,
       0,   364,     0,   253,    63,    64,    65,    66,   254,     0,
     759,     0,    67,     0,     0,   295,   296,  -389,   255,     0,
    -411,    68,  -411,    69,     0,    70,    71,   192,     0,     0,
     771,    73,     0,     0,     0,   141,     0,    75,     0,   142,
     555,   316,     0,   778,     0,   782,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,   256,     0,     0,     0,
     257,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   645,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,   666,     0,     0,     0,     0,   216,     0,   259,
       0,   260,   261,   262,     0,   263,   264,   265,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,     0,   275,
     276,   277,     0,   720,   278,   279,   280,     0,     0,     0,
       0,     0,     0,     0,     0,   736,     0,     0,     0,     0,
       0,   833,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   838,     0,     0,     0,   840,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    -2,     4,   489,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -279,    18,
      19,    20,     0,    21,    22,     0,    23,   347,    24,    25,
      26,    27,    28,    29,     0,    30,    31,     0,  -166,    32,
      33,    34,    35,    36,   -40,    37,    38,   666,    39,     0,
       0,     0,  -279,    40,    41,    42,    43,  -166,    44,    45,
     255,  -279,    46,    47,     0,    48,    49,    50,    51,    52,
      53,     0,    54,     0,    55,    56,    57,     0,    58,    59,
    -279,   794,     0,    60,     0,    61,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
       0,     0,   257,    63,    64,    65,    66,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    69,     0,    70,    71,   258,    72,     0,     0,
      73,     0,     0,     0,    74,     0,    75,     0,    76,     0,
       0,   259,     0,   260,   261,   262,     0,   263,   264,   265,
       0,   266,   267,   268,   269,     0,   271,   272,   273,   274,
       0,   275,   276,   277,     0,     0,   278,   279,   280,     4,
     134,     5,     6,     7,     8,     9,    10,     0,  -430,   844,
      11,    12,    13,  -430,     0,    14,    15,    16,    17,  -279,
      18,    19,    20,  -430,    21,    22,  -430,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,  -166,
      32,    33,    34,    35,    36,   -40,    37,    38,     0,    39,
       0,     0,     0,  -279,    40,     0,    42,    43,  -166,    44,
      45,  -430,  -279,    46,    47,  -430,    48,    49,    50,    51,
      52,    53,     0,    54,     0,    55,    56,    57,     0,    58,
      59,  -279,     0,     0,    60,     0,    61,     0,     0,  -430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -430,    64,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,     0,  -430,  -430,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,    69,  -430,  -430,  -430,     0,    72,  -430,
    -430,  -430,     0,     0,     0,    74,  -430,    75,     4,    76,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -279,    18,
      19,    20,     0,    21,    22,     0,    23,     0,    24,    25,
      26,    27,    28,    29,     0,    30,    31,     0,  -166,    32,
      33,    34,    35,    36,   -40,    37,    38,     0,    39,     0,
       0,     0,  -279,    40,    41,    42,    43,  -166,    44,    45,
       0,  -279,    46,    47,     0,    48,    49,    50,    51,    52,
      53,     0,    54,     0,    55,    56,    57,     0,    58,    59,
    -279,     0,     0,    60,     0,    61,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    69,     0,    70,    71,   192,    72,     0,     0,
      73,     0,     0,     0,    74,   227,    75,     4,    76,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,     0,    14,    15,    16,    17,  -279,    18,    19,
      20,     0,    21,    22,     0,    23,     0,    24,    25,    26,
      27,    28,    29,     0,    30,    31,     0,  -166,    32,    33,
      34,    35,    36,   -40,    37,    38,     0,    39,     0,     0,
       0,  -279,    40,    41,    42,    43,  -166,    44,    45,     0,
    -279,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -279,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,   381,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -279,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -166,    32,    33,    34,
      35,    36,   -40,    37,    38,     0,    39,     0,     0,     0,
    -279,    40,    41,    42,    43,  -166,    44,    45,     0,  -279,
      46,    47,     0,    48,    49,    50,    51,    52,    53,     0,
      54,     0,    55,    56,    57,     0,    58,    59,  -279,     0,
       0,    60,     0,    61,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,     0,    72,     0,     0,    73,     0,
       0,     0,    74,   227,    75,     4,    76,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
       0,    14,    15,    16,    17,  -279,    18,    19,    20,     0,
      21,    22,     0,    23,     0,    24,    25,    26,    27,    28,
      29,     0,    30,    31,     0,  -166,    32,    33,    34,    35,
      36,   -40,    37,    38,     0,    39,     0,     0,     0,  -279,
      40,    41,    42,    43,  -166,    44,    45,     0,  -279,    46,
      47,     0,    48,    49,    50,    51,    52,    53,     0,    54,
       0,    55,    56,    57,     0,    58,    59,  -279,     0,     0,
      60,     0,    61,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,    72,     0,     0,    73,     0,     0,
       0,    74,   678,    75,     4,    76,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,     0,
      14,    15,    16,    17,  -279,    18,    19,    20,     0,    21,
      22,     0,    23,     0,    24,    25,    26,    27,    28,    29,
       0,    30,    31,     0,  -166,    32,    33,    34,    35,    36,
     -40,    37,    38,     0,    39,     0,     0,     0,  -279,    40,
      41,    42,    43,  -166,    44,    45,     0,  -279,    46,    47,
       0,    48,    49,    50,    51,    52,    53,     0,    54,     0,
      55,    56,    57,     0,    58,    59,  -279,     0,     0,    60,
       0,    61,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,     0,     0,     0,     0,    67,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,    69,     0,
      70,    71,     0,    72,     0,     0,    73,     0,     0,     0,
      74,   786,    75,     4,    76,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,     0,    14,
      15,    16,    17,  -279,    18,    19,    20,     0,    21,    22,
       0,    23,     0,    24,    25,    26,    27,    28,    29,     0,
      30,    31,     0,  -166,    32,    33,    34,    35,    36,   -40,
      37,    38,     0,    39,     0,     0,     0,  -279,    40,   240,
      42,    43,  -166,    44,    45,     0,  -279,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -279,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -279,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -166,    32,    33,    34,    35,    36,   -40,    37,
      38,     0,    39,     0,     0,     0,  -279,    40,     0,    42,
      43,  -166,    44,    45,     0,  -279,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -279,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -279,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,   496,     0,    30,    31,
       0,  -166,    32,    33,    34,    35,    36,   -40,    37,    38,
       0,    39,     0,     0,     0,  -279,    40,     0,    42,    43,
    -166,    44,    45,     0,  -279,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -279,     0,     0,    60,     0,    61,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
      72,     0,     0,    73,     0,     0,     0,    74,     0,    75,
       4,    76,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -279,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,   500,     0,    30,    31,     0,
    -166,    32,    33,    34,    35,    36,   -40,    37,    38,     0,
      39,     0,     0,     0,  -279,    40,     0,    42,    43,  -166,
      44,    45,     0,  -279,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -279,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -279,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,   744,     0,    30,    31,     0,  -166,
      32,    33,    34,    35,    36,   -40,    37,    38,     0,    39,
       0,     0,     0,  -279,    40,     0,    42,    43,  -166,    44,
      45,     0,  -279,    46,    47,     0,    48,    49,    50,    51,
      52,    53,     0,    54,     0,    55,    56,    57,     0,    58,
      59,  -279,     0,     0,    60,     0,    61,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,    70,    71,     0,    72,     0,
       0,    73,     0,     0,     0,    74,     0,    75,     4,    76,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -279,    18,
      19,    20,     0,    21,    22,     0,    23,     0,    24,    25,
      26,    27,    28,   746,     0,    30,    31,     0,  -166,    32,
      33,    34,    35,    36,   -40,    37,    38,     0,    39,     0,
       0,     0,  -279,    40,     0,    42,    43,  -166,    44,    45,
       0,  -279,    46,    47,     0,    48,    49,    50,    51,    52,
      53,     0,    54,     0,    55,    56,    57,     0,    58,    59,
    -279,     0,     0,    60,     0,    61,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    69,     0,    70,    71,     0,    72,     0,     0,
      73,     0,     0,     0,    74,     0,    75,     4,    76,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,     0,    14,    15,    16,    17,  -279,    18,    19,
      20,     0,    21,    22,     0,    23,     0,    24,    25,    26,
      27,    28,   749,     0,    30,    31,     0,  -166,    32,    33,
      34,    35,    36,   -40,    37,    38,     0,    39,     0,     0,
       0,  -279,    40,     0,    42,    43,  -166,    44,    45,     0,
    -279,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -279,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -279,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   752,     0,    30,    31,     0,  -166,    32,    33,    34,
      35,    36,   -40,    37,    38,     0,    39,     0,     0,     0,
    -279,    40,     0,    42,    43,  -166,    44,    45,     0,  -279,
      46,    47,     0,    48,    49,    50,    51,    52,    53,     0,
      54,     0,    55,    56,    57,     0,    58,    59,  -279,     0,
       0,    60,     0,    61,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,     0,    72,     0,     0,    73,     0,
       0,     0,    74,     0,    75,     4,    76,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
       0,    14,    15,    16,    17,  -279,    18,    19,    20,     0,
     770,    22,     0,    23,     0,    24,    25,    26,    27,    28,
      29,     0,    30,    31,     0,  -166,    32,    33,    34,    35,
      36,   -40,    37,    38,     0,    39,     0,     0,     0,  -279,
      40,     0,    42,    43,  -166,    44,    45,     0,  -279,    46,
      47,     0,    48,    49,    50,    51,    52,    53,     0,    54,
       0,    55,    56,    57,     0,    58,    59,  -279,     0,     0,
      60,     0,    61,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,    72,     0,     0,    73,     0,     0,
       0,    74,     0,    75,     4,    76,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,     0,
      14,    15,    16,    17,  -279,    18,    19,    20,     0,    21,
      22,     0,    23,     0,    24,    25,    26,    27,    28,   777,
       0,    30,    31,     0,  -166,    32,    33,    34,    35,    36,
     -40,    37,    38,     0,    39,     0,     0,     0,  -279,    40,
       0,    42,    43,  -166,    44,    45,     0,  -279,    46,    47,
       0,    48,    49,    50,    51,    52,    53,     0,    54,     0,
      55,    56,    57,     0,    58,    59,  -279,     0,     0,    60,
       0,    61,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,     0,     0,     0,     0,    67,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,    69,     0,
      70,    71,     0,    72,     0,     0,    73,     0,     0,     0,
      74,     0,    75,     4,    76,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,     0,    14,
      15,    16,    17,  -279,    18,    19,    20,     0,    21,    22,
       0,    23,     0,    24,    25,    26,    27,    28,   781,     0,
      30,    31,     0,  -166,    32,    33,    34,    35,    36,   -40,
      37,    38,     0,    39,     0,     0,     0,  -279,    40,     0,
      42,    43,  -166,    44,    45,     0,  -279,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -279,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,   524,    76,     5,     6,     7,     8,     9,    10,
       0,  -430,     0,   136,     0,     0,  -430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -430,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,  -430,     0,     0,     0,  -430,     0,
       0,    50,    51,    52,   140,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -297,  -430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -430,    64,  -430,
    -430,  -430,     0,  -430,  -430,  -430,     0,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,  -430,    69,  -430,  -430,  -430,
       0,     0,  -430,  -430,  -430,     0,     0,     0,   141,     0,
      75,   524,   541,     5,     6,     7,     8,     9,    10,     0,
    -430,     0,   136,     0,     0,  -430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -430,     0,    22,     0,     0,
       0,     0,     0,   137,   138,     0,   139,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,     0,    40,     0,    42,     0,
       0,     0,     0,  -430,     0,     0,     0,  -430,     0,     0,
      50,    51,    52,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -430,    64,  -430,  -430,
    -430,     0,  -430,  -430,  -430,     0,  -430,  -430,  -430,  -430,
    -430,  -430,  -430,  -430,  -430,    69,  -430,  -430,  -430,     0,
       0,  -430,  -430,  -430,     0,     0,     0,   141,     0,    75,
     524,   541,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   137,   138,     0,   139,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,     0,    40,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   140,     0,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,   295,   296,     0,     0,
       0,  -411,    68,  -411,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   141,     0,    75,     0,
     541,     5,     6,     7,     8,     9,    10,     0,     0,     0,
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
       0,    68,     0,    69,     0,    70,    71,   192,     0,     0,
       0,    73,     0,     0,     0,   141,     0,    75,     0,   142,
     861,     5,     6,     7,     8,     9,    10,     0,     0,     0,
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
     560,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,   462,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   137,   138,     0,   139,   463,    30,     0,   464,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,   465,   466,    40,     0,    42,     0,     0,     0,
       0,     0,   467,     0,     0,     0,     0,     0,    50,    51,
      52,   140,     0,     0,     0,     0,     0,   468,     0,     0,
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
     140,     0,     0,     0,     0,     0,     0,   231,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,     0,     0,     0,
       0,    67,   232,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    69,     0,    70,    71,   192,     0,     0,     0,
      73,     0,     0,     0,   141,     0,    75,     0,   142,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   136,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   137,
     138,     0,   139,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,     0,    40,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    52,   140,
       0,     0,     0,     0,     0,     0,   231,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,   192,     0,     0,     0,    73,
       0,     0,     0,   141,     0,    75,   549,   142,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   137,   138,
       0,   139,     0,    30,     0,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
       0,    40,     0,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,   231,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   192,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,   551,   142,     5,     6,     7,
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
      71,   192,     0,     0,     0,    73,     0,     0,     0,   141,
     547,    75,     0,   142,     5,     6,     7,     8,     9,    10,
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
     192,     0,     0,     0,    73,     0,     0,     0,   141,     0,
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
       0,     0,     0,    68,     0,    69,     0,    70,    71,     0,
     196,     0,     0,    73,     0,     0,     0,   141,     0,    75,
       0,   142,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   199,    22,     0,     0,     0,
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
       0,     0,    73,     0,     0,     0,    74,     0,    75,     0,
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
      33,    34,     0,    36,     0,    37,    38,   657,     0,     0,
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
       0,     0,     0,   818,     0,     0,     0,     0,     0,   137,
     138,     0,   139,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,     0,    40,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   819,    51,    52,   820,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,   192,     0,     0,     0,    73,
       0,     0,     0,   141,     0,    75,     0,   821,     5,     6,
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
      69,     0,    70,    71,   192,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   821,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     807,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,   192,     0,     0,     0,    73,     0,     0,
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
       0,     0,     0,     0,   506,     0,     0,     0,   141,     0,
      75,     0,   142,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   137,   138,     0,   601,     0,    30,     0,
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
       0,     0,   137,   138,     0,   604,     0,    30,     0,     0,
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
       0,   137,   138,     0,   801,     0,    30,     0,     0,     0,
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
     137,   138,     0,   802,     0,    30,     0,     0,     0,     0,
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
     138,     0,   804,     0,    30,     0,     0,     0,     0,    33,
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
       0,   805,     0,    30,     0,     0,     0,     0,    33,    34,
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
     806,     0,    30,     0,     0,     0,     0,    33,    34,     0,
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
      22,     0,     0,     0,     0,     0,   137,   138,     0,   807,
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
       0,     0,    50,    51,    52,   140,     0,   255,     0,     0,
       0,     0,     0,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,    63,    64,
      65,    66,     0,   137,   138,   256,   139,     0,    30,   257,
       0,     0,     0,    33,    34,    68,    36,    69,    37,    38,
      71,     0,     0,     5,     0,    73,    40,     0,    42,   141,
     683,    75,     0,   142,     0,   684,     0,     0,     0,     0,
      50,    51,    52,   140,     0,     0,     0,     0,   259,     0,
     260,   261,   262,     0,   263,   264,   265,     0,   266,   267,
     268,   269,  -306,   271,   272,   273,   274,     0,   275,     0,
     277,     5,     0,   278,   279,   280,     0,    64,   683,     0,
       0,  -306,     0,   684,     0,  -306,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,     0,     0,     0,   192,
       0,     0,     0,     0,     0,     0,     0,   141,     0,    75,
    -306,   821,   685,   686,   687,   688,   689,   690,     0,     0,
     691,   692,   693,   694,   695,   696,   697,   698,   699,   700,
       0,     0,   701,     0,     0,     0,   702,   703,   704,   705,
     706,   707,   708,   709,   710,   711,   712,     0,   713,     0,
       0,   714,   715,   716,   717,     0,   718,     0,     0,   719,
     685,   686,   687,   688,   689,   690,     0,     0,   691,   692,
     693,   694,   695,   696,   697,   698,   699,   700,     0,   255,
     701,     0,     0,     0,   702,   703,   704,   705,   706,   707,
     708,   709,   710,   711,   712,     0,   713,     0,   253,   714,
     715,   716,   717,   254,   718,     0,     0,     0,     0,     0,
       0,     0,     0,   255,     0,     0,     0,   256,     0,     0,
       0,   257,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   253,     0,     0,   258,     0,   254,     0,     0,
       0,   256,     0,     0,     0,   257,     0,   255,     0,     0,
     259,     0,   260,   261,   262,     0,   263,   264,   265,     0,
     266,   267,   268,   269,     0,   271,   272,   273,   274,   258,
     275,     0,   277,     0,     0,   278,   279,   280,     0,     0,
       0,     0,     0,     0,   259,   256,   260,   261,   262,   257,
     263,   264,   265,     0,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   253,   275,   276,   277,     0,   254,   278,
     279,   280,     0,   258,     0,     0,     0,     0,   255,     0,
     870,     0,     0,     0,     0,     0,     0,     0,   259,     0,
     260,   261,   262,     0,   263,   264,   265,     0,   266,   267,
     268,   269,   270,   271,   272,   273,   274,     0,   275,   276,
     277,     0,     0,   278,   279,   280,   256,     0,     0,     0,
     257,     0,     0,     0,   907,   253,     0,     0,     0,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     255,   331,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,   332,     0,     0,     0,     0,     0,     0,   259,
       0,   260,   261,   262,     0,   263,   264,   265,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   256,   275,
     276,   277,   257,     0,   278,   279,   280,     0,     0,     0,
       0,     0,   253,   843,     0,     0,     0,   254,     0,     0,
       0,     0,   333,     0,     0,     0,   258,   255,   328,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   329,
       0,   259,     0,   260,   261,   262,     0,   263,   264,   265,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
       0,   275,   276,   277,     0,   256,   278,   279,   280,   257,
       0,     0,   304,     0,   253,     0,     0,     0,     0,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   255,
     630,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
     260,   261,   262,     0,   263,   264,   265,     0,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   256,   275,   276,
     277,   257,     0,   278,   279,   280,     0,     0,     0,   304,
       0,   253,     0,     0,     0,     0,   254,     0,     0,     0,
       0,   333,     0,     0,     0,   258,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,   260,   261,   262,     0,   263,   264,   265,     0,
     266,   267,   268,   269,   270,   271,   272,   273,   274,     0,
     275,   276,   277,     0,   256,   278,   279,   280,   257,     0,
       0,   304,     0,   253,     0,   339,     0,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   255,   199,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   260,
     261,   262,     0,   263,   264,   265,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   256,   275,   276,   277,
     257,     0,   278,   279,   280,   253,     0,     0,   304,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     255,   624,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,   260,   261,   262,     0,   263,   264,   265,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   256,   275,
     276,   277,   257,     0,   278,   279,   280,   253,     0,     0,
     304,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   255,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,   260,   261,   262,     0,   263,   264,   265,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     256,   275,   276,   277,   257,     0,   278,   279,   280,   253,
       0,   743,   304,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   255,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,   260,   261,   262,     0,   263,
     264,   265,     0,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   256,   275,   276,   277,   257,     0,   278,   279,
     280,   253,     0,   748,   304,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,     0,   260,   261,   262,
       0,   263,   264,   265,     0,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   256,   275,   276,   277,   257,     0,
     278,   279,   280,   253,     0,   855,   304,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   255,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   260,
     261,   262,     0,   263,   264,   265,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   256,   275,   276,   277,
     257,     0,   278,   279,   280,   253,     0,   856,   304,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     255,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,   260,   261,   262,     0,   263,   264,   265,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   256,   275,
     276,   277,   257,     0,   278,   279,   280,   253,     0,   857,
     304,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   255,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,   260,   261,   262,     0,   263,   264,   265,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     256,   275,   276,   277,   257,     0,   278,   279,   280,   253,
       0,   858,   304,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   255,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,   260,   261,   262,     0,   263,
     264,   265,     0,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   256,   275,   276,   277,   257,     0,   278,   279,
     280,   253,     0,   872,   304,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,     0,   260,   261,   262,
       0,   263,   264,   265,     0,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   256,   275,   276,   277,   257,     0,
     278,   279,   280,   253,     0,   873,   304,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   255,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   260,
     261,   262,     0,   263,   264,   265,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   256,   275,   276,   277,
     257,     0,   278,   279,   280,   253,     0,     0,   304,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     255,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,   260,   261,   262,     0,   263,   264,   265,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   256,   275,
     276,   277,   257,     0,   278,   279,   280,     0,     0,     0,
     363,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   253,     0,     0,     0,   258,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   255,     0,     0,
       0,   259,   336,   260,   261,   262,     0,   263,   264,   265,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
       0,   275,   276,   277,   337,     0,   278,   279,   280,     0,
       0,  -390,     0,     0,     0,   256,     0,     0,     0,   257,
       0,     0,     0,     0,   253,     0,     0,     0,     0,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   255,
       0,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,   310,     0,     0,     0,     0,     0,     0,   259,     0,
     260,   261,   262,     0,   263,   264,   265,     0,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   256,   275,   276,
     277,   257,   338,   278,   279,   280,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   253,     0,     0,
       0,   130,   254,     0,     0,   258,     0,     0,     0,     0,
       0,     0,   255,     0,     0,     0,     0,   377,     0,     0,
     259,     0,   260,   261,   262,     0,   263,   264,   265,     0,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   378,
     275,   276,   277,     0,     0,   278,   279,   280,     0,     0,
     256,     0,     0,     0,   257,     0,     0,     0,     0,   253,
       0,     0,     0,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   255,   474,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,   475,     0,     0,     0,
       0,     0,     0,   259,     0,   260,   261,   262,     0,   263,
     264,   265,     0,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   256,   275,   276,   277,   257,     0,   278,   279,
     280,   253,     0,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,   476,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,   477,     0,
       0,     0,     0,     0,     0,   259,     0,   260,   261,   262,
       0,   263,   264,   265,     0,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   256,   275,   276,   277,   257,     0,
     278,   279,   280,   253,     0,     0,     0,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   255,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   260,
     261,   262,     0,   263,   264,   265,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   256,   275,   276,   277,
     257,     0,   278,   279,   280,   253,     0,     0,     0,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     255,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,   260,   261,   262,     0,   263,   264,   265,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   256,   275,
     276,   277,   257,   314,   278,   279,   280,   253,     0,     0,
       0,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   255,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,   260,   261,   262,     0,   263,   264,   265,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     256,   275,   276,   277,   257,   362,   278,   279,   280,   253,
       0,     0,     0,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   255,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,   260,   261,   262,     0,   263,
     264,   265,     0,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   256,   275,   276,   277,   257,   368,   278,   279,
     280,   253,     0,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,     0,   260,   261,   262,
       0,   263,   264,   265,     0,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   256,   275,   276,   277,   257,   370,
     278,   279,   280,   253,     0,     0,     0,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   255,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   260,
     261,   262,     0,   263,   264,   265,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   256,   275,   276,   277,
     257,   372,   278,   279,   280,   253,     0,     0,     0,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     255,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,   260,   261,   262,     0,   263,   264,   265,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   256,   275,
     276,   277,   257,   380,   278,   279,   280,   253,     0,     0,
       0,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   255,     0,     0,   394,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,   260,   261,   262,     0,   263,   264,   265,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     256,   275,   276,   277,   257,     0,   278,   279,   280,   253,
       0,   478,     0,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   255,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,   260,   261,   262,     0,   263,
     264,   265,     0,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   256,   275,   276,   277,   257,     0,   278,   279,
     280,   253,     0,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,     0,   260,   261,   262,
       0,   263,   264,   265,     0,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   256,   275,   276,   277,   257,   576,
     278,   279,   280,   253,     0,     0,     0,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   255,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   260,
     261,   262,     0,   263,   264,   265,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   256,   275,   276,   277,
     257,   577,   278,   279,   280,   253,     0,     0,     0,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     255,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,   260,   261,   262,     0,   263,   264,   265,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   256,   275,
     276,   277,   257,   578,   278,   279,   280,   253,     0,     0,
       0,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   255,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,   260,   261,   262,     0,   263,   264,   265,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     256,   275,   276,   277,   257,   579,   278,   279,   280,   253,
       0,     0,     0,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   255,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,   260,   261,   262,     0,   263,
     264,   265,     0,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   256,   275,   276,   277,   257,   580,   278,   279,
     280,   253,     0,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,     0,   260,   261,   262,
       0,   263,   264,   265,     0,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   256,   275,   276,   277,   257,   581,
     278,   279,   280,   253,     0,     0,     0,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   255,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   260,
     261,   262,     0,   263,   264,   265,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   256,   275,   276,   277,
     257,   582,   278,   279,   280,   253,     0,     0,     0,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     255,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,   260,   261,   262,     0,   263,   264,   265,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   256,   275,
     276,   277,   257,   583,   278,   279,   280,   253,     0,     0,
       0,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   255,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,   260,   261,   262,     0,   263,   264,   265,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     256,   275,   276,   277,   257,   584,   278,   279,   280,   253,
       0,     0,     0,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   255,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,   260,   261,   262,     0,   263,
     264,   265,     0,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   256,   275,   276,   277,   257,   585,   278,   279,
     280,   253,     0,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,     0,   260,   261,   262,
       0,   263,   264,   265,     0,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   256,   275,   276,   277,   257,   586,
     278,   279,   280,   253,     0,     0,     0,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   255,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   260,
     261,   262,     0,   263,   264,   265,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   256,   275,   276,   277,
     257,   587,   278,   279,   280,   253,     0,     0,     0,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     255,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,   260,   261,   262,     0,   263,   264,   265,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   256,   275,
     276,   277,   257,   588,   278,   279,   280,   253,     0,     0,
       0,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   255,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,   260,   261,   262,     0,   263,   264,   265,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     256,   275,   276,   277,   257,   589,   278,   279,   280,   253,
       0,     0,     0,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   255,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,   260,   261,   262,     0,   263,
     264,   265,     0,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   256,   275,   276,   277,   257,   590,   278,   279,
     280,   253,     0,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,     0,   260,   261,   262,
       0,   263,   264,   265,     0,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   256,   275,   276,   277,   257,   591,
     278,   279,   280,     0,     0,     0,     0,     0,   253,     0,
       0,     0,     0,   254,     0,     0,     0,     0,   130,     0,
       0,     0,   258,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   260,
     261,   262,     0,   263,   264,   265,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,     0,   275,   276,   277,
       0,   256,   278,   279,   280,   257,     0,     0,     0,     0,
     253,   637,     0,     0,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   255,     0,     0,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,   260,   261,   262,     0,
     263,   264,   265,     0,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   256,   275,   276,   277,   257,   611,   278,
     279,   280,   253,     0,     0,     0,     0,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   255,     0,     0,
     640,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,   260,   261,
     262,     0,   263,   264,   265,     0,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   256,   275,   276,   277,   257,
       0,   278,   279,   280,   253,     0,     0,     0,     0,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   255,
       0,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
     260,   261,   262,     0,   263,   264,   265,     0,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   256,   275,   276,
     277,   257,     0,   278,   279,   280,   253,     0,     0,     0,
       0,   254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   255,   731,     0,   677,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,   260,   261,   262,     0,   263,   264,   265,     0,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   256,
     275,   276,   277,   257,     0,   278,   279,   280,   253,     0,
       0,     0,     0,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   255,   734,     0,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,     0,   260,   261,   262,     0,   263,   264,
     265,     0,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   256,   275,   276,   277,   257,     0,   278,   279,   280,
     253,     0,     0,     0,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   255,     0,     0,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,   260,   261,   262,     0,
     263,   264,   265,     0,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   256,   275,   276,   277,   257,     0,   278,
     279,   280,   253,     0,   800,     0,     0,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   255,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,   260,   261,
     262,     0,   263,   264,   265,     0,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   256,   275,   276,   277,   257,
       0,   278,   279,   280,   253,   816,   803,     0,     0,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   255,
       0,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
     260,   261,   262,     0,   263,   264,   265,     0,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   256,   275,   276,
     277,   257,     0,   278,   279,   280,   253,     0,     0,     0,
       0,   254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   255,     0,     0,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,   260,   261,   262,     0,   263,   264,   265,     0,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   256,
     275,   276,   277,   257,     0,   278,   279,   280,   253,     0,
     879,     0,     0,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   255,     0,     0,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,     0,   260,   261,   262,     0,   263,   264,
     265,     0,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   256,   275,   276,   277,   257,     0,   278,   279,   280,
     253,     0,   880,     0,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   255,     0,     0,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,   260,   261,   262,     0,
     263,   264,   265,     0,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   256,   275,   276,   277,   257,     0,   278,
     279,   280,   253,     0,   881,     0,     0,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   255,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,   260,   261,
     262,     0,   263,   264,   265,     0,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   256,   275,   276,   277,   257,
       0,   278,   279,   280,   253,     0,   882,     0,     0,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   255,
       0,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
     260,   261,   262,     0,   263,   264,   265,     0,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   256,   275,   276,
     277,   257,     0,   278,   279,   280,   253,     0,   883,     0,
       0,   254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   255,     0,     0,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,   260,   261,   262,     0,   263,   264,   265,     0,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   256,
     275,   276,   277,   257,     0,   278,   279,   280,   253,     0,
     884,     0,     0,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   255,     0,     0,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,     0,   260,   261,   262,     0,   263,   264,
     265,     0,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   256,   275,   276,   277,   257,     0,   278,   279,   280,
     253,     0,     0,     0,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   255,     0,     0,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,   260,   261,   262,     0,
     263,   264,   265,     0,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   599,   275,   276,   277,   257,     0,   278,
     279,   280,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,   260,   261,
     262,   256,   263,   264,   265,   257,   266,   267,   268,   269,
     270,   271,   272,   273,   274,     0,   275,   276,   277,     0,
       0,   278,   279,   280,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,   260,   261,   262,     0,
     263,   264,   265,     0,     0,   267,   268,   269,     0,   271,
     272,   273,   274,     0,     0,     0,   277,     0,     0,   278,
     279,   280
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-768))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,    43,    44,   343,     2,    16,    15,   301,   654,    20,
     152,   346,   351,    26,   373,    26,    27,    28,    29,    24,
     680,   163,   523,     3,     3,    36,    37,     3,    39,    40,
     247,   248,   249,   250,   373,    46,    47,    48,    49,    50,
     237,    66,    53,    54,    55,    56,   175,   504,    59,    60,
      61,    66,     1,    64,    26,    87,    67,    68,    69,   188,
      71,    46,   829,    74,    75,    76,     1,     1,     3,     4,
       5,     6,     7,     8,    59,     3,   205,    12,    26,     3,
      37,    37,     1,    41,    64,   723,   130,   111,    37,   111,
      26,    76,    27,   337,   680,   139,    92,   141,    33,    34,
      75,    36,    60,    38,    80,   130,   130,     0,    43,    44,
     142,    46,   137,    48,    49,    87,    26,   111,   764,   115,
     142,    56,   137,    58,   137,   136,   137,   138,   139,   140,
     141,   142,    66,   900,   378,    70,    71,    72,    73,   148,
     597,    90,    76,    92,   138,   861,   157,    66,   808,   650,
      87,    85,    17,     3,   111,   111,    91,   142,   137,   139,
     139,   110,   111,   139,   175,   137,    85,   896,   297,   298,
     111,   106,   107,   108,   109,   111,   110,   188,   907,   114,
     896,   130,   157,    79,   189,   402,   142,   522,   123,   137,
     125,   110,   127,   128,   205,    21,   130,    62,   133,   140,
     175,   137,   137,   137,   139,   791,   141,   845,   846,    53,
     504,   139,    37,   188,   860,   139,    81,   111,   137,    13,
       7,   232,   808,   139,    18,   110,   139,    37,   113,    55,
     205,   229,   117,    37,    37,   139,   130,   366,    64,    64,
      84,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,    79,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   111,   793,     3,   138,     3,     3,   111,   755,   310,
       7,   111,   642,   597,   315,   502,   503,   111,   111,   644,
     130,    21,    72,    37,   653,   326,   111,   328,   329,    27,
     331,   332,   297,   298,   138,   336,   337,    37,   339,    47,
      40,     2,   142,   111,    41,   130,   111,   139,   142,   142,
      11,   326,     1,   354,    54,    55,   256,   257,    19,   360,
      21,    92,   136,    60,    64,   366,   553,   554,    21,   139,
     130,   141,   140,    87,    35,   140,   377,   378,   111,    79,
     111,    79,   383,   130,   115,   386,   111,   388,    37,   390,
     391,   392,    53,   394,   395,   360,   111,   111,   399,   299,
     300,   366,    55,   139,     3,   141,   130,   140,     7,   140,
     310,    64,   544,    74,    21,   140,   111,    78,   383,   111,
     769,   386,    21,   388,   399,   140,   391,   776,   142,   329,
      83,   111,   332,   139,   763,     2,   111,   111,   130,   139,
     769,   130,   130,    92,    11,   140,     3,   776,    55,    63,
       7,   130,    19,    67,    21,   130,    55,    64,   130,    63,
     140,   110,   111,    67,    25,    64,   140,   797,    35,   137,
     131,   755,    79,   474,   475,   476,   477,   478,   479,   480,
      79,   130,   481,   482,    83,   486,    53,   488,    55,    56,
     390,   111,   139,   494,   141,   496,   130,    20,   111,   500,
     130,    25,    63,   504,    55,    56,    67,    74,    31,    32,
     130,    78,   167,   168,   169,   176,    39,   130,    41,   137,
      25,   486,    86,   488,   525,   113,    92,   528,   529,   386,
     185,   388,   861,   139,    92,   130,   139,    60,   199,    63,
     541,   870,   139,    67,     7,   106,   130,   202,   109,   110,
     525,   112,   113,   111,   110,    92,   117,   115,    63,   140,
     130,   111,    67,   124,   131,   220,   541,   896,   229,    37,
     131,   132,   133,    17,   139,   475,    20,   477,   907,    28,
     140,   111,   140,    90,    92,    29,   110,    31,    32,   113,
      16,    35,   140,   117,   140,    39,   597,    41,   599,   600,
     601,   140,   111,   604,   142,   110,   137,   112,   113,   176,
     137,   140,   117,    57,   615,    59,    60,    61,    62,   124,
     142,   622,    92,   624,    37,   111,   627,   130,   133,   630,
     130,    21,   199,   130,   635,    79,   637,    81,   140,   640,
       3,    28,   140,   304,     7,   130,     9,    37,   130,   111,
      40,    26,    64,    66,    26,   142,   311,   137,    21,   660,
     110,    64,   229,    76,    54,    55,    37,   328,   110,   142,
     331,   672,    85,   674,    64,   142,   677,   113,   339,    92,
     335,   113,   139,    85,   140,   660,   130,   140,    76,    79,
     562,   679,    55,   378,   138,    66,   377,   213,   111,   599,
     600,    64,   493,   664,   164,    76,   741,   796,   616,   796,
     642,   797,   900,   860,    85,   682,    79,   130,   719,   875,
      83,    92,    28,    37,   137,   817,   355,   488,    -1,   730,
     731,   392,   733,   734,   735,   736,    -1,   304,    -1,    -1,
     111,    -1,   743,   744,    -1,   746,    -1,   748,   749,    -1,
      -1,   752,    66,    -1,   755,    -1,   788,   789,    -1,   130,
       2,   328,    76,    -1,   331,    -1,   137,   768,   769,    11,
      -1,    85,   339,   774,    -1,   776,   777,    19,    92,    21,
     781,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,   111,   763,   800,
     801,   802,   803,   804,   805,   806,   807,    -1,   469,    -1,
      20,    53,    -1,    -1,    -1,   816,   130,    -1,   819,   820,
     821,    31,    32,   137,   825,   392,    -1,   828,    -1,    39,
      -1,    41,    74,    -1,    -1,    -1,    78,    47,    -1,    -1,
      -1,    -1,     2,   498,   499,    -1,   821,   502,   503,    -1,
      60,    11,    -1,   514,   855,   856,   857,   858,    -1,    19,
     825,    21,    -1,    -1,    -1,    20,    -1,    -1,    -1,   870,
      -1,   872,   873,    -1,    -1,    35,    31,    32,   879,   880,
     881,   882,   883,   884,    39,    -1,    41,    -1,    -1,   131,
      -1,    -1,    47,    53,   895,   896,   861,   558,    -1,    -1,
      -1,   562,    -1,    -1,    -1,    60,   907,    -1,    -1,   570,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   896,     2,    -1,   176,    -1,    -1,    -1,   599,   600,
      -1,    11,   907,    -1,    -1,    -1,    -1,   514,    -1,    19,
      -1,    21,    -1,    -1,   609,   610,    -1,   199,    -1,    -1,
      -1,   622,    -1,   624,   619,    35,   627,    -1,    -1,   630,
      -1,   131,    -1,    -1,   629,    -1,    -1,   632,   639,    -1,
      -1,    -1,    -1,    53,    -1,    -1,    -1,   229,    -1,    -1,
      -1,   558,    -1,    -1,    -1,   562,    -1,    -1,   659,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    17,
      -1,   672,    20,   674,    -1,    -1,   176,    -1,   679,    -1,
      -1,    29,    -1,    31,    32,    -1,    -1,    35,    25,    -1,
      -1,    39,    -1,    41,    -1,    -1,    -1,    -1,    -1,   199,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    60,    61,    62,   622,    -1,   624,    -1,    -1,
     627,   131,   304,   630,    -1,    -1,    63,    -1,    -1,   229,
      67,    79,   639,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     2,   743,    -1,    -1,    -1,   328,   748,    -1,   331,
      11,    -1,   659,    -1,    -1,    -1,    -1,   339,    19,    -1,
      21,    -1,    -1,    -1,    -1,   672,   176,   674,    -1,   770,
      -1,    -1,   679,   110,    35,   112,   113,   772,    -1,   780,
     117,    -1,   130,   784,    -1,    -1,    -1,   124,    -1,   199,
     138,    -1,    53,    -1,   131,   132,   133,    -1,    -1,    -1,
      17,    -1,    -1,    20,   304,    -1,    -1,    -1,    -1,    -1,
     392,    -1,    29,    74,    31,    32,    -1,    78,    35,   229,
      -1,    -1,    39,    -1,    41,    -1,    -1,    -1,   328,    -1,
      -1,   331,    -1,    -1,    -1,    -1,   743,    -1,    -1,   339,
      57,   748,    59,    60,    61,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   855,   856,   857,   858,    -1,    -1,
      -1,    -1,    79,   770,    81,    -1,    -1,    -1,    -1,    -1,
     131,   872,   873,   780,    -1,    -1,    -1,   784,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,   392,    15,   304,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,    -1,   176,    -1,    -1,   328,    -1,
      -1,   331,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,
      -1,    -1,   514,    -1,    -1,    -1,    -1,    -1,   199,    -1,
      -1,    63,    -1,    -1,    -1,    67,    -1,    -1,   855,   856,
     857,   858,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   872,   873,    -1,   229,    91,
      -1,    -1,    -1,    -1,    -1,    -1,   558,    -1,    -1,    -1,
     562,    -1,   392,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,     2,   126,   127,   128,    -1,    -1,   131,
     132,   133,    11,    -1,   514,    -1,    -1,    -1,   140,    -1,
      19,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
     622,    -1,   624,   304,    -1,   627,    -1,    -1,   630,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    -1,   639,   558,    -1,
      -1,    -1,   562,    -1,    -1,    -1,    -1,   328,    -1,    -1,
     331,    -1,    -1,    -1,    -1,    74,    -1,   659,   339,    78,
      -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,
     672,    -1,   674,    -1,    -1,    11,    -1,   679,    -1,    -1,
      -1,    -1,    25,    19,   514,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,   622,    -1,   624,    -1,    -1,   627,    -1,    -1,
     630,   392,   131,    -1,    -1,    -1,    -1,    53,    -1,   639,
      63,    -1,    -1,    -1,    67,    -1,    -1,    -1,   558,    -1,
      -1,    -1,   562,    -1,    -1,    -1,    -1,    -1,    74,   659,
      -1,   743,    78,    -1,    -1,    -1,   748,    -1,    -1,    -1,
      -1,    -1,   672,    -1,   674,    -1,    -1,   176,    -1,   679,
      -1,    -1,    -1,   106,    -1,   108,   109,   110,   770,   112,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,   780,    -1,
     199,   124,   784,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,    -1,   622,    -1,   624,   131,    -1,   627,    -1,    25,
     630,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,   639,
     229,    -1,    -1,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   743,    -1,    -1,    -1,    -1,   748,   659,
      -1,    -1,    -1,   514,    -1,    -1,    -1,    63,    -1,    -1,
     176,    67,   672,    -1,   674,    -1,    -1,    -1,    -1,   679,
     770,    -1,    -1,   855,   856,   857,   858,    -1,    -1,    -1,
     780,    -1,    -1,   199,   784,    -1,    -1,    -1,    -1,    -1,
     872,   873,    -1,    -1,    -1,    -1,    -1,   558,    -1,    -1,
     106,   562,   108,   109,   110,   304,   112,   113,   114,    -1,
      -1,   117,    -1,   229,   135,    -1,    -1,   123,   124,    -1,
      -1,    -1,   128,    -1,    -1,   131,   132,   133,    -1,   328,
      -1,    -1,   331,   743,    -1,    -1,    -1,    -1,   748,    -1,
     339,    -1,    -1,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    25,   174,    -1,   855,   856,   857,   858,    -1,
     770,   622,    -1,   624,   185,    -1,   627,    -1,    -1,   630,
     780,    -1,   872,   873,   784,    -1,    -1,    -1,   639,    -1,
      -1,   202,    -1,    -1,    -1,    -1,    -1,    -1,   304,    -1,
      63,    -1,    25,   392,    67,    -1,    -1,    -1,   659,   220,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   672,   328,   674,    -1,   331,    -1,    -1,   679,    -1,
      -1,    -1,    -1,   339,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,   106,    67,   108,   109,   110,    -1,   112,
     113,    -1,    -1,    -1,   117,   855,   856,   857,   858,    -1,
     123,   124,    -1,    -1,    -1,   128,    -1,    -1,   131,   132,
     133,    -1,   872,   873,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,   392,   110,    -1,   112,
     113,    -1,   743,    -1,   117,    -1,    -1,   748,    -1,    -1,
     311,   124,    -1,     2,    -1,    -1,    -1,    -1,   131,   132,
     133,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,   770,
      19,    -1,    21,    -1,   335,   514,    -1,    -1,    -1,   780,
      -1,    -1,    -1,   784,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,   558,
      -1,    -1,    -1,   562,    -1,    74,    -1,    -1,    -1,    78,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,   855,   856,   857,   858,   514,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   872,   873,    -1,    70,    71,    72,    73,    -1,    -1,
      -1,    -1,   131,   622,    -1,   624,    -1,    -1,   627,    -1,
      -1,   630,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     639,    -1,   558,    -1,    -1,    -1,   562,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     659,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,   125,
      -1,    -1,    -1,   672,    -1,   674,    -1,   498,   499,    -1,
     679,   502,   503,   139,    -1,   141,    13,    -1,    -1,    -1,
     199,    18,    -1,    -1,    -1,    -1,    23,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    32,   622,    34,   624,    -1,
      -1,   627,    -1,    -1,   630,    -1,    -1,    -1,   539,    -1,
     229,    -1,    -1,   639,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   659,   743,    -1,    -1,    -1,    -1,   748,
      -1,    -1,    -1,    -1,    -1,    -1,   672,    -1,   674,    -1,
      -1,    -1,    -1,   679,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   770,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   780,    -1,    -1,    -1,   784,    10,    -1,   609,   610,
      -1,    15,    -1,    -1,    -1,   304,    -1,    -1,   619,    -1,
     621,    25,    -1,    -1,    -1,   626,    -1,    -1,   629,    -1,
      -1,   632,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   328,
      -1,    -1,   331,    -1,    -1,   152,    -1,   743,    -1,    -1,
     157,    -1,   748,    -1,    -1,    -1,   163,    -1,   165,    63,
      -1,    -1,    -1,    67,    -1,    -1,   667,    -1,   175,    -1,
      -1,   178,    -1,    -1,   770,    -1,   855,   856,   857,   858,
      -1,   188,   189,    87,   780,   192,    -1,    91,   784,    -1,
      -1,    -1,    -1,   872,   873,    -1,    -1,    -1,   205,    -1,
      -1,    -1,   106,   392,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,   126,   127,   128,    -1,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,   242,    -1,    -1,   142,    -1,
     247,   248,   249,   250,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   264,    -1,   855,
     856,   857,   858,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   772,    -1,    -1,    -1,    -1,   872,   873,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     297,   298,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   810,
     811,    -1,   813,   814,    -1,   322,    -1,   324,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   514,    -1,    -1,   829,    -1,
      -1,    -1,    -1,    -1,   835,    -1,   837,    -1,   839,   346,
      -1,     3,     4,     5,     6,     7,     8,    -1,   355,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   366,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,   558,
      -1,    33,    34,   562,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,   402,    58,    -1,    -1,   900,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    91,
      -1,    -1,    -1,   622,    -1,   624,    -1,    -1,   627,    -1,
      -1,   630,    -1,    10,   106,   107,   108,   109,    15,    -1,
     639,    -1,   114,    -1,    -1,   134,   135,   136,    25,    -1,
     139,   123,   141,   125,    -1,   127,   128,   129,    -1,    -1,
     659,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,   141,
     142,   488,    -1,   672,    -1,   674,    -1,    -1,    -1,    -1,
     679,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   518,    -1,    -1,    -1,   522,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   539,    -1,    -1,    -1,    -1,   544,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    -1,   126,
     127,   128,    -1,   570,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,   770,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   780,    -1,    -1,    -1,   784,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,     1,   616,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    26,    27,    -1,    29,   644,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,   664,    51,    -1,
      -1,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      25,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    -1,    75,    -1,    77,    78,    79,    -1,    81,    82,
      83,   698,    -1,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    67,   106,   107,   108,   109,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,   125,    -1,   127,   128,    91,   130,    -1,    -1,
     133,    -1,    -1,    -1,   137,    -1,   139,    -1,   141,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      -1,   126,   127,   128,    -1,    -1,   131,   132,   133,     1,
     787,     3,     4,     5,     6,     7,     8,    -1,    10,   796,
      12,    13,    14,    15,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    75,    -1,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    -1,    88,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,    -1,    -1,    -1,   137,   138,   139,     1,   141,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    26,    27,    -1,    29,    -1,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    41,    42,
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
      71,    72,    73,    -1,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,
      -1,    -1,    -1,   114,    -1,    -1,   134,   135,    -1,    -1,
      -1,   139,   123,   141,   125,    -1,   127,   128,    -1,    -1,
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
     129,    -1,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,
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
     130,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,   139,
      -1,   141,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
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
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    73,    -1,    25,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,   106,   107,
     108,   109,    -1,    33,    34,    63,    36,    -1,    38,    67,
      -1,    -1,    -1,    43,    44,   123,    46,   125,    48,    49,
     128,    -1,    -1,     3,    -1,   133,    56,    -1,    58,   137,
      10,   139,    -1,   141,    -1,    15,    -1,    -1,    -1,    -1,
      70,    71,    72,    73,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,    92,   121,   122,   123,   124,    -1,   126,    -1,
     128,     3,    -1,   131,   132,   133,    -1,   107,    10,    -1,
      -1,   111,    -1,    15,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,   139,
     140,   141,    92,    93,    94,    95,    96,    97,    -1,    -1,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
      -1,    -1,   112,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,    -1,   128,    -1,
      -1,   131,   132,   133,   134,    -1,   136,    -1,    -1,   139,
      92,    93,    94,    95,    96,    97,    -1,    -1,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,    -1,    25,
     112,    -1,    -1,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,    -1,   128,    -1,    10,   131,
     132,   133,   134,    15,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    91,    -1,    15,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    67,    -1,    25,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    91,
     126,    -1,   128,    -1,    -1,   131,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    63,   108,   109,   110,    67,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    10,   126,   127,   128,    -1,    15,   131,
     132,   133,    -1,    91,    -1,    -1,    -1,    -1,    25,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    -1,   126,   127,
     128,    -1,    -1,   131,   132,   133,    63,    -1,    -1,    -1,
      67,    -1,    -1,    -1,   142,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,    -1,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    10,   140,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    91,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,   126,   127,   128,    -1,    63,   131,   132,   133,    67,
      -1,    -1,   137,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,    -1,   131,   132,   133,    -1,    -1,    -1,   137,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    -1,
     126,   127,   128,    -1,    63,   131,   132,   133,    67,    -1,
      -1,   137,    -1,    10,    -1,    74,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,    -1,   131,   132,   133,    10,    -1,    -1,   137,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,    -1,   131,   132,   133,    10,    -1,    -1,
     137,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,    -1,   131,   132,   133,    10,
      -1,    74,   137,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    63,   126,   127,   128,    67,    -1,   131,   132,
     133,    10,    -1,    74,   137,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    63,   126,   127,   128,    67,    -1,
     131,   132,   133,    10,    -1,    74,   137,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,    -1,   131,   132,   133,    10,    -1,    74,   137,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,    -1,   131,   132,   133,    10,    -1,    74,
     137,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,    -1,   131,   132,   133,    10,
      -1,    74,   137,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    63,   126,   127,   128,    67,    -1,   131,   132,
     133,    10,    -1,    74,   137,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    63,   126,   127,   128,    67,    -1,
     131,   132,   133,    10,    -1,    74,   137,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,    -1,   131,   132,   133,    10,    -1,    -1,   137,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,    -1,   131,   132,   133,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    91,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,   106,    30,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,   126,   127,   128,    52,    -1,   131,   132,   133,    -1,
      -1,   136,    -1,    -1,    -1,    63,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    87,    15,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    52,
     126,   127,   128,    -1,    -1,   131,   132,   133,    -1,    -1,
      63,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    63,   126,   127,   128,    67,    -1,   131,   132,
     133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    63,   126,   127,   128,    67,    -1,
     131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,    -1,   131,   132,   133,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,   130,   131,   132,   133,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,   130,   131,   132,   133,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    63,   126,   127,   128,    67,   130,   131,   132,
     133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    63,   126,   127,   128,    67,   130,
     131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,   130,   131,   132,   133,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,   130,   131,   132,   133,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    90,    91,    -1,    -1,    -1,
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
     133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    63,   126,   127,   128,    67,   130,
     131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,   130,   131,   132,   133,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,   130,   131,   132,   133,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,   130,   131,   132,   133,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    63,   126,   127,   128,    67,   130,   131,   132,
     133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    63,   126,   127,   128,    67,   130,
     131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,   130,   131,   132,   133,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,   130,   131,   132,   133,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,   130,   131,   132,   133,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    63,   126,   127,   128,    67,   130,   131,   132,
     133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    63,   126,   127,   128,    67,   130,
     131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,   130,   131,   132,   133,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,   130,   131,   132,   133,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,   130,   131,   132,   133,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    63,   126,   127,   128,    67,   130,   131,   132,
     133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    63,   126,   127,   128,    67,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    -1,   126,   127,   128,
      -1,    63,   131,   132,   133,    67,    -1,    -1,    -1,    -1,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    63,   126,   127,   128,    67,   130,   131,
     132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      28,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    63,   126,   127,   128,    67,
      -1,   131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,    -1,   131,   132,   133,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    90,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,    -1,   131,   132,   133,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,    -1,   131,   132,   133,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,
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
      -1,   131,   132,   133,    10,    11,    74,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,    -1,   131,   132,   133,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,    -1,   131,   132,   133,    10,    -1,
      74,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,    -1,   131,   132,   133,
      10,    -1,    74,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,
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
      -1,   131,   132,   133,    10,    -1,    74,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,    -1,   131,   132,   133,    10,    -1,    74,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,    -1,   131,   132,   133,    10,    -1,
      74,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,    -1,   131,   132,   133,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    63,   126,   127,   128,    67,    -1,   131,
     132,   133,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,
     110,    63,   112,   113,   114,    67,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    -1,   126,   127,   128,    -1,
      -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,    -1,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,   128,    -1,    -1,   131,
     132,   133
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
     222,   223,   151,   254,   254,   254,   254,     7,   139,    32,
     168,   168,   129,   213,   233,   254,   130,   254,   254,    26,
     154,   165,   254,   254,    72,   139,   151,   254,   254,   154,
     161,   254,   255,   154,   161,   254,   163,   218,   233,   254,
     254,   254,   254,   254,   254,   254,   254,   138,   149,   155,
     233,    80,   115,   213,   234,   235,   254,   233,   254,   261,
      57,   151,    47,   163,    41,    60,   208,    21,    55,    64,
      83,   130,   136,    10,    15,    25,    63,    67,    91,   106,
     108,   109,   110,   112,   113,   114,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   126,   127,   128,   131,   132,
     133,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   134,   135,   139,   141,    63,
      67,   139,   151,   130,   137,   154,   254,   254,   254,   233,
      37,   245,   218,   130,   130,    86,   163,   213,   236,   237,
     238,   254,   137,   218,   184,   163,   139,   165,    26,    37,
     165,    26,    37,    87,   165,   248,    30,    52,   130,    74,
     154,   236,   151,   139,   202,    80,   139,   163,   224,   225,
       1,   110,   227,   228,    37,   111,   165,   236,   164,   163,
     111,   130,   130,   137,   151,   165,   139,   236,   130,   176,
     130,   176,   130,    92,   219,   130,   130,    30,    52,   165,
     130,   138,   149,   111,   138,   254,   111,   140,   111,   140,
      37,   111,   142,   248,    90,   111,   142,     7,   163,   110,
     185,   195,    64,   222,   222,   222,   222,   254,   254,   254,
     254,   172,   254,   172,   254,   254,   254,   254,   254,   254,
     254,    27,    79,   163,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   236,   236,   172,   254,
     172,   254,    21,    37,    40,    54,    55,    64,    79,   206,
     247,   250,   254,   266,    26,    37,    26,    37,    74,    37,
     142,   172,   254,   165,   130,   254,    92,   140,   111,   163,
     188,   189,   130,   163,    37,   233,    36,   254,   172,   254,
      36,   254,   172,   254,   139,   165,   133,   156,   158,   254,
     156,   157,   151,   254,    28,   140,   203,   204,   205,   206,
     192,   225,   111,   140,     1,   141,   229,   240,    90,    92,
     226,   254,   223,   140,   213,   254,   181,   236,   140,    16,
     177,   141,   229,   240,   111,   158,   157,   138,   140,   140,
     234,   140,   234,   172,   254,   142,   151,   254,   142,   254,
     142,   254,   137,   233,   137,    21,    55,    64,    79,   197,
     207,   222,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   140,   142,    37,    64,   239,   111,   140,    63,
      63,    36,   172,   254,    36,   172,   254,   172,   254,   245,
     245,   130,   213,   254,   238,    92,   111,   138,   185,   254,
     140,   254,    26,   165,    26,   165,   254,    26,   165,   248,
      26,   165,   248,   249,   250,   111,   130,    11,   130,    28,
      28,   151,   111,   140,   139,   163,    21,    55,    64,    79,
     209,   140,   225,   110,   228,   233,   254,    50,   254,    53,
      84,   138,   182,   140,   139,   154,   163,   178,   233,   218,
     130,   130,   142,   248,   142,   248,   151,    90,   138,   155,
     186,    64,   196,    10,    15,    92,    93,    94,    95,    96,
      97,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   112,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   128,   131,   132,   133,   134,   136,   139,
     163,   198,   199,   200,   239,   130,   250,   239,   239,   254,
      26,    26,   254,    26,    26,   142,   142,   165,   165,   254,
     189,   137,   165,    74,    36,   254,    36,   254,    74,    36,
     254,   165,    36,   254,   165,   111,   140,   254,   254,   151,
     254,   204,   225,   110,   232,    64,   228,   226,    37,   142,
      26,   151,   233,   178,   110,   154,   142,    36,   151,   254,
     142,    36,   151,   254,   142,   254,   138,    32,    59,    61,
     138,   150,   167,   209,   163,   254,   113,   139,   201,   201,
      74,    36,    36,    74,    36,    36,    36,    36,   186,   254,
     254,   254,   254,   254,   254,   250,    11,   140,    27,    70,
      73,   141,   213,   231,   240,   115,   215,   226,    85,   216,
     254,   229,   240,   151,   165,   140,   254,   254,   151,   254,
     151,   167,   228,   140,   163,   199,   200,   203,   254,   254,
     254,   254,   254,   254,   138,    74,    74,    74,    74,   254,
     232,   142,   233,   213,   214,   254,   254,   154,   166,   212,
     142,   154,    74,    74,    76,   210,   201,   201,   140,    74,
      74,    74,    74,    74,    74,   254,   254,   254,   254,   215,
     226,   213,   230,   231,   240,    37,   142,   240,   254,   254,
     216,   254,   230,   231,   130,   211,   212,   142,   230
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
#line 437 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 442 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 443 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 450 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 455 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 456 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 475 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 476 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 477 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 478 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 479 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 483 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 489 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 494 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), currentModuleType, new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), currentModuleType, (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 518 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 523 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 524 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 529 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 539 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 543 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 550 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 559 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 568 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 569 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 570 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 589 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 590 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 591 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 592 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 593 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 597 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 607 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 611 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 617 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 621 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 623 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 624 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 638 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 645 "chapel.ypp"
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

  case 99:

/* Line 1806 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 660 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 661 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 662 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 663 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 664 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 665 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 666 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 667 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 668 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 669 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 670 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pForallIntents),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 671 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), zipToTuple((yyvsp[(4) - (5)].pcallexpr)), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pForallIntents),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 673 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pForallIntents),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 676 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 682 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pForallIntents),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 688 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 694 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (7)].pcallexpr)), (yyvsp[(5) - (7)].pForallIntents),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 700 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 707 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pForallIntents),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pForallIntents),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 716 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 720 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 721 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 722 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 723 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 731 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 780 "chapel.ypp"
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

  case 148:

/* Line 1806 of yacc.c  */
#line 790 "chapel.ypp"
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

  case 149:

/* Line 1806 of yacc.c  */
#line 802 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 807 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 812 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 820 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 821 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 826 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 828 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 830 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 835 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 845 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[(1) - (1)].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[(1) - (1)].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[(1) - (1)].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 854 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 858 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 866 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 871 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 876 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 881 "chapel.ypp"
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

  case 166:

/* Line 1806 of yacc.c  */
#line 901 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 907 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 914 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 922 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 934 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 940 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 947 "chapel.ypp"
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

  case 173:

/* Line 1806 of yacc.c  */
#line 969 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 973 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 978 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 982 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 990 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 995 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 996 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 997 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 998 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 999 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 1000 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 1001 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 1002 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 1003 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 1004 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 1005 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 1006 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 1007 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 1008 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 1010 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 1011 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 1012 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1039 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1043 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1047 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1048 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1049 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1054 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1056 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1058 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1060 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1064 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1065 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1069 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1070 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1071 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1072 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1073 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1074 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1075 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1076 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1077 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1082 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1083 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1085 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1090 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1091 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1125 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1135 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1136 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1141 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1143 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1150 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1163 "chapel.ypp"
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

  case 271:

/* Line 1806 of yacc.c  */
#line 1179 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1181 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1183 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1188 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1196 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1204 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1212 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1221 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1231 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1232 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1238 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1248 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      USR_WARN(var, "support for '=>' to alias arrays is deprecated as of chpl version 1.15.  Use a 'ref' declaration instead.");
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1255 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1260 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1262 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1294 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1323 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1330 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1335 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1336 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1349 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1358 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1376 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1392 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 1397 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1398 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1399 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1404 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 344:

/* Line 1806 of yacc.c  */
#line 1421 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1423 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1425 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1427 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1429 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 1442 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 354:

/* Line 1806 of yacc.c  */
#line 1444 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1446 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1448 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1452 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1454 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1456 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1458 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1460 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1462 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1469 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1475 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1481 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1487 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1537 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pForallIntents) = (yyvsp[(3) - (4)].pForallIntents); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pForallIntents) = new ForallIntents();
      addForallIntent((yyval.pForallIntents), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1550 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pForallIntents), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1555 "chapel.ypp"
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[(1) - (2)].pt); (yyval.pIntentExpr).iVar = (yyvsp[(2) - (2)].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1565 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1570 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1593 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1625 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1626 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1641 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1642 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1643 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1647 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1648 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1649 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1651 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1652 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1653 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1654 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1657 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1661 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1668 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1676 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 1680 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 1682 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 1685 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 1686 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 1687 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 1688 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 452:

/* Line 1806 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 453:

/* Line 1806 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 454:

/* Line 1806 of yacc.c  */
#line 1691 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 455:

/* Line 1806 of yacc.c  */
#line 1692 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 456:

/* Line 1806 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:

/* Line 1806 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 458:

/* Line 1806 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 459:

/* Line 1806 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 460:

/* Line 1806 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 461:

/* Line 1806 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 462:

/* Line 1806 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 463:

/* Line 1806 of yacc.c  */
#line 1703 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 464:

/* Line 1806 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 465:

/* Line 1806 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 466:

/* Line 1806 of yacc.c  */
#line 1709 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 467:

/* Line 1806 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 468:

/* Line 1806 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 469:

/* Line 1806 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 470:

/* Line 1806 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 471:

/* Line 1806 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 472:

/* Line 1806 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 473:

/* Line 1806 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 474:

/* Line 1806 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 475:

/* Line 1806 of yacc.c  */
#line 1725 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 476:

/* Line 1806 of yacc.c  */
#line 1726 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 477:

/* Line 1806 of yacc.c  */
#line 1727 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 478:

/* Line 1806 of yacc.c  */
#line 1728 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 479:

/* Line 1806 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 8557 "bison-chapel.cpp"
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



