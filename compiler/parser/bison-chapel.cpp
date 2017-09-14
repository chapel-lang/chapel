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
#line 188 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 340 of yacc.c  */
#line 452 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 458 "bison-chapel.cpp"
/* Unqualified %code blocks.  */

/* Line 344 of yacc.c  */
#line 38 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;


/* Line 344 of yacc.c  */
#line 196 "chapel.ypp"

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
#define YYLAST   13301

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  146
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  122
/* YYNRULES -- Number of rules.  */
#define YYNRULES  483
/* YYNRULES -- Number of states.  */
#define YYNSTATES  916

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
    1366,  1368,  1370,  1375,  1379,  1383,  1386,  1389,  1391,  1394,
    1397,  1399,  1401,  1403,  1405,  1407,  1409,  1411,  1416,  1421,
    1426,  1430,  1434,  1438,  1442,  1447,  1451,  1456,  1458,  1460,
    1462,  1464,  1466,  1470,  1475,  1479,  1484,  1488,  1493,  1497,
    1503,  1507,  1511,  1515,  1519,  1523,  1527,  1531,  1535,  1539,
    1543,  1547,  1551,  1555,  1559,  1563,  1567,  1571,  1575,  1579,
    1583,  1587,  1591,  1595,  1598,  1601,  1604,  1607,  1610,  1613,
    1617,  1621,  1625,  1629,  1633,  1637,  1641,  1645,  1647,  1649,
    1651,  1653,  1655,  1657
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
     254,    52,   157,   130,    -1,    65,   233,   130,    -1,   256,
      92,   255,   130,    -1,   256,   103,   255,   130,    -1,   256,
     100,   255,   130,    -1,   256,   102,   255,   130,    -1,   256,
      96,   255,   130,    -1,   256,   101,   255,   130,    -1,   256,
      97,   255,   130,    -1,   256,    93,   255,   130,    -1,   256,
      94,   255,   130,    -1,   256,    95,   255,   130,    -1,   256,
     105,   255,   130,    -1,   256,   104,   255,   130,    -1,   256,
     134,   255,   130,    -1,   256,   135,   255,   130,    -1,   256,
      98,   255,   130,    -1,   256,    99,   255,   130,    -1,    -1,
     163,    -1,     3,    -1,   162,    -1,     7,    -1,    26,   151,
      -1,   154,    -1,    66,   130,    -1,    66,   255,   130,    -1,
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
     255,    -1,    -1,   110,   141,   233,   142,    -1,    -1,   110,
     240,    -1,   110,   229,    -1,     1,    -1,   141,   233,   142,
     240,    -1,   141,   233,   142,   229,    -1,   141,   233,    37,
     254,   142,   240,    -1,     1,    -1,    -1,   240,    -1,   213,
      -1,   141,   142,   230,    -1,   141,   233,   142,   230,    -1,
     141,   142,   231,    -1,   141,   233,   142,   231,    -1,   141,
     233,    37,   254,   142,   230,    -1,    -1,   110,   240,    -1,
     110,   213,    -1,   110,    27,    -1,   110,    70,    -1,   110,
      73,    -1,   110,   231,    -1,   254,    -1,   213,    -1,   233,
     111,   254,    -1,   233,   111,   213,    -1,    80,    -1,   255,
      -1,   213,    -1,   234,   111,   234,    -1,   235,   111,   234,
      -1,    -1,   237,    -1,   238,    -1,   237,   111,   238,    -1,
     163,    92,   213,    -1,   163,    92,   255,    -1,   213,    -1,
     255,    -1,   163,    -1,   243,    -1,   256,    -1,   242,    -1,
     264,    -1,   263,    -1,    70,   254,    -1,    38,   139,   236,
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
     259,    -1,   258,    -1,   190,    -1,   252,    -1,   253,    -1,
     251,   136,   254,    -1,    -1,   246,    -1,    87,   139,   247,
     140,    -1,   250,    -1,   247,   111,   250,    -1,    87,   139,
     249,   140,    -1,   250,    -1,   249,   111,   250,    -1,   206,
     239,    -1,   254,    63,   239,    -1,   267,    63,   239,    -1,
     256,    -1,   251,   136,   254,    -1,    48,   254,    -1,    44,
     222,    37,   254,    -1,   261,    -1,   240,    -1,   241,    -1,
     265,    -1,   266,    -1,   190,    -1,   252,    -1,   253,    -1,
     139,   115,   254,   140,    -1,   254,   110,   254,    -1,   254,
     114,   254,    -1,   254,   114,    -1,   114,   254,    -1,   114,
      -1,    77,   254,    -1,    78,   254,    -1,   254,    -1,   239,
      -1,   258,    -1,   259,    -1,   260,    -1,   256,    -1,   190,
      -1,   257,   139,   236,   140,    -1,   257,   141,   236,   142,
      -1,    58,   139,   236,   140,    -1,   254,   113,   163,    -1,
     254,   113,    79,    -1,   254,   113,    27,    -1,   139,   234,
     140,    -1,   139,   234,   111,   140,    -1,   139,   235,   140,
      -1,   139,   235,   111,   140,    -1,     4,    -1,     5,    -1,
       6,    -1,     7,    -1,     8,    -1,   137,   233,   138,    -1,
     137,   233,   111,   138,    -1,   141,   233,   142,    -1,   141,
     233,   111,   142,    -1,   141,   262,   142,    -1,   141,   262,
     111,   142,    -1,   254,    90,   254,    -1,   262,   111,   254,
      90,   254,    -1,   254,   128,   254,    -1,   254,   123,   254,
      -1,   254,   133,   254,    -1,   254,   112,   254,    -1,   254,
     131,   254,    -1,   254,   132,   254,    -1,   254,   124,   254,
      -1,   254,   116,   254,    -1,   254,   126,   254,    -1,   254,
     122,   254,    -1,   254,   119,   254,    -1,   254,   121,   254,
      -1,   254,   118,   254,    -1,   254,   106,   254,    -1,   254,
     108,   254,    -1,   254,   109,   254,    -1,   254,    91,   254,
      -1,   254,   127,   254,    -1,   254,   117,   254,    -1,   254,
      15,   254,    -1,   254,    10,   254,    -1,   254,   120,   254,
      -1,   254,    25,   254,    -1,   128,   254,    -1,   123,   254,
      -1,    46,   254,    -1,    56,   254,    -1,   125,   254,    -1,
     107,   254,    -1,   254,    63,   254,    -1,   254,    63,   172,
      -1,   267,    63,   254,    -1,   267,    63,   172,    -1,   254,
      67,   254,    -1,   254,    67,   172,    -1,   267,    67,   254,
      -1,   267,    67,   172,    -1,   128,    -1,   133,    -1,    91,
      -1,   127,    -1,   106,    -1,   108,    -1,   109,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   439,   439,   444,   445,   451,   452,   457,   458,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   496,
     498,   503,   504,   505,   520,   521,   526,   527,   532,   537,
     542,   546,   553,   558,   562,   567,   571,   572,   573,   577,
     581,   583,   585,   587,   589,   591,   593,   595,   597,   599,
     601,   603,   605,   607,   609,   611,   616,   617,   621,   625,
     626,   630,   631,   635,   636,   640,   641,   642,   643,   644,
     645,   646,   647,   651,   652,   656,   657,   658,   659,   663,
     678,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,   691,   692,   693,   694,   700,   706,   712,
     718,   725,   735,   739,   740,   741,   742,   746,   749,   750,
     751,   752,   753,   754,   758,   759,   763,   764,   765,   769,
     770,   774,   777,   782,   783,   787,   789,   791,   798,   808,
     821,   826,   831,   839,   840,   845,   846,   848,   853,   863,
     872,   876,   884,   885,   890,   895,   889,   920,   926,   933,
     941,   953,   959,   952,   987,   991,   996,  1000,  1008,  1009,
    1013,  1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,
    1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,
    1033,  1034,  1035,  1036,  1037,  1038,  1042,  1043,  1044,  1045,
    1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,  1057,  1058,
    1062,  1066,  1067,  1068,  1072,  1074,  1076,  1078,  1083,  1084,
    1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,  1100,
    1101,  1102,  1103,  1104,  1105,  1109,  1110,  1114,  1115,  1116,
    1117,  1118,  1119,  1123,  1124,  1127,  1128,  1132,  1133,  1137,
    1139,  1144,  1145,  1149,  1150,  1154,  1155,  1159,  1161,  1163,
    1168,  1181,  1198,  1199,  1201,  1206,  1214,  1222,  1230,  1239,
    1249,  1250,  1251,  1255,  1256,  1264,  1266,  1273,  1278,  1280,
    1282,  1287,  1289,  1291,  1298,  1299,  1300,  1304,  1305,  1310,
    1311,  1312,  1313,  1333,  1337,  1341,  1349,  1353,  1354,  1355,
    1359,  1361,  1367,  1369,  1371,  1376,  1377,  1378,  1379,  1380,
    1381,  1382,  1388,  1389,  1390,  1391,  1395,  1396,  1397,  1401,
    1402,  1406,  1407,  1411,  1412,  1416,  1417,  1418,  1419,  1423,
    1434,  1435,  1436,  1437,  1438,  1439,  1441,  1443,  1445,  1447,
    1449,  1451,  1456,  1458,  1460,  1462,  1464,  1466,  1468,  1470,
    1472,  1474,  1476,  1478,  1480,  1487,  1493,  1499,  1505,  1514,
    1524,  1532,  1533,  1534,  1535,  1536,  1537,  1538,  1539,  1544,
    1545,  1549,  1553,  1556,  1561,  1565,  1568,  1573,  1577,  1578,
    1582,  1583,  1588,  1593,  1601,  1602,  1603,  1604,  1605,  1606,
    1607,  1608,  1609,  1611,  1613,  1615,  1617,  1619,  1624,  1625,
    1626,  1629,  1630,  1631,  1632,  1643,  1644,  1648,  1649,  1650,
    1654,  1655,  1656,  1664,  1665,  1666,  1667,  1671,  1672,  1673,
    1674,  1675,  1676,  1677,  1678,  1679,  1680,  1684,  1692,  1693,
    1697,  1698,  1699,  1700,  1701,  1702,  1703,  1704,  1705,  1706,
    1707,  1708,  1709,  1710,  1711,  1712,  1713,  1714,  1715,  1716,
    1717,  1718,  1719,  1723,  1724,  1725,  1726,  1727,  1728,  1732,
    1733,  1734,  1735,  1739,  1740,  1741,  1742,  1747,  1748,  1749,
    1750,  1751,  1752,  1753
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
  "intent_expr", "io_expr", "new_expr", "let_expr", "expr", "opt_try_expr",
  "lhs_expr", "fun_expr", "call_expr", "dot_expr", "parenthesized_expr",
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
     255,   256,   256,   256,   256,   257,   257,   258,   258,   258,
     259,   259,   259,   260,   260,   260,   260,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   262,   262,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   264,   264,   264,   264,   264,   264,   265,
     265,   265,   265,   266,   266,   266,   266,   267,   267,   267,
     267,   267,   267,   267
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
       1,     1,     4,     3,     3,     2,     2,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     4,
       3,     3,     3,     3,     4,     3,     4,     1,     1,     1,
       1,     1,     3,     4,     3,     4,     3,     4,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    38,    78,   427,   428,   429,   430,
     431,     0,   379,    76,   150,   379,     0,   281,    76,     0,
       0,     0,     0,     0,    76,    76,     0,     0,   280,     0,
       0,   168,     0,   164,     0,     0,     0,     0,   370,     0,
       0,     0,     0,   280,   280,   151,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   152,     0,
       0,     0,   479,   481,     0,   482,   483,   407,     0,     0,
     480,   477,    85,   478,     0,     0,     0,     4,     0,     5,
       9,     0,    10,    11,    12,    14,   339,    22,    13,    86,
      92,    15,    17,    16,    19,    20,    21,    18,    91,     0,
      89,   399,     0,    93,    90,    94,     0,   411,   395,   396,
     342,   340,     0,     0,   400,   401,     0,   341,     0,   412,
     413,   414,   394,   344,   343,   397,   398,     0,    24,   350,
       0,     0,   380,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   411,   340,   400,   401,   379,   341,
     412,   413,     0,     0,   127,     0,     0,   331,     0,    80,
      79,   169,    99,     0,   170,     0,     0,     0,     0,     0,
     281,   282,    98,     0,     0,   331,     0,     0,     0,     0,
       0,   283,     0,    82,    32,     0,   465,   392,     0,   466,
       7,   331,   282,    88,    87,   260,   323,     0,   322,     0,
       0,    83,   410,     0,     0,    35,     0,   345,     0,   331,
      36,   351,     0,   134,   130,     0,   341,   134,   131,     0,
     272,     0,     0,   322,     0,     0,   468,   406,   464,   467,
     463,    44,    46,     0,     0,   326,     0,   328,     0,     0,
     327,     0,   322,     0,     0,     6,     0,   153,   246,   245,
     171,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   405,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   331,   331,     0,     0,     0,    25,    26,     0,    27,
       0,     0,     0,     0,     0,     0,     0,    28,    29,     0,
     339,   337,     0,   332,   333,   338,     0,     0,     0,     0,
       0,   108,     0,     0,   107,     0,     0,     0,   114,     0,
       0,    52,    95,     0,   124,     0,    30,   221,   165,   288,
       0,   289,   291,     0,   302,     0,     0,   294,     0,     0,
      81,    31,    33,     0,   170,   259,     0,    59,   408,   409,
      84,   143,    34,   331,     0,   141,   132,   128,   133,   129,
       0,   270,   267,    56,     0,    52,   101,    37,    45,    47,
       0,   432,     0,     0,   423,     0,   425,     0,     0,     0,
       0,     0,     0,   436,     8,     0,     0,     0,   239,     0,
       0,     0,     0,     0,   378,   460,   459,   462,   470,   469,
     474,   473,   456,   453,   454,   455,   403,   443,   422,   421,
     420,   404,   447,   458,   452,   450,   461,   451,   449,   441,
     446,   448,   457,   440,   444,   445,   442,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   472,   471,   476,   475,   233,
     230,   231,   232,   236,   237,   238,     0,     0,   382,     0,
       0,     0,     0,     0,     0,     0,     0,   434,   379,   379,
     104,   268,     0,     0,   347,     0,   162,     0,   159,   269,
     153,     0,     0,     0,   354,     0,     0,     0,   360,     0,
       0,     0,   115,   478,    55,     0,    48,    53,     0,   123,
       0,     0,   346,     0,   222,     0,   229,   247,     0,   292,
       0,   306,     0,   301,   395,     0,     0,   285,   393,   284,
     419,   325,   324,     0,     0,   348,     0,   135,     0,   274,
     395,     0,     0,     0,   433,   402,   424,   329,   426,   330,
       0,     0,   435,   120,   364,     0,   438,   437,     0,     0,
     154,   155,   243,   240,   241,   244,   172,     0,     0,   276,
     275,   277,   279,    60,    67,    68,    69,    64,    66,    74,
      75,    62,    65,    63,    61,    71,    70,    72,    73,   417,
     418,   234,   235,   387,     0,   381,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   100,   335,
     336,   334,     0,   160,   158,     0,     0,   122,     0,     0,
     106,     0,   105,     0,     0,   112,     0,     0,   110,     0,
       0,   385,     0,    96,     0,    97,     0,     0,   126,   228,
     220,     0,   315,   248,   251,   250,   252,     0,   290,   293,
       0,   294,     0,   286,   295,   296,     0,     0,   142,   144,
     349,     0,   136,   139,     0,     0,   271,    57,    58,     0,
       0,     0,     0,   121,     0,    39,     0,   280,   242,   247,
     203,   201,   206,   213,   214,   215,   210,   212,   208,   211,
     209,   207,   217,   216,   182,   185,   183,   184,   195,   186,
     199,   191,   189,   202,   190,   188,   193,   198,   200,   187,
     192,   196,   197,   194,   204,   205,     0,   180,     0,   218,
     218,   178,   278,   383,   411,   411,     0,     0,     0,     0,
       0,     0,     0,     0,   103,   102,   163,   161,   155,   109,
       0,     0,   353,     0,   352,     0,     0,   359,   113,     0,
     358,   111,     0,   384,    50,    49,   125,   369,   223,     0,
       0,   294,   249,   265,   287,     0,     0,     0,   146,     0,
       0,     0,   137,     0,     0,   118,   366,     0,     0,   116,
     365,     0,   439,    40,    76,   280,   280,   148,   280,   156,
       0,   181,     0,     0,   221,   174,   175,     0,     0,     0,
       0,     0,     0,     0,     0,   280,   357,     0,     0,   363,
       0,     0,   386,     0,   315,   318,   319,   320,     0,   317,
     321,   395,   261,   225,   224,     0,     0,     0,   304,   395,
     147,   145,     0,   140,     0,   119,     0,   117,   157,   253,
     179,   180,   218,   218,     0,     0,     0,     0,     0,     0,
       0,   149,     0,     0,     0,     0,    51,   294,   307,     0,
     262,   264,   263,   266,   257,   258,   166,     0,   138,     0,
       0,   254,   265,   176,   177,   219,     0,     0,     0,     0,
       0,     0,   356,   355,   362,   361,   227,   226,   309,   310,
     312,   395,     0,   434,   395,   368,   367,     0,     0,   311,
     313,   255,   173,   256,   307,   314
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   232,    78,   519,    80,    81,    82,   233,
     514,   518,   515,    83,    84,    85,   160,    86,   164,   184,
      87,    88,    89,    90,    91,    92,   614,    93,    94,    95,
     376,   547,   674,    96,    97,   543,   669,    98,    99,   407,
     687,   100,   497,   498,   143,   177,   527,   102,   103,   408,
     689,   576,   728,   729,   730,   805,   348,   523,   524,   525,
     476,   577,   250,   657,   882,   912,   876,   196,   871,   833,
     836,   104,   221,   381,   105,   106,   180,   181,   352,   353,
     537,   356,   357,   533,   899,   830,   771,   234,   238,   239,
     322,   323,   324,   144,   108,   109,   110,   145,   112,   131,
     132,   477,   339,   640,   478,   113,   146,   147,   202,   325,
     149,   118,   150,   151,   121,   122,   243,   123,   124,   125,
     126,   127
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -787
static const yytype_int16 yypact[] =
{
    -787,   104,  2605,  -787,  -787,  -787,  -787,  -787,  -787,  -787,
    -787,  3749,    66,   121,  -787,    66,  8338,    32,   121,  3749,
    8338,  3749,    71,   121,   200,   535,  6948,  8338,  7087,  8338,
      74,  -787,   121,  -787,    37,  7504,  8338,  8338,  -787,  8338,
    8338,   238,   149,   455,   811,  -787,  7643,  6531,  8338,  7504,
    8338,   133,   160,  3749,  8338,  8477,  8477,   121,  -787,  7643,
    8338,  8338,  -787,  -787,  8338,  -787,  -787,  2312,  8338,  8338,
    -787,  8338,  -787,  -787,  2915,  6114,  7643,  -787,  3610,  -787,
    -787,   272,  -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,
    -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,   121,
    -787,   -27,    59,  -787,  -787,  -787,   290,   174,  -787,  -787,
    -787,   175,   195,   199,   207,   217, 13106,  1952,   108,   230,
     241,  -787,  -787,  -787,  -787,  -787,  -787,   148,  -787, 13106,
     235,  3749,  -787,   250,  -787,   273,  8338,  8338,  8338,  8338,
    8338,  7643,  7643,   112,  -787,  -787,  -787,  -787, 11416,   181,
    -787,  -787,   121,   279,  -787, 11675,   336,  6670,   286,  -787,
    -787,  -787,  -787,   121,   204,   121,   288,    23, 10404, 10337,
    -787,  -787,  -787, 11354, 10533,  6670,  3749,   292,    19,   109,
      33,  -787,  3749,  -787,  -787, 10595,   468,   312, 10595,   468,
    -787,  6670,   182,  -787,  -787,   121,  -787,    82, 13106,  8338,
    8338,  -787, 13106,   300, 11215,  -787, 10595, 13106,   319,  6670,
    -787, 13106, 11737,  -787,  -787, 11799,  5400,  -787,  -787, 11861,
     367,   330,   216, 11489, 10595, 11923,   183,  1256,   468,   183,
     468,  -787,  -787,  3054,   -33,  -787,  8338,  -787,    14,    89,
    -787,    31, 11985,   -48,   454,  -787,   121,   353,  -787,  -787,
    -787,    26,    37,    37,    37,  -787,  8338,  8338,  8338,  8338,
    7782,  7782,  8338,  8338,  8338,  8338,  8338,  8338,   130,  2312,
    8338,  8338,  8338,  8338,  8338,  8338,  8338,  8338,  8338,  8338,
    8338,  8338,  8338,  8338,  8338,  7226,  7226,  7226,  7226,  7226,
    7226,  7226,  7226,  7226,  7226,  7226,  7226,  7226,  7226,  7226,
    7226,  6670,  6670,  7782,  7782,  5975,  -787,  -787,  3193,  -787,
   11551, 11613, 12047,    48,  7782,    23,   337,  -787,  -787,  8338,
     374,  -787,   328,   359,  -787,  -787,   121,   341,   121,   435,
    7643,  -787,  3888,  7782,  -787,  4027,  7782,   334,  -787,    23,
    8616,  8338,  -787,  3749,   446,   351,  -787,    52,  -787,  -787,
      19,  -787,   365,   352,  -787,  5417,   405,   406,  8338,    37,
    -787,  -787,  -787,   357,  -787,  -787,  7643,  -787, 13106, 13106,
    -787,  -787,  -787,  6670,   360,  -787,   487,  -787,   487,  -787,
    5556,   396,  -787,  -787,  8616,  8338,  -787,  -787,  -787,  -787,
    7365,  -787, 10204,  6253,  -787,  6392,  -787,  7782,  5695,  2776,
     368,  8338,  1887,  -787,  -787,   375,  7643,   376,   173,    37,
     257,   283,   291,   294, 11277,  2643,  2643,   322,  -787,   322,
    -787,   322,  1525,   703,   594,  1481,   312,   183,  -787,  -787,
    -787,  1256,  1022,   322,  1736,  1736,  2643,  1736,  1736,  1997,
     183,  1022,  9839,  1997,   468,   468,   183,   381,   386,   387,
     391,   392,   393,   394,   395,   398,   399,   402,   404,   407,
     409,   410,   411,   412,   384,  -787,   322,  -787,   322,    86,
    -787,  -787,  -787,  -787,  -787,  -787,   121,   184,  -787, 13168,
     318,  8755,  7782,  8894,  7782,  8338,  7782,  1242,    66, 12109,
    -787,  -787, 12176,  6670,  -787,  6670,   444,   198,  -787,  -787,
     353,  8338,   186,  8338, 13106,    24, 10657,  8338, 13106,    10,
   10466,  5975,  -787,   415,   436,   421, 12238,   436,   424,   527,
   12300,  3749,  -787,   210,  -787,    39,  -787,   297,   417,    19,
     114,  -787,  7643,  -787,   506,  8338,  6809,  -787, 13106,  -787,
    -787,  -787, 13106,    18,   419,  -787,    38,  -787,  7643,  -787,
     304,   121,   430,   431,  -787,  -787,  -787,  -787,  -787,  -787,
     -12,  9965,  -787,  -787, 13106,  3749, 13106,  -787, 12362,  3332,
     451,  -787,   501,  -787,  -787,  -787,  -787,  2447,   326,  -787,
    -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,
    -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,
    -787,  -787,  -787,  -787,  5975,  -787,  7782,  7782,  8338,   543,
   12424,  8338,   544, 12486,   432, 10045,    23,    23,  -787,  -787,
    -787,  -787,  8338,   121,  -787,   438, 10595,  -787, 10719,  4166,
    -787,  4305,  -787, 10781,  4444,  -787,    23,  4583,  -787,    23,
     218,  -787,  8338,  -787,  8338,  -787,  3749,  8338,  -787,   311,
    -787,    19,   463,   512,  -787,  -787,  -787,    42,  -787,  -787,
    5556,   406,    76, 13106,  -787,  -787,  4722,  7643,  -787,  -787,
    -787,   121,  -787,   467,   273,    85,  -787,  -787,  -787,  4861,
     437,  5000,   441,  -787,  8338,  -787,  3471,   948,  -787,   297,
    -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,
    -787,  -787,  -787,  -787,  -787,   121,  -787,  -787,  -787,  -787,
    -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,  -787,
    -787,  -787,  -787,  -787,  -787,  -787,  8338,   471,   475,   450,
     450,  -787,  -787,  -787,   244,   258, 12548,  9033,  9172, 12610,
    9311,  9450,  9589,  9728,  -787,  -787, 13106,  -787,  -787,  -787,
    3749,  8338, 13106,  8338, 13106,  3749,  8338, 13106,  -787,  8338,
   13106,  -787,  5975,  -787, 12672, 13106,  -787, 13106,  -787,   453,
    7921,    -6,  -787,   509,  -787,  7782,  5139,  3749,  -787,    58,
     470,  8338,  -787,  5278,  8338,  -787, 13106,  3749,  8338,  -787,
   13106,  3749, 13106,  -787,   634,   397,   397,  -787,  1460,  -787,
      11,  -787, 10275,  9917,    52,  -787,  -787,  8338,  8338,  8338,
    8338,  8338,  8338,  8338,  8338,  1042, 12300, 10843, 10905, 12300,
   10967, 11029,  -787,  8338,   463,    71,  8338,  8338,  5835,  -787,
    -787,   252,  7643,  -787,  -787,  8338,    27, 10089,  -787,   780,
     336,  -787,   273, 13106, 11091,  -787, 11153,  -787,  -787,   531,
    -787,  -787,   450,   450,   261, 12734, 12796, 12858, 12920, 12982,
   13044,  -787,  3749,  3749,  3749,  3749, 13106,    -6,  8060,   106,
    -787,  -787, 13106, 13106,  -787,  -787,  -787,  9728,  -787,  3749,
    3749,  -787,   509,  -787,  -787,  -787,  8338,  8338,  8338,  8338,
    8338,  8338, 12300, 12300, 12300, 12300,  -787,  -787,  -787,  -787,
    -787,   278,  7782,  9867,   881, 12300, 12300,    96, 10160,  -787,
    -787,  -787,  -787,  -787,  8199,  -787
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -787,  -787,  -787,    -1,  -623,  1810,  -787,  -787,  1651,    34,
    -240,   223,   227,  -787,  -787,   180,   150,  1984,   -11,   428,
    -741,  -631,   -40,  -787,  -787,  -787,   -20,  -787,  -787,  -787,
     413,  -787,   -59,  -787,  -787,  -787,  -787,  -787,   458,   115,
    -124,  -787,  -787,     3,   547,  -787,  -787,  -787,  -787,  -787,
    -787,  -787,  -787,  -175,  -172,  -670,  -787,  -166,   -16,  -787,
    -326,  -787,  -787,   -50,  -787,  -787,  -260,    13,  -787,  -217,
    -233,  -787,  -132,  -787,   623,  -787,  -171,   295,  -787,  -343,
    -651,  -787,  -502,  -369,  -786,  -777,  -168,   -44,  -106,  -787,
    -130,  -787,   158,   338,  -350,  -787,  -787,   830,  -787,     1,
    -787,  -787,  -218,  -787,  -472,  -787,   900,  1039,    -2,   155,
     691,  -787,  1396,  1434,  -787,  -787,  -787,  -787,  -787,  -787,
    -787,  -297
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -435
static const yytype_int16 yytable[] =
{
     116,    77,   197,   193,   194,   534,   167,   528,   480,   129,
     774,   549,   354,   161,   148,   222,   135,   116,   155,   116,
     316,   526,     5,   400,   168,   169,   173,   174,   661,     5,
     550,   327,   241,   185,   186,   187,   634,   188,   189,   641,
       5,     5,     5,   354,   198,   345,   204,   206,   207,   182,
     629,   211,   212,   215,   219,  -228,   799,   223,   224,   225,
     806,   363,   226,   402,   798,   227,   228,   229,   397,   230,
     358,   666,   198,   469,   242,   337,   116,  -299,   390,   374,
     410,   411,   412,   413,   182,   486,   536,  -299,   237,   470,
     409,   900,   471,    47,   403,   875,  -299,   337,   313,   349,
     248,   517,   667,  -375,     3,   391,   472,   473,  -299,   832,
     354,   152,  -416,   775,  -416,   354,   474,   909,   337,   249,
     834,   660,   775,   601,     5,   393,   910,  -299,   915,   116,
     679,   475,   733,     5,   129,   310,   311,   312,   211,   198,
     242,  -299,   398,   902,   359,   517,  -299,   308,  -299,   315,
     602,  -299,   660,   130,   394,   773,   668,   428,   350,   398,
     308,   308,    47,   133,   308,   178,   875,   848,   153,   366,
     321,   463,   464,   399,   116,   308,   178,   671,   651,  -299,
     116,   364,   883,   884,   799,     5,   659,   398,   321,   159,
     487,  -228,   798,   366,   572,   308,   398,   368,   369,  -297,
     395,  -299,   203,     5,   321,   208,  -299,   159,   259,   429,
     157,   303,   367,   175,   480,   304,   897,   398,   776,   355,
    -299,    14,   321,   -76,   660,  -299,   911,   783,   573,   396,
     240,   116,   389,   308,   392,   214,   218,   574,   578,  -299,
     418,   420,   -76,   544,  -299,   190,   260,   301,   903,   302,
     261,  -416,   575,  -416,   414,   415,   416,   417,   419,   421,
     422,   423,   424,   425,   426,   427,    45,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   465,   467,    58,   502,   557,   191,   559,
     822,   636,   639,   266,   488,   604,   268,   366,   849,   209,
     271,   466,   468,   479,  -372,  -371,   116,   480,   769,   623,
     534,   251,   489,   505,   321,   321,   509,   492,   653,   246,
    -415,   649,  -415,   526,   605,   255,   627,   366,   198,   762,
     504,   506,   469,   508,   510,   256,   624,  -376,   516,   516,
     107,   520,   680,   682,  -316,   252,   383,  -377,   470,   107,
     650,   471,   654,   116,   253,  -388,   538,   107,   763,   107,
    -374,   655,   570,  -316,   542,   472,   473,  -316,   359,  -389,
    -308,  -373,   649,   254,   305,   474,   656,   560,   116,   541,
     307,   607,   516,   516,  -388,   304,   321,   579,   542,  -308,
     475,   107,  -316,  -308,   359,   561,   542,   564,  -389,   566,
     568,   885,   359,   541,   198,   359,   237,   838,   237,   317,
     308,   541,   107,   580,   838,  -273,   107,   170,  -308,   676,
     831,   581,   319,   326,   582,   268,   839,   330,    24,   192,
     370,   347,   266,   839,  -273,   268,    31,   359,  -167,   271,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   732,  -167,   373,   380,
     382,   404,   609,   406,   612,   480,   493,   491,   494,   107,
     495,   499,   501,   511,   521,   170,   529,   205,   526,   504,
     610,   508,   613,   520,   615,   564,    24,   192,   662,   616,
     617,   522,   530,   259,    31,   535,  -167,   540,   536,   626,
     545,   628,   -43,   546,   675,   633,   619,   551,   321,   479,
     565,   583,   569,   571,   107,  -167,   584,   585,   901,   116,
     107,   586,   587,   588,   589,   590,   600,   904,   591,   592,
     242,   260,   593,   663,   594,   261,   622,   595,     5,   596,
     597,   598,   159,  -300,   162,   -54,   242,   642,   240,   101,
     240,   643,   599,   901,   645,   646,  -282,   658,   101,   670,
     677,   678,   366,   116,   901,   688,   101,   116,   101,   737,
     740,   107,  -300,   770,   742,   748,   772,   781,   266,   787,
     267,   268,  -300,   791,  -339,   271,   418,   465,   803,   804,
    -282,  -300,   278,   824,   835,   331,   334,   338,  -300,  -282,
     101,   284,   479,   686,   419,   466,   736,   881,   553,   739,
     842,   552,   780,   361,   163,   625,   362,  -300,  -282,   259,
     746,   101,   328,   779,   815,   101,   747,   752,   852,   754,
     378,   853,   757,   768,   372,   760,  -300,     5,   854,   800,
     764,   159,   765,  -300,   116,   767,   107,   913,   620,   907,
     896,   172,   386,   621,   539,  -282,   867,   260,   116,     0,
       0,   261,     0,     0,   116,   198,     0,     0,     0,     0,
     107,     0,     0,   107,     0,     0,     0,   786,   101,   790,
       0,   107,   792,     0,   116,   389,     0,     0,     0,  -282,
       0,   665,     0,   117,     0,     0,     0,     0,  -282,     0,
     263,     0,   117,   265,   266,     0,   267,   268,     0,     0,
     117,   271,   117,   163,     0,     0,     0,  -282,   278,     0,
       0,     0,     0,   101,   802,   282,   283,   284,   259,   101,
       0,     0,     0,     0,     0,   752,   754,   107,   757,   760,
     786,   790,     0,   490,   117,     0,   216,   216,   816,   817,
       0,   818,     0,   819,   820,   193,   194,   821,     0,     0,
     479,     0,     0,     0,     0,   117,   260,   512,   116,   117,
     261,     0,     0,   837,   564,   116,     0,     0,     0,   843,
     101,   564,   844,   829,   869,   116,   846,     0,     0,   116,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   816,   855,   856,   819,   857,
     858,   859,   860,   266,   603,   267,   268,  -303,     0,     0,
     271,   866,   117,     0,   207,   211,   242,   278,     0,     0,
     872,   170,   111,   873,   282,   283,   284,     0,     0,     0,
       0,   111,    24,   192,     0,   870,  -303,     0,     0,   111,
      31,   111,  -167,     0,     0,   101,  -303,     0,   -42,   107,
     892,   893,   894,   895,     0,  -303,   116,   117,     0,     0,
       0,  -167,  -303,   117,     0,   790,     0,   905,   906,   101,
       0,   898,   101,   111,   892,   893,   894,   895,   905,   906,
     101,  -303,     0,     0,     0,     0,     0,     0,     0,     0,
     908,   564,   114,   107,   111,     0,     0,   107,   111,     0,
    -303,   114,   790,     0,     0,   731,   898,  -303,  -305,   114,
       0,   114,     0,     0,   117,     0,     0,   898,     0,     0,
       0,     0,     0,   630,   632,     0,     0,   635,   638,     0,
       0,     0,     0,     0,   734,   735,   101,  -305,     0,     0,
       0,     0,     0,   114,     0,     0,     0,  -305,     0,     0,
       0,   111,     0,     0,     0,    14,  -305,   107,    17,   107,
       0,     0,   107,  -305,   114,   107,     0,    23,   114,    24,
     794,     0,     0,    28,   107,     0,     0,    31,     0,  -167,
       0,     0,  -305,     0,     0,     0,     0,     0,     0,   117,
       0,     0,     0,     0,   107,    41,   111,   795,  -167,   796,
      45,  -305,   111,     0,     0,     0,     0,   107,  -305,   107,
       0,     0,     0,   117,   107,     0,   117,    57,     0,    58,
       0,   114,     0,     0,   117,     0,     0,     0,     0,     0,
       0,   115,     0,     0,   744,   745,     0,   259,     0,     0,
     115,     0,     0,     0,   749,     0,     0,     0,   115,    14,
     115,     0,    17,   111,   758,     0,     0,   761,   101,     0,
       0,    23,     0,    24,   794,     0,   114,    28,    72,     0,
       0,    31,   114,  -167,     0,   260,   797,     0,   107,   261,
     117,     0,   115,   107,     0,     0,     0,     0,     0,    41,
       0,   795,  -167,   796,    45,     0,     0,     0,     0,     0,
       0,     0,   101,   115,     0,   107,   101,   115,     0,     0,
       0,    57,     0,    58,     0,   107,     0,     0,   263,   107,
     264,   265,   266,   114,   267,   268,   269,     0,   111,   271,
     272,   273,     0,   275,   276,   277,   278,     0,     0,     0,
     281,     0,     0,   282,   283,   284,     0,     0,     0,     0,
       0,     0,   111,     0,     0,   111,     0,     0,     0,     0,
     115,     0,    72,   111,     0,     0,   101,     0,   101,     0,
     861,   101,     0,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
     107,   107,   107,   107,     0,     0,     0,   841,   114,     0,
       0,     0,   117,   101,     0,   115,     0,   107,   107,     0,
       0,   115,     0,     0,     0,     0,   101,     0,   101,   111,
       0,     0,   114,   101,     0,   114,     0,     0,     0,     0,
       0,     0,     0,   114,     0,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   136,     0,   117,     0,     0,     0,
     117,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,   115,     0,     0,   137,   138,     0,   139,     0,
      30,   259,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,   101,    40,   114,
      42,     0,   101,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,   260,
     117,     0,   117,   261,   101,   117,     0,     0,   117,     0,
       0,     0,     0,     0,   101,     0,     0,   117,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   115,     0,    64,
       0,   111,     0,     0,     0,     0,     0,   117,     0,     0,
       0,     0,   263,     0,   264,   265,   266,    69,   267,   268,
     117,   115,   117,   271,   115,     0,     0,   117,     0,   277,
     278,    75,   115,   142,   281,     0,     0,   282,   283,   284,
       0,     0,     0,     0,     0,   111,     0,     0,   119,   111,
       0,     0,     0,     0,     0,     0,     0,   119,     0,   101,
     101,   101,   101,     0,     0,   119,     0,   119,     0,     0,
       0,   114,     0,     0,     0,     0,   101,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   120,     0,   115,     0,
       0,   117,     0,     0,     0,   120,   117,     0,     0,   119,
       0,     0,     0,   120,     0,   120,     0,     0,     0,   111,
       0,   111,     0,     0,   111,   114,     0,   111,   117,   114,
     119,     0,     0,     0,   119,     0,   111,    14,   117,     0,
      17,     0,   117,     0,     0,     0,     0,   120,     0,    23,
       0,    24,   794,     0,     0,    28,   111,     0,     0,    31,
       0,  -167,     0,     0,     0,     0,   259,     0,   120,   111,
       0,   111,   120,     0,     0,     0,   111,   244,     0,   795,
    -167,   796,    45,     0,     0,     0,     0,   119,     0,   114,
       0,   114,     0,     0,   114,     0,     0,   114,     0,    57,
       0,    58,     0,     0,   260,     0,   114,     0,   261,     0,
     259,     0,     0,   117,   117,   117,   117,     0,     0,     0,
     115,     0,     0,     0,     0,   120,   114,     0,     0,     0,
     117,   117,   119,     0,     0,     0,     0,     0,   119,   114,
     111,   114,     0,     0,     0,   111,   114,   263,   260,     0,
      72,   266,   261,   267,   268,     0,     0,     0,   271,     0,
       0,     0,     0,     0,   115,   278,     0,   111,   115,     0,
     120,     0,   282,   283,   284,     0,   120,   111,     0,     0,
       0,   111,     0,     0,     0,     0,     0,     0,     0,   119,
       0,   263,     0,   264,   265,   266,     0,   267,   268,   269,
       0,   270,   271,   272,   273,     0,   275,   276,   277,   278,
     114,   279,     0,   281,     0,   114,   282,   283,   284,     0,
       0,     0,     0,     0,     0,     0,     0,   120,   115,     0,
     115,     0,     0,   115,     0,     0,   115,   114,     0,     0,
       0,     0,     0,     0,     0,   115,   183,   114,     0,     0,
       0,   114,   111,   111,   111,   111,     0,     0,     0,     0,
     183,     0,     0,     0,   119,   115,   213,   217,     0,   111,
     111,     0,     0,     0,     0,     0,     0,     0,   115,     0,
     115,     0,     0,     0,     0,   115,     0,     0,   119,     0,
       0,   119,     0,     0,     0,     0,     0,     0,     0,   119,
       0,     0,   120,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   114,   114,   114,   114,   120,     0,     0,   120,
       0,     0,     0,     0,     0,     0,     0,   120,     0,   114,
     114,     0,     0,     0,     0,     0,   309,     0,     0,   115,
       0,     0,     0,     0,   115,   119,     0,     0,     0,   260,
       0,     0,     0,   261,     0,     0,     0,     0,     0,     0,
       0,     0,    79,     0,     0,     0,   115,     0,   183,   183,
     183,   128,     0,     0,     0,   344,   115,     0,     0,   154,
     115,   156,     0,   120,     0,     0,   183,     0,     0,   183,
       0,     0,   263,     0,   264,   265,   266,     0,   267,   268,
     269,     0,     0,   271,     0,     0,     0,   183,     0,   277,
     278,     0,     0,   210,   281,     0,     0,   282,   283,   284,
       0,     0,     0,     0,     0,   183,     0,     0,     0,     0,
       0,     0,     0,     0,    79,     0,     0,     0,   245,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   136,
       0,   115,   115,   115,   115,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,   119,   115,   115,
     137,   138,     0,   139,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,   306,     0,    40,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   120,     0,    50,    51,    52,
     140,   119,     0,     0,     0,   119,   183,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   346,     0,     0,     0,
     183,     0,   360,    63,    64,    65,    66,   134,     0,   120,
       0,    67,   134,   120,     0,     0,     0,   158,   134,   134,
      68,     0,    69,     0,    70,    71,   176,     0,   179,     0,
      73,     0,   259,     0,   141,   119,    75,   119,   142,   567,
     119,     0,     0,   119,     0,     0,     0,     0,     0,     0,
       0,   220,   119,    79,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,     0,     0,
     260,     0,   119,   120,   261,   120,     0,     0,   120,     0,
       0,   120,     0,     0,     0,   119,     0,   119,     0,     0,
     120,     0,   119,   247,     0,     0,   299,   300,  -390,     0,
       0,  -415,     0,  -415,     0,     0,     0,     0,     0,     0,
     120,     0,     0,   263,     0,   264,   265,   266,     0,   267,
     268,     0,     0,   120,   271,   120,     0,     0,    79,     0,
     120,   278,     0,     0,     0,     0,     0,     0,   282,   283,
     284,     0,     0,     0,     0,     0,   220,     0,     0,     0,
       0,   320,   360,     0,     0,   360,   119,   220,     0,   329,
       0,   119,     0,     0,     0,     0,   183,   183,     0,   320,
     183,   183,   351,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   119,     0,   320,   134,     0,     0,   365,
       0,     0,     0,   119,   120,     0,     0,   119,     0,   120,
       0,     0,     0,   320,     0,     0,     0,   672,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   563,
       0,   120,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   120,     0,     0,     0,   120,     0,     0,     0,     0,
     405,     0,     0,     0,     0,   179,   179,   179,   179,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,   119,   119,
     119,   119,     0,     0,     0,     0,     0,   183,   183,     0,
       0,     0,     0,     0,     0,   119,   119,   183,     0,   344,
       0,     0,     0,     0,   344,   320,   320,   183,     0,     0,
     183,     0,     0,     0,     0,     0,   120,   120,   120,   120,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,     0,   500,   120,   120,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   136,   782,     0,     0,     0,     0,
       0,   648,     0,     0,   351,     0,     0,     0,     0,    22,
       0,     0,     0,   179,     0,   137,   138,     0,   139,     0,
      30,     0,     0,     0,     0,    33,    34,   320,    36,     0,
      37,    38,     0,     0,     0,     0,     0,     0,    40,     0,
      42,     0,     0,     0,     0,   683,     0,     0,     0,    79,
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,   179,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,     0,     0,     0,     0,
     183,     0,     0,     0,     0,    68,     0,    69,     0,   360,
      71,   360,     0,     0,   360,    73,     0,   360,     0,   141,
       5,    75,     0,   142,     0,     0,   766,   690,     0,     0,
       0,     0,   691,     0,     0,     0,     0,     0,   344,   344,
       0,   344,   344,     0,     0,     0,   778,     0,     0,   320,
       0,     0,     0,     0,     0,     0,     0,   874,     0,   785,
       0,   789,     0,   878,     0,   344,    79,   344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   652,
       0,     0,     0,   351,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     673,     0,     0,     0,     0,   220,     0,     0,     0,   692,
     693,   694,   695,   696,   697,     0,     0,   698,   699,   700,
     701,   702,   703,   704,   705,   706,   707,     0,   874,   708,
       0,   727,     0,   709,   710,   711,   712,   713,   714,   715,
     716,   717,   718,   719,     0,   720,     0,     0,   721,   722,
     723,   724,     0,   725,     0,     0,   726,   840,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   845,     0,     0,
       0,   847,     0,     0,     0,    -2,     4,   496,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -280,    18,    19,    20,
       0,    21,    22,     0,    23,   351,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -167,    32,    33,    34,
      35,    36,   -41,    37,    38,   673,    39,     0,     0,     0,
    -280,    40,    41,    42,    43,  -167,    44,    45,   259,  -280,
      46,    47,     0,    48,    49,    50,    51,    52,    53,     0,
      54,     0,    55,    56,    57,     0,    58,    59,  -280,   801,
       0,    60,     0,    61,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,     0,     0,
     261,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   262,    72,     0,     0,    73,     0,
       0,     0,    74,     0,    75,     0,    76,     0,     0,   263,
       0,   264,   265,   266,     0,   267,   268,   269,     0,   270,
     271,   272,   273,     0,   275,   276,   277,   278,     0,   279,
     280,   281,     0,     0,   282,   283,   284,     4,   134,     5,
       6,     7,     8,     9,    10,     0,  -434,   851,    11,    12,
      13,  -434,     0,    14,    15,    16,    17,  -280,    18,    19,
      20,  -434,    21,    22,  -434,    23,     0,    24,    25,    26,
      27,    28,    29,     0,    30,    31,     0,  -167,    32,    33,
      34,    35,    36,   -41,    37,    38,     0,    39,     0,     0,
       0,  -280,    40,     0,    42,    43,  -167,    44,    45,  -434,
    -280,    46,    47,  -434,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -280,
       0,     0,    60,     0,    61,     0,     0,  -434,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -434,    64,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,     0,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,    69,  -434,  -434,  -434,     0,    72,  -434,  -434,  -434,
       0,     0,     0,    74,  -434,    75,     4,    76,     5,     6,
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
      69,     0,    70,    71,   195,    72,     0,     0,    73,     0,
       0,     0,    74,   231,    75,     4,    76,     5,     6,     7,
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
       0,    74,   388,    75,     4,    76,     5,     6,     7,     8,
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
      74,   231,    75,     4,    76,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,     0,    14,
      15,    16,    17,  -280,    18,    19,    20,     0,    21,    22,
       0,    23,     0,    24,    25,    26,    27,    28,    29,     0,
      30,    31,     0,  -167,    32,    33,    34,    35,    36,   -41,
      37,    38,     0,    39,     0,     0,     0,  -280,    40,    41,
      42,    43,  -167,    44,    45,     0,  -280,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -280,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
     685,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -280,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -167,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -280,    40,    41,    42,
      43,  -167,    44,    45,     0,  -280,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -280,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,   793,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -280,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -167,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -280,    40,   244,    42,    43,
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
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
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
      25,    26,    27,    28,   503,     0,    30,    31,     0,  -167,
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
      26,    27,    28,   507,     0,    30,    31,     0,  -167,    32,
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
      28,   753,     0,    30,    31,     0,  -167,    32,    33,    34,
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
      21,    22,     0,    23,     0,    24,    25,    26,    27,    28,
     756,     0,    30,    31,     0,  -167,    32,    33,    34,    35,
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
      22,     0,    23,     0,    24,    25,    26,    27,    28,   759,
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
      15,    16,    17,  -280,    18,    19,    20,     0,   777,    22,
       0,    23,     0,    24,    25,    26,    27,    28,    29,     0,
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
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -280,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,   784,     0,    30,
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
       0,    24,    25,    26,    27,    28,   788,     0,    30,    31,
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
     531,    76,     5,     6,     7,     8,     9,    10,     0,  -434,
       0,   136,     0,     0,  -434,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -434,     0,    22,     0,     0,     0,
       0,     0,   137,   138,     0,   139,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,     0,    40,     0,    42,     0,     0,
       0,     0,  -434,     0,     0,     0,  -434,     0,     0,    50,
      51,    52,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -298,
    -434,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -434,    64,  -434,  -434,  -434,
       0,  -434,  -434,  -434,     0,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,    69,  -434,  -434,  -434,     0,     0,
    -434,  -434,  -434,     0,     0,     0,   141,     0,    75,   531,
     548,     5,     6,     7,     8,     9,    10,     0,  -434,     0,
     136,     0,     0,  -434,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -434,     0,    22,     0,     0,     0,     0,
       0,   137,   138,     0,   139,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,  -434,     0,     0,     0,  -434,     0,     0,    50,    51,
      52,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -434,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -434,    64,  -434,  -434,  -434,     0,
    -434,  -434,  -434,     0,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,    69,  -434,  -434,  -434,     0,     0,  -434,
    -434,  -434,     0,     0,     0,   141,     0,    75,   531,   548,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     137,   138,     0,   139,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,     0,    40,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,    52,
     140,     0,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,     0,     0,     0,
       0,    67,     0,     0,   299,   300,     0,     0,     0,  -415,
      68,  -415,    69,     0,    70,    71,     0,     0,     0,     0,
      73,     0,     0,     0,   141,     0,    75,   531,   532,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   136,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   137,
     138,     0,   139,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,     0,    40,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    52,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,     0,     0,     0,    73,
       0,     0,     0,   141,     0,    75,     0,   548,     5,     6,
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
       0,     0,   141,     0,    75,     0,   142,   562,     5,     6,
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
       0,     0,   141,     0,    75,     0,   142,   868,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   136,     0,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   137,   138,
       0,   139,   470,    30,     0,   471,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,   472,
     473,    40,     0,    42,     0,     0,     0,     0,     0,   474,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,   475,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,     0,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,   199,   200,     0,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,   236,
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
     199,   200,     0,   235,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,     0,     0,     0,     0,    67,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,    69,     0,
      70,    71,   195,     0,     0,     0,    73,     0,     0,     0,
     141,     0,    75,   556,   142,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   136,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   137,   138,     0,   139,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,     0,    40,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,   199,
     200,     0,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   195,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,   558,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,    52,   140,     0,     0,     0,   199,   200,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,   201,     0,     0,    73,     0,     0,     0,   141,     0,
      75,     0,   142,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   137,   138,     0,   139,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,     0,    40,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,    52,   140,     0,     0,     0,   199,   200,     0,
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
       0,     0,    33,    34,     0,    36,     0,    37,    38,   664,
       0,     0,     0,     0,     0,    40,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   140,     0,     0,     0,   199,   200,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   141,     0,    75,     0,
     142,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   137,   138,     0,   139,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,   165,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   166,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,    70,    71,     0,     0,     0,
       0,    73,     0,     0,     0,   141,     0,    75,     0,   142,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   136,
       0,     0,     0,     0,     0,     0,     0,   170,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,   171,
     137,   138,     0,   139,     0,    30,     0,     0,     0,     0,
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
     138,     0,   139,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,     0,    40,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    52,   140,
       0,     0,     0,   199,   200,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,     0,     0,     0,    73,
       0,     0,     0,   141,     0,    75,     0,   142,     5,     6,
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
       0,     0,   141,   554,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     182,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,     0,     0,     0,    73,     0,     0,
       0,    74,     0,    75,     0,   142,     5,     6,     7,     8,
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
      70,    71,   195,     0,     0,     0,    73,     0,     0,     0,
     141,     0,    75,     0,   142,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   136,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   137,   138,     0,   139,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,     0,    40,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   825,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   826,    51,    52,   827,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
     195,     0,     0,     0,    73,     0,     0,     0,   141,     0,
      75,     0,   828,     5,     6,     7,     8,     9,    10,     0,
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
       0,   828,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   137,   138,     0,   814,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,     0,    40,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   195,     0,
       0,     0,    73,     0,     0,     0,   141,     0,    75,     0,
     142,     5,     6,     7,     8,     9,    10,     0,     0,     0,
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
       5,     6,     7,     8,     9,    10,     0,     0,     0,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     137,   138,     0,   139,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,     0,    40,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,    52,
     140,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    69,     0,    70,    71,     0,     0,     0,     0,
      73,     0,     0,     0,    74,     0,    75,     0,   142,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   136,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   137,
     138,     0,   139,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,     0,    40,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    52,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,     0,     0,     0,   513,
       0,     0,     0,   141,     0,    75,     0,   142,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   137,   138,
       0,   608,     0,    30,     0,     0,     0,     0,    33,    34,
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
     611,     0,    30,     0,     0,     0,     0,    33,    34,     0,
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
      22,     0,     0,     0,     0,     0,   137,   138,     0,   808,
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
       0,     0,     0,     0,     0,   137,   138,     0,   809,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,     0,    40,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   811,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,    52,   140,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,     0,     0,     0,    73,     0,     0,     0,   141,     0,
      75,     0,   142,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   137,   138,     0,   812,     0,    30,     0,
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
       0,     0,   137,   138,     0,   813,     0,    30,     0,     0,
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
       0,   137,   138,     0,   814,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,    70,    71,     0,     0,     0,
       0,    73,     0,     0,   259,   141,     0,    75,     0,   142,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     137,   138,   260,   139,     0,    30,   261,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       5,     0,     0,    40,     0,    42,     0,   690,     0,     0,
     262,     0,   691,     0,     0,     0,     0,    50,    51,    52,
     140,     0,     0,     0,     0,   263,     0,   264,   265,   266,
       0,   267,   268,   269,     0,   270,   271,   272,   273,  -307,
     275,   276,   277,   278,     0,   279,     0,   281,     0,     0,
     282,   283,   284,     0,    64,   257,     0,     0,  -307,     0,
     258,     0,  -307,     0,     0,     0,     0,     0,     0,     0,
     259,     0,    69,     0,     0,     0,   195,     0,     0,     0,
       0,     0,     0,     0,   141,     0,    75,  -307,   828,   692,
     693,   694,   695,   696,   697,     0,     0,   698,   699,   700,
     701,   702,   703,   704,   705,   706,   707,     0,   260,   708,
       0,     0,   261,   709,   710,   711,   712,   713,   714,   715,
     716,   717,   718,   719,     0,   720,     0,     0,   721,   722,
     723,   724,   337,   725,     0,   257,   262,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,   263,     0,   264,   265,   266,     0,   267,   268,   269,
       0,   270,   271,   272,   273,   274,   275,   276,   277,   278,
       0,   279,   280,   281,     0,     0,   282,   283,   284,   257,
       0,     0,     0,     0,   258,     0,     0,   681,   260,     0,
       0,     0,   261,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   263,   260,   264,   265,   266,   261,   267,   268,   269,
       0,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     257,   279,   280,   281,     0,   258,   282,   283,   284,     0,
     262,     0,     0,     0,     0,   259,     0,   743,     0,     0,
       0,     0,     0,     0,     0,   263,     0,   264,   265,   266,
       0,   267,   268,   269,     0,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   257,   279,   280,   281,     0,   258,
     282,   283,   284,   260,     0,     0,     0,   261,     0,   259,
       0,   877,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   263,   260,   264,   265,
     266,   261,   267,   268,   269,     0,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   257,   279,   280,   281,     0,
     258,   282,   283,   284,     0,   262,     0,     0,     0,     0,
     259,     0,   914,     0,     0,     0,     0,     0,     0,     0,
     263,     0,   264,   265,   266,     0,   267,   268,   269,     0,
     270,   271,   272,   273,   274,   275,   276,   277,   278,     0,
     279,   280,   281,     0,     0,   282,   283,   284,   260,     0,
       0,     0,   261,     0,   555,     0,     0,   257,     0,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   335,     0,     0,   262,     0,     0,     0,
       0,     0,     0,     0,   336,     0,     0,     0,     0,     0,
       0,   263,     0,   264,   265,   266,     0,   267,   268,   269,
       0,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     260,   279,   280,   281,   261,     0,   282,   283,   284,     0,
       0,     0,     0,     0,   257,   850,     0,     0,     0,   258,
       0,     0,     0,     0,   337,     0,     0,     0,   262,   259,
     332,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   333,     0,   263,     0,   264,   265,   266,     0,   267,
     268,   269,     0,   270,   271,   272,   273,   274,   275,   276,
     277,   278,     0,   279,   280,   281,     0,   260,   282,   283,
     284,   261,     0,     0,   308,     0,   257,     0,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   637,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     263,     0,   264,   265,   266,     0,   267,   268,   269,     0,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   260,
     279,   280,   281,   261,     0,   282,   283,   284,     0,     0,
       0,   308,     0,   257,     0,     0,     0,     0,   258,     0,
       0,     0,     0,   337,     0,     0,     0,   262,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   263,     0,   264,   265,   266,     0,   267,   268,
     269,     0,   270,   271,   272,   273,   274,   275,   276,   277,
     278,     0,   279,   280,   281,     0,   260,   282,   283,   284,
     261,     0,     0,   308,     0,   257,     0,   343,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,   182,     0,     0,   262,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,   264,   265,   266,     0,   267,   268,   269,     0,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   260,   279,
     280,   281,   261,     0,   282,   283,   284,   257,     0,     0,
     308,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   631,     0,     0,   262,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   263,     0,   264,   265,   266,     0,   267,   268,   269,
       0,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     260,   279,   280,   281,   261,     0,   282,   283,   284,   257,
       0,     0,   308,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,   262,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   263,     0,   264,   265,   266,     0,   267,
     268,   269,     0,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   260,   279,   280,   281,   261,     0,   282,   283,
     284,   257,     0,   750,   308,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   263,     0,   264,   265,   266,
       0,   267,   268,   269,     0,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   260,   279,   280,   281,   261,     0,
     282,   283,   284,   257,     0,   755,   308,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
       0,     0,   262,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   263,     0,   264,
     265,   266,     0,   267,   268,   269,     0,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   260,   279,   280,   281,
     261,     0,   282,   283,   284,   257,     0,   862,   308,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,   262,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,   264,   265,   266,     0,   267,   268,   269,     0,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   260,   279,
     280,   281,   261,     0,   282,   283,   284,   257,     0,   863,
     308,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,     0,     0,     0,   262,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   263,     0,   264,   265,   266,     0,   267,   268,   269,
       0,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     260,   279,   280,   281,   261,     0,   282,   283,   284,   257,
       0,   864,   308,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,   262,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   263,     0,   264,   265,   266,     0,   267,
     268,   269,     0,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   260,   279,   280,   281,   261,     0,   282,   283,
     284,   257,     0,   865,   308,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   263,     0,   264,   265,   266,
       0,   267,   268,   269,     0,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   260,   279,   280,   281,   261,     0,
     282,   283,   284,   257,     0,   879,   308,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
       0,     0,   262,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   263,     0,   264,
     265,   266,     0,   267,   268,   269,     0,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   260,   279,   280,   281,
     261,     0,   282,   283,   284,   257,     0,   880,   308,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,   262,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,   264,   265,   266,     0,   267,   268,   269,     0,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   260,   279,
     280,   281,   261,     0,   282,   283,   284,   257,     0,     0,
     308,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,     0,     0,     0,   262,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   263,     0,   264,   265,   266,     0,   267,   268,   269,
       0,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     260,   279,   280,   281,   261,     0,   282,   283,   284,     0,
       0,     0,   371,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   257,     0,     0,     0,   262,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,     0,     0,   263,   340,   264,   265,   266,     0,   267,
     268,   269,     0,   270,   271,   272,   273,   274,   275,   276,
     277,   278,     0,   279,   280,   281,   341,     0,   282,   283,
     284,     0,     0,  -391,     0,     0,     0,   260,     0,     0,
       0,   261,     0,     0,     0,     0,   257,     0,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   314,     0,     0,     0,     0,     0,     0,
     263,     0,   264,   265,   266,     0,   267,   268,   269,     0,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   260,
     279,   280,   281,   261,   342,   282,   283,   284,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   257,
       0,     0,     0,   130,   258,     0,     0,   262,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,     0,   384,
       0,     0,   263,     0,   264,   265,   266,     0,   267,   268,
     269,     0,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   385,   279,   280,   281,     0,     0,   282,   283,   284,
       0,     0,   260,     0,     0,     0,   261,     0,     0,     0,
       0,   257,     0,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,   481,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,   263,     0,   264,   265,   266,
       0,   267,   268,   269,     0,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   260,   279,   280,   281,   261,     0,
     282,   283,   284,   257,     0,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,   483,
       0,     0,   262,     0,     0,     0,     0,     0,     0,     0,
     484,     0,     0,     0,     0,     0,     0,   263,     0,   264,
     265,   266,     0,   267,   268,   269,     0,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   260,   279,   280,   281,
     261,     0,   282,   283,   284,   257,     0,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,   262,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,   264,   265,   266,     0,   267,   268,   269,     0,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   260,   279,
     280,   281,   261,     0,   282,   283,   284,   257,     0,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,     0,     0,     0,   262,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   263,     0,   264,   265,   266,     0,   267,   268,   269,
       0,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     260,   279,   280,   281,   261,   318,   282,   283,   284,   257,
       0,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,   262,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   263,     0,   264,   265,   266,     0,   267,
     268,   269,     0,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   260,   279,   280,   281,   261,   375,   282,   283,
     284,   257,     0,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   263,     0,   264,   265,   266,
       0,   267,   268,   269,     0,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   260,   279,   280,   281,   261,   377,
     282,   283,   284,   257,     0,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
       0,     0,   262,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   263,     0,   264,
     265,   266,     0,   267,   268,   269,     0,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   260,   279,   280,   281,
     261,   379,   282,   283,   284,   257,     0,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,   262,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,   264,   265,   266,     0,   267,   268,   269,     0,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   260,   279,
     280,   281,   261,   387,   282,   283,   284,   257,     0,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,     0,     0,   401,   262,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   263,     0,   264,   265,   266,     0,   267,   268,   269,
       0,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     260,   279,   280,   281,   261,     0,   282,   283,   284,   257,
       0,   485,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,   262,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   263,     0,   264,   265,   266,     0,   267,
     268,   269,     0,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   260,   279,   280,   281,   261,     0,   282,   283,
     284,     0,     0,     0,     0,     0,   257,     0,     0,     0,
       0,   258,     0,     0,     0,     0,   130,     0,     0,     0,
     262,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   263,     0,   264,   265,   266,
       0,   267,   268,   269,     0,   270,   271,   272,   273,   274,
     275,   276,   277,   278,     0,   279,   280,   281,     0,   260,
     282,   283,   284,   261,     0,     0,     0,     0,   257,   644,
       0,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,     0,     0,   262,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   263,     0,   264,   265,   266,     0,   267,   268,
     269,     0,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   260,   279,   280,   281,   261,   618,   282,   283,   284,
     257,     0,     0,     0,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,     0,     0,   647,   262,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   263,     0,   264,   265,   266,     0,
     267,   268,   269,     0,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   260,   279,   280,   281,   261,     0,   282,
     283,   284,   257,     0,     0,     0,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,     0,
       0,   262,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   263,     0,   264,   265,
     266,     0,   267,   268,   269,     0,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   260,   279,   280,   281,   261,
       0,   282,   283,   284,   257,     0,     0,     0,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
     738,     0,   684,   262,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   263,     0,
     264,   265,   266,     0,   267,   268,   269,     0,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   260,   279,   280,
     281,   261,     0,   282,   283,   284,   257,     0,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   741,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     263,     0,   264,   265,   266,     0,   267,   268,   269,     0,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   260,
     279,   280,   281,   261,     0,   282,   283,   284,   257,     0,
       0,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,     0,     0,   262,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   263,     0,   264,   265,   266,     0,   267,   268,
     269,     0,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   260,   279,   280,   281,   261,     0,   282,   283,   284,
     257,     0,   807,     0,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,     0,     0,     0,   262,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   263,     0,   264,   265,   266,     0,
     267,   268,   269,     0,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   260,   279,   280,   281,   261,     0,   282,
     283,   284,   257,   823,   810,     0,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,     0,
       0,   262,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   263,     0,   264,   265,
     266,     0,   267,   268,   269,     0,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   260,   279,   280,   281,   261,
       0,   282,   283,   284,   257,     0,     0,     0,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,     0,     0,   262,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   263,     0,
     264,   265,   266,     0,   267,   268,   269,     0,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   260,   279,   280,
     281,   261,     0,   282,   283,   284,   257,     0,   886,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     263,     0,   264,   265,   266,     0,   267,   268,   269,     0,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   260,
     279,   280,   281,   261,     0,   282,   283,   284,   257,     0,
     887,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,     0,     0,   262,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   263,     0,   264,   265,   266,     0,   267,   268,
     269,     0,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   260,   279,   280,   281,   261,     0,   282,   283,   284,
     257,     0,   888,     0,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,     0,     0,     0,   262,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   263,     0,   264,   265,   266,     0,
     267,   268,   269,     0,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   260,   279,   280,   281,   261,     0,   282,
     283,   284,   257,     0,   889,     0,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,     0,
       0,   262,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   263,     0,   264,   265,
     266,     0,   267,   268,   269,     0,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   260,   279,   280,   281,   261,
       0,   282,   283,   284,   257,     0,   890,     0,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,     0,     0,   262,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   263,     0,
     264,   265,   266,     0,   267,   268,   269,     0,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   260,   279,   280,
     281,   261,     0,   282,   283,   284,   257,     0,   891,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     263,     0,   264,   265,   266,     0,   267,   268,   269,     0,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   260,
     279,   280,   281,   261,     0,   282,   283,   284,   257,     0,
       0,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,     0,     0,   262,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   263,     0,   264,   265,   266,     0,   267,   268,
     269,     0,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   606,   279,   280,   281,   261,     0,   282,   283,   284,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   263,     0,   264,   265,   266,     0,
     267,   268,   269,     0,   270,   271,   272,   273,   274,   275,
     276,   277,   278,     0,   279,   280,   281,     0,     0,   282,
     283,   284
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-787))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,     2,    46,    43,    44,   355,    26,   350,   305,    11,
     661,   380,     1,    24,    16,    59,    15,    19,    20,    21,
     152,   347,     3,   241,    26,    27,    28,    29,   530,     3,
     380,   163,    76,    35,    36,    37,    26,    39,    40,   511,
       3,     3,     3,     1,    46,   175,    48,    49,    50,    26,
      26,    53,    54,    55,    56,     3,   687,    59,    60,    61,
     730,   191,    64,   111,   687,    67,    68,    69,    37,    71,
      37,    53,    74,    21,    76,    87,    78,    66,   111,   209,
     251,   252,   253,   254,    26,    37,    92,    76,    75,    37,
      64,   868,    40,    66,   142,   836,    85,    87,   142,    80,
      41,   341,    84,   130,     0,   138,    54,    55,    66,   115,
       1,    79,   139,    37,   141,     1,    64,   903,    87,    60,
     771,   110,    37,    37,     3,   111,   903,    85,   914,   131,
     142,    79,   604,     3,   136,   137,   138,   139,   140,   141,
     142,   130,   111,    37,   111,   385,    37,   137,   137,   148,
      64,    37,   110,    87,   140,   657,   138,    27,   139,   111,
     137,   137,    66,    13,   137,   139,   907,   798,    18,   111,
     157,   301,   302,   142,   176,   137,   139,   139,   139,   137,
     182,   192,   852,   853,   815,     3,   529,   111,   175,     7,
     142,   139,   815,   111,    21,   137,   111,   199,   200,    90,
     111,    92,    47,     3,   191,    72,    92,     7,    25,    79,
     139,    63,   130,   139,   511,    67,   867,   111,   142,   110,
     111,    17,   209,    41,   110,   111,   130,   142,    55,   140,
      75,   233,   233,   137,   236,    55,    56,    64,   409,   130,
     260,   261,    60,   373,   130,     7,    63,   139,   142,   141,
      67,   139,    79,   141,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,    62,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   303,   304,    81,   330,   393,   139,   395,
     762,   509,   510,   110,   314,   111,   113,   111,   800,   139,
     117,   303,   304,   305,   130,   130,   308,   604,   651,   111,
     660,    21,   314,   333,   301,   302,   336,   319,    21,    47,
     139,   111,   141,   649,   140,   130,   140,   111,   330,   111,
     332,   333,    21,   335,   336,   136,   138,   130,   340,   341,
       2,   343,   560,   561,    92,    55,   130,   130,    37,    11,
     140,    40,    55,   355,    64,   111,   358,    19,   140,    21,
     130,    64,   406,   111,   366,    54,    55,   115,   111,   111,
      92,   130,   111,    83,   139,    64,    79,   397,   380,   366,
     130,    63,   384,   385,   140,    67,   373,   130,   390,   111,
      79,    53,   140,   115,   111,   397,   398,   399,   140,   401,
     402,   140,   111,   390,   406,   111,   393,   776,   395,   130,
     137,   398,    74,   130,   783,   111,    78,    20,   140,   551,
     770,   130,    86,   137,   130,   113,   776,   139,    31,    32,
     130,   139,   110,   783,   130,   113,    39,   111,    41,   117,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   130,    60,   139,    92,
     130,     7,   482,   110,   484,   762,    92,   130,   140,   131,
     111,   130,    37,   139,    28,    20,   111,    49,   804,   481,
     482,   483,   484,   485,   486,   487,    31,    32,   532,   488,
     489,   140,   140,    25,    39,    90,    41,   140,    92,   501,
     140,   503,    47,    16,   548,   507,   493,   111,   495,   511,
     142,   130,   137,   137,   176,    60,   130,   130,   868,   521,
     182,   130,   130,   130,   130,   130,   142,   877,   130,   130,
     532,    63,   130,   535,   130,    67,    92,   130,     3,   130,
     130,   130,     7,    37,     9,   130,   548,   111,   393,     2,
     395,   130,   140,   903,   130,    28,    21,   140,    11,   140,
     130,   130,   111,   565,   914,    64,    19,   569,    21,    26,
      26,   233,    66,   110,   142,   137,    64,   110,   110,   142,
     112,   113,    76,   142,   113,   117,   606,   607,   113,   139,
      55,    85,   124,   140,    85,   167,   168,   169,    92,    64,
      53,   133,   604,   569,   606,   607,   608,    76,   385,   611,
     140,   384,   671,   185,    79,   500,   188,   111,    83,    25,
     622,    74,   164,   667,   748,    78,   623,   629,   803,   631,
     217,   803,   634,   649,   206,   637,   130,     3,   804,   689,
     642,     7,   644,   137,   646,   647,   308,   907,   493,   882,
     867,    28,   224,   495,   359,    21,   824,    63,   660,    -1,
      -1,    67,    -1,    -1,   666,   667,    -1,    -1,    -1,    -1,
     332,    -1,    -1,   335,    -1,    -1,    -1,   679,   131,   681,
      -1,   343,   684,    -1,   686,   686,    -1,    -1,    -1,    55,
      -1,   536,    -1,     2,    -1,    -1,    -1,    -1,    64,    -1,
     106,    -1,    11,   109,   110,    -1,   112,   113,    -1,    -1,
      19,   117,    21,    79,    -1,    -1,    -1,    83,   124,    -1,
      -1,    -1,    -1,   176,   726,   131,   132,   133,    25,   182,
      -1,    -1,    -1,    -1,    -1,   737,   738,   399,   740,   741,
     742,   743,    -1,   315,    53,    -1,    55,    56,   750,   751,
      -1,   753,    -1,   755,   756,   795,   796,   759,    -1,    -1,
     762,    -1,    -1,    -1,    -1,    74,    63,   339,   770,    78,
      67,    -1,    -1,   775,   776,   777,    -1,    -1,    -1,   781,
     233,   783,   784,   770,   828,   787,   788,    -1,    -1,   791,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   807,   808,   809,   810,   811,
     812,   813,   814,   110,   476,   112,   113,    37,    -1,    -1,
     117,   823,   131,    -1,   826,   827,   828,   124,    -1,    -1,
     832,    20,     2,   835,   131,   132,   133,    -1,    -1,    -1,
      -1,    11,    31,    32,    -1,   832,    66,    -1,    -1,    19,
      39,    21,    41,    -1,    -1,   308,    76,    -1,    47,   521,
     862,   863,   864,   865,    -1,    85,   868,   176,    -1,    -1,
      -1,    60,    92,   182,    -1,   877,    -1,   879,   880,   332,
      -1,   868,   335,    53,   886,   887,   888,   889,   890,   891,
     343,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     902,   903,     2,   565,    74,    -1,    -1,   569,    78,    -1,
     130,    11,   914,    -1,    -1,   577,   903,   137,    37,    19,
      -1,    21,    -1,    -1,   233,    -1,    -1,   914,    -1,    -1,
      -1,    -1,    -1,   505,   506,    -1,    -1,   509,   510,    -1,
      -1,    -1,    -1,    -1,   606,   607,   399,    66,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    76,    -1,    -1,
      -1,   131,    -1,    -1,    -1,    17,    85,   629,    20,   631,
      -1,    -1,   634,    92,    74,   637,    -1,    29,    78,    31,
      32,    -1,    -1,    35,   646,    -1,    -1,    39,    -1,    41,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,   308,
      -1,    -1,    -1,    -1,   666,    57,   176,    59,    60,    61,
      62,   130,   182,    -1,    -1,    -1,    -1,   679,   137,   681,
      -1,    -1,    -1,   332,   686,    -1,   335,    79,    -1,    81,
      -1,   131,    -1,    -1,   343,    -1,    -1,    -1,    -1,    -1,
      -1,     2,    -1,    -1,   616,   617,    -1,    25,    -1,    -1,
      11,    -1,    -1,    -1,   626,    -1,    -1,    -1,    19,    17,
      21,    -1,    20,   233,   636,    -1,    -1,   639,   521,    -1,
      -1,    29,    -1,    31,    32,    -1,   176,    35,   130,    -1,
      -1,    39,   182,    41,    -1,    63,   138,    -1,   750,    67,
     399,    -1,    53,   755,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    60,    61,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   565,    74,    -1,   777,   569,    78,    -1,    -1,
      -1,    79,    -1,    81,    -1,   787,    -1,    -1,   106,   791,
     108,   109,   110,   233,   112,   113,   114,    -1,   308,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
     128,    -1,    -1,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,   332,    -1,    -1,   335,    -1,    -1,    -1,    -1,
     131,    -1,   130,   343,    -1,    -1,   629,    -1,   631,    -1,
     138,   634,    -1,    -1,   637,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   646,    -1,    -1,    -1,    -1,    -1,    -1,
     862,   863,   864,   865,    -1,    -1,    -1,   779,   308,    -1,
      -1,    -1,   521,   666,    -1,   176,    -1,   879,   880,    -1,
      -1,   182,    -1,    -1,    -1,    -1,   679,    -1,   681,   399,
      -1,    -1,   332,   686,    -1,   335,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   343,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,   565,    -1,    -1,    -1,
     569,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,   233,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    25,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,   750,    56,   399,
      58,    -1,   755,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    73,    -1,    -1,    -1,    63,
     629,    -1,   631,    67,   777,   634,    -1,    -1,   637,    -1,
      -1,    -1,    -1,    -1,   787,    -1,    -1,   646,   791,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,   107,
      -1,   521,    -1,    -1,    -1,    -1,    -1,   666,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,   125,   112,   113,
     679,   332,   681,   117,   335,    -1,    -1,   686,    -1,   123,
     124,   139,   343,   141,   128,    -1,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,   565,    -1,    -1,     2,   569,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,   862,
     863,   864,   865,    -1,    -1,    19,    -1,    21,    -1,    -1,
      -1,   521,    -1,    -1,    -1,    -1,   879,   880,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,   399,    -1,
      -1,   750,    -1,    -1,    -1,    11,   755,    -1,    -1,    53,
      -1,    -1,    -1,    19,    -1,    21,    -1,    -1,    -1,   629,
      -1,   631,    -1,    -1,   634,   565,    -1,   637,   777,   569,
      74,    -1,    -1,    -1,    78,    -1,   646,    17,   787,    -1,
      20,    -1,   791,    -1,    -1,    -1,    -1,    53,    -1,    29,
      -1,    31,    32,    -1,    -1,    35,   666,    -1,    -1,    39,
      -1,    41,    -1,    -1,    -1,    -1,    25,    -1,    74,   679,
      -1,   681,    78,    -1,    -1,    -1,   686,    57,    -1,    59,
      60,    61,    62,    -1,    -1,    -1,    -1,   131,    -1,   629,
      -1,   631,    -1,    -1,   634,    -1,    -1,   637,    -1,    79,
      -1,    81,    -1,    -1,    63,    -1,   646,    -1,    67,    -1,
      25,    -1,    -1,   862,   863,   864,   865,    -1,    -1,    -1,
     521,    -1,    -1,    -1,    -1,   131,   666,    -1,    -1,    -1,
     879,   880,   176,    -1,    -1,    -1,    -1,    -1,   182,   679,
     750,   681,    -1,    -1,    -1,   755,   686,   106,    63,    -1,
     130,   110,    67,   112,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,   565,   124,    -1,   777,   569,    -1,
     176,    -1,   131,   132,   133,    -1,   182,   787,    -1,    -1,
      -1,   791,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   233,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
     750,   126,    -1,   128,    -1,   755,   131,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   233,   629,    -1,
     631,    -1,    -1,   634,    -1,    -1,   637,   777,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   646,    35,   787,    -1,    -1,
      -1,   791,   862,   863,   864,   865,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,   308,   666,    55,    56,    -1,   879,
     880,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   679,    -1,
     681,    -1,    -1,    -1,    -1,   686,    -1,    -1,   332,    -1,
      -1,   335,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   343,
      -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,   862,   863,   864,   865,   332,    -1,    -1,   335,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   343,    -1,   879,
     880,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,   750,
      -1,    -1,    -1,    -1,   755,   399,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     2,    -1,    -1,    -1,   777,    -1,   167,   168,
     169,    11,    -1,    -1,    -1,   174,   787,    -1,    -1,    19,
     791,    21,    -1,   399,    -1,    -1,   185,    -1,    -1,   188,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,    -1,   117,    -1,    -1,    -1,   206,    -1,   123,
     124,    -1,    -1,    53,   128,    -1,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,   224,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,   862,   863,   864,   865,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,   521,   879,   880,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,   131,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   521,    -1,    70,    71,    72,
      73,   565,    -1,    -1,    -1,   569,   315,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,
     339,    -1,   182,   106,   107,   108,   109,    13,    -1,   565,
      -1,   114,    18,   569,    -1,    -1,    -1,    23,    24,    25,
     123,    -1,   125,    -1,   127,   128,    32,    -1,    34,    -1,
     133,    -1,    25,    -1,   137,   629,   139,   631,   141,   142,
     634,    -1,    -1,   637,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,   646,   233,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      63,    -1,   666,   629,    67,   631,    -1,    -1,   634,    -1,
      -1,   637,    -1,    -1,    -1,   679,    -1,   681,    -1,    -1,
     646,    -1,   686,    99,    -1,    -1,   134,   135,   136,    -1,
      -1,   139,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
     666,    -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,    -1,    -1,   679,   117,   681,    -1,    -1,   308,    -1,
     686,   124,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,   157,   332,    -1,    -1,   335,   750,   163,    -1,   165,
      -1,   755,    -1,    -1,    -1,    -1,   505,   506,    -1,   175,
     509,   510,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   777,    -1,   191,   192,    -1,    -1,   195,
      -1,    -1,    -1,   787,   750,    -1,    -1,   791,    -1,   755,
      -1,    -1,    -1,   209,    -1,    -1,    -1,   546,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   399,
      -1,   777,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   787,    -1,    -1,    -1,   791,    -1,    -1,    -1,    -1,
     246,    -1,    -1,    -1,    -1,   251,   252,   253,   254,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,   862,   863,
     864,   865,    -1,    -1,    -1,    -1,    -1,   616,   617,    -1,
      -1,    -1,    -1,    -1,    -1,   879,   880,   626,    -1,   628,
      -1,    -1,    -1,    -1,   633,   301,   302,   636,    -1,    -1,
     639,    -1,    -1,    -1,    -1,    -1,   862,   863,   864,   865,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     326,    -1,   328,   879,   880,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,   674,    -1,    -1,    -1,    -1,
      -1,   521,    -1,    -1,   350,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,   359,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,   373,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      58,    -1,    -1,    -1,    -1,   565,    -1,    -1,    -1,   569,
      -1,    -1,    70,    71,    72,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   409,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     779,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,   629,
     128,   631,    -1,    -1,   634,   133,    -1,   637,    -1,   137,
       3,   139,    -1,   141,    -1,    -1,   646,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,   817,   818,
      -1,   820,   821,    -1,    -1,    -1,   666,    -1,    -1,   495,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   836,    -1,   679,
      -1,   681,    -1,   842,    -1,   844,   686,   846,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   525,
      -1,    -1,    -1,   529,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     546,    -1,    -1,    -1,    -1,   551,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    -1,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,    -1,   907,   112,
      -1,   577,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    -1,   128,    -1,    -1,   131,   132,
     133,   134,    -1,   136,    -1,    -1,   139,   777,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   787,    -1,    -1,
      -1,   791,    -1,    -1,    -1,     0,     1,   623,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,   651,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,   671,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    25,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    -1,
      75,    -1,    77,    78,    79,    -1,    81,    82,    83,   705,
      -1,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      67,   106,   107,   108,   109,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
     125,    -1,   127,   128,    91,   130,    -1,    -1,   133,    -1,
      -1,    -1,   137,    -1,   139,    -1,   141,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,    -1,    -1,   131,   132,   133,     1,   794,     3,
       4,     5,     6,     7,     8,    -1,    10,   803,    12,    13,
      14,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    75,    -1,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,   133,
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
     125,    -1,   127,   128,   129,   130,    -1,    -1,   133,    -1,
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
     138,   139,     1,   141,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    -1,    75,    -1,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    -1,    88,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,   128,
      -1,   130,    -1,    -1,   133,    -1,    -1,    -1,   137,   138,
     139,     1,   141,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
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
       1,   141,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    67,    -1,    -1,    70,
      71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,    -1,
     131,   132,   133,    -1,    -1,    -1,   137,    -1,   139,     1,
     141,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    67,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,   131,
     132,   133,    -1,    -1,    -1,   137,    -1,   139,     1,   141,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      73,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,    -1,
      -1,   114,    -1,    -1,   134,   135,    -1,    -1,    -1,   139,
     123,   141,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,   137,    -1,   139,     1,   141,     3,
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
     125,    -1,   127,   128,   129,    -1,    -1,    -1,   133,    -1,
      -1,    -1,   137,    -1,   139,    -1,   141,   142,     3,     4,
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
      -1,    -1,   137,    -1,   139,    -1,   141,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    36,    37,    38,    -1,    40,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    54,
      55,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    70,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
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
      -1,    77,    78,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,    -1,    -1,    -1,    -1,   114,   115,
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
      77,    78,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,
     127,   128,   129,    -1,    -1,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    73,    -1,    -1,    -1,    77,
      78,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,
     128,   129,    -1,    -1,    -1,   133,    -1,    -1,    -1,   137,
      -1,   139,   140,   141,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    72,    73,    -1,    -1,    -1,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,   128,
      -1,   130,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,
     139,    -1,   141,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    72,    73,    -1,    -1,    -1,    77,    78,    -1,
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
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    50,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    72,    73,    -1,    -1,    -1,    77,    78,    -1,    -1,
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
      -1,    -1,    -1,    55,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,   141,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    32,
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
      -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,
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
     125,    -1,   127,   128,   129,    -1,    -1,    -1,   133,    -1,
      -1,    -1,   137,   138,   139,    -1,   141,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
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
     127,   128,   129,    -1,    -1,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,    -1,   141,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   123,    -1,   125,    -1,   127,   128,   129,    -1,
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
      -1,   133,    -1,    -1,    25,   137,    -1,   139,    -1,   141,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    63,    36,    -1,    38,    67,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
       3,    -1,    -1,    56,    -1,    58,    -1,    10,    -1,    -1,
      91,    -1,    15,    -1,    -1,    -1,    -1,    70,    71,    72,
      73,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,    92,
     121,   122,   123,   124,    -1,   126,    -1,   128,    -1,    -1,
     131,   132,   133,    -1,   107,    10,    -1,    -1,   111,    -1,
      15,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,   125,    -1,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,   139,   140,   141,    92,
      93,    94,    95,    96,    97,    -1,    -1,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,    -1,    63,   112,
      -1,    -1,    67,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    -1,   128,    -1,    -1,   131,   132,
     133,   134,    87,   136,    -1,    10,    91,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,   126,   127,   128,    -1,    -1,   131,   132,   133,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,   142,    63,    -1,
      -1,    -1,    67,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    63,   108,   109,   110,    67,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      10,   126,   127,   128,    -1,    15,   131,   132,   133,    -1,
      91,    -1,    -1,    -1,    -1,    25,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    10,   126,   127,   128,    -1,    15,
     131,   132,   133,    63,    -1,    -1,    -1,    67,    -1,    25,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    63,   108,   109,
     110,    67,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    10,   126,   127,   128,    -1,
      15,   131,   132,   133,    -1,    91,    -1,    -1,    -1,    -1,
      25,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    -1,
     126,   127,   128,    -1,    -1,   131,   132,   133,    63,    -1,
      -1,    -1,    67,    -1,   140,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    26,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,    -1,   131,   132,   133,    -1,
      -1,    -1,    -1,    -1,    10,   140,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    -1,   126,   127,   128,    -1,    63,   131,   132,
     133,    67,    -1,    -1,   137,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,    -1,   131,   132,   133,    -1,    -1,
      -1,   137,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,   126,   127,   128,    -1,    63,   131,   132,   133,
      67,    -1,    -1,   137,    -1,    10,    -1,    74,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,    -1,   131,   132,   133,    10,    -1,    -1,
     137,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    26,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,    -1,   131,   132,   133,    10,
      -1,    -1,   137,    -1,    15,    -1,    -1,    -1,    -1,    -1,
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
      67,    -1,   131,   132,   133,    10,    -1,    74,   137,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,    -1,   131,   132,   133,    10,    -1,    -1,
     137,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      63,   126,   127,   128,    67,    -1,   131,   132,   133,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    91,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,   106,    30,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    -1,   126,   127,   128,    52,    -1,   131,   132,
     133,    -1,    -1,   136,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    87,    15,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    30,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    52,   126,   127,   128,    -1,    -1,   131,   132,   133,
      -1,    -1,    63,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    63,   126,   127,   128,    67,    -1,
     131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    63,   126,   127,   128,
      67,    -1,   131,   132,   133,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    63,   126,
     127,   128,    67,    -1,   131,   132,   133,    10,    -1,    -1,
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
     133,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,    -1,   126,   127,   128,    -1,    63,
     131,   132,   133,    67,    -1,    -1,    -1,    -1,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,   130,   131,   132,   133,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    63,   126,   127,   128,    67,    -1,   131,
     132,   133,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    63,   126,   127,   128,    67,
      -1,   131,   132,   133,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    63,   126,   127,
     128,    67,    -1,   131,   132,   133,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    63,
     126,   127,   128,    67,    -1,   131,   132,   133,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
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
     132,   133,    10,    11,    74,    -1,    -1,    15,    -1,    -1,
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
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    63,   126,   127,   128,    67,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    -1,   126,   127,   128,    -1,    -1,   131,
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
     242,   243,   244,   251,   252,   253,   254,   256,   257,   258,
     259,   260,   261,   263,   264,   265,   266,   267,   151,   254,
      87,   245,   246,   162,   163,   245,    12,    33,    34,    36,
      73,   137,   141,   190,   239,   243,   252,   253,   254,   256,
     258,   259,    79,   162,   151,   254,   151,   139,   163,     7,
     162,   164,     9,    79,   164,    55,    89,   172,   254,   254,
      20,    32,   220,   254,   254,   139,   163,   191,   139,   163,
     222,   223,    26,   154,   165,   254,   254,   254,   254,   254,
       7,   139,    32,   168,   168,   129,   213,   233,   254,    77,
      78,   130,   254,   255,   254,   165,   254,   254,    72,   139,
     151,   254,   254,   154,   161,   254,   256,   154,   161,   254,
     163,   218,   233,   254,   254,   254,   254,   254,   254,   254,
     254,   138,   149,   155,   233,    80,   115,   213,   234,   235,
     255,   233,   254,   262,    57,   151,    47,   163,    41,    60,
     208,    21,    55,    64,    83,   130,   136,    10,    15,    25,
      63,    67,    91,   106,   108,   109,   110,   112,   113,   114,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   126,
     127,   128,   131,   132,   133,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   134,
     135,   139,   141,    63,    67,   139,   151,   130,   137,   154,
     254,   254,   254,   233,    37,   245,   218,   130,   130,    86,
     163,   213,   236,   237,   238,   255,   137,   218,   184,   163,
     139,   165,    26,    37,   165,    26,    37,    87,   165,   248,
      30,    52,   130,    74,   154,   236,   151,   139,   202,    80,
     139,   163,   224,   225,     1,   110,   227,   228,    37,   111,
     151,   165,   165,   236,   164,   163,   111,   130,   254,   254,
     130,   137,   165,   139,   236,   130,   176,   130,   176,   130,
      92,   219,   130,   130,    30,    52,   165,   130,   138,   149,
     111,   138,   254,   111,   140,   111,   140,    37,   111,   142,
     248,    90,   111,   142,     7,   163,   110,   185,   195,    64,
     222,   222,   222,   222,   254,   254,   254,   254,   172,   254,
     172,   254,   254,   254,   254,   254,   254,   254,    27,    79,
     163,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   236,   236,   172,   254,   172,   254,    21,
      37,    40,    54,    55,    64,    79,   206,   247,   250,   254,
     267,    26,    37,    26,    37,    74,    37,   142,   172,   254,
     165,   130,   254,    92,   140,   111,   163,   188,   189,   130,
     163,    37,   233,    36,   254,   172,   254,    36,   254,   172,
     254,   139,   165,   133,   156,   158,   254,   156,   157,   151,
     254,    28,   140,   203,   204,   205,   206,   192,   225,   111,
     140,     1,   141,   229,   240,    90,    92,   226,   254,   223,
     140,   213,   254,   181,   236,   140,    16,   177,   141,   229,
     240,   111,   158,   157,   138,   140,   140,   234,   140,   234,
     172,   254,   142,   151,   254,   142,   254,   142,   254,   137,
     233,   137,    21,    55,    64,    79,   197,   207,   222,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   140,
     142,    37,    64,   239,   111,   140,    63,    63,    36,   172,
     254,    36,   172,   254,   172,   254,   245,   245,   130,   213,
     255,   238,    92,   111,   138,   185,   254,   140,   254,    26,
     165,    26,   165,   254,    26,   165,   248,    26,   165,   248,
     249,   250,   111,   130,    11,   130,    28,    28,   151,   111,
     140,   139,   163,    21,    55,    64,    79,   209,   140,   225,
     110,   228,   233,   254,    50,   255,    53,    84,   138,   182,
     140,   139,   154,   163,   178,   233,   218,   130,   130,   142,
     248,   142,   248,   151,    90,   138,   155,   186,    64,   196,
      10,    15,    92,    93,    94,    95,    96,    97,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   112,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     128,   131,   132,   133,   134,   136,   139,   163,   198,   199,
     200,   239,   130,   250,   239,   239,   254,    26,    26,   254,
      26,    26,   142,   142,   165,   165,   254,   189,   137,   165,
      74,    36,   254,    36,   254,    74,    36,   254,   165,    36,
     254,   165,   111,   140,   254,   254,   151,   254,   204,   225,
     110,   232,    64,   228,   226,    37,   142,    26,   151,   233,
     178,   110,   154,   142,    36,   151,   254,   142,    36,   151,
     254,   142,   254,   138,    32,    59,    61,   138,   150,   167,
     209,   163,   254,   113,   139,   201,   201,    74,    36,    36,
      74,    36,    36,    36,    36,   186,   254,   254,   254,   254,
     254,   254,   250,    11,   140,    27,    70,    73,   141,   213,
     231,   240,   115,   215,   226,    85,   216,   254,   229,   240,
     151,   165,   140,   254,   254,   151,   254,   151,   167,   228,
     140,   163,   199,   200,   203,   254,   254,   254,   254,   254,
     254,   138,    74,    74,    74,    74,   254,   232,   142,   233,
     213,   214,   254,   254,   154,   166,   212,   142,   154,    74,
      74,    76,   210,   201,   201,   140,    74,    74,    74,    74,
      74,    74,   254,   254,   254,   254,   215,   226,   213,   230,
     231,   240,    37,   142,   240,   254,   254,   216,   254,   230,
     231,   130,   211,   212,   142,   230
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
#line 439 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 444 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 445 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 452 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 457 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 458 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 477 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 478 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 479 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 483 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 489 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 491 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 492 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 497 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), currentModuleType, new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), currentModuleType, (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 520 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 521 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 526 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 542 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 546 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 553 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 562 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 590 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 592 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 594 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 596 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 598 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 600 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 621 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 625 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 626 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 643 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 656 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 657 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 658 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 664 "chapel.ypp"
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
#line 678 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 679 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 680 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 681 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 682 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 683 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 684 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 686 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 695 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 701 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 707 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 713 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 719 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 726 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 749 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 765 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 782 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 790 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 792 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 799 "chapel.ypp"
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
#line 809 "chapel.ypp"
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
#line 821 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 826 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 831 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 839 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 840 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 845 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 847 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 849 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 854 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 864 "chapel.ypp"
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
#line 873 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 877 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 890 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 895 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 900 "chapel.ypp"
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
#line 920 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 926 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 933 "chapel.ypp"
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
#line 941 "chapel.ypp"
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
#line 953 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 959 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 966 "chapel.ypp"
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
#line 988 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 992 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 997 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 1001 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 1035 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 1036 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 1037 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1042 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1043 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1044 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1045 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1046 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1047 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1048 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1049 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1050 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1051 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1052 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1053 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1057 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1058 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1062 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1066 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1067 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1068 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1073 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1075 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1077 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1079 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1083 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1089 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1090 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1091 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1101 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1102 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1103 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1123 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1149 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1150 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1155 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1160 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1162 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1169 "chapel.ypp"
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
#line 1182 "chapel.ypp"
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
#line 1198 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1207 "chapel.ypp"
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
#line 1215 "chapel.ypp"
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
#line 1223 "chapel.ypp"
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
#line 1231 "chapel.ypp"
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
#line 1240 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1249 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1250 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1251 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1257 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1265 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1267 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      USR_WARN(var, "support for '=>' to alias arrays is deprecated as of chpl version 1.15.  Use a 'ref' declaration instead.");
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1313 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1342 "chapel.ypp"
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
#line 1349 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1354 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1376 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1388 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1389 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1390 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1391 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1395 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1396 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1397 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1402 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1406 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1411 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 1412 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1416 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1417 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1418 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1419 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1423 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1442 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1444 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1446 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1448 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1452 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 1459 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 354:

/* Line 1806 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1465 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1471 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1481 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1488 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1494 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1500 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1506 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1557 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1566 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1569 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1574 "chapel.ypp"
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[(1) - (2)].pt); (yyval.pIntentExpr).iVar = (yyvsp[(2) - (2)].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1577 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1578 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1612 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1614 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1616 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1618 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1620 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1625 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1626 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1648 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1649 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1654 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1665 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1666 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1667 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1671 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1672 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1676 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1681 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1685 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1692 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 1698 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 1703 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 1705 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 452:

/* Line 1806 of yacc.c  */
#line 1709 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 453:

/* Line 1806 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 454:

/* Line 1806 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 455:

/* Line 1806 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 456:

/* Line 1806 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:

/* Line 1806 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 458:

/* Line 1806 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 459:

/* Line 1806 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 460:

/* Line 1806 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 461:

/* Line 1806 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 462:

/* Line 1806 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 463:

/* Line 1806 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 464:

/* Line 1806 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 465:

/* Line 1806 of yacc.c  */
#line 1725 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 466:

/* Line 1806 of yacc.c  */
#line 1726 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 467:

/* Line 1806 of yacc.c  */
#line 1727 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 468:

/* Line 1806 of yacc.c  */
#line 1728 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 469:

/* Line 1806 of yacc.c  */
#line 1732 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 470:

/* Line 1806 of yacc.c  */
#line 1733 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 471:

/* Line 1806 of yacc.c  */
#line 1734 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 472:

/* Line 1806 of yacc.c  */
#line 1735 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 473:

/* Line 1806 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 474:

/* Line 1806 of yacc.c  */
#line 1740 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 475:

/* Line 1806 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 476:

/* Line 1806 of yacc.c  */
#line 1742 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 477:

/* Line 1806 of yacc.c  */
#line 1747 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 478:

/* Line 1806 of yacc.c  */
#line 1748 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 479:

/* Line 1806 of yacc.c  */
#line 1749 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 480:

/* Line 1806 of yacc.c  */
#line 1750 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 481:

/* Line 1806 of yacc.c  */
#line 1751 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 482:

/* Line 1806 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 483:

/* Line 1806 of yacc.c  */
#line 1753 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 8460 "bison-chapel.cpp"
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



