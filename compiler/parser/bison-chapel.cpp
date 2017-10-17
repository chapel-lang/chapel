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
     TPROTOTYPE = 316,
     TPUBLIC = 317,
     TRECORD = 318,
     TREDUCE = 319,
     TREF = 320,
     TREQUIRE = 321,
     TRETURN = 322,
     TSCAN = 323,
     TSELECT = 324,
     TSERIAL = 325,
     TSINGLE = 326,
     TSPARSE = 327,
     TSUBDOMAIN = 328,
     TSYNC = 329,
     TTHEN = 330,
     TTHROW = 331,
     TTHROWS = 332,
     TTRY = 333,
     TTRYBANG = 334,
     TTYPE = 335,
     TUNDERSCORE = 336,
     TUNION = 337,
     TUSE = 338,
     TVAR = 339,
     TWHEN = 340,
     TWHERE = 341,
     TWHILE = 342,
     TWITH = 343,
     TYIELD = 344,
     TZIP = 345,
     TALIAS = 346,
     TAND = 347,
     TASSIGN = 348,
     TASSIGNBAND = 349,
     TASSIGNBOR = 350,
     TASSIGNBXOR = 351,
     TASSIGNDIVIDE = 352,
     TASSIGNEXP = 353,
     TASSIGNLAND = 354,
     TASSIGNLOR = 355,
     TASSIGNMINUS = 356,
     TASSIGNMOD = 357,
     TASSIGNMULTIPLY = 358,
     TASSIGNPLUS = 359,
     TASSIGNSL = 360,
     TASSIGNSR = 361,
     TBAND = 362,
     TBNOT = 363,
     TBOR = 364,
     TBXOR = 365,
     TCOLON = 366,
     TCOMMA = 367,
     TDIVIDE = 368,
     TDOT = 369,
     TDOTDOT = 370,
     TDOTDOTDOT = 371,
     TEQUAL = 372,
     TEXP = 373,
     TGREATER = 374,
     TGREATEREQUAL = 375,
     THASH = 376,
     TLESS = 377,
     TLESSEQUAL = 378,
     TMINUS = 379,
     TMOD = 380,
     TNOT = 381,
     TNOTEQUAL = 382,
     TOR = 383,
     TPLUS = 384,
     TQUESTION = 385,
     TSEMI = 386,
     TSHIFTLEFT = 387,
     TSHIFTRIGHT = 388,
     TSTAR = 389,
     TSWAP = 390,
     TASSIGNREDUCE = 391,
     TIO = 392,
     TLCBR = 393,
     TRCBR = 394,
     TLP = 395,
     TRP = 396,
     TLSBR = 397,
     TRSBR = 398,
     TNOELSE = 399,
     TUMINUS = 400,
     TUPLUS = 401
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
#define YYLAST   13323

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  147
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  123
/* YYNRULES -- Number of rules.  */
#define YYNRULES  484
/* YYNRULES -- Number of states.  */
#define YYNSTATES  914

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   401

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
     145,   146
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
     113,   121,   122,   124,   126,   127,   129,   132,   136,   138,
     141,   143,   147,   151,   157,   158,   160,   162,   164,   168,
     174,   180,   184,   189,   194,   199,   204,   209,   214,   219,
     224,   229,   234,   239,   244,   249,   254,   259,   264,   265,
     267,   269,   271,   273,   276,   278,   281,   285,   287,   289,
     292,   295,   297,   299,   301,   303,   305,   307,   311,   317,
     323,   326,   329,   335,   339,   346,   353,   358,   364,   370,
     374,   378,   385,   391,   398,   404,   411,   415,   420,   427,
     435,   442,   450,   455,   461,   466,   471,   475,   482,   488,
     491,   495,   499,   502,   505,   509,   513,   514,   517,   520,
     524,   530,   532,   536,   540,   546,   547,   550,   554,   557,
     561,   568,   577,   579,   581,   583,   584,   587,   588,   591,
     595,   601,   603,   606,   610,   612,   616,   617,   618,   627,
     628,   630,   633,   636,   637,   638,   649,   653,   657,   663,
     669,   671,   675,   677,   680,   682,   684,   686,   688,   690,
     692,   694,   696,   698,   700,   702,   704,   706,   708,   710,
     712,   714,   716,   718,   720,   722,   724,   726,   728,   730,
     732,   734,   736,   738,   740,   742,   744,   746,   748,   750,
     752,   753,   757,   761,   762,   764,   768,   773,   778,   785,
     792,   793,   795,   797,   799,   801,   803,   806,   809,   811,
     813,   815,   816,   818,   820,   823,   825,   827,   829,   831,
     832,   834,   837,   839,   841,   843,   844,   846,   848,   850,
     852,   854,   857,   859,   860,   862,   865,   868,   869,   872,
     876,   881,   886,   889,   894,   895,   898,   901,   906,   911,
     916,   922,   927,   928,   930,   932,   934,   938,   942,   948,
     950,   952,   956,   958,   961,   965,   966,   969,   972,   973,
     976,   979,   981,   986,   991,   998,  1000,  1001,  1003,  1005,
    1009,  1014,  1018,  1023,  1030,  1031,  1034,  1037,  1040,  1043,
    1046,  1049,  1051,  1053,  1057,  1061,  1063,  1067,  1069,  1071,
    1073,  1077,  1081,  1082,  1084,  1086,  1090,  1094,  1098,  1100,
    1102,  1104,  1106,  1108,  1110,  1112,  1114,  1117,  1122,  1127,
    1132,  1138,  1141,  1144,  1151,  1158,  1163,  1173,  1183,  1191,
    1198,  1205,  1210,  1220,  1230,  1238,  1243,  1250,  1257,  1267,
    1277,  1284,  1286,  1288,  1290,  1292,  1294,  1296,  1298,  1300,
    1304,  1305,  1307,  1312,  1314,  1318,  1323,  1325,  1329,  1332,
    1336,  1340,  1342,  1346,  1349,  1354,  1356,  1358,  1360,  1362,
    1364,  1366,  1368,  1370,  1375,  1379,  1383,  1386,  1389,  1391,
    1394,  1397,  1399,  1401,  1403,  1405,  1407,  1409,  1411,  1416,
    1421,  1426,  1430,  1434,  1438,  1442,  1447,  1451,  1456,  1458,
    1460,  1462,  1464,  1466,  1470,  1475,  1479,  1484,  1488,  1493,
    1497,  1503,  1507,  1511,  1515,  1519,  1523,  1527,  1531,  1535,
    1539,  1543,  1547,  1551,  1555,  1559,  1563,  1567,  1571,  1575,
    1579,  1583,  1587,  1591,  1595,  1598,  1601,  1604,  1607,  1610,
    1613,  1617,  1621,  1625,  1629,  1633,  1637,  1641,  1645,  1647,
    1649,  1651,  1653,  1655,  1657
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     148,     0,    -1,   149,    -1,    -1,   149,   150,    -1,   152,
      -1,   151,   152,    -1,    57,     7,    -1,   151,    57,     7,
      -1,   153,    -1,   156,    -1,   161,    -1,   162,    -1,   169,
      -1,   163,    -1,   172,    -1,   175,    -1,   173,    -1,   182,
      -1,   176,    -1,   177,    -1,   181,    -1,   168,    -1,   246,
     131,    -1,    12,   152,    -1,    13,   247,   152,    -1,    14,
     164,   131,    -1,    18,   247,   156,    -1,    22,   164,   131,
      -1,    24,   235,   131,    -1,    42,   165,   152,    -1,    45,
     256,   167,    -1,    45,   167,    -1,    51,   256,   167,    -1,
      70,   256,   167,    -1,    70,   167,    -1,    74,   152,    -1,
      89,   256,   131,    -1,     1,    -1,   154,   155,    47,   165,
     138,   139,    -1,   154,   155,    47,   165,   138,   157,   139,
      -1,    -1,    62,    -1,    59,    -1,    -1,    61,    -1,   138,
     139,    -1,   138,   157,   139,    -1,   150,    -1,   157,   150,
      -1,   256,    -1,   256,    11,   256,    -1,   158,   112,   256,
      -1,   158,   112,   256,    11,   256,    -1,    -1,   158,    -1,
     134,    -1,   158,    -1,    83,   234,   131,    -1,    83,   256,
      30,   160,   131,    -1,    83,   256,    52,   159,   131,    -1,
      66,   234,   131,    -1,   258,    93,   257,   131,    -1,   258,
     104,   257,   131,    -1,   258,   101,   257,   131,    -1,   258,
     103,   257,   131,    -1,   258,    97,   257,   131,    -1,   258,
     102,   257,   131,    -1,   258,    98,   257,   131,    -1,   258,
      94,   257,   131,    -1,   258,    95,   257,   131,    -1,   258,
      96,   257,   131,    -1,   258,   106,   257,   131,    -1,   258,
     105,   257,   131,    -1,   258,   135,   257,   131,    -1,   258,
     136,   257,   131,    -1,   258,    99,   257,   131,    -1,   258,
     100,   257,   131,    -1,    -1,   165,    -1,     3,    -1,   164,
      -1,     7,    -1,    26,   152,    -1,   156,    -1,    67,   131,
      -1,    67,   257,   131,    -1,   131,    -1,   170,    -1,    62,
     170,    -1,    59,   170,    -1,   189,    -1,   219,    -1,   185,
      -1,   171,    -1,   196,    -1,   222,    -1,    35,   256,   131,
      -1,    35,   256,    30,   160,   131,    -1,    35,   256,    52,
     159,   131,    -1,    35,   222,    -1,    32,     9,    -1,    26,
     152,    87,   256,   131,    -1,    87,   256,   167,    -1,    19,
     256,    37,   256,   247,   167,    -1,    19,   256,    37,   174,
     247,   167,    -1,    19,   256,   247,   167,    -1,    33,   256,
      37,   256,   167,    -1,    33,   256,    37,   174,   167,    -1,
      33,   256,   167,    -1,    33,   174,   167,    -1,    33,    55,
     165,    37,   256,   167,    -1,    34,   256,    37,   256,   167,
      -1,    34,   256,    37,   256,   250,   167,    -1,    34,   256,
      37,   174,   167,    -1,    34,   256,    37,   174,   250,   167,
      -1,    34,   256,   167,    -1,    34,   256,   250,   167,    -1,
     142,   234,    37,   256,   143,   152,    -1,   142,   234,    37,
     256,   250,   143,   152,    -1,   142,   234,    37,   174,   143,
     152,    -1,   142,   234,    37,   174,   250,   143,   152,    -1,
     142,   234,   143,   152,    -1,   142,   234,   250,   143,   152,
      -1,    90,   140,   234,   141,    -1,    36,   256,    75,   152,
      -1,    36,   256,   156,    -1,    36,   256,    75,   152,    28,
     152,    -1,    36,   256,   156,    28,   152,    -1,    23,   152,
      -1,    78,   256,   131,    -1,    79,   256,   131,    -1,    78,
     163,    -1,    79,   163,    -1,    78,   156,   178,    -1,    79,
     156,   178,    -1,    -1,   178,   179,    -1,    16,   156,    -1,
      16,   180,   156,    -1,    16,   140,   180,   141,   156,    -1,
     165,    -1,   165,   111,   256,    -1,    76,   256,   131,    -1,
      69,   256,   138,   183,   139,    -1,    -1,   183,   184,    -1,
      85,   234,   167,    -1,    53,   152,    -1,    53,    26,   152,
      -1,   186,   165,   187,   138,   188,   139,    -1,    32,   166,
     186,   165,   187,   138,   188,   139,    -1,    17,    -1,    63,
      -1,    82,    -1,    -1,   111,   234,    -1,    -1,   188,   169,
      -1,   188,   151,   169,    -1,    29,   165,   138,   190,   139,
      -1,   191,    -1,   190,   112,    -1,   190,   112,   191,    -1,
     165,    -1,   165,    93,   256,    -1,    -1,    -1,    43,   193,
     204,   194,   211,   229,   218,   214,    -1,    -1,    39,    -1,
      31,   166,    -1,    32,   166,    -1,    -1,    -1,   195,   210,
     197,   199,   198,   211,   229,   212,   218,   213,    -1,   209,
     201,   203,    -1,   209,   202,   203,    -1,   209,   200,   114,
     201,   203,    -1,   209,   200,   114,   202,   203,    -1,   241,
      -1,   140,   256,   141,    -1,   165,    -1,   108,   165,    -1,
     107,    -1,   109,    -1,   110,    -1,   108,    -1,   117,    -1,
     127,    -1,   123,    -1,   120,    -1,   122,    -1,   119,    -1,
     129,    -1,   124,    -1,   134,    -1,   113,    -1,   132,    -1,
     133,    -1,   125,    -1,   118,    -1,   126,    -1,    15,    -1,
     121,    -1,    10,    -1,   135,    -1,   137,    -1,    93,    -1,
     104,    -1,   101,    -1,   103,    -1,    97,    -1,   102,    -1,
      98,    -1,    94,    -1,    95,    -1,    96,    -1,   106,    -1,
     105,    -1,    -1,   140,   205,   141,    -1,   140,   205,   141,
      -1,    -1,   206,    -1,   205,   112,   206,    -1,   207,   165,
     233,   228,    -1,   207,   165,   233,   217,    -1,   207,   140,
     227,   141,   233,   228,    -1,   207,   140,   227,   141,   233,
     217,    -1,    -1,   208,    -1,    37,    -1,    40,    -1,    54,
      -1,    21,    -1,    21,    37,    -1,    21,    65,    -1,    55,
      -1,    65,    -1,    80,    -1,    -1,    55,    -1,    65,    -1,
      21,    65,    -1,    21,    -1,    80,    -1,    60,    -1,    41,
      -1,    -1,    21,    -1,    21,    65,    -1,    65,    -1,    55,
      -1,    80,    -1,    -1,    77,    -1,   131,    -1,   214,    -1,
     156,    -1,   168,    -1,   130,   165,    -1,   130,    -1,    -1,
     215,    -1,   116,   256,    -1,   116,   216,    -1,    -1,    86,
     256,    -1,    80,   220,   131,    -1,    20,    80,   220,   131,
      -1,    32,    80,   220,   131,    -1,   165,   221,    -1,   165,
     221,   112,   220,    -1,    -1,    93,   242,    -1,    93,   230,
      -1,   223,    55,   224,   131,    -1,   223,    21,   224,   131,
      -1,   223,    65,   224,   131,    -1,   223,    21,    65,   224,
     131,    -1,   223,    84,   224,   131,    -1,    -1,    20,    -1,
      32,    -1,   225,    -1,   224,   112,   225,    -1,   165,   229,
     228,    -1,   140,   227,   141,   229,   228,    -1,    81,    -1,
     165,    -1,   140,   227,   141,    -1,   226,    -1,   226,   112,
      -1,   226,   112,   227,    -1,    -1,    93,    50,    -1,    93,
     257,    -1,    -1,   111,   242,    -1,   111,   230,    -1,     1,
      -1,   142,   234,   143,   242,    -1,   142,   234,   143,   230,
      -1,   142,   234,    37,   256,   143,   242,    -1,     1,    -1,
      -1,   242,    -1,   215,    -1,   142,   143,   231,    -1,   142,
     234,   143,   231,    -1,   142,   143,   232,    -1,   142,   234,
     143,   232,    -1,   142,   234,    37,   256,   143,   231,    -1,
      -1,   111,   242,    -1,   111,   215,    -1,   111,    27,    -1,
     111,    71,    -1,   111,    74,    -1,   111,   232,    -1,   256,
      -1,   215,    -1,   234,   112,   256,    -1,   234,   112,   215,
      -1,   256,    -1,   235,   112,   256,    -1,    81,    -1,   257,
      -1,   215,    -1,   236,   112,   236,    -1,   237,   112,   236,
      -1,    -1,   239,    -1,   240,    -1,   239,   112,   240,    -1,
     165,    93,   215,    -1,   165,    93,   257,    -1,   215,    -1,
     257,    -1,   165,    -1,   245,    -1,   258,    -1,   244,    -1,
     266,    -1,   265,    -1,    71,   256,    -1,    38,   140,   238,
     141,    -1,    27,   140,   238,   141,    -1,    73,   140,   238,
     141,    -1,    72,    73,   140,   238,   141,    -1,    12,   256,
      -1,    74,   256,    -1,    33,   256,    37,   256,    26,   256,
      -1,    33,   256,    37,   174,    26,   256,    -1,    33,   256,
      26,   256,    -1,    33,   256,    37,   256,    26,    36,   256,
      75,   256,    -1,    33,   256,    37,   174,    26,    36,   256,
      75,   256,    -1,    33,   256,    26,    36,   256,    75,   256,
      -1,    34,   256,    37,   256,    26,   256,    -1,    34,   256,
      37,   174,    26,   256,    -1,    34,   256,    26,   256,    -1,
      34,   256,    37,   256,    26,    36,   256,    75,   256,    -1,
      34,   256,    37,   174,    26,    36,   256,    75,   256,    -1,
      34,   256,    26,    36,   256,    75,   256,    -1,   142,   234,
     143,   256,    -1,   142,   234,    37,   256,   143,   256,    -1,
     142,   234,    37,   174,   143,   256,    -1,   142,   234,    37,
     256,   143,    36,   256,    75,   256,    -1,   142,   234,    37,
     174,   143,    36,   256,    75,   256,    -1,    36,   256,    75,
     256,    28,   256,    -1,    49,    -1,   245,    -1,   241,    -1,
     261,    -1,   260,    -1,   192,    -1,   254,    -1,   255,    -1,
     253,   137,   256,    -1,    -1,   248,    -1,    88,   140,   249,
     141,    -1,   252,    -1,   249,   112,   252,    -1,    88,   140,
     251,   141,    -1,   252,    -1,   251,   112,   252,    -1,   208,
     241,    -1,   256,    64,   241,    -1,   269,    64,   241,    -1,
     258,    -1,   253,   137,   256,    -1,    48,   256,    -1,    44,
     224,    37,   256,    -1,   263,    -1,   242,    -1,   243,    -1,
     267,    -1,   268,    -1,   192,    -1,   254,    -1,   255,    -1,
     140,   116,   256,   141,    -1,   256,   111,   256,    -1,   256,
     115,   256,    -1,   256,   115,    -1,   115,   256,    -1,   115,
      -1,    78,   256,    -1,    79,   256,    -1,   256,    -1,   241,
      -1,   260,    -1,   261,    -1,   262,    -1,   258,    -1,   192,
      -1,   259,   140,   238,   141,    -1,   259,   142,   238,   143,
      -1,    58,   140,   238,   141,    -1,   256,   114,   165,    -1,
     256,   114,    80,    -1,   256,   114,    27,    -1,   140,   236,
     141,    -1,   140,   236,   112,   141,    -1,   140,   237,   141,
      -1,   140,   237,   112,   141,    -1,     4,    -1,     5,    -1,
       6,    -1,     7,    -1,     8,    -1,   138,   234,   139,    -1,
     138,   234,   112,   139,    -1,   142,   234,   143,    -1,   142,
     234,   112,   143,    -1,   142,   264,   143,    -1,   142,   264,
     112,   143,    -1,   256,    91,   256,    -1,   264,   112,   256,
      91,   256,    -1,   256,   129,   256,    -1,   256,   124,   256,
      -1,   256,   134,   256,    -1,   256,   113,   256,    -1,   256,
     132,   256,    -1,   256,   133,   256,    -1,   256,   125,   256,
      -1,   256,   117,   256,    -1,   256,   127,   256,    -1,   256,
     123,   256,    -1,   256,   120,   256,    -1,   256,   122,   256,
      -1,   256,   119,   256,    -1,   256,   107,   256,    -1,   256,
     109,   256,    -1,   256,   110,   256,    -1,   256,    92,   256,
      -1,   256,   128,   256,    -1,   256,   118,   256,    -1,   256,
      15,   256,    -1,   256,    10,   256,    -1,   256,   121,   256,
      -1,   256,    25,   256,    -1,   129,   256,    -1,   124,   256,
      -1,    46,   256,    -1,    56,   256,    -1,   126,   256,    -1,
     108,   256,    -1,   256,    64,   256,    -1,   256,    64,   174,
      -1,   269,    64,   256,    -1,   269,    64,   174,    -1,   256,
      68,   256,    -1,   256,    68,   174,    -1,   269,    68,   256,
      -1,   269,    68,   174,    -1,   129,    -1,   134,    -1,    92,
      -1,   128,    -1,   107,    -1,   109,    -1,   110,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   439,   439,   444,   445,   451,   452,   457,   458,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   496,
     499,   504,   505,   506,   510,   511,   524,   525,   530,   531,
     536,   541,   546,   550,   557,   562,   566,   571,   575,   576,
     577,   581,   585,   587,   589,   591,   593,   595,   597,   599,
     601,   603,   605,   607,   609,   611,   613,   615,   620,   621,
     625,   629,   630,   634,   635,   639,   640,   644,   645,   646,
     647,   648,   649,   650,   651,   655,   656,   660,   661,   662,
     663,   667,   682,   683,   684,   685,   686,   687,   688,   689,
     690,   691,   692,   693,   694,   695,   696,   697,   698,   704,
     710,   716,   722,   729,   739,   743,   744,   745,   746,   750,
     753,   754,   755,   756,   757,   758,   762,   763,   767,   768,
     769,   773,   774,   778,   781,   786,   787,   791,   793,   795,
     802,   812,   825,   830,   835,   843,   844,   849,   850,   852,
     857,   867,   876,   880,   888,   889,   894,   899,   893,   924,
     930,   937,   945,   957,   963,   956,   991,   995,  1000,  1004,
    1012,  1013,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,
    1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,
    1035,  1036,  1037,  1038,  1039,  1040,  1041,  1042,  1046,  1047,
    1048,  1049,  1050,  1051,  1052,  1053,  1054,  1055,  1056,  1057,
    1061,  1062,  1066,  1070,  1071,  1072,  1076,  1078,  1080,  1082,
    1087,  1088,  1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,
    1100,  1104,  1105,  1106,  1107,  1108,  1109,  1113,  1114,  1118,
    1119,  1120,  1121,  1122,  1123,  1127,  1128,  1131,  1132,  1136,
    1137,  1141,  1143,  1148,  1149,  1153,  1154,  1158,  1159,  1163,
    1165,  1167,  1172,  1185,  1202,  1203,  1205,  1210,  1218,  1226,
    1234,  1243,  1253,  1254,  1255,  1259,  1260,  1268,  1270,  1275,
    1277,  1279,  1284,  1286,  1288,  1295,  1296,  1297,  1301,  1302,
    1303,  1304,  1324,  1328,  1332,  1340,  1344,  1345,  1346,  1350,
    1352,  1358,  1360,  1362,  1367,  1368,  1369,  1370,  1371,  1372,
    1373,  1379,  1380,  1381,  1382,  1386,  1387,  1391,  1392,  1393,
    1397,  1398,  1402,  1403,  1407,  1408,  1412,  1413,  1414,  1415,
    1419,  1430,  1431,  1432,  1433,  1434,  1435,  1437,  1439,  1441,
    1443,  1445,  1447,  1452,  1454,  1456,  1458,  1460,  1462,  1464,
    1466,  1468,  1470,  1472,  1474,  1476,  1483,  1489,  1495,  1501,
    1510,  1520,  1528,  1529,  1530,  1531,  1532,  1533,  1534,  1535,
    1540,  1541,  1545,  1549,  1552,  1557,  1561,  1564,  1569,  1573,
    1574,  1578,  1579,  1584,  1589,  1597,  1598,  1599,  1600,  1601,
    1602,  1603,  1604,  1605,  1607,  1609,  1611,  1613,  1615,  1620,
    1621,  1622,  1625,  1626,  1627,  1628,  1639,  1640,  1644,  1645,
    1646,  1650,  1651,  1652,  1660,  1661,  1662,  1663,  1667,  1668,
    1669,  1670,  1671,  1672,  1673,  1674,  1675,  1676,  1680,  1688,
    1689,  1693,  1694,  1695,  1696,  1697,  1698,  1699,  1700,  1701,
    1702,  1703,  1704,  1705,  1706,  1707,  1708,  1709,  1710,  1711,
    1712,  1713,  1714,  1715,  1719,  1720,  1721,  1722,  1723,  1724,
    1728,  1729,  1730,  1731,  1735,  1736,  1737,  1738,  1743,  1744,
    1745,  1746,  1747,  1748,  1749
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
  "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC", "TRECORD",
  "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL",
  "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHROW",
  "TTHROWS", "TTRY", "TTRYBANG", "TTYPE", "TUNDERSCORE", "TUNION", "TUSE",
  "TVAR", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS",
  "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR",
  "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR",
  "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY", "TASSIGNPLUS",
  "TASSIGNSL", "TASSIGNSR", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON",
  "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP",
  "TGREATER", "TGREATEREQUAL", "THASH", "TLESS", "TLESSEQUAL", "TMINUS",
  "TMOD", "TNOT", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TASSIGNREDUCE", "TIO",
  "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR", "TNOELSE", "TUMINUS",
  "TUPLUS", "$accept", "program", "toplevel_stmt_ls", "toplevel_stmt",
  "pragma_ls", "stmt", "module_decl_stmt", "access_control",
  "opt_prototype", "block_stmt", "stmt_ls", "only_ls", "opt_only_ls",
  "except_ls", "use_stmt", "require_stmt", "assignment_stmt", "opt_ident",
  "ident", "opt_ident_or_string", "do_stmt", "return_stmt",
  "class_level_stmt", "private_decl", "forwarding_stmt",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "defer_stmt", "try_stmt", "catch_stmt_ls", "catch_stmt", "catch_expr",
  "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
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
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "opt_type",
  "array_type", "opt_formal_array_elt_type", "formal_array_type",
  "opt_formal_type", "expr_ls", "simple_expr_ls", "tuple_component",
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
     395,   396,   397,   398,   399,   400,   401
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   147,   148,   149,   149,   150,   150,   151,   151,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   153,
     153,   154,   154,   154,   155,   155,   156,   156,   157,   157,
     158,   158,   158,   158,   159,   159,   160,   160,   161,   161,
     161,   162,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   164,   164,
     165,   166,   166,   167,   167,   168,   168,   169,   169,   169,
     169,   169,   169,   169,   169,   170,   170,   171,   171,   171,
     171,   172,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   174,   175,   175,   175,   175,   176,
     177,   177,   177,   177,   177,   177,   178,   178,   179,   179,
     179,   180,   180,   181,   182,   183,   183,   184,   184,   184,
     185,   185,   186,   186,   186,   187,   187,   188,   188,   188,
     189,   190,   190,   190,   191,   191,   193,   194,   192,   195,
     195,   195,   195,   197,   198,   196,   199,   199,   199,   199,
     200,   200,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     203,   203,   204,   205,   205,   205,   206,   206,   206,   206,
     207,   207,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   209,   209,   209,   209,   209,   209,   210,   210,   211,
     211,   211,   211,   211,   211,   212,   212,   213,   213,   214,
     214,   215,   215,   216,   216,   217,   217,   218,   218,   219,
     219,   219,   220,   220,   221,   221,   221,   222,   222,   222,
     222,   222,   223,   223,   223,   224,   224,   225,   225,   226,
     226,   226,   227,   227,   227,   228,   228,   228,   229,   229,
     229,   229,   230,   230,   230,   230,   231,   231,   231,   232,
     232,   232,   232,   232,   233,   233,   233,   233,   233,   233,
     233,   234,   234,   234,   234,   235,   235,   236,   236,   236,
     237,   237,   238,   238,   239,   239,   240,   240,   240,   240,
     241,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     244,   245,   246,   246,   246,   246,   246,   246,   246,   246,
     247,   247,   248,   249,   249,   250,   251,   251,   252,   252,
     252,   253,   253,   254,   255,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   257,
     257,   257,   258,   258,   258,   258,   259,   259,   260,   260,
     260,   261,   261,   261,   262,   262,   262,   262,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   264,
     264,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   266,   266,   266,   266,   266,   266,
     267,   267,   267,   267,   268,   268,   268,   268,   269,   269,
     269,   269,   269,   269,   269
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     2,     2,     3,     1,     6,
       7,     0,     1,     1,     0,     1,     2,     3,     1,     2,
       1,     3,     3,     5,     0,     1,     1,     1,     3,     5,
       5,     3,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     0,     1,
       1,     1,     1,     2,     1,     2,     3,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     3,     5,     5,
       2,     2,     5,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     6,     7,
       6,     7,     4,     5,     4,     4,     3,     6,     5,     2,
       3,     3,     2,     2,     3,     3,     0,     2,     2,     3,
       5,     1,     3,     3,     5,     0,     2,     3,     2,     3,
       6,     8,     1,     1,     1,     0,     2,     0,     2,     3,
       5,     1,     2,     3,     1,     3,     0,     0,     8,     0,
       1,     2,     2,     0,     0,    10,     3,     3,     5,     5,
       1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     3,     0,     1,     3,     4,     4,     6,     6,
       0,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     0,     1,     1,     2,     1,     1,     1,     1,     0,
       1,     2,     1,     1,     1,     0,     1,     1,     1,     1,
       1,     2,     1,     0,     1,     2,     2,     0,     2,     3,
       4,     4,     2,     4,     0,     2,     2,     4,     4,     4,
       5,     4,     0,     1,     1,     1,     3,     3,     5,     1,
       1,     3,     1,     2,     3,     0,     2,     2,     0,     2,
       2,     1,     4,     4,     6,     1,     0,     1,     1,     3,
       4,     3,     4,     6,     0,     2,     2,     2,     2,     2,
       2,     1,     1,     3,     3,     1,     3,     1,     1,     1,
       3,     3,     0,     1,     1,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     4,     4,     4,
       5,     2,     2,     6,     6,     4,     9,     9,     7,     6,
       6,     4,     9,     9,     7,     4,     6,     6,     9,     9,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     4,     1,     3,     4,     1,     3,     2,     3,
       3,     1,     3,     2,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     3,     2,     2,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       4,     3,     3,     3,     3,     4,     3,     4,     1,     1,
       1,     1,     1,     3,     4,     3,     4,     3,     4,     3,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    38,    80,   428,   429,   430,   431,
     432,     0,   380,    78,   152,   380,     0,   283,    78,     0,
       0,     0,     0,     0,    78,    78,     0,     0,   282,     0,
       0,   170,     0,   166,     0,     0,     0,     0,   371,     0,
       0,     0,     0,   282,   282,   153,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   154,     0,
       0,     0,   480,   482,     0,   483,   484,   408,     0,     0,
     481,   478,    87,   479,     0,     0,     0,     4,     0,     5,
       9,    44,    10,    11,    12,    14,   340,    22,    13,    88,
      94,    15,    17,    16,    19,    20,    21,    18,    93,     0,
      91,   400,     0,    95,    92,    96,     0,   412,   396,   397,
     343,   341,     0,     0,   401,   402,     0,   342,     0,   413,
     414,   415,   395,   345,   344,   398,   399,     0,    24,   351,
       0,     0,   381,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,   400,   412,   341,   401,   402,   380,   342,
     413,   414,     0,     0,   129,     0,   325,     0,   332,     0,
      82,    81,   171,   101,     0,   172,     0,     0,     0,     0,
       0,   283,   284,   100,     0,     0,   332,     0,     0,     0,
       0,     0,   285,     0,    84,    32,     0,   466,   393,     0,
     467,     7,   332,   284,    90,    89,   262,   322,     0,   321,
       0,     0,    85,   411,     0,     0,    35,     0,   346,     0,
     332,    36,   352,     0,   136,   132,     0,   342,   136,   133,
       0,   274,     0,     0,   321,     0,     0,   469,   407,   465,
     468,   464,    46,    48,     0,     0,   327,     0,   329,     0,
       0,   328,     0,   321,     0,     0,     6,    45,     0,   155,
     248,   247,   173,     0,     0,     0,     0,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   406,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,   332,     0,     0,     0,    25,    26,
       0,    27,     0,     0,     0,     0,     0,     0,     0,    28,
       0,    29,     0,   340,   338,     0,   333,   334,   339,     0,
       0,     0,     0,     0,   110,     0,     0,   109,     0,     0,
       0,   116,     0,     0,    54,    97,     0,   126,     0,    30,
     223,   167,   289,     0,   290,   292,     0,   301,     0,   295,
       0,     0,    83,    31,    33,     0,   172,   261,     0,    61,
     409,   410,    86,   145,    34,   332,     0,   143,   134,   130,
     135,   131,     0,   272,   269,    58,     0,    54,   103,    37,
      47,    49,     0,   433,     0,     0,   424,     0,   426,     0,
       0,     0,     0,     0,     0,   437,     8,     0,     0,     0,
     241,     0,     0,     0,     0,     0,   379,   461,   460,   463,
     471,   470,   475,   474,   457,   454,   455,   456,   404,   444,
     423,   422,   421,   405,   448,   459,   453,   451,   462,   452,
     450,   442,   447,   449,   458,   441,   445,   446,   443,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,   472,   477,
     476,   235,   232,   233,   234,   238,   239,   240,     0,     0,
     383,     0,     0,     0,     0,     0,     0,     0,     0,   435,
     380,   380,   106,   270,   326,     0,     0,   348,     0,   164,
       0,   161,   271,   155,     0,     0,     0,   355,     0,     0,
       0,   361,     0,     0,     0,   117,   479,    57,     0,    50,
      55,     0,   125,     0,     0,   347,     0,   224,     0,   231,
     249,     0,   293,     0,   305,     0,   300,   396,     0,   287,
     394,   286,   420,   324,   323,     0,     0,   349,     0,   137,
     276,   396,     0,     0,     0,   434,   403,   425,   330,   427,
     331,     0,     0,   436,   122,   365,     0,   439,   438,     0,
       0,   156,   157,   245,   242,   243,   246,   174,     0,     0,
     278,   277,   279,   281,    62,    69,    70,    71,    66,    68,
      76,    77,    64,    67,    65,    63,    73,    72,    74,    75,
     418,   419,   236,   237,   388,     0,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
     336,   337,   335,     0,   162,   160,     0,     0,   124,     0,
       0,   108,     0,   107,     0,     0,   114,     0,     0,   112,
       0,     0,   386,     0,    98,     0,    99,     0,     0,   128,
     230,   222,     0,   314,   250,   253,   252,   254,     0,   291,
     294,   295,     0,   296,   297,     0,     0,   144,   146,   350,
       0,   138,   141,     0,   273,    59,    60,     0,     0,     0,
       0,   123,     0,     0,   282,   244,   249,   205,   203,   208,
     215,   216,   217,   212,   214,   210,   213,   211,   209,   219,
     218,   184,   187,   185,   186,   197,   188,   201,   193,   191,
     204,   192,   190,   195,   200,   202,   189,   194,   198,   199,
     196,   206,   207,     0,   182,     0,   220,   220,   180,   280,
     384,   412,   412,     0,     0,     0,     0,     0,     0,     0,
       0,   105,   104,   165,   163,   157,   111,     0,     0,   354,
       0,   353,     0,     0,   360,   115,     0,   359,   113,     0,
     385,    52,    51,   127,   370,   225,     0,     0,   295,   251,
     267,   288,     0,     0,     0,   148,     0,     0,     0,   139,
       0,   120,   367,     0,     0,   118,   366,     0,   440,    39,
       0,    78,   282,   282,   150,   282,   158,     0,   183,     0,
       0,   223,   176,   177,     0,     0,     0,     0,     0,     0,
       0,     0,   282,   358,     0,     0,   364,     0,     0,   387,
       0,   314,   317,   318,   319,     0,   316,   320,   396,   263,
     227,   226,     0,     0,     0,   303,   396,   149,   147,     0,
     142,     0,   121,     0,   119,    40,   159,   255,   181,   182,
     220,   220,     0,     0,     0,     0,     0,     0,     0,   151,
       0,     0,     0,     0,    53,   295,   306,     0,   264,   266,
     265,   268,   259,   260,   168,     0,   140,     0,     0,   256,
     267,   178,   179,   221,     0,     0,     0,     0,     0,     0,
     357,   356,   363,   362,   229,   228,   308,   309,   311,   396,
       0,   435,   396,   369,   368,     0,     0,   310,   312,   257,
     175,   258,   306,   313
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   233,    78,   522,    80,    81,   248,    82,
     234,   517,   521,   518,    83,    84,    85,   161,    86,   165,
     185,    87,    88,    89,    90,    91,    92,   615,    93,    94,
      95,   378,   549,   673,    96,    97,   545,   668,    98,    99,
     409,   684,   100,   500,   501,   143,   178,   530,   102,   103,
     410,   686,   577,   725,   726,   727,   802,   351,   526,   527,
     528,   478,   578,   252,   658,   880,   910,   874,   197,   869,
     830,   833,   104,   222,   383,   105,   106,   181,   182,   355,
     356,   539,   359,   536,   897,   827,   768,   235,   155,   239,
     240,   325,   326,   327,   144,   108,   109,   110,   145,   112,
     131,   132,   479,   342,   641,   480,   113,   146,   147,   203,
     328,   149,   118,   150,   151,   121,   122,   244,   123,   124,
     125,   126,   127
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -788
static const yytype_int16 yypact[] =
{
    -788,    83,  2471,  -788,  -788,  -788,  -788,  -788,  -788,  -788,
    -788,  3721,    74,   102,  -788,    74,  8204,   124,   102,  3721,
    8204,  3721,    94,   102,   123,   452,  6804,  8204,  6944,  8204,
     149,  -788,   102,  -788,    36,  7364,  8204,  8204,  -788,  8204,
    8204,   217,   161,   683,   903,  -788,  7504,  6384,  8204,  7364,
    8204,   171,   183,  3721,  8204,  8344,  8344,   102,  -788,  7504,
    8204,  8204,  -788,  -788,  8204,  -788,  -788,  9744,  8204,  8204,
    -788,  8204,  -788,  -788,  2881,  5964,  7504,  -788,  3581,  -788,
    -788,   127,  -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,
    -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,   102,
    -788,   239,   255,  -788,  -788,  -788,   153,   223,  -788,  -788,
    -788,   246,   252,   193,   261,   264, 13118,  1374,   -21,   279,
     280,  -788,  -788,  -788,  -788,  -788,  -788,   336,  -788, 13118,
     273,  3721,  -788,   284,  -788,   248,  8204,  8204,  8204,  8204,
    8204,  7504,  7504,   152,  -788,  -788,  -788,  -788, 11386,   225,
    -788,  -788,   102,   302,  -788,   195, 13118,   351,  6524,   304,
    -788,  -788,  -788,  -788,   102,   128,   102,   300,    17, 10371,
   10292,  -788,  -788,  -788, 11324, 10491,  6524,  3721,   305,     9,
      54,    55,  -788,  3721,  -788,  -788, 10535,   177,   330, 10535,
     177,  -788,  6524,   186,  -788,  -788,   102,  -788,   204, 13118,
    8204,  8204,  -788, 13118,   319, 11188,  -788, 10535, 13118,   313,
    6524,  -788, 13118, 11655,  -788,  -788, 11726,  5384,  -788,  -788,
   11788,   358,   329,   207, 11460, 10535, 11859,   325,  2596,   177,
     325,   177,  -788,  -788,  3021,   100,  -788,  8204,  -788,   -30,
      10,  -788,    40, 11921,   -37,   455,  -788,  -788,   416,   353,
    -788,  -788,  -788,    28,    36,    36,    36,  -788,  8204,  8204,
    8204,  8204,  7644,  7644,  8204,  8204,  8204,  8204,  8204,  8204,
     115,  9744,  8204,  8204,  8204,  8204,  8204,  8204,  8204,  8204,
    8204,  8204,  8204,  8204,  8204,  8204,  8204,  7084,  7084,  7084,
    7084,  7084,  7084,  7084,  7084,  7084,  7084,  7084,  7084,  7084,
    7084,  7084,  7084,  6524,  6524,  7644,  7644,  5824,  -788,  -788,
    3161,  -788, 11522, 11593, 11992,    58,  7644,    17,   334,  -788,
    8204,  -788,  8204,   375,  -788,   331,   359,  -788,  -788,   102,
     339,   102,   438,  7504,  -788,  3861,  7644,  -788,  4001,  7644,
     348,  -788,    17,  8484,  8204,  -788,  3721,   450,   352,  -788,
      47,  -788,  -788,     9,  -788,   377,   355,  -788,  5261,   399,
    8204,    36,  -788,  -788,  -788,   357,  -788,  -788,  7504,  -788,
   13118, 13118,  -788,  -788,  -788,  6524,   360,  -788,   478,  -788,
     478,  -788,  5261,   388,  -788,  -788,  8484,  8204,  -788,  -788,
    -788,  -788,  7224,  -788, 10172,  6104,  -788,  6244,  -788,  7644,
    5401,  2741,   362,  8204,  5683,  -788,  -788,   102,  7504,   365,
     232,    36,   215,   233,   240,   251, 11250,  5092,  5092,   214,
    -788,   214,  -788,   214,  5232,   586,  1207,   875,   330,   325,
    -788,  -788,  -788,  2596,  2299,   214,  2205,  2205,  5092,  2205,
    2205,   722,   325,  2299,  1970,   722,   177,   177,   325,   378,
     379,   382,   387,   392,   393,   395,   404,   406,   410,   411,
     412,   413,   419,   420,   421,   389,   363,  -788,   214,  -788,
     214,   142,  -788,  -788,  -788,  -788,  -788,  -788,   102,    80,
    -788, 13189,   341,  8624,  7644,  8764,  7644,  8204,  7644,  9939,
      74, 12054,  -788,  -788, 13118, 12125,  6524,  -788,  6524,   426,
     104,  -788,  -788,   353,  8204,    84,  8204, 13118,    23, 10569,
    8204, 13118,    16, 10419,  5824,  -788,   422,   435,   428, 12187,
     435,   430,   529, 12258,  3721,  -788,    85,  -788,    38,  -788,
     296,   424,     9,    54,  -788,  7504,  -788,   276,  6664,  -788,
   13118,  -788,  -788,  -788, 13118,    11,   425,  -788,    33,  -788,
    -788,   256,   102,   436,   440,  -788,  -788,  -788,  -788,  -788,
    -788,   -27,   926,  -788,  -788, 13118,  3721, 13118,  -788, 12320,
     439,   464,  -788,   514,  -788,  -788,  -788,  -788,  2549,   263,
    -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,
    -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,
    -788,  -788,  -788,  -788,  -788,  5824,  -788,  7644,  7644,  8204,
     554, 12391,  8204,   556, 12453,   441,  2264,    17,    17,  -788,
    -788,  -788,  -788,  8204,   102,  -788,   447, 10535,  -788, 10656,
    4141,  -788,  4281,  -788, 10718,  4421,  -788,    17,  4561,  -788,
      17,    99,  -788,  8204,  -788,  8204,  -788,  3721,  8204,  -788,
     474,  -788,     9,   476,   526,  -788,  -788,  -788,    21,  -788,
    -788,   399,    60,  -788,  -788,  4701,  7504,  -788,  -788,  -788,
     102,  -788,   481,   248,  -788,  -788,  -788,  4841,   453,  4981,
     458,  -788,  8204,  3301,   417,  -788,   296,  -788,  -788,  -788,
    -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,
    -788,  -788,   102,  -788,  -788,  -788,  -788,  -788,  -788,  -788,
    -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,  -788,
    -788,  -788,  -788,  8204,   494,   495,   472,   472,  -788,  -788,
    -788,   108,   110, 12524,  8904,  9044, 12586,  9184,  9324,  9464,
    9604,  -788,  -788, 13118,  -788,  -788,  -788,  3721,  8204, 13118,
    8204, 13118,  3721,  8204, 13118,  -788,  8204, 13118,  -788,  5824,
    -788, 12657, 13118,  -788, 13118,  -788,   473,  7784,     7,  -788,
     527,  -788,  7644,  5121,  3721,  -788,    68,   475,  8204,  -788,
    8204,  -788, 13118,  3721,  8204,  -788, 13118,  3721, 13118,  -788,
    3441,   513,   563,   563,  -788,  2121,  -788,    22,  -788, 10244,
    9989,    47,  -788,  -788,  8204,  8204,  8204,  8204,  8204,  8204,
    8204,  8204,  1834, 12258, 10789, 10851, 12258, 10922, 10984,  -788,
    8204,   476,    94,  8204,  8204,  5542,  -788,  -788,   101,  7504,
    -788,  -788,  8204,    48, 10036,  -788,   975,   351,  -788,   248,
   13118, 11055,  -788, 11117,  -788,  -788,  -788,   538,  -788,  -788,
     472,   472,   181, 12719, 12790, 12852, 12923, 12985, 13056,  -788,
    3721,  3721,  3721,  3721, 13118,     7,  7924,    72,  -788,  -788,
   13118, 13118,  -788,  -788,  -788,  9604,  -788,  3721,  3721,  -788,
     527,  -788,  -788,  -788,  8204,  8204,  8204,  8204,  8204,  8204,
   12258, 12258, 12258, 12258,  -788,  -788,  -788,  -788,  -788,   107,
    7644,  9823,  1037, 12258, 12258,    -7, 10110,  -788,  -788,  -788,
    -788,  -788,  8064,  -788
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -788,  -788,  -788,    -1,  -668,  1670,  -788,  -788,  -788,  1359,
     -66,  -274,   231,   234,  -788,  -788,   199,   311,  2041,   -11,
      61,  -762,  -654,   -40,  -788,  -788,  -788,   283,  -788,  -788,
    -788,   407,  -788,   -46,  -788,  -788,  -788,  -788,  -788,   462,
     126,  -114,  -788,  -788,    13,   772,  -788,  -788,  -788,  -788,
    -788,  -788,  -788,  -788,  -162,  -161,  -687,  -788,  -159,   -10,
    -788,  -344,  -788,  -788,   -42,  -788,  -788,  -258,   163,  -788,
    -216,  -229,  -788,  -144,  -788,   624,  -788,  -175,   294,  -788,
    -342,  -651,  -512,  -377,  -787,  -781,  -165,    27,  -788,   -23,
    -788,  -147,  -788,   160,   338,  -326,  -788,  -788,   900,  -788,
       0,  -788,  -788,  -214,  -788,  -507,  -788,   960,  1151,    -2,
     130,   579,  -788,  1270,  1480,  -788,  -788,  -788,  -788,  -788,
    -788,  -788,  -305
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -436
static const yytype_int16 yytable[] =
{
     116,    77,   482,   194,   195,   550,   529,   642,   318,   129,
     771,   531,     5,   162,   148,   135,   795,   116,   156,   116,
     330,   661,   357,   357,   169,   170,   174,   175,   402,   348,
     796,     5,   537,   186,   187,   188,     5,   189,   190,     5,
     803,     5,   635,   183,   199,   365,   205,   207,   208,   630,
    -230,   212,   213,   216,   220,   357,   551,   224,   225,   226,
      47,   340,   227,   376,   665,   228,   229,   230,   471,   231,
     520,   873,   199,   198,   243,   404,   116,   399,   412,   413,
     414,   415,   395,     3,   472,   898,   223,   473,  -298,  -298,
     352,  -298,   360,   411,   183,   488,   666,   772,   730,  -298,
     538,   474,   475,   242,   340,     5,   405,  -298,  -298,   900,
     206,   396,   476,   520,   907,    47,   677,   831,     5,   303,
     908,   304,   397,   829,   909,   913,     5,   477,   340,   116,
     160,   310,   358,   358,   129,   312,   313,   314,   212,   199,
     243,   846,   430,   873,   795,    14,   770,  -298,   317,   353,
     667,   398,   400,  -298,   310,   310,   465,   466,   796,  -298,
    -298,   310,   130,   881,   882,   358,  -298,   361,   179,   315,
     400,   310,   400,   670,   253,   116,   179,   204,   652,   602,
     368,   116,   366,   401,   400,  -298,   310,  -230,   247,     5,
     660,    45,   605,   160,  -315,   431,   368,   650,   370,   371,
    -307,   489,   261,   773,   152,   241,   310,   603,   254,   482,
      58,   759,   392,  -315,   895,   901,   624,  -315,   255,  -307,
    -389,   606,  -390,  -307,   191,   628,   651,   -78,   546,   334,
     337,   341,   116,   391,   158,   394,   579,   256,   238,   393,
     760,   262,  -315,   625,   209,   263,   -78,   363,  -307,  -389,
     364,  -390,   819,   573,   215,   219,   416,   417,   418,   419,
     421,   423,   424,   425,   426,   427,   428,   429,   374,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   847,   388,   574,   268,   176,
     269,   270,  -417,   650,  -417,   273,   250,   575,   637,   640,
     482,   192,   280,   468,   470,   481,   529,   320,   116,   168,
     766,   286,   576,  -299,   491,   251,   368,   654,   494,   368,
     495,   324,   883,   210,   133,   268,   321,   361,   270,   153,
     258,   199,   273,   507,   509,   369,   511,   513,   385,   324,
     107,   519,   519,  -299,   523,   361,   580,   678,   680,   107,
     261,   655,   361,  -299,  -373,   324,   116,   107,   540,   107,
     505,   656,  -299,   361,   581,  -416,   544,  -416,  -275,  -299,
    -376,   582,   558,   324,   560,   361,   657,  -372,   492,  -417,
     116,  -417,   583,   257,   519,   519,   310,  -275,  -299,   262,
     544,   107,  -377,   263,   729,  -378,   835,   562,   544,   565,
     305,   567,   569,   515,   306,   608,   199,  -299,   674,   306,
    -375,  -374,   107,   307,  -299,   309,   107,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   319,    14,   571,   268,    17,   322,   270,
     333,   828,   329,   273,   270,   350,    23,   836,    24,   791,
     372,   382,    28,   375,   482,     5,    31,   529,  -169,   160,
     384,   163,   406,   407,   408,   493,   324,   324,   496,   107,
     502,   498,   497,  -284,    41,   504,   792,  -169,   524,   793,
      45,   507,   611,   511,   614,   523,   616,   565,   514,   532,
     617,   618,   538,   525,   548,   471,   533,    57,   542,    58,
     552,   547,   627,   572,   629,   566,   601,  -284,   634,   584,
     585,   472,   481,   586,   473,   107,     5,  -284,   587,   623,
     160,   107,   116,   588,   589,   241,   590,   241,   474,   475,
     600,   543,   164,   243,  -284,   591,  -284,   592,   324,   476,
     899,   593,   594,   595,   596,   420,   422,   643,    72,   902,
     597,   598,   599,   -56,   477,   543,   794,   647,   238,   644,
     238,   646,   662,   543,   116,   659,   669,   675,  -284,   631,
     633,   676,   107,   636,   639,   899,   368,   683,  -284,   685,
     734,   117,   737,   171,   739,   745,   899,   767,   467,   469,
     117,   769,   778,   164,    24,   193,   783,  -284,   117,   490,
     117,   787,    31,   481,  -169,   421,   468,   733,  -340,   800,
     736,   261,   801,   832,   821,   879,   839,   790,   554,   508,
     553,   743,   512,  -169,   777,   380,   621,   331,   749,   626,
     751,   812,   117,   754,   217,   217,   757,   744,   850,   851,
     765,   761,   852,   762,   797,   116,   764,   911,   107,   894,
     262,   905,   173,   117,   263,   541,   865,   117,   622,   620,
       0,   324,     0,   116,   199,     0,     0,     0,   664,     0,
       0,     0,     0,   107,     0,   782,   107,   786,   741,   742,
     788,   116,   561,     0,   107,     0,     0,     0,   746,     0,
       0,     0,     0,   776,     0,     0,     0,   268,   755,   269,
     270,   758,     0,   171,   273,     0,     0,     0,     0,     0,
     117,   280,     0,     0,    24,   193,     0,     0,   284,   285,
     286,   799,    31,     0,  -169,     0,     0,     0,     0,     0,
     -43,     0,   749,   751,     0,   754,   757,   782,   786,   107,
       0,     0,     0,  -169,   -43,   813,   814,   261,   815,     0,
     816,   817,   194,   195,   818,     0,   117,   481,     0,     0,
       0,     0,   117,     0,     0,   116,     0,   610,     0,   613,
     834,   565,   116,     0,   101,     0,   840,     0,   841,     0,
       0,   116,   843,   101,     0,   116,   262,     0,   116,   391,
     263,   101,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,   813,   853,   854,   816,   855,   856,   857,   858,
       0,     0,     0,   117,     0,     0,   604,     0,   864,     0,
       0,   208,   212,   243,     0,   101,     0,   870,     0,   265,
     871,   266,   267,   268,     0,   269,   270,   838,     0,     0,
     273,     0,     0,     0,     0,     0,   101,   280,     0,     0,
     101,     0,   867,     0,   284,   285,   286,     0,   890,   891,
     892,   893,   107,     0,   116,     0,     0,     0,     0,     0,
       0,     0,     0,   786,     0,   903,   904,     0,     0,     0,
       0,     0,   890,   891,   892,   893,   903,   904,     0,   117,
     420,   467,     0,     0,     0,     0,     0,     0,   906,   565,
     261,     0,   111,   101,   107,     0,     0,     0,     0,     0,
     786,   111,     0,     0,   117,     0,   728,   117,     0,   111,
       0,   111,     0,   171,     0,   117,     0,     0,     0,     0,
     826,     0,     0,     0,    24,   193,   259,     0,     0,   262,
       0,   260,    31,   263,  -169,   731,   732,     0,     0,   101,
     -42,   261,     0,   111,     0,   101,     0,     0,     0,     0,
       0,     0,   114,  -169,   -42,     0,     0,     0,   107,     0,
     107,   114,     0,   107,   111,     0,   107,     0,   111,   114,
     117,   114,   265,     0,     0,   107,   268,     0,   269,   270,
     262,     0,   868,   273,   263,     0,     0,     0,     0,     0,
     280,     0,     0,   107,     0,     0,   101,   284,   285,   286,
       0,     0,  -302,   114,   340,   107,     0,   107,   264,     0,
       0,   107,     0,     0,     0,     0,     0,     0,     0,   896,
       0,   111,     0,   265,   114,   266,   267,   268,   114,   269,
     270,   271,  -302,   272,   273,   274,   275,   276,   277,   278,
     279,   280,  -302,   281,   282,   283,     0,     0,   284,   285,
     286,  -302,     0,     0,   896,     0,     0,     0,  -302,   679,
       0,     0,     0,     0,  -304,   896,     0,   111,     0,     0,
       0,     0,   101,   111,     0,   107,     0,  -302,     0,     0,
     107,   114,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   117,  -304,     0,  -302,   101,     0,     0,
     101,     0,   107,  -302,  -304,     0,     0,     0,   101,     0,
       0,   107,     0,  -304,     0,   107,     0,     0,   107,     0,
    -304,     0,     0,     0,   111,     0,     0,   114,     0,     0,
       0,     0,     0,   114,     0,   117,     0,     0,     0,  -304,
       0,     0,     0,   115,     0,     0,     0,     0,     0,     0,
       0,     0,   115,     0,     0,     0,     0,     0,  -304,     0,
     115,     0,   115,   101,     0,  -304,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,     0,     0,     0,   107,   107,
     107,   107,     0,     0,   115,     0,     0,     0,     0,   117,
     111,   117,     0,     0,   117,   107,   107,   117,     0,     0,
       0,     0,     0,     0,     0,   115,   117,     0,     0,   115,
       0,     0,   261,     0,     0,   111,     0,     0,   111,     0,
       0,     0,     0,     0,   117,     0,   111,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   117,     0,   117,     0,
       0,     0,   117,     0,     0,     0,     0,     0,     0,     0,
     114,   262,   119,     0,     0,   263,     0,     0,     0,     0,
       0,   119,   115,     0,     0,     0,     0,     0,     0,   119,
       0,   119,     0,     0,     0,   114,   101,     0,   114,     0,
       0,   111,     0,     0,     0,     0,   114,     0,     0,     0,
       0,     0,     0,     0,   265,     0,     0,   267,   268,     0,
     269,   270,     0,   119,     0,   273,   117,     0,   115,     0,
       0,   117,   280,     0,   115,     0,     0,     0,   101,   284,
     285,   286,     0,     0,   119,     0,     0,     0,   119,     0,
       0,     0,     0,   117,     0,     0,     0,     0,     0,     0,
       0,   114,   117,     0,     0,     0,   117,     0,     0,   117,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   115,     0,     0,     0,     0,
       0,     0,     0,     0,   184,     0,     0,     0,     0,     0,
       0,   119,   101,     0,   101,     0,     0,   101,   184,     0,
     101,     0,     0,     0,   214,   218,     0,     0,     0,   101,
       0,     0,     0,     0,   111,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,   117,
     117,   117,   117,     0,     0,     0,     0,   119,     0,   101,
       0,   101,     0,   119,     0,   101,   117,   117,     0,     0,
       0,   115,     0,     0,     0,     0,   111,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,     0,   120,     0,   114,     0,   115,     0,     0,   115,
       0,   120,     0,     0,   311,     0,     0,   115,     0,   120,
       0,   120,     0,     0,   119,     0,     0,     0,     0,   301,
     302,  -391,     0,     0,  -416,     0,  -416,     0,     0,   101,
       0,     0,     0,     0,   101,     0,   114,   184,   184,   184,
     111,     0,   111,   120,   347,   111,     0,     0,   111,     0,
       0,     0,     0,     0,     0,   184,   101,   111,   184,     0,
       0,     0,   115,     0,   120,   101,     0,     0,   120,   101,
       0,     0,   101,     0,     0,   111,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   111,     0,   111,
     119,     0,     0,   111,   184,     0,     0,     0,     0,     0,
     114,     0,   114,     0,     0,   114,     0,     0,   114,     0,
       0,     0,     0,     0,     0,   119,     0,   114,   119,     0,
       0,   120,     0,     0,     0,     0,   119,     0,     0,     0,
       0,     0,     0,     0,     0,   114,     0,     0,     0,     0,
       0,     0,   101,   101,   101,   101,     0,   114,     0,   114,
       0,     0,     0,   114,     0,     0,     0,   111,     0,   101,
     101,     0,   111,     0,     0,     0,     0,   120,     0,     0,
       0,     0,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   119,    79,     0,   111,   115,   184,     0,     0,     0,
       0,   128,     0,   111,     0,     0,     0,   111,     0,   154,
     111,   157,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   184,     0,     0,     0,     0,     0,   114,     0,     0,
       0,     0,   114,     0,   120,     0,     0,   115,     0,     0,
       0,     0,     0,   211,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,     0,     0,     0,     0,     0,
       0,     0,     0,   114,    79,     0,     0,   114,   246,     0,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,   111,   111,   111,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   111,   111,     0,
       0,   115,     0,   115,     0,     0,   115,     0,     0,   115,
     120,     0,     0,     0,   119,     0,     0,     0,   115,     0,
       0,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   120,   115,     0,   120,     0,
     114,   114,   114,   114,     0,     0,   120,     0,   115,     0,
     115,     0,     0,     0,   115,     0,   119,   114,   114,     0,
       0,     0,     0,     0,     0,     0,     0,   349,     0,     0,
       0,    14,     0,   362,    17,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,    24,   791,   184,   184,    28,
       0,   184,   184,    31,     0,  -169,     0,     0,     0,     0,
       0,   120,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    41,     0,   792,  -169,     0,   793,    45,   115,     0,
     119,     0,   119,   115,    79,   119,     0,   671,   119,     0,
       0,     0,     0,     0,    57,     0,    58,   119,     0,     0,
       0,     0,     0,     0,     0,   115,     0,     0,     0,     0,
       0,     0,     0,     0,   115,   119,     0,     0,   115,     0,
       0,   115,     0,     0,     0,     0,     0,   119,     0,   119,
       0,     0,     0,   119,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    72,     0,     0,     0,     0,
       0,     0,     0,   859,     0,     0,   184,   184,     0,     0,
      79,     0,     0,     0,     0,     0,   184,     0,   347,     0,
       0,     0,     0,   347,     0,   261,   184,     0,     0,   184,
       0,     0,     0,     0,   120,   362,     0,     0,   362,     0,
       0,   115,   115,   115,   115,     0,     0,   119,     0,     0,
       0,     0,   119,     0,     0,     0,     0,     0,   115,   115,
       0,     0,   779,     0,   262,     0,     0,     0,   263,     0,
       0,     0,     0,     0,   119,     0,   120,     0,     0,     0,
       0,     0,     0,   119,   134,     0,     0,   119,     0,   134,
     119,     0,   264,     0,   159,   134,   134,     0,     0,     0,
       0,   564,     0,   177,     0,   180,     0,   265,     0,   266,
     267,   268,     0,   269,   270,   271,     0,   272,   273,   274,
     275,     0,   277,   278,   279,   280,     0,   281,   221,   283,
       0,     0,   284,   285,   286,     0,     0,     0,     0,     0,
     120,     0,   120,     0,     0,   120,     0,     0,   120,     0,
       0,     0,     0,     0,     0,     0,     0,   120,     0,     0,
     119,   119,   119,   119,     0,   184,     0,     0,    14,     0,
     249,    17,     0,     0,     0,   120,     0,   119,   119,     0,
      23,     0,    24,   791,     0,     0,    28,   120,     0,   120,
      31,     0,  -169,   120,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,   347,     0,   347,   347,   245,     0,
     792,  -169,     0,   793,    45,     0,     0,     0,     0,     0,
       0,     0,   872,   221,   649,     0,     0,     0,   876,   323,
     347,    57,   347,    58,     0,   221,     0,   332,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   323,     0,     0,
     354,     0,     0,     0,     0,     0,     0,   120,     0,     0,
     261,     0,   120,   323,   134,     0,   681,   367,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,    72,     0,   120,     0,     0,     0,     0,     0,
       0,     0,     0,   120,   872,     0,     0,   120,     0,   262,
     120,     0,     0,   263,   259,     0,     0,     0,     0,   260,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   261,
       0,     0,     0,     0,   180,   180,   180,   180,     0,     0,
     362,     0,   362,     0,     0,   362,     0,     0,   362,     0,
       0,   432,   265,     0,   266,   267,   268,   763,   269,   270,
     271,     0,     0,   273,   261,     0,     0,     0,   262,   279,
     280,     0,   263,     0,   283,   775,     0,   284,   285,   286,
     120,   120,   120,   120,   323,   323,     0,   781,     0,   785,
       0,     0,     0,    79,     0,     0,   264,   120,   120,     0,
       0,     0,     0,   262,     0,     0,     0,   263,     0,     0,
     499,   265,   503,   266,   267,   268,     0,   269,   270,   271,
       0,   272,   273,   274,   275,   276,   277,   278,   279,   280,
       0,   281,   282,   283,   354,     0,   284,   285,   286,     0,
       0,     0,   180,     0,     0,     0,   265,   740,   266,   267,
     268,     0,   269,   270,   271,     0,   323,   273,   274,   275,
       0,   277,   278,   279,   280,     0,     0,     0,   283,     0,
       0,   284,   285,   286,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   837,     0,     0,     0,   570,     0,
       0,     0,   180,   842,     0,     0,     0,   844,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    -2,     4,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,    41,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,   323,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     5,    58,    59,  -282,     0,     0,    60,   687,
      61,     0,     0,    62,   688,     0,     0,     0,     0,   653,
       0,     0,     0,   354,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,   672,
       0,     0,     0,   221,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     0,    76,     0,     0,     0,     0,     0,   724,
       0,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   689,   690,   691,   692,   693,   694,     0,     0,
     695,   696,   697,   698,   699,   700,   701,   702,   703,   704,
     262,     0,   705,     0,   263,   499,   706,   707,   708,   709,
     710,   711,   712,   713,   714,   715,   716,     0,   717,     0,
       0,   718,   719,   720,   721,     0,   722,     0,     0,   723,
       0,     0,     0,   354,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   265,     0,   266,   267,   268,     0,   269,
     270,   672,     0,     0,   273,     0,     0,     0,     0,     0,
     279,   280,     0,     0,     0,   283,     0,     0,   284,   285,
     286,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,   798,     5,     6,     7,     8,     9,    10,
       0,  -435,     0,    11,    12,    13,  -435,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,  -435,    21,    22,  -435,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,     0,    42,
      43,  -169,   -41,    44,    45,  -435,  -282,    46,    47,  -435,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,   134,  -435,     0,     0,     0,     0,     0,     0,
       0,   849,     0,     0,     0,     0,     0,     0,  -435,    64,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,     0,  -435,  -435,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,    69,  -435,  -435,
    -435,     0,    72,  -435,  -435,  -435,     0,     0,     0,    74,
    -435,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,    41,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   196,    72,     0,     0,    73,     0,     0,     0,    74,
     232,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,    41,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
     390,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,    41,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
     232,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,    41,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
     789,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,    41,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
     845,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,   245,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,     0,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,   506,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,     0,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,   510,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,     0,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,   748,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,     0,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,   750,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,     0,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,   753,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,     0,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,   756,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,     0,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,   774,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,     0,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,   780,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,     0,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     4,    76,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -282,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,   784,     0,    30,
      31,     0,  -169,    32,    33,    34,    35,    36,   -41,    37,
      38,     0,    39,     0,     0,     0,  -282,    40,     0,    42,
      43,  -169,   -41,    44,    45,     0,  -282,    46,    47,     0,
      48,    49,    50,    51,    52,    53,     0,    54,     0,    55,
      56,    57,     0,    58,    59,  -282,     0,     0,    60,     0,
      61,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,    72,     0,     0,    73,     0,   261,     0,    74,
       0,    75,   534,    76,     5,     6,     7,     8,     9,    10,
       0,  -435,     0,   136,     0,     0,  -435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -435,     0,    22,     0,
       0,     0,     0,     0,   137,   138,   262,   139,     0,    30,
     263,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,   264,  -435,     0,     0,     0,  -435,
       0,     0,    50,    51,    52,   140,     0,     0,     0,   265,
       0,   266,   267,   268,     0,   269,   270,   271,     0,   272,
     273,   274,   275,  -435,   277,   278,   279,   280,     0,   281,
     282,   283,     0,     0,   284,   285,   286,     0,  -435,    64,
    -435,  -435,  -435,     0,  -435,  -435,  -435,     0,  -435,  -435,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,    69,  -435,  -435,
    -435,     0,     0,  -435,  -435,  -435,     0,   261,     0,   141,
       0,    75,   534,   535,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,   262,   139,     0,    30,
     263,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,   265,
       0,   266,   267,   268,     0,   269,   270,   271,     0,   272,
     273,   274,   275,    62,   277,   278,   279,   280,     0,   281,
       0,   283,     0,     0,   284,   285,   286,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   535,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,   301,
     302,     0,     0,     0,  -416,    68,  -416,    69,     0,    70,
      71,   196,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,   563,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   136,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   137,   138,     0,   139,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,     0,    40,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,    51,    52,   140,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,     0,     0,     0,     0,    67,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,    69,     0,
      70,    71,   196,     0,     0,     0,    73,     0,     0,     0,
     141,     0,    75,     0,   142,   866,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,     0,   142,   568,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,   472,    30,     0,   473,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,   474,   475,
      40,     0,    42,     0,     0,     0,     0,     0,     0,   476,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,   477,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,   200,   201,     0,   236,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
     237,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   196,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,   200,   201,     0,   236,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   196,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,   557,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,   200,   201,     0,   236,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   196,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,   559,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,   200,   201,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,     0,   202,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,   200,   201,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   196,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,   663,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,   200,   201,     0,     0,     0,     0,     0,     0,
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
      36,     0,    37,    38,     0,     0,     0,     0,     0,   166,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,     0,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,   171,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,   172,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
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
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,   200,   201,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   196,     0,     0,     0,    73,     0,
       0,     0,   141,   555,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     183,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,     0,     0,     0,     0,    73,     0,
       0,     0,    74,     0,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   196,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,     0,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   822,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   823,    51,    52,   824,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   196,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   825,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   196,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   825,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     811,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,   196,     0,     0,     0,    73,     0,
       0,     0,   141,     0,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
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
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,     0,     0,     0,     0,    73,     0,
       0,     0,    74,     0,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      69,     0,    70,    71,     0,     0,     0,     0,   516,     0,
       0,     0,   141,     0,    75,     0,   142,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   137,   138,     0,
     609,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
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
     612,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
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
     805,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
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
     809,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
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
     810,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
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
     811,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
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
     139,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,     0,
      40,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    52,   140,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,    63,    64,    65,    66,     0,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,    68,    36,
      69,    37,    38,    71,     0,     0,     0,     0,    73,    40,
       0,    42,   141,     0,    75,     0,   142,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -306,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,  -306,     0,     0,     0,  -306,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    69,
       0,   136,     0,   196,     0,     0,     0,     0,     0,     0,
       0,   141,     0,    75,  -306,   825,    22,     0,     0,     0,
       0,     0,   137,   138,     0,   139,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     5,     0,     0,    40,     0,    42,     0,   687,
       0,     0,     0,     0,   688,     0,     0,     0,     0,     0,
      50,    51,    52,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,    64,     0,     0,
       0,   260,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   261,     0,     0,     0,    69,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
       0,   142,   689,   690,   691,   692,   693,   694,     0,     0,
     695,   696,   697,   698,   699,   700,   701,   702,   703,   704,
     262,     0,   705,     0,   263,     0,   706,   707,   708,   709,
     710,   711,   712,   713,   714,   715,   716,     0,   717,     0,
     259,   718,   719,   720,   721,   260,   722,     0,   264,     0,
       0,     0,     0,     0,     0,   261,     0,     0,     0,     0,
       0,     0,     0,   265,     0,   266,   267,   268,     0,   269,
     270,   271,     0,   272,   273,   274,   275,   276,   277,   278,
     279,   280,     0,   281,   282,   283,     0,     0,   284,   285,
     286,     0,     0,     0,   262,     0,     0,     0,   263,   875,
       0,     0,   259,     0,     0,     0,     0,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   261,     0,     0,
       0,     0,   264,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   265,     0,   266,
     267,   268,     0,   269,   270,   271,     0,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   262,   281,   282,   283,
     263,     0,   284,   285,   286,     0,     0,     0,     0,     0,
       0,     0,     0,   912,   259,     0,     0,     0,     0,   260,
       0,     0,     0,     0,   264,     0,     0,     0,     0,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   265,
       0,   266,   267,   268,     0,   269,   270,   271,     0,   272,
     273,   274,   275,   276,   277,   278,   279,   280,     0,   281,
     282,   283,   259,     0,   284,   285,   286,   260,   262,     0,
       0,     0,   263,   556,     0,     0,     0,   261,   338,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   339,
       0,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   265,     0,   266,   267,   268,   262,   269,   270,   271,
     263,   272,   273,   274,   275,   276,   277,   278,   279,   280,
       0,   281,   282,   283,     0,     0,   284,   285,   286,     0,
     340,   259,     0,     0,   264,   848,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   261,   335,     0,   265,
       0,   266,   267,   268,     0,   269,   270,   271,   336,   272,
     273,   274,   275,   276,   277,   278,   279,   280,     0,   281,
     282,   283,     0,     0,   284,   285,   286,     0,     0,   259,
     310,     0,     0,     0,   260,   262,     0,     0,     0,   263,
       0,     0,     0,     0,   261,   638,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   265,     0,
     266,   267,   268,   262,   269,   270,   271,   263,   272,   273,
     274,   275,   276,   277,   278,   279,   280,     0,   281,   282,
     283,   259,     0,   284,   285,   286,   260,   340,     0,   310,
       0,   264,     0,     0,     0,     0,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   265,     0,   266,   267,
     268,     0,   269,   270,   271,     0,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   259,   281,   282,   283,     0,
     260,   284,   285,   286,     0,   262,     0,   310,     0,   263,
     261,   183,     0,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,     0,     0,   264,   260,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   261,   632,     0,     0,   265,   262,
     266,   267,   268,   263,   269,   270,   271,     0,   272,   273,
     274,   275,   276,   277,   278,   279,   280,     0,   281,   282,
     283,     0,     0,   284,   285,   286,     0,   264,     0,   310,
       0,     0,     0,   262,     0,     0,     0,   263,     0,     0,
       0,     0,   265,     0,   266,   267,   268,     0,   269,   270,
     271,     0,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   264,   281,   282,   283,     0,   259,   284,   285,   286,
       0,   260,     0,   310,     0,     0,   265,     0,   266,   267,
     268,   261,   269,   270,   271,     0,   272,   273,   274,   275,
     276,   277,   278,   279,   280,     0,   281,   282,   283,     0,
       0,   284,   285,   286,     0,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,   263,     0,     0,     0,   259,     0,
       0,   747,     0,   260,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   261,     0,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   265,     0,   266,   267,   268,     0,   269,
     270,   271,     0,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   262,   281,   282,   283,   263,     0,   284,   285,
     286,     0,     0,   752,   310,     0,     0,     0,     0,   259,
       0,     0,     0,     0,   260,     0,     0,     0,     0,     0,
     264,     0,     0,     0,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   265,     0,   266,   267,   268,
       0,   269,   270,   271,     0,   272,   273,   274,   275,   276,
     277,   278,   279,   280,     0,   281,   282,   283,     0,     0,
     284,   285,   286,   262,     0,     0,   310,   263,     0,     0,
       0,   259,     0,     0,   860,     0,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   261,     0,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   265,     0,   266,   267,
     268,     0,   269,   270,   271,     0,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   262,   281,   282,   283,   263,
       0,   284,   285,   286,     0,     0,   861,   310,     0,     0,
       0,     0,   259,     0,     0,     0,     0,   260,     0,     0,
       0,     0,     0,   264,     0,     0,     0,   261,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   265,     0,
     266,   267,   268,     0,   269,   270,   271,     0,   272,   273,
     274,   275,   276,   277,   278,   279,   280,     0,   281,   282,
     283,     0,     0,   284,   285,   286,   262,     0,     0,   310,
     263,     0,     0,     0,   259,     0,     0,   862,     0,   260,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   261,
       0,     0,     0,     0,   264,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   265,
       0,   266,   267,   268,     0,   269,   270,   271,     0,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   262,   281,
     282,   283,   263,     0,   284,   285,   286,     0,     0,   863,
     310,     0,     0,     0,     0,   259,     0,     0,     0,     0,
     260,     0,     0,     0,     0,     0,   264,     0,     0,     0,
     261,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   265,     0,   266,   267,   268,     0,   269,   270,   271,
       0,   272,   273,   274,   275,   276,   277,   278,   279,   280,
       0,   281,   282,   283,     0,     0,   284,   285,   286,   262,
       0,     0,   310,   263,     0,     0,     0,   259,     0,     0,
     877,     0,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   261,     0,     0,     0,     0,   264,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   265,     0,   266,   267,   268,     0,   269,   270,
     271,     0,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   262,   281,   282,   283,   263,     0,   284,   285,   286,
       0,     0,   878,   310,     0,     0,     0,     0,   259,     0,
       0,     0,     0,   260,     0,     0,     0,     0,     0,   264,
       0,     0,     0,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   265,     0,   266,   267,   268,     0,
     269,   270,   271,     0,   272,   273,   274,   275,   276,   277,
     278,   279,   280,     0,   281,   282,   283,     0,     0,   284,
     285,   286,   262,     0,     0,   310,   263,     0,     0,     0,
     259,     0,     0,     0,     0,   260,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   261,     0,     0,     0,     0,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   265,     0,   266,   267,   268,
       0,   269,   270,   271,     0,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   262,   281,   282,   283,   263,     0,
     284,   285,   286,     0,     0,     0,   373,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,     0,   260,
       0,     0,   264,     0,     0,     0,     0,     0,     0,   261,
       0,     0,     0,     0,   343,     0,     0,   265,     0,   266,
     267,   268,     0,   269,   270,   271,     0,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   344,   281,   282,   283,
       0,     0,   284,   285,   286,     0,     0,  -392,   262,     0,
       0,     0,   263,     0,     0,     0,   259,     0,     0,     0,
       0,   260,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   261,     0,     0,     0,     0,   264,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,     0,     0,     0,
       0,   265,     0,   266,   267,   268,     0,   269,   270,   271,
       0,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     262,   281,   282,   283,   263,   345,   284,   285,   286,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,   130,   260,     0,     0,   264,     0,
       0,     0,     0,     0,     0,   261,     0,     0,     0,     0,
     386,     0,     0,   265,     0,   266,   267,   268,     0,   269,
     270,   271,     0,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   387,   281,   282,   283,     0,     0,   284,   285,
     286,     0,     0,     0,   262,     0,     0,     0,   263,     0,
       0,     0,   259,     0,     0,     0,     0,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   261,   483,     0,
       0,     0,   264,     0,     0,     0,     0,     0,     0,   484,
       0,     0,     0,     0,     0,     0,     0,   265,     0,   266,
     267,   268,     0,   269,   270,   271,     0,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   262,   281,   282,   283,
     263,     0,   284,   285,   286,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,     0,     0,     0,   260,     0,
       0,     0,     0,     0,   264,     0,     0,     0,   261,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   265,
     486,   266,   267,   268,     0,   269,   270,   271,     0,   272,
     273,   274,   275,   276,   277,   278,   279,   280,     0,   281,
     282,   283,     0,     0,   284,   285,   286,   262,     0,     0,
       0,   263,     0,     0,     0,   259,     0,     0,     0,     0,
     260,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     261,     0,     0,     0,     0,   264,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     265,     0,   266,   267,   268,     0,   269,   270,   271,     0,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   262,
     281,   282,   283,   263,     0,   284,   285,   286,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
       0,   260,     0,     0,     0,     0,     0,   264,     0,     0,
       0,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   265,     0,   266,   267,   268,     0,   269,   270,
     271,     0,   272,   273,   274,   275,   276,   277,   278,   279,
     280,     0,   281,   282,   283,     0,   377,   284,   285,   286,
     262,     0,     0,     0,   263,     0,     0,     0,   259,     0,
       0,     0,     0,   260,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   261,     0,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   265,     0,   266,   267,   268,     0,   269,
     270,   271,     0,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   262,   281,   282,   283,   263,   379,   284,   285,
     286,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,     0,     0,     0,   260,     0,     0,     0,     0,     0,
     264,     0,     0,     0,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   265,     0,   266,   267,   268,
       0,   269,   270,   271,     0,   272,   273,   274,   275,   276,
     277,   278,   279,   280,     0,   281,   282,   283,     0,   381,
     284,   285,   286,   262,     0,     0,     0,   263,     0,     0,
       0,   259,     0,     0,     0,     0,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   261,     0,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   265,     0,   266,   267,
     268,     0,   269,   270,   271,     0,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   262,   281,   282,   283,   263,
     389,   284,   285,   286,     0,     0,     0,     0,     0,     0,
       0,     0,   259,     0,     0,     0,     0,   260,     0,     0,
       0,     0,   403,   264,     0,     0,     0,   261,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   265,     0,
     266,   267,   268,     0,   269,   270,   271,     0,   272,   273,
     274,   275,   276,   277,   278,   279,   280,     0,   281,   282,
     283,     0,     0,   284,   285,   286,   262,     0,     0,     0,
     263,     0,     0,     0,   259,     0,     0,   487,     0,   260,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   261,
       0,     0,     0,     0,   264,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   265,
       0,   266,   267,   268,     0,   269,   270,   271,     0,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   262,   281,
     282,   283,   263,     0,   284,   285,   286,     0,     0,     0,
       0,     0,     0,     0,     0,   259,     0,     0,     0,     0,
     260,     0,   130,     0,     0,     0,   264,     0,     0,     0,
     261,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   265,     0,   266,   267,   268,     0,   269,   270,   271,
       0,   272,   273,   274,   275,   276,   277,   278,   279,   280,
       0,   281,   282,   283,     0,     0,   284,   285,   286,   262,
       0,     0,     0,   263,     0,     0,     0,   259,   645,     0,
       0,     0,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   261,     0,     0,     0,     0,   264,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   265,     0,   266,   267,   268,     0,   269,   270,
     271,     0,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   262,   281,   282,   283,   263,   619,   284,   285,   286,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
       0,     0,     0,   260,     0,     0,     0,     0,     0,   264,
       0,     0,     0,   261,     0,     0,   648,     0,     0,     0,
       0,     0,     0,     0,   265,     0,   266,   267,   268,     0,
     269,   270,   271,     0,   272,   273,   274,   275,   276,   277,
     278,   279,   280,     0,   281,   282,   283,     0,     0,   284,
     285,   286,   262,     0,     0,     0,   263,     0,     0,     0,
     259,     0,     0,     0,     0,   260,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   261,     0,     0,     0,     0,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   265,     0,   266,   267,   268,
       0,   269,   270,   271,     0,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   262,   281,   282,   283,   263,     0,
     284,   285,   286,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,     0,     0,     0,   260,     0,     0,     0,
       0,   682,   264,     0,     0,     0,   261,   735,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   265,     0,   266,
     267,   268,     0,   269,   270,   271,     0,   272,   273,   274,
     275,   276,   277,   278,   279,   280,     0,   281,   282,   283,
       0,     0,   284,   285,   286,   262,     0,     0,     0,   263,
       0,     0,     0,   259,     0,     0,     0,     0,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   261,   738,
       0,     0,     0,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   265,     0,
     266,   267,   268,     0,   269,   270,   271,     0,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   262,   281,   282,
     283,   263,     0,   284,   285,   286,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,     0,   260,
       0,     0,     0,     0,     0,   264,     0,     0,     0,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     265,     0,   266,   267,   268,     0,   269,   270,   271,     0,
     272,   273,   274,   275,   276,   277,   278,   279,   280,     0,
     281,   282,   283,     0,     0,   284,   285,   286,   262,     0,
       0,     0,   263,     0,     0,     0,   259,     0,     0,   804,
       0,   260,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   261,     0,     0,     0,     0,   264,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   265,     0,   266,   267,   268,     0,   269,   270,   271,
       0,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     262,   281,   282,   283,   263,     0,   284,   285,   286,     0,
       0,   807,     0,     0,     0,     0,     0,   259,   820,     0,
       0,     0,   260,     0,     0,     0,     0,     0,   264,     0,
       0,     0,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   265,     0,   266,   267,   268,     0,   269,
     270,   271,     0,   272,   273,   274,   275,   276,   277,   278,
     279,   280,     0,   281,   282,   283,     0,     0,   284,   285,
     286,   262,     0,     0,     0,   263,     0,     0,     0,   259,
       0,     0,     0,     0,   260,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   261,     0,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   265,     0,   266,   267,   268,     0,
     269,   270,   271,     0,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   262,   281,   282,   283,   263,     0,   284,
     285,   286,     0,     0,   884,     0,     0,     0,     0,     0,
     259,     0,     0,     0,     0,   260,     0,     0,     0,     0,
       0,   264,     0,     0,     0,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   265,     0,   266,   267,
     268,     0,   269,   270,   271,     0,   272,   273,   274,   275,
     276,   277,   278,   279,   280,     0,   281,   282,   283,     0,
       0,   284,   285,   286,   262,     0,     0,     0,   263,     0,
       0,     0,   259,     0,     0,   885,     0,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   261,     0,     0,
       0,     0,   264,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   265,     0,   266,
     267,   268,     0,   269,   270,   271,     0,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   262,   281,   282,   283,
     263,     0,   284,   285,   286,     0,     0,   886,     0,     0,
       0,     0,     0,   259,     0,     0,     0,     0,   260,     0,
       0,     0,     0,     0,   264,     0,     0,     0,   261,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   265,
       0,   266,   267,   268,     0,   269,   270,   271,     0,   272,
     273,   274,   275,   276,   277,   278,   279,   280,     0,   281,
     282,   283,     0,     0,   284,   285,   286,   262,     0,     0,
       0,   263,     0,     0,     0,   259,     0,     0,   887,     0,
     260,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     261,     0,     0,     0,     0,   264,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     265,     0,   266,   267,   268,     0,   269,   270,   271,     0,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   262,
     281,   282,   283,   263,     0,   284,   285,   286,     0,     0,
     888,     0,     0,     0,     0,     0,   259,     0,     0,     0,
       0,   260,     0,     0,     0,     0,     0,   264,     0,     0,
       0,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   265,     0,   266,   267,   268,     0,   269,   270,
     271,     0,   272,   273,   274,   275,   276,   277,   278,   279,
     280,     0,   281,   282,   283,     0,     0,   284,   285,   286,
     262,     0,     0,     0,   263,     0,     0,     0,   259,     0,
       0,   889,     0,   260,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   261,     0,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   265,     0,   266,   267,   268,     0,   269,
     270,   271,     0,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   262,   281,   282,   283,   263,     0,   284,   285,
     286,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,     0,     0,     0,   260,     0,     0,     0,     0,     0,
     264,     0,     0,     0,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   265,     0,   266,   267,   268,
       0,   269,   270,   271,     0,   272,   273,   274,   275,   276,
     277,   278,   279,   280,     0,   281,   282,   283,     0,     0,
     284,   285,   286,   607,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   265,     0,   266,   267,
     268,     0,   269,   270,   271,     0,   272,   273,   274,   275,
     276,   277,   278,   279,   280,     0,   281,   282,   283,     0,
       0,   284,   285,   286
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-788))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,     2,   307,    43,    44,   382,   350,   514,   152,    11,
     661,   353,     3,    24,    16,    15,   684,    19,    20,    21,
     164,   533,     1,     1,    26,    27,    28,    29,   242,   176,
     684,     3,   358,    35,    36,    37,     3,    39,    40,     3,
     727,     3,    26,    26,    46,   192,    48,    49,    50,    26,
       3,    53,    54,    55,    56,     1,   382,    59,    60,    61,
      67,    88,    64,   210,    53,    67,    68,    69,    21,    71,
     344,   833,    74,    46,    76,   112,    78,    37,   253,   254,
     255,   256,   112,     0,    37,   866,    59,    40,    67,    67,
      81,    37,    37,    65,    26,    37,    85,    37,   605,    77,
      93,    54,    55,    76,    88,     3,   143,    86,    86,    37,
      49,   141,    65,   387,   901,    67,   143,   768,     3,   140,
     901,   142,   112,   116,   131,   912,     3,    80,    88,   131,
       7,   138,   111,   111,   136,   137,   138,   139,   140,   141,
     142,   795,    27,   905,   812,    17,   658,    93,   148,   140,
     139,   141,   112,   131,   138,   138,   303,   304,   812,   138,
     138,   138,    88,   850,   851,   111,   112,   112,   140,   142,
     112,   138,   112,   140,    21,   177,   140,    47,   140,    37,
     112,   183,   193,   143,   112,   131,   138,   140,    61,     3,
     532,    63,   112,     7,    93,    80,   112,   112,   200,   201,
      93,   143,    25,   143,    80,    75,   138,    65,    55,   514,
      82,   112,   112,   112,   865,   143,   112,   116,    65,   112,
     112,   141,   112,   116,     7,   141,   141,    41,   375,   168,
     169,   170,   234,   234,   140,   237,   411,    84,    75,   139,
     141,    64,   141,   139,    73,    68,    60,   186,   141,   141,
     189,   141,   759,    21,    55,    56,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   207,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   797,   225,    55,   111,   140,
     113,   114,   140,   112,   142,   118,    41,    65,   512,   513,
     605,   140,   125,   305,   306,   307,   650,   112,   310,    26,
     652,   134,    80,    37,   316,    60,   112,    21,   320,   112,
     322,   158,   141,   140,    13,   111,   131,   112,   114,    18,
     137,   333,   118,   335,   336,   131,   338,   339,   131,   176,
       2,   343,   344,    67,   346,   112,   131,   561,   562,    11,
      25,    55,   112,    77,   131,   192,   358,    19,   360,    21,
     333,    65,    86,   112,   131,   140,   368,   142,   112,    93,
     131,   131,   395,   210,   397,   112,    80,   131,   317,   140,
     382,   142,   131,   131,   386,   387,   138,   131,   112,    64,
     392,    53,   131,    68,   131,   131,   773,   399,   400,   401,
      64,   403,   404,   342,    68,    64,   408,   131,   552,    68,
     131,   131,    74,   140,   138,   131,    78,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   131,    17,   408,   111,    20,    87,   114,
     140,   767,   138,   118,   114,   140,    29,   773,    31,    32,
     131,    93,    35,   140,   759,     3,    39,   801,    41,     7,
     131,     9,     7,    47,   111,   131,   303,   304,    93,   131,
     131,   112,   141,    21,    57,    37,    59,    60,    28,    62,
      63,   483,   484,   485,   486,   487,   488,   489,   140,   112,
     490,   491,    93,   141,    16,    21,   141,    80,   141,    82,
     112,   141,   504,   138,   506,   143,   143,    55,   510,   131,
     131,    37,   514,   131,    40,   177,     3,    65,   131,    93,
       7,   183,   524,   131,   131,   395,   131,   397,    54,    55,
     141,   368,    80,   535,    21,   131,    84,   131,   375,    65,
     866,   131,   131,   131,   131,   262,   263,   112,   131,   875,
     131,   131,   131,   131,    80,   392,   139,    28,   395,   131,
     397,   131,   535,   400,   566,   141,   141,   131,    55,   508,
     509,   131,   234,   512,   513,   901,   112,   138,    65,    65,
      26,     2,    26,    20,   143,   138,   912,   111,   305,   306,
      11,    65,   111,    80,    31,    32,   143,    84,    19,   316,
      21,   143,    39,   605,    41,   607,   608,   609,   114,   114,
     612,    25,   140,    86,   141,    77,   141,   683,   387,   336,
     386,   623,   339,    60,   670,   218,   496,   165,   630,   503,
     632,   745,    53,   635,    55,    56,   638,   624,   800,   800,
     650,   643,   801,   645,   686,   647,   648,   905,   310,   865,
      64,   880,    28,    74,    68,   361,   821,    78,   498,   496,
      -1,   498,    -1,   665,   666,    -1,    -1,    -1,   538,    -1,
      -1,    -1,    -1,   335,    -1,   677,   338,   679,   617,   618,
     682,   683,   399,    -1,   346,    -1,    -1,    -1,   627,    -1,
      -1,    -1,    -1,   666,    -1,    -1,    -1,   111,   637,   113,
     114,   640,    -1,    20,   118,    -1,    -1,    -1,    -1,    -1,
     131,   125,    -1,    -1,    31,    32,    -1,    -1,   132,   133,
     134,   723,    39,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      47,    -1,   734,   735,    -1,   737,   738,   739,   740,   401,
      -1,    -1,    -1,    60,    61,   747,   748,    25,   750,    -1,
     752,   753,   792,   793,   756,    -1,   177,   759,    -1,    -1,
      -1,    -1,   183,    -1,    -1,   767,    -1,   484,    -1,   486,
     772,   773,   774,    -1,     2,    -1,   778,    -1,   780,    -1,
      -1,   783,   784,    11,    -1,   787,    64,    -1,   790,   790,
      68,    19,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   804,   805,   806,   807,   808,   809,   810,   811,
      -1,    -1,    -1,   234,    -1,    -1,   478,    -1,   820,    -1,
      -1,   823,   824,   825,    -1,    53,    -1,   829,    -1,   107,
     832,   109,   110,   111,    -1,   113,   114,   776,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    74,   125,    -1,    -1,
      78,    -1,   825,    -1,   132,   133,   134,    -1,   860,   861,
     862,   863,   524,    -1,   866,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   875,    -1,   877,   878,    -1,    -1,    -1,
      -1,    -1,   884,   885,   886,   887,   888,   889,    -1,   310,
     607,   608,    -1,    -1,    -1,    -1,    -1,    -1,   900,   901,
      25,    -1,     2,   131,   566,    -1,    -1,    -1,    -1,    -1,
     912,    11,    -1,    -1,   335,    -1,   578,   338,    -1,    19,
      -1,    21,    -1,    20,    -1,   346,    -1,    -1,    -1,    -1,
     767,    -1,    -1,    -1,    31,    32,    10,    -1,    -1,    64,
      -1,    15,    39,    68,    41,   607,   608,    -1,    -1,   177,
      47,    25,    -1,    53,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,     2,    60,    61,    -1,    -1,    -1,   630,    -1,
     632,    11,    -1,   635,    74,    -1,   638,    -1,    78,    19,
     401,    21,   107,    -1,    -1,   647,   111,    -1,   113,   114,
      64,    -1,   829,   118,    68,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,   665,    -1,    -1,   234,   132,   133,   134,
      -1,    -1,    37,    53,    88,   677,    -1,   679,    92,    -1,
      -1,   683,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   866,
      -1,   131,    -1,   107,    74,   109,   110,   111,    78,   113,
     114,   115,    67,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    77,   127,   128,   129,    -1,    -1,   132,   133,
     134,    86,    -1,    -1,   901,    -1,    -1,    -1,    93,   143,
      -1,    -1,    -1,    -1,    37,   912,    -1,   177,    -1,    -1,
      -1,    -1,   310,   183,    -1,   747,    -1,   112,    -1,    -1,
     752,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   524,    67,    -1,   131,   335,    -1,    -1,
     338,    -1,   774,   138,    77,    -1,    -1,    -1,   346,    -1,
      -1,   783,    -1,    86,    -1,   787,    -1,    -1,   790,    -1,
      93,    -1,    -1,    -1,   234,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,   183,    -1,   566,    -1,    -1,    -1,   112,
      -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,   131,    -1,
      19,    -1,    21,   401,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   234,    -1,    -1,    -1,   860,   861,
     862,   863,    -1,    -1,    53,    -1,    -1,    -1,    -1,   630,
     310,   632,    -1,    -1,   635,   877,   878,   638,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,   647,    -1,    -1,    78,
      -1,    -1,    25,    -1,    -1,   335,    -1,    -1,   338,    -1,
      -1,    -1,    -1,    -1,   665,    -1,   346,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   677,    -1,   679,    -1,
      -1,    -1,   683,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     310,    64,     2,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    11,   131,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    21,    -1,    -1,    -1,   335,   524,    -1,   338,    -1,
      -1,   401,    -1,    -1,    -1,    -1,   346,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,    -1,   110,   111,    -1,
     113,   114,    -1,    53,    -1,   118,   747,    -1,   177,    -1,
      -1,   752,   125,    -1,   183,    -1,    -1,    -1,   566,   132,
     133,   134,    -1,    -1,    74,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,   774,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   401,   783,    -1,    -1,    -1,   787,    -1,    -1,   790,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   234,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   630,    -1,   632,    -1,    -1,   635,    49,    -1,
     638,    -1,    -1,    -1,    55,    56,    -1,    -1,    -1,   647,
      -1,    -1,    -1,    -1,   524,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   665,    -1,   860,
     861,   862,   863,    -1,    -1,    -1,    -1,   177,    -1,   677,
      -1,   679,    -1,   183,    -1,   683,   877,   878,    -1,    -1,
      -1,   310,    -1,    -1,    -1,    -1,   566,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,    -1,     2,    -1,   524,    -1,   335,    -1,    -1,   338,
      -1,    11,    -1,    -1,   135,    -1,    -1,   346,    -1,    19,
      -1,    21,    -1,    -1,   234,    -1,    -1,    -1,    -1,   135,
     136,   137,    -1,    -1,   140,    -1,   142,    -1,    -1,   747,
      -1,    -1,    -1,    -1,   752,    -1,   566,   168,   169,   170,
     630,    -1,   632,    53,   175,   635,    -1,    -1,   638,    -1,
      -1,    -1,    -1,    -1,    -1,   186,   774,   647,   189,    -1,
      -1,    -1,   401,    -1,    74,   783,    -1,    -1,    78,   787,
      -1,    -1,   790,    -1,    -1,   665,   207,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   677,    -1,   679,
     310,    -1,    -1,   683,   225,    -1,    -1,    -1,    -1,    -1,
     630,    -1,   632,    -1,    -1,   635,    -1,    -1,   638,    -1,
      -1,    -1,    -1,    -1,    -1,   335,    -1,   647,   338,    -1,
      -1,   131,    -1,    -1,    -1,    -1,   346,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   665,    -1,    -1,    -1,    -1,
      -1,    -1,   860,   861,   862,   863,    -1,   677,    -1,   679,
      -1,    -1,    -1,   683,    -1,    -1,    -1,   747,    -1,   877,
     878,    -1,   752,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   401,     2,    -1,   774,   524,   317,    -1,    -1,    -1,
      -1,    11,    -1,   783,    -1,    -1,    -1,   787,    -1,    19,
     790,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   342,    -1,    -1,    -1,    -1,    -1,   747,    -1,    -1,
      -1,    -1,   752,    -1,   234,    -1,    -1,   566,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   774,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   783,    74,    -1,    -1,   787,    78,    -1,
     790,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     860,   861,   862,   863,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   877,   878,    -1,
      -1,   630,    -1,   632,    -1,    -1,   635,    -1,    -1,   638,
     310,    -1,    -1,    -1,   524,    -1,    -1,    -1,   647,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   335,   665,    -1,   338,    -1,
     860,   861,   862,   863,    -1,    -1,   346,    -1,   677,    -1,
     679,    -1,    -1,    -1,   683,    -1,   566,   877,   878,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    17,    -1,   183,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    31,    32,   508,   509,    35,
      -1,   512,   513,    39,    -1,    41,    -1,    -1,    -1,    -1,
      -1,   401,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    59,    60,    -1,    62,    63,   747,    -1,
     630,    -1,   632,   752,   234,   635,    -1,   548,   638,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    82,   647,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   774,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   783,   665,    -1,    -1,   787,    -1,
      -1,   790,    -1,    -1,    -1,    -1,    -1,   677,    -1,   679,
      -1,    -1,    -1,   683,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,    -1,   617,   618,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,   627,    -1,   629,    -1,
      -1,    -1,    -1,   634,    -1,    25,   637,    -1,    -1,   640,
      -1,    -1,    -1,    -1,   524,   335,    -1,    -1,   338,    -1,
      -1,   860,   861,   862,   863,    -1,    -1,   747,    -1,    -1,
      -1,    -1,   752,    -1,    -1,    -1,    -1,    -1,   877,   878,
      -1,    -1,   673,    -1,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    -1,   774,    -1,   566,    -1,    -1,    -1,
      -1,    -1,    -1,   783,    13,    -1,    -1,   787,    -1,    18,
     790,    -1,    92,    -1,    23,    24,    25,    -1,    -1,    -1,
      -1,   401,    -1,    32,    -1,    34,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,    -1,   122,   123,   124,   125,    -1,   127,    57,   129,
      -1,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
     630,    -1,   632,    -1,    -1,   635,    -1,    -1,   638,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   647,    -1,    -1,
     860,   861,   862,   863,    -1,   776,    -1,    -1,    17,    -1,
      99,    20,    -1,    -1,    -1,   665,    -1,   877,   878,    -1,
      29,    -1,    31,    32,    -1,    -1,    35,   677,    -1,   679,
      39,    -1,    41,   683,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   814,   815,    -1,   817,   818,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   833,   152,   524,    -1,    -1,    -1,   839,   158,
     841,    80,   843,    82,    -1,   164,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,   747,    -1,    -1,
      25,    -1,   752,   192,   193,    -1,   566,   196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   210,   131,    -1,   774,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   783,   905,    -1,    -1,   787,    -1,    64,
     790,    -1,    -1,    68,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,   253,   254,   255,   256,    -1,    -1,
     630,    -1,   632,    -1,    -1,   635,    -1,    -1,   638,    -1,
      -1,   270,   107,    -1,   109,   110,   111,   647,   113,   114,
     115,    -1,    -1,   118,    25,    -1,    -1,    -1,    64,   124,
     125,    -1,    68,    -1,   129,   665,    -1,   132,   133,   134,
     860,   861,   862,   863,   303,   304,    -1,   677,    -1,   679,
      -1,    -1,    -1,   683,    -1,    -1,    92,   877,   878,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,    -1,
     329,   107,   331,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,   353,    -1,   132,   133,   134,    -1,
      -1,    -1,   361,    -1,    -1,    -1,   107,   143,   109,   110,
     111,    -1,   113,   114,   115,    -1,   375,   118,   119,   120,
      -1,   122,   123,   124,   125,    -1,    -1,    -1,   129,    -1,
      -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   774,    -1,    -1,    -1,   407,    -1,
      -1,    -1,   411,   783,    -1,    -1,    -1,   787,    -1,    -1,
     790,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,   498,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,     3,    82,    83,    84,    -1,    -1,    87,    10,
      89,    -1,    -1,    92,    15,    -1,    -1,    -1,    -1,   528,
      -1,    -1,    -1,   532,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,   548,
      -1,    -1,    -1,   552,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,    -1,    -1,    -1,    -1,    -1,   578,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    97,    98,    -1,    -1,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      64,    -1,   113,    -1,    68,   624,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    -1,   129,    -1,
      -1,   132,   133,   134,   135,    -1,   137,    -1,    -1,   140,
      -1,    -1,    -1,   652,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   670,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
     124,   125,    -1,    -1,    -1,   129,    -1,    -1,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,   702,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    12,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,   791,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   800,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,    -1,    -1,    -1,   138,
     139,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
     139,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
     139,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
     139,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
     139,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
     139,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    25,    -1,   138,
      -1,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    64,    36,    -1,    38,
      68,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    92,    64,    -1,    -1,    -1,    68,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,    92,   122,   123,   124,   125,    -1,   127,
     128,   129,    -1,    -1,   132,   133,   134,    -1,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,    -1,   132,   133,   134,    -1,    25,    -1,   138,
      -1,   140,     1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    64,    36,    -1,    38,
      68,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,    92,   122,   123,   124,   125,    -1,   127,
      -1,   129,    -1,    -1,   132,   133,   134,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,   135,
     136,    -1,    -1,    -1,   140,   124,   142,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,   143,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
     108,   109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,
     128,   129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,
     138,    -1,   140,    -1,   142,   143,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,   143,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    37,    38,    -1,    40,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    54,    55,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,   141,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,   141,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    50,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    32,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,   139,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,   142,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,   107,   108,   109,   110,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,   124,    46,
     126,    48,    49,   129,    -1,    -1,    -1,    -1,   134,    56,
      -1,    58,   138,    -1,   140,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   126,
      -1,    12,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,   140,   141,   142,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,     3,    -1,    -1,    56,    -1,    58,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,   108,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,   142,    93,    94,    95,    96,    97,    98,    -1,    -1,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      64,    -1,   113,    -1,    68,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    -1,   129,    -1,
      10,   132,   133,   134,   135,    15,   137,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,    -1,    -1,   132,   133,
     134,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,   143,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,    10,    -1,   132,   133,   134,    15,    64,    -1,
      -1,    -1,    68,   141,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    64,   113,   114,   115,
      68,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,    -1,   132,   133,   134,    -1,
      88,    10,    -1,    -1,    92,   141,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    37,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,    -1,    -1,   132,   133,   134,    -1,    -1,    10,
     138,    -1,    -1,    -1,    15,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    64,   113,   114,   115,    68,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,    10,    -1,   132,   133,   134,    15,    88,    -1,   138,
      -1,    92,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    10,   127,   128,   129,    -1,
      15,   132,   133,   134,    -1,    64,    -1,   138,    -1,    68,
      25,    26,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    92,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    -1,    -1,   107,    64,
     109,   110,   111,    68,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,    -1,    -1,   132,   133,   134,    -1,    92,    -1,   138,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    92,   127,   128,   129,    -1,    10,   132,   133,   134,
      -1,    15,    -1,   138,    -1,    -1,   107,    -1,   109,   110,
     111,    25,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,    -1,
      -1,   132,   133,   134,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,
      -1,    75,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    64,   127,   128,   129,    68,    -1,   132,   133,
     134,    -1,    -1,    75,   138,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,    -1,
     132,   133,   134,    64,    -1,    -1,   138,    68,    -1,    -1,
      -1,    10,    -1,    -1,    75,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    64,   127,   128,   129,    68,
      -1,   132,   133,   134,    -1,    -1,    75,   138,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,   138,
      68,    -1,    -1,    -1,    10,    -1,    -1,    75,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    64,   127,
     128,   129,    68,    -1,   132,   133,   134,    -1,    -1,    75,
     138,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,    -1,   132,   133,   134,    64,
      -1,    -1,   138,    68,    -1,    -1,    -1,    10,    -1,    -1,
      75,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    64,   127,   128,   129,    68,    -1,   132,   133,   134,
      -1,    -1,    75,   138,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,    -1,    -1,   132,
     133,   134,    64,    -1,    -1,   138,    68,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    64,   127,   128,   129,    68,    -1,
     132,   133,   134,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    30,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    52,   127,   128,   129,
      -1,    -1,   132,   133,   134,    -1,    -1,   137,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      64,   127,   128,   129,    68,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    88,    15,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      30,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    52,   127,   128,   129,    -1,    -1,   132,   133,
     134,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      37,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    64,
     127,   128,   129,    68,    -1,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,   131,   132,   133,   134,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    64,   127,   128,   129,    68,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,   131,
     132,   133,   134,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    64,   127,   128,   129,    68,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    91,    92,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    10,    -1,    -1,    75,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    64,   127,
     128,   129,    68,    -1,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,    -1,   132,   133,   134,    64,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    64,   127,   128,   129,    68,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    25,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,    -1,    -1,   132,
     133,   134,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    64,   127,   128,   129,    68,    -1,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    91,    92,    -1,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
      -1,    -1,   132,   133,   134,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    64,   127,   128,
     129,    68,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,    -1,   132,   133,   134,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    75,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      64,   127,   128,   129,    68,    -1,   132,   133,   134,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,    -1,    -1,   132,   133,
     134,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    64,   127,   128,   129,    68,    -1,   132,
     133,   134,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,    -1,
      -1,   132,   133,   134,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    10,    -1,    -1,    75,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,    -1,   132,   133,   134,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    75,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    64,
     127,   128,   129,    68,    -1,   132,   133,   134,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,
      -1,    75,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    64,   127,   128,   129,    68,    -1,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,    -1,
     132,   133,   134,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,    -1,
      -1,   132,   133,   134
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   148,   149,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    17,    18,    19,    20,    22,    23,
      24,    26,    27,    29,    31,    32,    33,    34,    35,    36,
      38,    39,    42,    43,    44,    45,    46,    48,    49,    51,
      56,    57,    58,    59,    62,    63,    66,    67,    69,    70,
      71,    72,    73,    74,    76,    78,    79,    80,    82,    83,
      87,    89,    92,   107,   108,   109,   110,   115,   124,   126,
     128,   129,   131,   134,   138,   140,   142,   150,   151,   152,
     153,   154,   156,   161,   162,   163,   165,   168,   169,   170,
     171,   172,   173,   175,   176,   177,   181,   182,   185,   186,
     189,   192,   195,   196,   219,   222,   223,   241,   242,   243,
     244,   245,   246,   253,   254,   255,   256,   258,   259,   260,
     261,   262,   263,   265,   266,   267,   268,   269,   152,   256,
      88,   247,   248,   164,   165,   247,    12,    33,    34,    36,
      74,   138,   142,   192,   241,   245,   254,   255,   256,   258,
     260,   261,    80,   164,   152,   235,   256,   152,   140,   165,
       7,   164,   166,     9,    80,   166,    55,    90,   174,   256,
     256,    20,    32,   222,   256,   256,   140,   165,   193,   140,
     165,   224,   225,    26,   156,   167,   256,   256,   256,   256,
     256,     7,   140,    32,   170,   170,   130,   215,   234,   256,
      78,    79,   131,   256,   257,   256,   167,   256,   256,    73,
     140,   152,   256,   256,   156,   163,   256,   258,   156,   163,
     256,   165,   220,   234,   256,   256,   256,   256,   256,   256,
     256,   256,   139,   150,   157,   234,    81,   116,   215,   236,
     237,   257,   234,   256,   264,    57,   152,    61,   155,   165,
      41,    60,   210,    21,    55,    65,    84,   131,   137,    10,
      15,    25,    64,    68,    92,   107,   109,   110,   111,   113,
     114,   115,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   127,   128,   129,   132,   133,   134,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   135,   136,   140,   142,    64,    68,   140,   152,   131,
     138,   156,   256,   256,   256,   234,    37,   247,   220,   131,
     112,   131,    87,   165,   215,   238,   239,   240,   257,   138,
     220,   186,   165,   140,   167,    26,    37,   167,    26,    37,
      88,   167,   250,    30,    52,   131,    75,   156,   238,   152,
     140,   204,    81,   140,   165,   226,   227,     1,   111,   229,
      37,   112,   152,   167,   167,   238,   166,   165,   112,   131,
     256,   256,   131,   138,   167,   140,   238,   131,   178,   131,
     178,   131,    93,   221,   131,   131,    30,    52,   167,   131,
     139,   150,   112,   139,   256,   112,   141,   112,   141,    37,
     112,   143,   250,    91,   112,   143,     7,    47,   111,   187,
     197,    65,   224,   224,   224,   224,   256,   256,   256,   256,
     174,   256,   174,   256,   256,   256,   256,   256,   256,   256,
      27,    80,   165,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   238,   238,   174,   256,   174,
     256,    21,    37,    40,    54,    55,    65,    80,   208,   249,
     252,   256,   269,    26,    37,    26,    37,    75,    37,   143,
     174,   256,   167,   131,   256,   256,    93,   141,   112,   165,
     190,   191,   131,   165,    37,   234,    36,   256,   174,   256,
      36,   256,   174,   256,   140,   167,   134,   158,   160,   256,
     158,   159,   152,   256,    28,   141,   205,   206,   207,   208,
     194,   227,   112,   141,     1,   142,   230,   242,    93,   228,
     256,   225,   141,   215,   256,   183,   238,   141,    16,   179,
     230,   242,   112,   160,   159,   139,   141,   141,   236,   141,
     236,   174,   256,   143,   152,   256,   143,   256,   143,   256,
     165,   234,   138,    21,    55,    65,    80,   199,   209,   224,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     141,   143,    37,    65,   241,   112,   141,    64,    64,    36,
     174,   256,    36,   174,   256,   174,   256,   247,   247,   131,
     215,   257,   240,    93,   112,   139,   187,   256,   141,   256,
      26,   167,    26,   167,   256,    26,   167,   250,    26,   167,
     250,   251,   252,   112,   131,    11,   131,    28,    28,   152,
     112,   141,   140,   165,    21,    55,    65,    80,   211,   141,
     227,   229,   234,    50,   257,    53,    85,   139,   184,   141,
     140,   156,   165,   180,   220,   131,   131,   143,   250,   143,
     250,   152,    91,   138,   188,    65,   198,    10,    15,    93,
      94,    95,    96,    97,    98,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   113,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   129,   132,   133,
     134,   135,   137,   140,   165,   200,   201,   202,   241,   131,
     252,   241,   241,   256,    26,    26,   256,    26,    26,   143,
     143,   167,   167,   256,   191,   138,   167,    75,    36,   256,
      36,   256,    75,    36,   256,   167,    36,   256,   167,   112,
     141,   256,   256,   152,   256,   206,   227,   111,   233,    65,
     229,   228,    37,   143,    26,   152,   234,   180,   111,   156,
      36,   152,   256,   143,    36,   152,   256,   143,   256,   139,
     157,    32,    59,    62,   139,   151,   169,   211,   165,   256,
     114,   140,   203,   203,    75,    36,    36,    75,    36,    36,
      36,    36,   188,   256,   256,   256,   256,   256,   256,   252,
      11,   141,    27,    71,    74,   142,   215,   232,   242,   116,
     217,   228,    86,   218,   256,   230,   242,   152,   167,   141,
     256,   256,   152,   256,   152,   139,   169,   229,   141,   165,
     201,   202,   205,   256,   256,   256,   256,   256,   256,   139,
      75,    75,    75,    75,   256,   233,   143,   234,   215,   216,
     256,   256,   156,   168,   214,   143,   156,    75,    75,    77,
     212,   203,   203,   141,    75,    75,    75,    75,    75,    75,
     256,   256,   256,   256,   217,   228,   215,   231,   232,   242,
      37,   143,   242,   256,   256,   218,   256,   231,   232,   131,
     213,   214,   143,   231
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
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
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
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (6)].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[(1) - (6)].b), (yyvsp[(2) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType, (yyvsp[(6) - (7)].pblockstmt), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 524 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 525 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 530 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 546 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 550 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 557 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 566 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 590 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 592 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 594 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 596 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 598 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 600 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 620 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 625 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 629 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 634 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 644 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 647 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 660 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 661 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 662 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 663 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 668 "chapel.ypp"
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

  case 102:

/* Line 1806 of yacc.c  */
#line 682 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 683 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 684 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 686 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 699 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 705 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 711 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 717 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 723 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 730 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 762 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 782 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 786 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 792 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 796 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 803 "chapel.ypp"
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

  case 151:

/* Line 1806 of yacc.c  */
#line 813 "chapel.ypp"
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

  case 152:

/* Line 1806 of yacc.c  */
#line 825 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 830 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 835 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 843 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 844 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 849 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 851 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 853 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 858 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 868 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[(1) - (1)].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[(1) - (1)].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[(1) - (1)].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 877 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 881 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 894 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 899 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 904 "chapel.ypp"
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

  case 169:

/* Line 1806 of yacc.c  */
#line 924 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 930 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 937 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 945 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 957 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 963 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 970 "chapel.ypp"
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

  case 176:

/* Line 1806 of yacc.c  */
#line 992 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 996 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 1001 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 1005 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 1035 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 1036 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 1037 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 1039 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1041 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1042 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1046 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1047 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1048 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1049 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1050 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1051 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1052 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1053 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1054 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1055 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1056 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1057 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1061 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1062 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1066 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1070 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1071 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1072 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1077 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1083 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1113 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1123 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1142 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1148 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1153 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1159 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1173 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1186 "chapel.ypp"
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

  case 274:

/* Line 1806 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1211 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1219 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1227 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1235 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1244 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1253 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1254 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1255 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1261 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1304 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1329 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1333 "chapel.ypp"
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
#line 1340 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1386 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1391 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1392 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1397 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1398 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1402 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1408 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1412 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1413 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1414 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1415 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1419 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1436 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1442 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1444 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1446 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1448 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 1453 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 354:

/* Line 1806 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1459 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1465 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1471 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1477 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1484 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1490 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1496 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1502 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1550 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1553 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1565 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1570 "chapel.ypp"
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[(1) - (2)].pt); (yyval.pIntentExpr).iVar = (yyvsp[(2) - (2)].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1612 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1614 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1616 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1620 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1621 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1622 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1644 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1646 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1651 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1652 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1660 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1661 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1667 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1668 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1670 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1671 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1672 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1677 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1681 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1688 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 1696 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 1698 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 1703 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 452:

/* Line 1806 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 453:

/* Line 1806 of yacc.c  */
#line 1705 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 454:

/* Line 1806 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 455:

/* Line 1806 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 456:

/* Line 1806 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:

/* Line 1806 of yacc.c  */
#line 1709 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 458:

/* Line 1806 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 459:

/* Line 1806 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 460:

/* Line 1806 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 461:

/* Line 1806 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 462:

/* Line 1806 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 463:

/* Line 1806 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 464:

/* Line 1806 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 465:

/* Line 1806 of yacc.c  */
#line 1720 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 466:

/* Line 1806 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 467:

/* Line 1806 of yacc.c  */
#line 1722 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 468:

/* Line 1806 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 469:

/* Line 1806 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 470:

/* Line 1806 of yacc.c  */
#line 1728 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 471:

/* Line 1806 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 472:

/* Line 1806 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 473:

/* Line 1806 of yacc.c  */
#line 1731 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 474:

/* Line 1806 of yacc.c  */
#line 1735 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 475:

/* Line 1806 of yacc.c  */
#line 1736 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 476:

/* Line 1806 of yacc.c  */
#line 1737 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 477:

/* Line 1806 of yacc.c  */
#line 1738 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 478:

/* Line 1806 of yacc.c  */
#line 1743 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 479:

/* Line 1806 of yacc.c  */
#line 1744 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 480:

/* Line 1806 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 481:

/* Line 1806 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 482:

/* Line 1806 of yacc.c  */
#line 1747 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 483:

/* Line 1806 of yacc.c  */
#line 1748 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 484:

/* Line 1806 of yacc.c  */
#line 1749 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 8469 "bison-chapel.cpp"
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



