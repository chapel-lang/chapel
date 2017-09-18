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
#define YYLAST   13548

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  147
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  123
/* YYNRULES -- Number of rules.  */
#define YYNRULES  485
/* YYNRULES -- Number of states.  */
#define YYNSTATES  918

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
     916,   922,   927,   928,   930,   932,   934,   938,   942,   947,
     953,   955,   957,   961,   963,   966,   970,   971,   974,   977,
     978,   983,   984,   987,   990,   992,   997,  1002,  1009,  1011,
    1012,  1014,  1016,  1020,  1025,  1029,  1034,  1041,  1042,  1045,
    1048,  1051,  1054,  1057,  1060,  1062,  1064,  1068,  1072,  1074,
    1076,  1078,  1082,  1086,  1087,  1089,  1091,  1095,  1099,  1103,
    1105,  1107,  1109,  1111,  1113,  1115,  1117,  1119,  1122,  1127,
    1132,  1137,  1143,  1146,  1149,  1156,  1163,  1168,  1178,  1188,
    1196,  1203,  1210,  1215,  1225,  1235,  1243,  1248,  1255,  1262,
    1272,  1282,  1289,  1291,  1293,  1295,  1297,  1299,  1301,  1303,
    1305,  1309,  1310,  1312,  1317,  1319,  1323,  1328,  1330,  1334,
    1337,  1341,  1345,  1347,  1351,  1354,  1359,  1361,  1363,  1365,
    1367,  1369,  1371,  1373,  1375,  1380,  1384,  1388,  1391,  1394,
    1396,  1399,  1402,  1404,  1406,  1408,  1410,  1412,  1414,  1416,
    1421,  1426,  1431,  1435,  1439,  1443,  1447,  1452,  1456,  1461,
    1463,  1465,  1467,  1469,  1471,  1475,  1480,  1484,  1489,  1493,
    1498,  1502,  1508,  1512,  1516,  1520,  1524,  1528,  1532,  1536,
    1540,  1544,  1548,  1552,  1556,  1560,  1564,  1568,  1572,  1576,
    1580,  1584,  1588,  1592,  1596,  1600,  1603,  1606,  1609,  1612,
    1615,  1618,  1622,  1626,  1630,  1634,  1638,  1642,  1646,  1650,
    1652,  1654,  1656,  1658,  1660,  1662
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
      -1,    24,   256,   131,    -1,    42,   165,   152,    -1,    45,
     256,   167,    -1,    45,   167,    -1,    51,   256,   167,    -1,
      70,   256,   167,    -1,    70,   167,    -1,    74,   152,    -1,
      89,   256,   131,    -1,     1,    -1,   154,   155,    47,   165,
     138,   139,    -1,   154,   155,    47,   165,   138,   157,   139,
      -1,    -1,    62,    -1,    59,    -1,    -1,    61,    -1,   138,
     139,    -1,   138,   157,   139,    -1,   150,    -1,   157,   150,
      -1,   256,    -1,   256,    11,   256,    -1,   158,   112,   256,
      -1,   158,   112,   256,    11,   256,    -1,    -1,   158,    -1,
     134,    -1,   158,    -1,    83,   235,   131,    -1,    83,   256,
      30,   160,   131,    -1,    83,   256,    52,   159,   131,    -1,
      66,   235,   131,    -1,   258,    93,   257,   131,    -1,   258,
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
     142,   235,    37,   256,   143,   152,    -1,   142,   235,    37,
     256,   250,   143,   152,    -1,   142,   235,    37,   174,   143,
     152,    -1,   142,   235,    37,   174,   250,   143,   152,    -1,
     142,   235,   143,   152,    -1,   142,   235,   250,   143,   152,
      -1,    90,   140,   235,   141,    -1,    36,   256,    75,   152,
      -1,    36,   256,   156,    -1,    36,   256,    75,   152,    28,
     152,    -1,    36,   256,   156,    28,   152,    -1,    23,   152,
      -1,    78,   256,   131,    -1,    79,   256,   131,    -1,    78,
     163,    -1,    79,   163,    -1,    78,   156,   178,    -1,    79,
     156,   178,    -1,    -1,   178,   179,    -1,    16,   156,    -1,
      16,   180,   156,    -1,    16,   140,   180,   141,   156,    -1,
     165,    -1,   165,   111,   256,    -1,    76,   256,   131,    -1,
      69,   256,   138,   183,   139,    -1,    -1,   183,   184,    -1,
      85,   235,   167,    -1,    53,   152,    -1,    53,    26,   152,
      -1,   186,   165,   187,   138,   188,   139,    -1,    32,   166,
     186,   165,   187,   138,   188,   139,    -1,    17,    -1,    63,
      -1,    82,    -1,    -1,   111,   235,    -1,    -1,   188,   169,
      -1,   188,   151,   169,    -1,    29,   165,   138,   190,   139,
      -1,   191,    -1,   190,   112,    -1,   190,   112,   191,    -1,
     165,    -1,   165,    93,   256,    -1,    -1,    -1,    43,   193,
     204,   194,   211,   230,   218,   214,    -1,    -1,    39,    -1,
      31,   166,    -1,    32,   166,    -1,    -1,    -1,   195,   210,
     197,   199,   198,   211,   230,   212,   218,   213,    -1,   209,
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
     234,   228,    -1,   207,   165,   234,   217,    -1,   207,   140,
     227,   141,   234,   228,    -1,   207,   140,   227,   141,   234,
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
     221,   112,   220,    -1,    -1,    93,   242,    -1,    93,   231,
      -1,   223,    55,   224,   131,    -1,   223,    21,   224,   131,
      -1,   223,    65,   224,   131,    -1,   223,    21,    65,   224,
     131,    -1,   223,    84,   224,   131,    -1,    -1,    20,    -1,
      32,    -1,   225,    -1,   224,   112,   225,    -1,   165,   230,
     228,    -1,   165,   229,    91,   256,    -1,   140,   227,   141,
     230,   228,    -1,    81,    -1,   165,    -1,   140,   227,   141,
      -1,   226,    -1,   226,   112,    -1,   226,   112,   227,    -1,
      -1,    93,    50,    -1,    93,   257,    -1,    -1,   111,   142,
     235,   143,    -1,    -1,   111,   242,    -1,   111,   231,    -1,
       1,    -1,   142,   235,   143,   242,    -1,   142,   235,   143,
     231,    -1,   142,   235,    37,   256,   143,   242,    -1,     1,
      -1,    -1,   242,    -1,   215,    -1,   142,   143,   232,    -1,
     142,   235,   143,   232,    -1,   142,   143,   233,    -1,   142,
     235,   143,   233,    -1,   142,   235,    37,   256,   143,   232,
      -1,    -1,   111,   242,    -1,   111,   215,    -1,   111,    27,
      -1,   111,    71,    -1,   111,    74,    -1,   111,   233,    -1,
     256,    -1,   215,    -1,   235,   112,   256,    -1,   235,   112,
     215,    -1,    81,    -1,   257,    -1,   215,    -1,   236,   112,
     236,    -1,   237,   112,   236,    -1,    -1,   239,    -1,   240,
      -1,   239,   112,   240,    -1,   165,    93,   215,    -1,   165,
      93,   257,    -1,   215,    -1,   257,    -1,   165,    -1,   245,
      -1,   258,    -1,   244,    -1,   266,    -1,   265,    -1,    71,
     256,    -1,    38,   140,   238,   141,    -1,    27,   140,   238,
     141,    -1,    73,   140,   238,   141,    -1,    72,    73,   140,
     238,   141,    -1,    12,   256,    -1,    74,   256,    -1,    33,
     256,    37,   256,    26,   256,    -1,    33,   256,    37,   174,
      26,   256,    -1,    33,   256,    26,   256,    -1,    33,   256,
      37,   256,    26,    36,   256,    75,   256,    -1,    33,   256,
      37,   174,    26,    36,   256,    75,   256,    -1,    33,   256,
      26,    36,   256,    75,   256,    -1,    34,   256,    37,   256,
      26,   256,    -1,    34,   256,    37,   174,    26,   256,    -1,
      34,   256,    26,   256,    -1,    34,   256,    37,   256,    26,
      36,   256,    75,   256,    -1,    34,   256,    37,   174,    26,
      36,   256,    75,   256,    -1,    34,   256,    26,    36,   256,
      75,   256,    -1,   142,   235,   143,   256,    -1,   142,   235,
      37,   256,   143,   256,    -1,   142,   235,    37,   174,   143,
     256,    -1,   142,   235,    37,   256,   143,    36,   256,    75,
     256,    -1,   142,   235,    37,   174,   143,    36,   256,    75,
     256,    -1,    36,   256,    75,   256,    28,   256,    -1,    49,
      -1,   245,    -1,   241,    -1,   261,    -1,   260,    -1,   192,
      -1,   254,    -1,   255,    -1,   253,   137,   256,    -1,    -1,
     248,    -1,    88,   140,   249,   141,    -1,   252,    -1,   249,
     112,   252,    -1,    88,   140,   251,   141,    -1,   252,    -1,
     251,   112,   252,    -1,   208,   241,    -1,   256,    64,   241,
      -1,   269,    64,   241,    -1,   258,    -1,   253,   137,   256,
      -1,    48,   256,    -1,    44,   224,    37,   256,    -1,   263,
      -1,   242,    -1,   243,    -1,   267,    -1,   268,    -1,   192,
      -1,   254,    -1,   255,    -1,   140,   116,   256,   141,    -1,
     256,   111,   256,    -1,   256,   115,   256,    -1,   256,   115,
      -1,   115,   256,    -1,   115,    -1,    78,   256,    -1,    79,
     256,    -1,   256,    -1,   241,    -1,   260,    -1,   261,    -1,
     262,    -1,   258,    -1,   192,    -1,   259,   140,   238,   141,
      -1,   259,   142,   238,   143,    -1,    58,   140,   238,   141,
      -1,   256,   114,   165,    -1,   256,   114,    80,    -1,   256,
     114,    27,    -1,   140,   236,   141,    -1,   140,   236,   112,
     141,    -1,   140,   237,   141,    -1,   140,   237,   112,   141,
      -1,     4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,
      -1,   138,   235,   139,    -1,   138,   235,   112,   139,    -1,
     142,   235,   143,    -1,   142,   235,   112,   143,    -1,   142,
     264,   143,    -1,   142,   264,   112,   143,    -1,   256,    91,
     256,    -1,   264,   112,   256,    91,   256,    -1,   256,   129,
     256,    -1,   256,   124,   256,    -1,   256,   134,   256,    -1,
     256,   113,   256,    -1,   256,   132,   256,    -1,   256,   133,
     256,    -1,   256,   125,   256,    -1,   256,   117,   256,    -1,
     256,   127,   256,    -1,   256,   123,   256,    -1,   256,   120,
     256,    -1,   256,   122,   256,    -1,   256,   119,   256,    -1,
     256,   107,   256,    -1,   256,   109,   256,    -1,   256,   110,
     256,    -1,   256,    92,   256,    -1,   256,   128,   256,    -1,
     256,   118,   256,    -1,   256,    15,   256,    -1,   256,    10,
     256,    -1,   256,   121,   256,    -1,   256,    25,   256,    -1,
     129,   256,    -1,   124,   256,    -1,    46,   256,    -1,    56,
     256,    -1,   126,   256,    -1,   108,   256,    -1,   256,    64,
     256,    -1,   256,    64,   174,    -1,   269,    64,   256,    -1,
     269,    64,   174,    -1,   256,    68,   256,    -1,   256,    68,
     174,    -1,   269,    68,   256,    -1,   269,    68,   174,    -1,
     129,    -1,   134,    -1,    92,    -1,   128,    -1,   107,    -1,
     109,    -1,   110,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   440,   440,   445,   446,   452,   453,   458,   459,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   497,
     500,   505,   506,   507,   511,   512,   525,   526,   531,   532,
     537,   542,   547,   551,   558,   563,   567,   572,   576,   577,
     578,   582,   586,   588,   590,   592,   594,   596,   598,   600,
     602,   604,   606,   608,   610,   612,   614,   616,   621,   622,
     626,   630,   631,   635,   636,   640,   641,   645,   646,   647,
     648,   649,   650,   651,   652,   656,   657,   661,   662,   663,
     664,   668,   683,   684,   685,   686,   687,   688,   689,   690,
     691,   692,   693,   694,   695,   696,   697,   698,   699,   705,
     711,   717,   723,   730,   740,   744,   745,   746,   747,   751,
     754,   755,   756,   757,   758,   759,   763,   764,   768,   769,
     770,   774,   775,   779,   782,   787,   788,   792,   794,   796,
     803,   813,   826,   831,   836,   844,   845,   850,   851,   853,
     858,   868,   877,   881,   889,   890,   895,   900,   894,   925,
     931,   938,   946,   958,   964,   957,   992,   996,  1001,  1005,
    1013,  1014,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,
    1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,  1035,
    1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,  1047,  1048,
    1049,  1050,  1051,  1052,  1053,  1054,  1055,  1056,  1057,  1058,
    1062,  1063,  1067,  1071,  1072,  1073,  1077,  1079,  1081,  1083,
    1088,  1089,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,
    1101,  1105,  1106,  1107,  1108,  1109,  1110,  1114,  1115,  1119,
    1120,  1121,  1122,  1123,  1124,  1128,  1129,  1132,  1133,  1137,
    1138,  1142,  1144,  1149,  1150,  1154,  1155,  1159,  1160,  1164,
    1166,  1168,  1173,  1186,  1203,  1204,  1206,  1211,  1219,  1227,
    1235,  1244,  1254,  1255,  1256,  1260,  1261,  1269,  1271,  1278,
    1283,  1285,  1287,  1292,  1294,  1296,  1303,  1304,  1305,  1309,
    1310,  1315,  1316,  1317,  1318,  1338,  1342,  1346,  1354,  1358,
    1359,  1360,  1364,  1366,  1372,  1374,  1376,  1381,  1382,  1383,
    1384,  1385,  1386,  1387,  1393,  1394,  1395,  1396,  1400,  1401,
    1402,  1406,  1407,  1411,  1412,  1416,  1417,  1421,  1422,  1423,
    1424,  1428,  1439,  1440,  1441,  1442,  1443,  1444,  1446,  1448,
    1450,  1452,  1454,  1456,  1461,  1463,  1465,  1467,  1469,  1471,
    1473,  1475,  1477,  1479,  1481,  1483,  1485,  1492,  1498,  1504,
    1510,  1519,  1529,  1537,  1538,  1539,  1540,  1541,  1542,  1543,
    1544,  1549,  1550,  1554,  1558,  1561,  1566,  1570,  1573,  1578,
    1582,  1583,  1587,  1588,  1593,  1598,  1606,  1607,  1608,  1609,
    1610,  1611,  1612,  1613,  1614,  1616,  1618,  1620,  1622,  1624,
    1629,  1630,  1631,  1634,  1635,  1636,  1637,  1648,  1649,  1653,
    1654,  1655,  1659,  1660,  1661,  1669,  1670,  1671,  1672,  1676,
    1677,  1678,  1679,  1680,  1681,  1682,  1683,  1684,  1685,  1689,
    1697,  1698,  1702,  1703,  1704,  1705,  1706,  1707,  1708,  1709,
    1710,  1711,  1712,  1713,  1714,  1715,  1716,  1717,  1718,  1719,
    1720,  1721,  1722,  1723,  1724,  1728,  1729,  1730,  1731,  1732,
    1733,  1737,  1738,  1739,  1740,  1744,  1745,  1746,  1747,  1752,
    1753,  1754,  1755,  1756,  1757,  1758
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
     222,   222,   223,   223,   223,   224,   224,   225,   225,   225,
     226,   226,   226,   227,   227,   227,   228,   228,   228,   229,
     229,   230,   230,   230,   230,   231,   231,   231,   231,   232,
     232,   232,   233,   233,   233,   233,   233,   234,   234,   234,
     234,   234,   234,   234,   235,   235,   235,   235,   236,   236,
     236,   237,   237,   238,   238,   239,   239,   240,   240,   240,
     240,   241,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   244,   245,   246,   246,   246,   246,   246,   246,   246,
     246,   247,   247,   248,   249,   249,   250,   251,   251,   252,
     252,   252,   253,   253,   254,   255,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   256,
     257,   257,   257,   258,   258,   258,   258,   259,   259,   260,
     260,   260,   261,   261,   261,   262,   262,   262,   262,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     264,   264,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   266,   266,   266,   266,   266,
     266,   267,   267,   267,   267,   268,   268,   268,   268,   269,
     269,   269,   269,   269,   269,   269
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
       5,     4,     0,     1,     1,     1,     3,     3,     4,     5,
       1,     1,     3,     1,     2,     3,     0,     2,     2,     0,
       4,     0,     2,     2,     1,     4,     4,     6,     1,     0,
       1,     1,     3,     4,     3,     4,     6,     0,     2,     2,
       2,     2,     2,     2,     1,     1,     3,     3,     1,     1,
       1,     3,     3,     0,     1,     1,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     4,     4,
       4,     5,     2,     2,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     4,     1,     3,     2,
       3,     3,     1,     3,     2,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     3,     3,     2,     2,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     4,     3,     3,     3,     3,     4,     3,     4,     1,
       1,     1,     1,     1,     3,     4,     3,     4,     3,     4,
       3,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    38,    80,   429,   430,   431,   432,
     433,     0,   381,    78,   152,   381,     0,   283,    78,     0,
       0,     0,     0,     0,    78,    78,     0,     0,   282,     0,
       0,   170,     0,   166,     0,     0,     0,     0,   372,     0,
       0,     0,     0,   282,   282,   153,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   154,     0,
       0,     0,   481,   483,     0,   484,   485,   409,     0,     0,
     482,   479,    87,   480,     0,     0,     0,     4,     0,     5,
       9,    44,    10,    11,    12,    14,   341,    22,    13,    88,
      94,    15,    17,    16,    19,    20,    21,    18,    93,     0,
      91,   401,     0,    95,    92,    96,     0,   413,   397,   398,
     344,   342,     0,     0,   402,   403,     0,   343,     0,   414,
     415,   416,   396,   346,   345,   399,   400,     0,    24,   352,
       0,     0,   382,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,   401,   413,   342,   402,   403,   381,   343,
     414,   415,     0,     0,   129,     0,     0,   333,     0,    82,
      81,   171,   101,     0,   172,     0,     0,     0,     0,     0,
     283,   284,   100,     0,     0,   333,     0,     0,     0,     0,
       0,   285,     0,    84,    32,     0,   467,   394,     0,   468,
       7,   333,   284,    90,    89,   262,   325,     0,   324,     0,
       0,    85,   412,     0,     0,    35,     0,   347,     0,   333,
      36,   353,     0,   136,   132,     0,   343,   136,   133,     0,
     274,     0,     0,   324,     0,     0,   470,   408,   466,   469,
     465,    46,    48,     0,     0,   328,     0,   330,     0,     0,
     329,     0,   324,     0,     0,     6,    45,     0,   155,   248,
     247,   173,     0,     0,     0,     0,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   333,   333,     0,     0,     0,    25,    26,     0,
      27,     0,     0,     0,     0,     0,     0,     0,    28,    29,
       0,   341,   339,     0,   334,   335,   340,     0,     0,     0,
       0,     0,   110,     0,     0,   109,     0,     0,     0,   116,
       0,     0,    54,    97,     0,   126,     0,    30,   223,   167,
     290,     0,   291,   293,     0,   304,     0,     0,   296,     0,
       0,    83,    31,    33,     0,   172,   261,     0,    61,   410,
     411,    86,   145,    34,   333,     0,   143,   134,   130,   135,
     131,     0,   272,   269,    58,     0,    54,   103,    37,    47,
      49,     0,   434,     0,     0,   425,     0,   427,     0,     0,
       0,     0,     0,     0,   438,     8,     0,     0,     0,   241,
       0,     0,     0,     0,     0,   380,   462,   461,   464,   472,
     471,   476,   475,   458,   455,   456,   457,   405,   445,   424,
     423,   422,   406,   449,   460,   454,   452,   463,   453,   451,
     443,   448,   450,   459,   442,   446,   447,   444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,   473,   478,   477,
     235,   232,   233,   234,   238,   239,   240,     0,     0,   384,
       0,     0,     0,     0,     0,     0,     0,     0,   436,   381,
     381,   106,   270,     0,     0,   349,     0,   164,     0,   161,
     271,   155,     0,     0,     0,   356,     0,     0,     0,   362,
       0,     0,     0,   117,   480,    57,     0,    50,    55,     0,
     125,     0,     0,   348,     0,   224,     0,   231,   249,     0,
     294,     0,   308,     0,   303,   397,     0,     0,   287,   395,
     286,   421,   327,   326,     0,     0,   350,     0,   137,     0,
     276,   397,     0,     0,     0,   435,   404,   426,   331,   428,
     332,     0,     0,   437,   122,   366,     0,   440,   439,     0,
       0,   156,   157,   245,   242,   243,   246,   174,     0,     0,
     278,   277,   279,   281,    62,    69,    70,    71,    66,    68,
      76,    77,    64,    67,    65,    63,    73,    72,    74,    75,
     419,   420,   236,   237,   389,     0,   383,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
     337,   338,   336,     0,   162,   160,     0,     0,   124,     0,
       0,   108,     0,   107,     0,     0,   114,     0,     0,   112,
       0,     0,   387,     0,    98,     0,    99,     0,     0,   128,
     230,   222,     0,   317,   250,   253,   252,   254,     0,   292,
     295,     0,   296,     0,   288,   297,   298,     0,     0,   144,
     146,   351,     0,   138,   141,     0,     0,   273,    59,    60,
       0,     0,     0,     0,   123,     0,     0,   282,   244,   249,
     205,   203,   208,   215,   216,   217,   212,   214,   210,   213,
     211,   209,   219,   218,   184,   187,   185,   186,   197,   188,
     201,   193,   191,   204,   192,   190,   195,   200,   202,   189,
     194,   198,   199,   196,   206,   207,     0,   182,     0,   220,
     220,   180,   280,   385,   413,   413,     0,     0,     0,     0,
       0,     0,     0,     0,   105,   104,   165,   163,   157,   111,
       0,     0,   355,     0,   354,     0,     0,   361,   115,     0,
     360,   113,     0,   386,    52,    51,   127,   371,   225,     0,
       0,   296,   251,   267,   289,     0,     0,     0,   148,     0,
       0,     0,   139,     0,     0,   120,   368,     0,     0,   118,
     367,     0,   441,    39,     0,    78,   282,   282,   150,   282,
     158,     0,   183,     0,     0,   223,   176,   177,     0,     0,
       0,     0,     0,     0,     0,     0,   282,   359,     0,     0,
     365,     0,     0,   388,     0,   317,   320,   321,   322,     0,
     319,   323,   397,   263,   227,   226,     0,     0,     0,   306,
     397,   149,   147,     0,   142,     0,   121,     0,   119,    40,
     159,   255,   181,   182,   220,   220,     0,     0,     0,     0,
       0,     0,     0,   151,     0,     0,     0,     0,    53,   296,
     309,     0,   264,   266,   265,   268,   259,   260,   168,     0,
     140,     0,     0,   256,   267,   178,   179,   221,     0,     0,
       0,     0,     0,     0,   358,   357,   364,   363,   229,   228,
     311,   312,   314,   397,     0,   436,   397,   370,   369,     0,
       0,   313,   315,   257,   175,   258,   309,   316
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   232,    78,   520,    80,    81,   247,    82,
     233,   515,   519,   516,    83,    84,    85,   160,    86,   164,
     184,    87,    88,    89,    90,    91,    92,   615,    93,    94,
      95,   377,   548,   675,    96,    97,   544,   670,    98,    99,
     408,   687,   100,   498,   499,   143,   177,   528,   102,   103,
     409,   689,   577,   728,   729,   730,   806,   349,   524,   525,
     526,   477,   578,   251,   658,   884,   914,   878,   196,   873,
     834,   837,   104,   221,   382,   105,   106,   180,   181,   353,
     354,   538,   357,   358,   534,   901,   831,   771,   234,   238,
     239,   323,   324,   325,   144,   108,   109,   110,   145,   112,
     131,   132,   478,   340,   641,   479,   113,   146,   147,   202,
     326,   149,   118,   150,   151,   121,   122,   243,   123,   124,
     125,   126,   127
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -789
static const yytype_int16 yypact[] =
{
    -789,   100,  2479,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  3678,    28,   118,  -789,    28,  8441,    86,   118,  3678,
    8441,  3678,    73,   118,   230,   291,  7041,  8441,  7181,  8441,
      99,  -789,   118,  -789,    27,  7601,  8441,  8441,  -789,  8441,
    8441,   244,   150,   327,   505,  -789,  7741,  6621,  8441,  7601,
    8441,   238,   187,  3678,  8441,  8581,  8581,   118,  -789,  7741,
    8441,  8441,  -789,  -789,  8441,  -789,  -789,  9981,  8441,  8441,
    -789,  8441,  -789,  -789,  2838,  6201,  7741,  -789,  3538,  -789,
    -789,   294,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,   118,
    -789,   -56,   149,  -789,  -789,  -789,   183,   255,  -789,  -789,
    -789,   277,   283,   224,   290,   292, 13352,  1670,    61,   293,
     295,  -789,  -789,  -789,  -789,  -789,  -789,   284,  -789, 13352,
     229,  3678,  -789,   297,  -789,   289,  8441,  8441,  8441,  8441,
    8441,  7741,  7741,   159,  -789,  -789,  -789,  -789, 11549,   264,
    -789,  -789,   118,   298,  -789, 11818,   306,  6761,   300,  -789,
    -789,  -789,  -789,   118,   158,   118,   301,    16, 10534, 10455,
    -789,  -789,  -789, 11487, 10654,  6761,  3678,   302,    10,    94,
      -6,  -789,  3678,  -789,  -789, 10698,   326,   316, 10698,   326,
    -789,  6761,   189,  -789,  -789,   118,  -789,   114, 13352,  8441,
    8441,  -789, 13352,   304, 11351,  -789, 10698, 13352,   307,  6761,
    -789, 13352, 11889,  -789,  -789, 11951,  2050,  -789,  -789, 12022,
     339,   319,   123, 11623, 10698, 12084,   262,   847,   326,   262,
     326,  -789,  -789,  2978,   176,  -789,  8441,  -789,   -52,    11,
    -789,    76, 12155,   -25,   426,  -789,  -789,   405,   348,  -789,
    -789,  -789,    25,    27,    27,    27,  -789,  8441,  8441,  8441,
    8441,  7881,  7881,  8441,  8441,  8441,  8441,  8441,  8441,    29,
    9981,  8441,  8441,  8441,  8441,  8441,  8441,  8441,  8441,  8441,
    8441,  8441,  8441,  8441,  8441,  8441,  7321,  7321,  7321,  7321,
    7321,  7321,  7321,  7321,  7321,  7321,  7321,  7321,  7321,  7321,
    7321,  7321,  6761,  6761,  7881,  7881,  6061,  -789,  -789,  3118,
    -789, 11685, 11756, 12217,    51,  7881,    16,   333,  -789,  -789,
    8441,   374,  -789,   328,   356,  -789,  -789,   118,   341,   118,
     433,  7741,  -789,  3818,  7881,  -789,  3958,  7881,   338,  -789,
      16,  8721,  8441,  -789,  3678,   459,   347,  -789,   104,  -789,
    -789,    10,  -789,   379,   351,  -789,  5358,   404,   418,  8441,
      27,  -789,  -789,  -789,   357,  -789,  -789,  7741,  -789, 13352,
   13352,  -789,  -789,  -789,  6761,   372,  -789,   481,  -789,   481,
    -789,  5498,   402,  -789,  -789,  8721,  8441,  -789,  -789,  -789,
    -789,  7461,  -789, 10335,  6341,  -789,  6481,  -789,  7881,  5638,
    2686,   381,  8441,  5920,  -789,  -789,   118,  7741,   377,    50,
      27,   179,   193,   210,   241, 11413,  2547,  2547,   132,  -789,
     132,  -789,   132,  5329,   588,  1547,   556,   316,   262,  -789,
    -789,  -789,   847,  2176,   132,   394,   394,  2547,   394,   394,
    1094,   262,  2176,  5189,  1094,   326,   326,   262,   399,   407,
     408,   409,   410,   411,   412,   417,   427,   428,   431,   432,
     440,   442,   443,   445,   392,   435,  -789,   132,  -789,   132,
      55,  -789,  -789,  -789,  -789,  -789,  -789,   118,    70,  -789,
   13414,   296,  8861,  7881,  9001,  7881,  8441,  7881,  1996,    28,
   12288,  -789,  -789, 12350,  6761,  -789,  6761,   464,   211,  -789,
    -789,   348,  8441,    88,  8441, 13352,    24, 10732,  8441, 13352,
      15, 10582,  6061,  -789,   449,   470,   453, 12421,   470,   455,
     562, 12483,  3678,  -789,   106,  -789,    37,  -789,   312,   450,
      10,    96,  -789,  7741,  -789,   958,  8441,  6901,  -789, 13352,
    -789,  -789,  -789, 13352,     8,   451,  -789,    33,  -789,  7741,
    -789,   251,   118,   465,   466,  -789,  -789,  -789,  -789,  -789,
    -789,   -15,  2319,  -789,  -789, 13352,  3678, 13352,  -789, 12554,
     457,   487,  -789,   535,  -789,  -789,  -789,  -789, 10031,   287,
    -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,  -789,  6061,  -789,  7881,  7881,  8441,
     576, 12616,  8441,   582, 12687,   469, 10157,    16,    16,  -789,
    -789,  -789,  -789,  8441,   118,  -789,   471, 10698,  -789, 10819,
    4098,  -789,  4238,  -789, 10881,  4378,  -789,    16,  4518,  -789,
      16,   111,  -789,  8441,  -789,  8441,  -789,  3678,  8441,  -789,
     564,  -789,    10,   503,   550,  -789,  -789,  -789,    82,  -789,
    -789,  5498,   418,    67, 13352,  -789,  -789,  4658,  7741,  -789,
    -789,  -789,   118,  -789,   506,   289,    71,  -789,  -789,  -789,
    4798,   479,  4938,   480,  -789,  8441,  3258,   414,  -789,   312,
    -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,  -789,   118,  -789,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,  -789,  -789,  8441,   513,   517,   492,
     492,  -789,  -789,  -789,   112,   184, 12749,  9141,  9281, 12820,
    9421,  9561,  9701,  9841,  -789,  -789, 13352,  -789,  -789,  -789,
    3678,  8441, 13352,  8441, 13352,  3678,  8441, 13352,  -789,  8441,
   13352,  -789,  6061,  -789, 12882, 13352,  -789, 13352,  -789,   494,
    8021,     6,  -789,   551,  -789,  7881,  5078,  3678,  -789,   -14,
     497,  8441,  -789,  5218,  8441,  -789, 13352,  3678,  8441,  -789,
   13352,  3678, 13352,  -789,  3398,   514,   529,   529,  -789,  1364,
    -789,    84,  -789, 10407, 10082,   104,  -789,  -789,  8441,  8441,
    8441,  8441,  8441,  8441,  8441,  8441,  1215, 12483, 10952, 11014,
   12483, 11085, 11147,  -789,  8441,   503,    73,  8441,  8441,  5779,
    -789,  -789,   204,  7741,  -789,  -789,  8441,    43, 10219,  -789,
     985,   306,  -789,   289, 13352, 11218,  -789, 11280,  -789,  -789,
    -789,   563,  -789,  -789,   492,   492,   196, 12953, 13015, 13086,
   13148, 13219, 13281,  -789,  3678,  3678,  3678,  3678, 13352,     6,
    8161,    74,  -789,  -789, 13352, 13352,  -789,  -789,  -789,  9841,
    -789,  3678,  3678,  -789,   551,  -789,  -789,  -789,  8441,  8441,
    8441,  8441,  8441,  8441, 12483, 12483, 12483, 12483,  -789,  -789,
    -789,  -789,  -789,   269,  7881,  1853,  1032, 12483, 12483,   -12,
   10291,  -789,  -789,  -789,  -789,  -789,  8301,  -789
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -789,  -789,  -789,    -1,  -638,  1559,  -789,  -789,  -789,  1500,
     -47,  -274,   256,   263,  -789,  -789,   361,    83,  2071,   -16,
     387,  -767,  -644,   -41,  -789,  -789,  -789,   -20,  -789,  -789,
    -789,   430,  -789,   -23,  -789,  -789,  -789,  -789,  -789,   490,
     154,   -91,  -789,  -789,    34,   693,  -789,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,  -144,  -140,  -709,  -789,  -137,    21,
    -789,  -344,  -789,  -789,   -17,  -789,  -789,  -236,   683,  -789,
    -194,  -208,  -789,  -141,  -789,   649,  -789,  -174,   322,  -789,
    -331,  -657,  -789,  -515,  -374,  -714,  -788,  -138,    18,    19,
    -789,  -146,  -789,   195,   317,  -254,  -789,  -789,   831,  -789,
       0,  -789,  -789,  -218,  -789,  -473,  -789,   916,  1054,    -2,
    1013,   401,  -789,  1195,  1411,  -789,  -789,  -789,  -789,  -789,
    -789,  -789,  -296
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -437
static const yytype_int16 yytable[] =
{
     116,    77,   193,   194,   527,   774,   167,   550,   161,   129,
     481,   317,   182,     5,   148,   135,   662,   116,   155,   116,
     529,   807,   328,   401,   168,   169,   173,   174,     5,   346,
       5,   359,     5,   185,   186,   187,     5,   188,   189,   642,
       5,   635,   182,   800,   198,   364,   204,   206,   207,   799,
     630,   211,   212,   215,   219,    47,   429,   223,   224,   225,
     394,   667,   226,   375,   197,   227,   228,   229,   518,   230,
     877,   573,   198,   338,   242,  -377,   116,   222,   411,   412,
     413,   414,   902,   355,  -418,   355,  -418,   403,   487,   395,
     410,   350,   602,   668,   241,   355,   133,   355,   367,   537,
       3,   153,   535,   338,   775,   574,   360,  -230,   775,   430,
      47,   904,   518,   398,   835,   575,   130,   912,   404,   913,
     603,     5,   833,   396,   309,   470,   309,   551,   680,   116,
     576,  -301,   733,  -301,   129,   311,   312,   313,   211,   198,
     242,   471,   877,   773,   472,   885,   886,   669,   316,  -301,
     351,  -301,   397,   309,   309,   850,   464,   465,   473,   474,
     314,  -301,   309,   399,   338,   178,   152,   178,  -301,   475,
    -301,   309,   800,   672,   116,    14,   365,   652,   799,   399,
     116,   309,   605,   399,   476,  -299,   399,  -301,   399,  -301,
     249,   911,     5,   661,   488,   661,   159,   369,   370,   660,
     367,   302,   917,   303,   252,   356,  -301,   661,  -301,   250,
     776,   606,   899,   157,   783,  -301,   481,   905,   650,   400,
    -301,    45,  -301,   762,  -390,  -301,   367,  -301,   545,   628,
     -78,   116,   390,     5,   393,   367,   579,   159,   253,   175,
      58,   419,   421,   267,  -230,   368,   269,   651,   254,   -78,
     272,   190,   763,  -390,   384,   415,   416,   417,   418,   420,
     422,   423,   424,   425,   426,   427,   428,   255,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   466,   468,   851,   260,   391,   823,
     191,   360,   637,   640,     5,   489,  -391,  -318,   159,  -418,
     162,  -418,   467,   469,   480,   360,   527,   116,   650,   481,
     580,   208,  -284,   490,   506,   392,  -318,   510,   493,   107,
    -318,   769,   360,   624,   581,  -391,   261,   209,   107,   198,
     262,   505,   507,   654,   509,   511,   107,   887,   107,   517,
     517,   582,   521,   681,   683,  -318,  -284,   170,   304,   503,
     625,   260,   305,   360,   116,   246,  -284,   539,    24,   192,
     608,   257,  -310,  -275,   305,   543,    31,   655,  -169,   306,
     107,   163,   583,   267,   -43,  -284,   269,   656,   561,   116,
     272,  -310,  -275,   517,   517,  -310,  -374,  -169,   -43,   543,
     261,   107,   657,   320,   262,   107,   562,   543,   565,   360,
     567,   569,   839,   117,  -417,   198,  -417,   535,  -373,   839,
    -310,   677,   117,   558,   256,   560,   214,   218,   732,   260,
     117,  -378,   117,  -379,  -376,   571,  -375,   309,   308,   318,
     269,    14,   381,   405,    17,   371,   205,   267,   327,   268,
     269,   331,   348,    23,   272,    24,   795,   374,   107,    28,
     383,   279,   406,    31,   117,  -169,   216,   216,   261,   407,
     285,   527,   262,   610,   492,   613,   481,   494,   496,   495,
     502,    41,   500,   796,  -169,   117,   797,    45,   512,   117,
     505,   611,   509,   614,   521,   616,   565,   522,   523,   617,
     618,   530,   531,   107,    57,   536,    58,   547,   541,   107,
     627,   264,   629,   265,   266,   267,   634,   268,   269,   270,
     480,   537,   272,   546,   552,   572,   832,     5,   278,   279,
     116,   159,   840,   282,   566,   170,   283,   284,   285,   840,
     584,   242,   117,   600,   664,  -284,    24,   192,   585,   586,
     587,   588,   589,   590,    31,    72,  -169,   242,   591,   170,
     107,   663,   -42,   798,   332,   335,   339,   623,   592,   593,
      24,   192,   594,   595,   116,  -169,   -42,   676,    31,  -284,
    -169,   596,   362,   597,   598,   363,   599,   117,   601,  -284,
     -56,   260,   643,   117,   644,   470,   646,   419,   466,  -169,
     647,   659,   671,   373,   163,   686,   678,   679,  -284,   367,
     688,   471,   737,   480,   472,   420,   467,   736,   740,   748,
     739,   387,   742,   260,   770,   772,   903,   781,   473,   474,
     261,   746,   787,   791,   262,   906,   107,  -341,   752,   475,
     754,   804,   805,   757,   117,   825,   760,   836,   843,   794,
     883,   764,   554,   765,   476,   116,   767,   379,   553,   780,
     107,   903,   261,   107,   329,   626,   262,   816,   747,   116,
     854,   107,   903,   264,   855,   116,   198,   267,   856,   268,
     269,   768,   801,   915,   272,   898,   909,   172,   786,     0,
     790,   279,   540,   792,   116,     0,   779,   869,   283,   284,
     285,   622,     0,     0,     0,   101,     0,     0,     0,   267,
       0,   268,   269,   491,   101,     0,   272,     0,     0,     0,
     117,     0,   101,   279,   101,     0,     0,   107,     0,     0,
     283,   284,   285,     0,   803,     0,     0,   513,     0,     0,
       0,     0,     0,     0,   117,   752,   754,   117,   757,   760,
     786,   790,     0,     0,     0,   117,   101,     0,   817,   818,
       0,   819,     0,   820,   821,   193,   194,   822,   237,     0,
     480,     0,     0,     0,     0,     0,     0,   101,   116,     0,
       0,   101,     0,   838,   565,   116,     0,     0,     0,   844,
       0,   565,   845,     0,     0,   116,   847,     0,     0,   116,
       0,     0,   116,   390,   604,     0,     0,     0,     0,     0,
       0,   117,     0,     0,     0,     0,   817,   857,   858,   820,
     859,   860,   861,   862,     0,     0,     0,     0,     0,     0,
       0,     0,   868,     0,   101,   207,   211,   242,     0,     0,
       0,   874,     0,   111,   875,     0,     0,     0,     0,   107,
     322,     0,   111,     0,     0,     0,     0,   871,     0,     0,
     111,     0,   111,     0,     0,     0,     0,     0,   322,     0,
       0,     0,   894,   895,   896,   897,     0,     0,   116,   101,
       0,     0,   260,     0,   322,   101,     0,   790,     0,   907,
     908,     0,     0,   107,   111,     0,   894,   895,   896,   897,
     907,   908,   322,   631,   633,   731,     0,   636,   639,     0,
       0,     0,   910,   565,     0,   111,     0,     0,     0,   111,
       0,   261,     0,     0,   790,   262,     0,     0,   114,     0,
       0,     0,     0,   117,   734,   735,   101,   114,     0,     0,
       0,     0,     0,     0,     0,   114,     0,   114,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   107,     0,   107,
       0,     0,   107,     0,   264,   107,   265,   266,   267,     0,
     268,   269,   111,     0,   107,   272,     0,   117,     0,   114,
       0,   278,   279,     0,     0,     0,   282,     0,     0,   283,
     284,   285,     0,     0,   107,   322,   322,     0,     0,     0,
     114,     0,     0,     0,   114,  -302,     0,   107,     0,   107,
       0,     0,   101,   107,   744,   745,     0,   111,     0,     0,
       0,     0,     0,   111,   749,     0,     0,     0,     0,     0,
       0,     0,  -305,     0,   758,  -302,   101,   761,     0,   101,
       0,   117,     0,   117,     0,  -302,   117,   101,     0,   117,
       0,     0,     0,     0,  -302,     0,     0,   114,   117,     0,
     542,  -302,  -305,     0,     0,     0,   115,   322,     0,     0,
     203,     0,  -305,     0,   111,   115,     0,   107,   117,  -307,
    -302,  -305,   107,   115,   542,   115,     0,   237,  -305,   237,
       0,   117,   542,   117,     0,     0,     0,   117,   240,  -302,
       0,     0,   114,   101,   107,     0,  -302,  -305,   114,  -307,
       0,     0,     0,     0,   107,     0,     0,   115,   107,  -307,
       0,   107,     0,     0,     0,     0,  -305,     0,  -307,   260,
       0,     0,     0,  -305,     0,  -307,     0,     0,   115,     0,
       0,     0,   115,     0,     0,     0,     0,     0,     0,     0,
     111,     0,     0,     0,  -307,     0,     0,     0,     0,   114,
       0,   117,     0,     0,     0,     0,   117,     0,   261,     0,
       0,     0,   262,  -307,   111,     0,   842,   111,     0,     0,
    -307,     0,     0,     0,     0,   111,     0,   620,   117,   322,
       0,   107,   107,   107,   107,   115,     0,     0,   117,     0,
       0,     0,   117,     0,     0,   117,     0,   119,   107,   107,
       0,   264,     0,   265,   266,   267,   119,   268,   269,     0,
       0,     0,   272,     0,   119,   101,   119,     0,     0,   279,
       0,     0,     0,     0,     0,   114,   283,   284,   285,     0,
     115,   111,    14,     0,     0,    17,   115,     0,     0,     0,
       0,     0,     0,     0,    23,     0,    24,   795,   119,   114,
      28,     0,   114,     0,    31,     0,  -169,     0,     0,   101,
     114,     0,     0,     0,     0,   117,   117,   117,   117,   119,
       0,     0,    41,   119,   796,  -169,     0,   797,    45,     0,
       0,     0,   117,   117,     0,     0,     0,   115,     0,     0,
       0,     0,     0,     0,     0,    57,     0,    58,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,     0,   114,     0,     0,     0,
       0,     0,     0,   101,     0,   101,   119,     0,   101,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,    72,     0,     0,     0,
       0,     0,     0,   111,   863,     0,     0,     0,     0,     0,
     101,     0,     0,   115,     0,     0,     0,     0,     0,     0,
       0,   119,     0,   101,     0,   101,     0,   119,     0,   101,
       0,    14,     0,     0,    17,     0,     0,   115,     0,     0,
     115,     0,     0,    23,     0,    24,   795,   111,   115,    28,
       0,     0,     0,    31,     0,  -169,     0,   240,     0,   240,
       0,     0,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   244,   120,   796,  -169,     0,   797,    45,   119,     0,
     120,     0,   120,     0,     0,     0,     0,     0,   114,     0,
       0,     0,     0,   101,    57,     0,    58,     0,   101,     0,
       0,     0,     0,   830,   115,     0,     0,     0,     0,     0,
       0,   111,     0,   111,   120,     0,   111,     0,     0,   111,
     101,     0,     0,     0,     0,     0,     0,     0,   111,     0,
     101,     0,   114,     0,   101,   120,     0,   101,     0,   120,
       0,     0,     0,     0,     0,    72,     0,     0,   111,     0,
       0,     0,     0,     0,   119,     0,     0,   621,     0,     0,
       0,   111,     0,   111,     0,     0,   872,   111,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   119,     0,
       0,   119,     0,     0,     0,   183,     0,     0,     0,   119,
       0,     0,   120,     0,     0,     0,   114,     0,   114,   183,
     666,   114,     0,   900,   114,   213,   217,   101,   101,   101,
     101,    79,     0,   114,     0,     0,     0,     0,     0,     0,
     128,     0,   260,     0,   101,   101,   115,     0,   154,     0,
     156,   111,     0,   114,     0,     0,   111,   120,   900,     0,
       0,     0,     0,   120,     0,   119,   114,     0,   114,   900,
       0,     0,   114,     0,     0,     0,     0,     0,   111,     0,
       0,   261,   210,     0,     0,   262,     0,     0,   111,     0,
     115,     0,   111,     0,     0,   111,     0,     0,     0,     0,
       0,     0,     0,    79,     0,   310,     0,   245,     0,     0,
       0,     0,     0,     0,   120,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,     0,   266,   267,     0,
     268,   269,     0,     0,     0,   272,   114,   183,   183,   183,
       0,   114,   279,     0,   345,     0,     0,     0,     0,   283,
     284,   285,     0,     0,   115,   183,   115,     0,   183,   115,
     307,     0,   115,   114,     0,   111,   111,   111,   111,     0,
       0,   115,     0,   114,     0,     0,   183,   114,     0,     0,
     114,     0,   111,   111,     0,     0,     0,   119,     0,     0,
     120,   115,     0,     0,   183,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   115,   347,   115,     0,     0,     0,
     115,   361,     0,     0,   120,     0,     0,   120,     0,     0,
       0,     0,     0,     0,     0,   120,     0,     0,     0,     0,
       0,   119,     0,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,     0,     0,     0,
     114,   114,   114,   114,     0,     0,     0,     0,     0,     0,
       0,     0,    79,     0,     0,     0,     0,   114,   114,     0,
       0,     0,     0,     0,   115,   300,   301,  -392,     0,   115,
    -417,   120,  -417,     0,     0,     0,   183,     0,     0,     0,
       0,     0,     0,     0,     0,   119,     0,   119,     0,     0,
     119,   115,     0,   119,     0,     0,     0,     0,     0,     0,
     183,   115,   119,     0,     0,   115,     0,     0,   115,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     8,
       9,    10,   119,     0,     0,   136,     0,     0,    79,     0,
       0,     0,     0,     0,     0,   119,     0,   119,     0,     0,
      22,   119,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,   361,     0,     0,   361,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,   115,   115,
     115,   115,     0,     0,    50,    51,    52,   140,     0,     0,
       0,     0,     0,   120,     0,   115,   115,     0,     0,     0,
       0,     0,     0,     0,     0,   119,  -309,     0,     0,     0,
     119,     0,     0,     0,     0,     0,     0,     0,     0,   564,
       0,    64,     0,     0,     0,  -309,     0,     0,     0,  -309,
       0,     0,   119,     0,     0,     0,     0,   120,     0,    69,
       0,     0,   119,   195,     0,     0,   119,     0,     0,   119,
       0,   141,     0,    75,  -309,   829,     0,     0,     0,     5,
       6,     7,     8,     9,    10,     0,   183,   183,   136,     0,
     183,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   137,
     138,     0,   139,     0,    30,     0,     0,     0,     0,    33,
      34,   120,    36,   120,    37,    38,   120,   673,     0,   120,
       0,     0,    40,     0,    42,     0,     0,     0,   120,   119,
     119,   119,   119,     0,     0,     0,     0,    50,    51,    52,
     140,     0,     0,     0,     0,     0,   119,   119,   120,     0,
       0,   649,     0,     0,   134,     0,     0,     0,     0,   134,
       0,   120,     0,   120,   158,   134,   134,   120,     0,     0,
       0,     0,     0,   176,    64,   179,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,   183,     0,
       0,     0,    69,     0,     0,   684,     0,   183,   220,   345,
       0,     0,     0,     0,   345,     0,    75,   183,   142,     0,
     183,     0,     0,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,     0,     0,     0,
       0,   120,     0,     0,     0,     0,   120,     0,     0,     0,
     248,     0,     0,     0,     0,   782,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   300,   301,     0,   120,   361,
    -417,   361,  -417,     0,   361,     0,     0,   361,   120,     0,
       0,   260,   120,     0,     0,   120,   766,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   220,     0,     0,   778,     0,   321,     0,
       0,     0,     0,     0,   220,     0,   330,     0,     0,   785,
     261,   789,     0,     0,   262,    79,   321,     0,     0,   352,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   321,   134,     0,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,   120,   120,   120,   120,   183,
     321,     0,     0,   264,     0,   265,   266,   267,     0,   268,
     269,   270,   120,   120,   272,   273,   274,     0,   276,   277,
     278,   279,     0,     0,     0,   282,     0,     0,   283,   284,
     285,     0,     0,     0,     0,     0,     0,     0,   345,   345,
       0,   345,   345,   179,   179,   179,   179,     0,     0,   258,
       0,     0,     0,     0,   259,     0,   841,   876,     0,     0,
     431,     0,     0,   880,   260,   345,   846,   345,     0,     0,
     848,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   321,   321,     0,     0,     0,     0,     0,
       0,     0,     0,   261,     0,     0,     0,   262,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,     0,
     501,     0,     0,     0,     0,     0,     0,   338,     0,   876,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   352,     0,     0,     0,   264,     0,   265,   266,
     267,   179,   268,   269,   270,     0,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   321,   280,   281,   282,     0,
       0,   283,   284,   285,     0,     0,     0,     0,     0,     0,
       0,     0,   682,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,     0,    -2,
       4,   179,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -282,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
    -169,    32,    33,    34,    35,    36,   -41,    37,    38,     0,
      39,     0,     0,     0,  -282,    40,    41,    42,    43,  -169,
     -41,    44,    45,     0,  -282,    46,    47,     0,    48,    49,
      50,    51,    52,    53,     0,    54,     0,    55,    56,    57,
       0,    58,    59,  -282,     0,     0,    60,   321,    61,     0,
       0,    62,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,   653,     0,     0,
       0,   352,     0,    68,     0,    69,     0,    70,    71,     0,
      72,   261,     0,    73,     0,   262,     0,    74,   674,    75,
       0,    76,     0,   220,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   727,
       0,     0,     0,     0,   264,     0,   265,   266,   267,     0,
     268,   269,   270,     0,   271,   272,   273,   274,     0,   276,
     277,   278,   279,     0,   280,   281,   282,     0,     0,   283,
     284,   285,     0,     0,     0,     0,     0,     4,     0,     5,
       6,     7,     8,     9,    10,   497,  -436,     0,    11,    12,
      13,  -436,     0,    14,    15,    16,    17,  -282,    18,    19,
      20,  -436,    21,    22,  -436,    23,     0,    24,    25,    26,
      27,    28,    29,   352,    30,    31,     0,  -169,    32,    33,
      34,    35,    36,   -41,    37,    38,     0,    39,     0,     0,
       0,  -282,    40,   674,    42,    43,  -169,   -41,    44,    45,
    -436,  -282,    46,    47,  -436,    48,    49,    50,    51,    52,
      53,     0,    54,     0,    55,    56,    57,     0,    58,    59,
    -282,     0,     0,    60,     0,    61,   802,     0,  -436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -436,    64,  -436,  -436,  -436,  -436,  -436,
    -436,  -436,     0,  -436,  -436,  -436,  -436,  -436,  -436,  -436,
    -436,  -436,    69,  -436,  -436,  -436,     0,    72,  -436,  -436,
    -436,     0,     0,     0,    74,  -436,    75,     0,    76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       0,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,   134,    23,     0,    24,
      25,    26,    27,    28,    29,   853,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,    41,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   195,    72,
       0,     0,    73,     0,     0,     0,    74,   231,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,    41,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,   389,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,    41,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,   231,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,    41,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,   793,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,    41,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,   849,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,   244,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,     0,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,   504,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,     0,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,   508,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,     0,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,   751,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,     0,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,   753,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,     0,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,   756,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,     0,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,   759,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,     0,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,   777,    22,     0,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,     0,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,   784,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,     0,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     4,
      76,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -282,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,   788,     0,    30,    31,     0,  -169,
      32,    33,    34,    35,    36,   -41,    37,    38,     0,    39,
       0,     0,     0,  -282,    40,     0,    42,    43,  -169,   -41,
      44,    45,     0,  -282,    46,    47,     0,    48,    49,    50,
      51,    52,    53,     0,    54,     0,    55,    56,    57,     0,
      58,    59,  -282,     0,     0,    60,     0,    61,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,    72,
       0,     0,    73,     0,     0,     0,    74,     0,    75,   532,
      76,     5,     6,     7,     8,     9,    10,     0,  -436,     0,
     136,     0,     0,  -436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -436,     0,    22,     0,     0,     0,     0,
       0,   137,   138,     0,   139,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,  -436,     0,     0,     0,  -436,     0,     0,    50,
      51,    52,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -300,
    -436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -436,    64,  -436,  -436,  -436,
       0,  -436,  -436,  -436,     0,  -436,  -436,  -436,  -436,  -436,
    -436,  -436,  -436,  -436,    69,  -436,  -436,  -436,     0,     0,
    -436,  -436,  -436,     0,   260,     0,   141,     0,    75,   532,
     549,     5,     6,     7,     8,     9,    10,     0,  -436,     0,
     136,     0,     0,  -436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -436,     0,    22,     0,     0,     0,     0,
       0,   137,   138,   261,   139,     0,    30,   262,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,   263,  -436,     0,     0,     0,  -436,     0,     0,    50,
      51,    52,   140,     0,     0,     0,   264,     0,   265,   266,
     267,     0,   268,   269,   270,     0,   271,   272,   273,   274,
    -436,   276,   277,   278,   279,     0,   280,     0,   282,     0,
       0,   283,   284,   285,     0,  -436,    64,  -436,  -436,  -436,
       0,  -436,  -436,  -436,     0,  -436,  -436,  -436,  -436,  -436,
    -436,  -436,  -436,  -436,    69,  -436,  -436,  -436,     0,     0,
    -436,  -436,  -436,     0,   260,     0,   141,     0,    75,   532,
     549,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   137,   138,   261,   139,     0,    30,   262,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   140,     0,     0,     0,   264,     0,   265,   266,
     267,     0,   268,   269,   270,     0,   271,   272,   273,   274,
      62,   276,   277,   278,   279,     0,   280,     0,   282,     0,
       0,   283,   284,   285,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   141,     0,    75,   532,
     533,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   137,   138,     0,   139,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   141,     0,    75,     0,
     549,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   137,   138,     0,   139,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   195,     0,
       0,     0,    73,     0,     0,     0,   141,     0,    75,     0,
     142,   563,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   137,   138,     0,   139,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,     0,    40,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,    52,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    69,     0,    70,    71,   195,
       0,     0,     0,    73,     0,     0,     0,   141,     0,    75,
       0,   142,   870,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   137,   138,     0,   139,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,     0,    40,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,    52,   140,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,     0,     0,     0,    73,     0,     0,     0,   141,     0,
      75,     0,   142,   568,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,   470,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,   471,    30,
       0,   472,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,   473,   474,    40,     0,    42,
       0,     0,     0,     0,     0,     0,   475,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,   476,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,   199,
     200,     0,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,   236,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   195,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,   199,
     200,     0,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   195,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,   557,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,   199,
     200,     0,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   195,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,   559,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,   199,
     200,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,   201,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,   199,
     200,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   195,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,   665,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,   199,
     200,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,   165,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,   170,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,   171,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,   199,
     200,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   195,     0,     0,     0,    73,     0,     0,     0,   141,
     555,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   182,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   195,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   826,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   827,    51,    52,   828,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   195,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   829,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   195,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   829,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   815,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,   195,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,    70,
      71,     0,     0,     0,     0,   514,     0,     0,     0,   141,
       0,    75,     0,   142,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   137,   138,     0,   609,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,   137,   138,     0,   612,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,   137,   138,     0,   809,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,   137,   138,     0,   810,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,   137,   138,     0,   812,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,   137,   138,     0,   813,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,   137,   138,     0,   814,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,   137,   138,     0,   815,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,     0,    40,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,   137,   138,     0,   139,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     5,     0,     0,    40,     0,    42,
       0,   690,     0,     0,     0,     0,   691,     0,     0,     0,
       0,     0,    50,    51,    52,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     0,     0,    63,    64,
      65,    66,   690,     0,     0,     0,     0,   691,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    69,     0,     0,
      71,     0,     0,     0,     0,    73,     0,     0,     0,   141,
       0,    75,     0,   142,   692,   693,   694,   695,   696,   697,
       0,     0,   698,   699,   700,   701,   702,   703,   704,   705,
     706,   707,     0,     0,   708,     0,     0,     0,   709,   710,
     711,   712,   713,   714,   715,   716,   717,   718,   719,     0,
     720,     0,     0,   721,   722,   723,   724,   258,   725,     0,
       0,   726,   259,     0,     0,   692,   693,   694,   695,   696,
     697,     0,   260,   698,   699,   700,   701,   702,   703,   704,
     705,   706,   707,     0,     0,   708,     0,     0,     0,   709,
     710,   711,   712,   713,   714,   715,   716,   717,   718,   719,
       0,   720,     0,     0,   721,   722,   723,   724,     0,   725,
       0,   261,     0,     0,     0,   262,     0,     0,     0,   258,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,   265,   266,   267,     0,
     268,   269,   270,     0,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   261,   280,   281,   282,   262,     0,   283,
     284,   285,     0,     0,     0,     0,     0,     0,     0,     0,
     743,   258,     0,     0,     0,     0,   259,     0,     0,     0,
       0,   263,     0,     0,     0,     0,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,   265,   266,
     267,     0,   268,   269,   270,     0,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   258,   280,   281,   282,     0,
     259,   283,   284,   285,     0,   261,     0,     0,     0,   262,
     260,     0,   879,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,   261,
     265,   266,   267,   262,   268,   269,   270,     0,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   258,   280,   281,
     282,     0,   259,   283,   284,   285,     0,   263,     0,     0,
       0,     0,   260,     0,   916,     0,     0,     0,     0,     0,
       0,     0,   264,     0,   265,   266,   267,     0,   268,   269,
     270,     0,   271,   272,   273,   274,   275,   276,   277,   278,
     279,     0,   280,   281,   282,   258,     0,   283,   284,   285,
     259,   261,     0,     0,     0,   262,   556,     0,     0,     0,
     260,   336,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   337,     0,     0,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,   265,   266,   267,   261,
     268,   269,   270,   262,   271,   272,   273,   274,   275,   276,
     277,   278,   279,     0,   280,   281,   282,     0,     0,   283,
     284,   285,     0,   338,   258,     0,     0,   263,   852,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
     333,     0,   264,     0,   265,   266,   267,     0,   268,   269,
     270,   334,   271,   272,   273,   274,   275,   276,   277,   278,
     279,     0,   280,   281,   282,     0,     0,   283,   284,   285,
       0,     0,   258,   309,     0,     0,     0,   259,   261,     0,
       0,     0,   262,     0,     0,     0,     0,   260,   638,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,   265,   266,   267,   261,   268,   269,   270,
     262,   271,   272,   273,   274,   275,   276,   277,   278,   279,
       0,   280,   281,   282,   258,     0,   283,   284,   285,   259,
     338,     0,   309,     0,   263,     0,     0,     0,     0,   260,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,   265,   266,   267,     0,   268,   269,   270,     0,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   258,   280,
     281,   282,     0,   259,   283,   284,   285,     0,   261,     0,
     309,     0,   262,   260,   182,     0,     0,     0,     0,   344,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   258,     0,     0,     0,   263,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,   632,     0,
       0,   264,   261,   265,   266,   267,   262,   268,   269,   270,
       0,   271,   272,   273,   274,   275,   276,   277,   278,   279,
       0,   280,   281,   282,     0,     0,   283,   284,   285,     0,
     263,     0,   309,     0,     0,     0,   261,     0,     0,     0,
     262,     0,     0,     0,     0,   264,     0,   265,   266,   267,
       0,   268,   269,   270,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   263,   280,   281,   282,     0,   258,
     283,   284,   285,     0,   259,     0,   309,     0,     0,   264,
       0,   265,   266,   267,   260,   268,   269,   270,     0,   271,
     272,   273,   274,   275,   276,   277,   278,   279,     0,   280,
     281,   282,     0,     0,   283,   284,   285,     0,     0,     0,
     309,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   261,     0,     0,     0,   262,     0,     0,
       0,   258,     0,     0,   750,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,   265,   266,
     267,     0,   268,   269,   270,     0,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   261,   280,   281,   282,   262,
       0,   283,   284,   285,     0,     0,   755,   309,     0,     0,
       0,     0,   258,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,   263,     0,     0,     0,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
     265,   266,   267,     0,   268,   269,   270,     0,   271,   272,
     273,   274,   275,   276,   277,   278,   279,     0,   280,   281,
     282,     0,     0,   283,   284,   285,   261,     0,     0,   309,
     262,     0,     0,     0,   258,     0,     0,   864,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,   265,   266,   267,     0,   268,   269,   270,     0,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   261,   280,
     281,   282,   262,     0,   283,   284,   285,     0,     0,   865,
     309,     0,     0,     0,     0,   258,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,   263,     0,     0,     0,
     260,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,   265,   266,   267,     0,   268,   269,   270,
       0,   271,   272,   273,   274,   275,   276,   277,   278,   279,
       0,   280,   281,   282,     0,     0,   283,   284,   285,   261,
       0,     0,   309,   262,     0,     0,     0,   258,     0,     0,
     866,     0,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,   265,   266,   267,     0,   268,   269,
     270,     0,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   261,   280,   281,   282,   262,     0,   283,   284,   285,
       0,     0,   867,   309,     0,     0,     0,     0,   258,     0,
       0,     0,     0,   259,     0,     0,     0,     0,     0,   263,
       0,     0,     0,   260,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,   265,   266,   267,     0,
     268,   269,   270,     0,   271,   272,   273,   274,   275,   276,
     277,   278,   279,     0,   280,   281,   282,     0,     0,   283,
     284,   285,   261,     0,     0,   309,   262,     0,     0,     0,
     258,     0,     0,   881,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,     0,   265,   266,   267,
       0,   268,   269,   270,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   261,   280,   281,   282,   262,     0,
     283,   284,   285,     0,     0,   882,   309,     0,     0,     0,
       0,   258,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,   263,     0,     0,     0,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,   265,
     266,   267,     0,   268,   269,   270,     0,   271,   272,   273,
     274,   275,   276,   277,   278,   279,     0,   280,   281,   282,
       0,     0,   283,   284,   285,   261,     0,     0,   309,   262,
       0,     0,     0,   258,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
     265,   266,   267,     0,   268,   269,   270,     0,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   261,   280,   281,
     282,   262,     0,   283,   284,   285,     0,     0,     0,   372,
       0,     0,     0,     0,     0,     0,     0,   258,     0,     0,
       0,     0,   259,     0,     0,   263,     0,     0,     0,     0,
       0,     0,   260,     0,     0,     0,     0,   341,     0,     0,
     264,     0,   265,   266,   267,     0,   268,   269,   270,     0,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   342,
     280,   281,   282,     0,     0,   283,   284,   285,     0,     0,
    -393,   261,     0,     0,     0,   262,     0,     0,     0,   258,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,   264,     0,   265,   266,   267,     0,
     268,   269,   270,     0,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   261,   280,   281,   282,   262,   343,   283,
     284,   285,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   258,     0,     0,     0,   130,   259,     0,
       0,   263,     0,     0,     0,     0,     0,     0,   260,     0,
       0,     0,     0,   385,     0,     0,   264,     0,   265,   266,
     267,     0,   268,   269,   270,     0,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   386,   280,   281,   282,     0,
       0,   283,   284,   285,     0,     0,     0,   261,     0,     0,
       0,   262,     0,     0,     0,   258,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,   482,     0,     0,     0,   263,     0,     0,     0,     0,
       0,     0,   483,     0,     0,     0,     0,     0,     0,     0,
     264,     0,   265,   266,   267,     0,   268,   269,   270,     0,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   261,
     280,   281,   282,   262,     0,   283,   284,   285,     0,     0,
       0,     0,     0,     0,     0,     0,   258,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,   263,     0,     0,
       0,   260,   484,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,   485,   265,   266,   267,     0,   268,   269,
     270,     0,   271,   272,   273,   274,   275,   276,   277,   278,
     279,     0,   280,   281,   282,     0,     0,   283,   284,   285,
     261,     0,     0,     0,   262,     0,     0,     0,   258,     0,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,     0,     0,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,   265,   266,   267,     0,   268,
     269,   270,     0,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   261,   280,   281,   282,   262,     0,   283,   284,
     285,     0,     0,     0,     0,     0,     0,     0,     0,   258,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
     263,     0,     0,     0,   260,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,     0,   265,   266,   267,
       0,   268,   269,   270,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,     0,   280,   281,   282,     0,   319,
     283,   284,   285,   261,     0,     0,     0,   262,     0,     0,
       0,   258,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,   265,   266,
     267,     0,   268,   269,   270,     0,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   261,   280,   281,   282,   262,
     376,   283,   284,   285,     0,     0,     0,     0,     0,     0,
       0,     0,   258,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,   263,     0,     0,     0,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
     265,   266,   267,     0,   268,   269,   270,     0,   271,   272,
     273,   274,   275,   276,   277,   278,   279,     0,   280,   281,
     282,     0,   378,   283,   284,   285,   261,     0,     0,     0,
     262,     0,     0,     0,   258,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,   265,   266,   267,     0,   268,   269,   270,     0,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   261,   280,
     281,   282,   262,   380,   283,   284,   285,     0,     0,     0,
       0,     0,     0,     0,     0,   258,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,   263,     0,     0,     0,
     260,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,   265,   266,   267,     0,   268,   269,   270,
       0,   271,   272,   273,   274,   275,   276,   277,   278,   279,
       0,   280,   281,   282,     0,   388,   283,   284,   285,   261,
       0,     0,     0,   262,     0,     0,     0,   258,     0,     0,
       0,     0,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,     0,     0,   402,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,   265,   266,   267,     0,   268,   269,
     270,     0,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   261,   280,   281,   282,   262,     0,   283,   284,   285,
       0,     0,   486,     0,     0,     0,     0,     0,   258,     0,
       0,     0,     0,   259,     0,     0,     0,     0,     0,   263,
       0,     0,     0,   260,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,   265,   266,   267,     0,
     268,   269,   270,     0,   271,   272,   273,   274,   275,   276,
     277,   278,   279,     0,   280,   281,   282,     0,     0,   283,
     284,   285,   261,     0,     0,     0,   262,     0,     0,     0,
     258,     0,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,   130,     0,     0,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,     0,   265,   266,   267,
       0,   268,   269,   270,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   261,   280,   281,   282,   262,     0,
     283,   284,   285,     0,     0,     0,     0,     0,     0,     0,
       0,   258,   645,     0,     0,     0,   259,     0,     0,     0,
       0,     0,   263,     0,     0,     0,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,   265,
     266,   267,     0,   268,   269,   270,     0,   271,   272,   273,
     274,   275,   276,   277,   278,   279,     0,   280,   281,   282,
       0,   619,   283,   284,   285,   261,     0,     0,     0,   262,
       0,     0,     0,   258,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
       0,   648,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
     265,   266,   267,     0,   268,   269,   270,     0,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   261,   280,   281,
     282,   262,     0,   283,   284,   285,     0,     0,     0,     0,
       0,     0,     0,     0,   258,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,   263,     0,     0,     0,   260,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,   265,   266,   267,     0,   268,   269,   270,     0,
     271,   272,   273,   274,   275,   276,   277,   278,   279,     0,
     280,   281,   282,     0,     0,   283,   284,   285,   261,     0,
       0,     0,   262,     0,     0,     0,   258,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   260,   738,     0,     0,   685,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,   265,   266,   267,     0,   268,   269,   270,
       0,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     261,   280,   281,   282,   262,     0,   283,   284,   285,     0,
       0,     0,     0,     0,     0,     0,     0,   258,     0,     0,
       0,     0,   259,     0,     0,     0,     0,     0,   263,     0,
       0,     0,   260,   741,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,   265,   266,   267,     0,   268,
     269,   270,     0,   271,   272,   273,   274,   275,   276,   277,
     278,   279,     0,   280,   281,   282,     0,     0,   283,   284,
     285,   261,     0,     0,     0,   262,     0,     0,     0,   258,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,   265,   266,   267,     0,
     268,   269,   270,     0,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   261,   280,   281,   282,   262,     0,   283,
     284,   285,     0,     0,   808,     0,     0,     0,     0,     0,
     258,     0,     0,     0,     0,   259,     0,     0,     0,     0,
       0,   263,     0,     0,     0,   260,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,   265,   266,
     267,     0,   268,   269,   270,     0,   271,   272,   273,   274,
     275,   276,   277,   278,   279,     0,   280,   281,   282,     0,
       0,   283,   284,   285,   261,     0,     0,     0,   262,     0,
       0,     0,   258,   824,     0,   811,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,     0,     0,
       0,     0,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,   265,
     266,   267,     0,   268,   269,   270,     0,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   261,   280,   281,   282,
     262,     0,   283,   284,   285,     0,     0,     0,     0,     0,
       0,     0,     0,   258,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,   263,     0,     0,     0,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,   265,   266,   267,     0,   268,   269,   270,     0,   271,
     272,   273,   274,   275,   276,   277,   278,   279,     0,   280,
     281,   282,     0,     0,   283,   284,   285,   261,     0,     0,
       0,   262,     0,     0,     0,   258,     0,     0,   888,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,     0,     0,     0,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,   265,   266,   267,     0,   268,   269,   270,     0,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   261,
     280,   281,   282,   262,     0,   283,   284,   285,     0,     0,
     889,     0,     0,     0,     0,     0,   258,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,   263,     0,     0,
       0,   260,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,   265,   266,   267,     0,   268,   269,
     270,     0,   271,   272,   273,   274,   275,   276,   277,   278,
     279,     0,   280,   281,   282,     0,     0,   283,   284,   285,
     261,     0,     0,     0,   262,     0,     0,     0,   258,     0,
       0,   890,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,     0,     0,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,   265,   266,   267,     0,   268,
     269,   270,     0,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   261,   280,   281,   282,   262,     0,   283,   284,
     285,     0,     0,   891,     0,     0,     0,     0,     0,   258,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
     263,     0,     0,     0,   260,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,     0,   265,   266,   267,
       0,   268,   269,   270,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,     0,   280,   281,   282,     0,     0,
     283,   284,   285,   261,     0,     0,     0,   262,     0,     0,
       0,   258,     0,     0,   892,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,   265,   266,
     267,     0,   268,   269,   270,     0,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   261,   280,   281,   282,   262,
       0,   283,   284,   285,     0,     0,   893,     0,     0,     0,
       0,     0,   258,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,   263,     0,     0,     0,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
     265,   266,   267,     0,   268,   269,   270,     0,   271,   272,
     273,   274,   275,   276,   277,   278,   279,     0,   280,   281,
     282,     0,     0,   283,   284,   285,   261,     0,     0,     0,
     262,     0,     0,     0,   258,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,   265,   266,   267,     0,   268,   269,   270,     0,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   607,   280,
     281,   282,   262,     0,   283,   284,   285,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,   265,   266,   267,     0,   268,   269,   270,
       0,   271,   272,   273,   274,   275,   276,   277,   278,   279,
       0,   280,   281,   282,     0,     0,   283,   284,   285
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-789))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,     2,    43,    44,   348,   662,    26,   381,    24,    11,
     306,   152,    26,     3,    16,    15,   531,    19,    20,    21,
     351,   730,   163,   241,    26,    27,    28,    29,     3,   175,
       3,    37,     3,    35,    36,    37,     3,    39,    40,   512,
       3,    26,    26,   687,    46,   191,    48,    49,    50,   687,
      26,    53,    54,    55,    56,    67,    27,    59,    60,    61,
     112,    53,    64,   209,    46,    67,    68,    69,   342,    71,
     837,    21,    74,    88,    76,   131,    78,    59,   252,   253,
     254,   255,   870,     1,   140,     1,   142,   112,    37,   141,
      65,    81,    37,    85,    76,     1,    13,     1,   112,    93,
       0,    18,   356,    88,    37,    55,   112,     3,    37,    80,
      67,    37,   386,    37,   771,    65,    88,   905,   143,   131,
      65,     3,   116,   112,   138,    21,   138,   381,   143,   131,
      80,    37,   605,    37,   136,   137,   138,   139,   140,   141,
     142,    37,   909,   658,    40,   854,   855,   139,   148,    67,
     140,    67,   141,   138,   138,   799,   302,   303,    54,    55,
     142,    77,   138,   112,    88,   140,    80,   140,    86,    65,
      86,   138,   816,   140,   176,    17,   192,   140,   816,   112,
     182,   138,   112,   112,    80,    91,   112,    93,   112,    93,
      41,   905,     3,   111,   143,   111,     7,   199,   200,   530,
     112,   140,   916,   142,    21,   111,   112,   111,   112,    60,
     143,   141,   869,   140,   143,   131,   512,   143,   112,   143,
     138,    63,   138,   112,   112,   131,   112,   131,   374,   141,
      41,   233,   233,     3,   236,   112,   410,     7,    55,   140,
      82,   261,   262,   111,   140,   131,   114,   141,    65,    60,
     118,     7,   141,   141,   131,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,    84,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   304,   305,   801,    25,   112,   762,
     140,   112,   510,   511,     3,   315,   112,    93,     7,   140,
       9,   142,   304,   305,   306,   112,   650,   309,   112,   605,
     131,    73,    21,   315,   334,   139,   112,   337,   320,     2,
     116,   652,   112,   112,   131,   141,    64,   140,    11,   331,
      68,   333,   334,    21,   336,   337,    19,   141,    21,   341,
     342,   131,   344,   561,   562,   141,    55,    20,    64,   331,
     139,    25,    68,   112,   356,    61,    65,   359,    31,    32,
      64,   137,    93,   112,    68,   367,    39,    55,    41,   140,
      53,    80,   131,   111,    47,    84,   114,    65,   398,   381,
     118,   112,   131,   385,   386,   116,   131,    60,    61,   391,
      64,    74,    80,    87,    68,    78,   398,   399,   400,   112,
     402,   403,   776,     2,   140,   407,   142,   661,   131,   783,
     141,   552,    11,   394,   131,   396,    55,    56,   131,    25,
      19,   131,    21,   131,   131,   407,   131,   138,   131,   131,
     114,    17,    93,     7,    20,   131,    49,   111,   138,   113,
     114,   140,   140,    29,   118,    31,    32,   140,   131,    35,
     131,   125,    47,    39,    53,    41,    55,    56,    64,   111,
     134,   805,    68,   483,   131,   485,   762,    93,   112,   141,
      37,    57,   131,    59,    60,    74,    62,    63,   140,    78,
     482,   483,   484,   485,   486,   487,   488,    28,   141,   489,
     490,   112,   141,   176,    80,    91,    82,    16,   141,   182,
     502,   107,   504,   109,   110,   111,   508,   113,   114,   115,
     512,    93,   118,   141,   112,   138,   770,     3,   124,   125,
     522,     7,   776,   129,   143,    20,   132,   133,   134,   783,
     131,   533,   131,   141,   536,    21,    31,    32,   131,   131,
     131,   131,   131,   131,    39,   131,    41,   549,   131,    20,
     233,   533,    47,   139,   167,   168,   169,    93,   131,   131,
      31,    32,   131,   131,   566,    60,    61,   549,    39,    55,
      41,   131,   185,   131,   131,   188,   131,   176,   143,    65,
     131,    25,   112,   182,   131,    21,   131,   607,   608,    60,
      28,   141,   141,   206,    80,   138,   131,   131,    84,   112,
      65,    37,    26,   605,    40,   607,   608,   609,    26,   138,
     612,   224,   143,    25,   111,    65,   870,   111,    54,    55,
      64,   623,   143,   143,    68,   879,   309,   114,   630,    65,
     632,   114,   140,   635,   233,   141,   638,    86,   141,   686,
      77,   643,   386,   645,    80,   647,   648,   217,   385,   672,
     333,   905,    64,   336,   164,   501,    68,   748,   624,   661,
     804,   344,   916,   107,   804,   667,   668,   111,   805,   113,
     114,   650,   689,   909,   118,   869,   884,    28,   680,    -1,
     682,   125,   360,   685,   686,    -1,   668,   825,   132,   133,
     134,   496,    -1,    -1,    -1,     2,    -1,    -1,    -1,   111,
      -1,   113,   114,   316,    11,    -1,   118,    -1,    -1,    -1,
     309,    -1,    19,   125,    21,    -1,    -1,   400,    -1,    -1,
     132,   133,   134,    -1,   726,    -1,    -1,   340,    -1,    -1,
      -1,    -1,    -1,    -1,   333,   737,   738,   336,   740,   741,
     742,   743,    -1,    -1,    -1,   344,    53,    -1,   750,   751,
      -1,   753,    -1,   755,   756,   796,   797,   759,    75,    -1,
     762,    -1,    -1,    -1,    -1,    -1,    -1,    74,   770,    -1,
      -1,    78,    -1,   775,   776,   777,    -1,    -1,    -1,   781,
      -1,   783,   784,    -1,    -1,   787,   788,    -1,    -1,   791,
      -1,    -1,   794,   794,   477,    -1,    -1,    -1,    -1,    -1,
      -1,   400,    -1,    -1,    -1,    -1,   808,   809,   810,   811,
     812,   813,   814,   815,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   824,    -1,   131,   827,   828,   829,    -1,    -1,
      -1,   833,    -1,     2,   836,    -1,    -1,    -1,    -1,   522,
     157,    -1,    11,    -1,    -1,    -1,    -1,   829,    -1,    -1,
      19,    -1,    21,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,   864,   865,   866,   867,    -1,    -1,   870,   176,
      -1,    -1,    25,    -1,   191,   182,    -1,   879,    -1,   881,
     882,    -1,    -1,   566,    53,    -1,   888,   889,   890,   891,
     892,   893,   209,   506,   507,   578,    -1,   510,   511,    -1,
      -1,    -1,   904,   905,    -1,    74,    -1,    -1,    -1,    78,
      -1,    64,    -1,    -1,   916,    68,    -1,    -1,     2,    -1,
      -1,    -1,    -1,   522,   607,   608,   233,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   630,    -1,   632,
      -1,    -1,   635,    -1,   107,   638,   109,   110,   111,    -1,
     113,   114,   131,    -1,   647,   118,    -1,   566,    -1,    53,
      -1,   124,   125,    -1,    -1,    -1,   129,    -1,    -1,   132,
     133,   134,    -1,    -1,   667,   302,   303,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    78,    37,    -1,   680,    -1,   682,
      -1,    -1,   309,   686,   617,   618,    -1,   176,    -1,    -1,
      -1,    -1,    -1,   182,   627,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    -1,   637,    67,   333,   640,    -1,   336,
      -1,   630,    -1,   632,    -1,    77,   635,   344,    -1,   638,
      -1,    -1,    -1,    -1,    86,    -1,    -1,   131,   647,    -1,
     367,    93,    67,    -1,    -1,    -1,     2,   374,    -1,    -1,
      47,    -1,    77,    -1,   233,    11,    -1,   750,   667,    37,
     112,    86,   755,    19,   391,    21,    -1,   394,    93,   396,
      -1,   680,   399,   682,    -1,    -1,    -1,   686,    75,   131,
      -1,    -1,   176,   400,   777,    -1,   138,   112,   182,    67,
      -1,    -1,    -1,    -1,   787,    -1,    -1,    53,   791,    77,
      -1,   794,    -1,    -1,    -1,    -1,   131,    -1,    86,    25,
      -1,    -1,    -1,   138,    -1,    93,    -1,    -1,    74,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     309,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,   233,
      -1,   750,    -1,    -1,    -1,    -1,   755,    -1,    64,    -1,
      -1,    -1,    68,   131,   333,    -1,   779,   336,    -1,    -1,
     138,    -1,    -1,    -1,    -1,   344,    -1,   494,   777,   496,
      -1,   864,   865,   866,   867,   131,    -1,    -1,   787,    -1,
      -1,    -1,   791,    -1,    -1,   794,    -1,     2,   881,   882,
      -1,   107,    -1,   109,   110,   111,    11,   113,   114,    -1,
      -1,    -1,   118,    -1,    19,   522,    21,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,   309,   132,   133,   134,    -1,
     176,   400,    17,    -1,    -1,    20,   182,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    31,    32,    53,   333,
      35,    -1,   336,    -1,    39,    -1,    41,    -1,    -1,   566,
     344,    -1,    -1,    -1,    -1,   864,   865,   866,   867,    74,
      -1,    -1,    57,    78,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,   881,   882,    -1,    -1,    -1,   233,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    82,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,    -1,   400,    -1,    -1,    -1,
      -1,    -1,    -1,   630,    -1,   632,   131,    -1,   635,    -1,
      -1,   638,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     647,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,
      -1,    -1,    -1,   522,   139,    -1,    -1,    -1,    -1,    -1,
     667,    -1,    -1,   309,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,   680,    -1,   682,    -1,   182,    -1,   686,
      -1,    17,    -1,    -1,    20,    -1,    -1,   333,    -1,    -1,
     336,    -1,    -1,    29,    -1,    31,    32,   566,   344,    35,
      -1,    -1,    -1,    39,    -1,    41,    -1,   394,    -1,   396,
      -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    11,    59,    60,    -1,    62,    63,   233,    -1,
      19,    -1,    21,    -1,    -1,    -1,    -1,    -1,   522,    -1,
      -1,    -1,    -1,   750,    80,    -1,    82,    -1,   755,    -1,
      -1,    -1,    -1,   770,   400,    -1,    -1,    -1,    -1,    -1,
      -1,   630,    -1,   632,    53,    -1,   635,    -1,    -1,   638,
     777,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   647,    -1,
     787,    -1,   566,    -1,   791,    74,    -1,   794,    -1,    78,
      -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,   667,    -1,
      -1,    -1,    -1,    -1,   309,    -1,    -1,   494,    -1,    -1,
      -1,   680,    -1,   682,    -1,    -1,   833,   686,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   333,    -1,
      -1,   336,    -1,    -1,    -1,    35,    -1,    -1,    -1,   344,
      -1,    -1,   131,    -1,    -1,    -1,   630,    -1,   632,    49,
     537,   635,    -1,   870,   638,    55,    56,   864,   865,   866,
     867,     2,    -1,   647,    -1,    -1,    -1,    -1,    -1,    -1,
      11,    -1,    25,    -1,   881,   882,   522,    -1,    19,    -1,
      21,   750,    -1,   667,    -1,    -1,   755,   176,   905,    -1,
      -1,    -1,    -1,   182,    -1,   400,   680,    -1,   682,   916,
      -1,    -1,   686,    -1,    -1,    -1,    -1,    -1,   777,    -1,
      -1,    64,    53,    -1,    -1,    68,    -1,    -1,   787,    -1,
     566,    -1,   791,    -1,    -1,   794,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,   135,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,   233,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,    -1,   110,   111,    -1,
     113,   114,    -1,    -1,    -1,   118,   750,   167,   168,   169,
      -1,   755,   125,    -1,   174,    -1,    -1,    -1,    -1,   132,
     133,   134,    -1,    -1,   630,   185,   632,    -1,   188,   635,
     131,    -1,   638,   777,    -1,   864,   865,   866,   867,    -1,
      -1,   647,    -1,   787,    -1,    -1,   206,   791,    -1,    -1,
     794,    -1,   881,   882,    -1,    -1,    -1,   522,    -1,    -1,
     309,   667,    -1,    -1,   224,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   680,   176,   682,    -1,    -1,    -1,
     686,   182,    -1,    -1,   333,    -1,    -1,   336,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   344,    -1,    -1,    -1,    -1,
      -1,   566,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,    -1,    -1,    -1,
     864,   865,   866,   867,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   233,    -1,    -1,    -1,    -1,   881,   882,    -1,
      -1,    -1,    -1,    -1,   750,   135,   136,   137,    -1,   755,
     140,   400,   142,    -1,    -1,    -1,   316,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   630,    -1,   632,    -1,    -1,
     635,   777,    -1,   638,    -1,    -1,    -1,    -1,    -1,    -1,
     340,   787,   647,    -1,    -1,   791,    -1,    -1,   794,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   667,    -1,    -1,    12,    -1,    -1,   309,    -1,
      -1,    -1,    -1,    -1,    -1,   680,    -1,   682,    -1,    -1,
      27,   686,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,   333,    -1,    -1,   336,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,   864,   865,
     866,   867,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,   522,    -1,   881,   882,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   750,    93,    -1,    -1,    -1,
     755,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   400,
      -1,   108,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,   777,    -1,    -1,    -1,    -1,   566,    -1,   126,
      -1,    -1,   787,   130,    -1,    -1,   791,    -1,    -1,   794,
      -1,   138,    -1,   140,   141,   142,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,   506,   507,    12,    -1,
     510,   511,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,
      44,   630,    46,   632,    48,    49,   635,   547,    -1,   638,
      -1,    -1,    56,    -1,    58,    -1,    -1,    -1,   647,   864,
     865,   866,   867,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,   881,   882,   667,    -1,
      -1,   522,    -1,    -1,    13,    -1,    -1,    -1,    -1,    18,
      -1,   680,    -1,   682,    23,    24,    25,   686,    -1,    -1,
      -1,    -1,    -1,    32,   108,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   617,   618,    -1,
      -1,    -1,   126,    -1,    -1,   566,    -1,   627,    57,   629,
      -1,    -1,    -1,    -1,   634,    -1,   140,   637,   142,    -1,
     640,    -1,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,    -1,    -1,    -1,
      -1,   750,    -1,    -1,    -1,    -1,   755,    -1,    -1,    -1,
      99,    -1,    -1,    -1,    -1,   675,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,   136,    -1,   777,   630,
     140,   632,   142,    -1,   635,    -1,    -1,   638,   787,    -1,
      -1,    25,   791,    -1,    -1,   794,   647,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   667,    -1,   157,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,   680,
      64,   682,    -1,    -1,    68,   686,   175,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   864,   865,   866,   867,   779,
     209,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   881,   882,   118,   119,   120,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,   129,    -1,    -1,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   818,   819,
      -1,   821,   822,   252,   253,   254,   255,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,   777,   837,    -1,    -1,
     269,    -1,    -1,   843,    25,   845,   787,   847,    -1,    -1,
     791,    -1,    -1,   794,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   302,   303,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   327,    -1,
     329,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,   909,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   351,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,   360,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   374,   127,   128,   129,    -1,
      -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   406,    -1,     0,
       1,   410,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,   496,    89,    -1,
      -1,    92,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,   526,    -1,    -1,
      -1,   530,    -1,   124,    -1,   126,    -1,   128,   129,    -1,
     131,    64,    -1,   134,    -1,    68,    -1,   138,   547,   140,
      -1,   142,    -1,   552,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   578,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,    -1,    -1,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
       4,     5,     6,     7,     8,   624,    10,    -1,    12,    13,
      14,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,   652,    38,    39,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    56,   672,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    -1,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    -1,    89,   705,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,   108,   109,   110,   111,   112,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,    -1,   131,   132,   133,
     134,    -1,    -1,    -1,   138,   139,   140,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,   795,    29,    -1,    31,
      32,    33,    34,    35,    36,   804,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,   139,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,   139,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,   139,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,   139,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,   139,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    68,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,    -1,    -1,
     132,   133,   134,    -1,    25,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    64,    36,    -1,    38,    68,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    92,    64,    -1,    -1,    -1,    68,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
      92,   122,   123,   124,   125,    -1,   127,    -1,   129,    -1,
      -1,   132,   133,   134,    -1,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,    -1,    -1,
     132,   133,   134,    -1,    25,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    64,    36,    -1,    38,    68,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
      92,   122,   123,   124,   125,    -1,   127,    -1,   129,    -1,
      -1,   132,   133,   134,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,     1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,   143,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,
      -1,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,
      -1,   142,   143,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,    -1,   142,   143,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    37,    38,
      -1,    40,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    54,    55,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,   141,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,   141,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    55,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    32,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
     139,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
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
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,     3,    -1,    -1,    56,    -1,    58,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,   107,   108,
     109,   110,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,    -1,
     129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,
      -1,   140,    -1,   142,    93,    94,    95,    96,    97,    98,
      -1,    -1,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    -1,
     129,    -1,    -1,   132,   133,   134,   135,    10,   137,    -1,
      -1,   140,    15,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,    25,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,   129,    -1,    -1,   132,   133,   134,   135,    -1,   137,
      -1,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    64,   127,   128,   129,    68,    -1,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    10,   127,   128,   129,    -1,
      15,   132,   133,   134,    -1,    64,    -1,    -1,    -1,    68,
      25,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    64,
     109,   110,   111,    68,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    10,   127,   128,
     129,    -1,    15,   132,   133,   134,    -1,    92,    -1,    -1,
      -1,    -1,    25,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    10,    -1,   132,   133,   134,
      15,    64,    -1,    -1,    -1,    68,   141,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    64,
     113,   114,   115,    68,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,    -1,    -1,   132,
     133,   134,    -1,    88,    10,    -1,    -1,    92,   141,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    37,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      -1,    -1,    10,   138,    -1,    -1,    -1,    15,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    64,   113,   114,   115,
      68,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    10,    -1,   132,   133,   134,    15,
      88,    -1,   138,    -1,    92,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    10,   127,
     128,   129,    -1,    15,   132,   133,   134,    -1,    64,    -1,
     138,    -1,    68,    25,    26,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    92,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,   107,    64,   109,   110,   111,    68,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,    -1,   132,   133,   134,    -1,
      92,    -1,   138,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    92,   127,   128,   129,    -1,    10,
     132,   133,   134,    -1,    15,    -1,   138,    -1,    -1,   107,
      -1,   109,   110,   111,    25,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,    -1,    -1,   132,   133,   134,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,    -1,
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
      10,    -1,    -1,    75,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    64,   127,   128,   129,    68,    -1,
     132,   133,   134,    -1,    -1,    75,   138,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
      -1,    -1,   132,   133,   134,    64,    -1,    -1,   138,    68,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    64,   127,   128,
     129,    68,    -1,   132,   133,   134,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    52,
     127,   128,   129,    -1,    -1,   132,   133,   134,    -1,    -1,
     137,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    64,   127,   128,   129,    68,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    88,    15,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    30,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    52,   127,   128,   129,    -1,
      -1,   132,   133,   134,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    64,
     127,   128,   129,    68,    -1,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    37,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,   134,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    64,   127,
     128,   129,    68,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,   131,   132,   133,   134,    64,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    91,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    64,   127,   128,   129,    68,    -1,   132,   133,   134,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,    -1,    -1,   132,
     133,   134,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    88,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    64,   127,   128,   129,    68,    -1,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,   134,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    28,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      64,   127,   128,   129,    68,    -1,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    10,    11,    -1,    75,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
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
     134,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,    -1,
     132,   133,   134,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    10,    -1,    -1,    75,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    64,   127,   128,   129,    68,
      -1,   132,   133,   134,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    64,   127,
     128,   129,    68,    -1,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,    -1,   132,   133,   134
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
     260,   261,    80,   164,   152,   256,   152,   140,   165,     7,
     164,   166,     9,    80,   166,    55,    90,   174,   256,   256,
      20,    32,   222,   256,   256,   140,   165,   193,   140,   165,
     224,   225,    26,   156,   167,   256,   256,   256,   256,   256,
       7,   140,    32,   170,   170,   130,   215,   235,   256,    78,
      79,   131,   256,   257,   256,   167,   256,   256,    73,   140,
     152,   256,   256,   156,   163,   256,   258,   156,   163,   256,
     165,   220,   235,   256,   256,   256,   256,   256,   256,   256,
     256,   139,   150,   157,   235,    81,   116,   215,   236,   237,
     257,   235,   256,   264,    57,   152,    61,   155,   165,    41,
      60,   210,    21,    55,    65,    84,   131,   137,    10,    15,
      25,    64,    68,    92,   107,   109,   110,   111,   113,   114,
     115,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     127,   128,   129,   132,   133,   134,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     135,   136,   140,   142,    64,    68,   140,   152,   131,   138,
     156,   256,   256,   256,   235,    37,   247,   220,   131,   131,
      87,   165,   215,   238,   239,   240,   257,   138,   220,   186,
     165,   140,   167,    26,    37,   167,    26,    37,    88,   167,
     250,    30,    52,   131,    75,   156,   238,   152,   140,   204,
      81,   140,   165,   226,   227,     1,   111,   229,   230,    37,
     112,   152,   167,   167,   238,   166,   165,   112,   131,   256,
     256,   131,   138,   167,   140,   238,   131,   178,   131,   178,
     131,    93,   221,   131,   131,    30,    52,   167,   131,   139,
     150,   112,   139,   256,   112,   141,   112,   141,    37,   112,
     143,   250,    91,   112,   143,     7,    47,   111,   187,   197,
      65,   224,   224,   224,   224,   256,   256,   256,   256,   174,
     256,   174,   256,   256,   256,   256,   256,   256,   256,    27,
      80,   165,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   238,   238,   174,   256,   174,   256,
      21,    37,    40,    54,    55,    65,    80,   208,   249,   252,
     256,   269,    26,    37,    26,    37,    75,    37,   143,   174,
     256,   167,   131,   256,    93,   141,   112,   165,   190,   191,
     131,   165,    37,   235,    36,   256,   174,   256,    36,   256,
     174,   256,   140,   167,   134,   158,   160,   256,   158,   159,
     152,   256,    28,   141,   205,   206,   207,   208,   194,   227,
     112,   141,     1,   142,   231,   242,    91,    93,   228,   256,
     225,   141,   215,   256,   183,   238,   141,    16,   179,   142,
     231,   242,   112,   160,   159,   139,   141,   141,   236,   141,
     236,   174,   256,   143,   152,   256,   143,   256,   143,   256,
     165,   235,   138,    21,    55,    65,    80,   199,   209,   224,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     141,   143,    37,    65,   241,   112,   141,    64,    64,    36,
     174,   256,    36,   174,   256,   174,   256,   247,   247,   131,
     215,   257,   240,    93,   112,   139,   187,   256,   141,   256,
      26,   167,    26,   167,   256,    26,   167,   250,    26,   167,
     250,   251,   252,   112,   131,    11,   131,    28,    28,   152,
     112,   141,   140,   165,    21,    55,    65,    80,   211,   141,
     227,   111,   230,   235,   256,    50,   257,    53,    85,   139,
     184,   141,   140,   156,   165,   180,   235,   220,   131,   131,
     143,   250,   143,   250,   152,    91,   138,   188,    65,   198,
      10,    15,    93,    94,    95,    96,    97,    98,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   113,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     129,   132,   133,   134,   135,   137,   140,   165,   200,   201,
     202,   241,   131,   252,   241,   241,   256,    26,    26,   256,
      26,    26,   143,   143,   167,   167,   256,   191,   138,   167,
      75,    36,   256,    36,   256,    75,    36,   256,   167,    36,
     256,   167,   112,   141,   256,   256,   152,   256,   206,   227,
     111,   234,    65,   230,   228,    37,   143,    26,   152,   235,
     180,   111,   156,   143,    36,   152,   256,   143,    36,   152,
     256,   143,   256,   139,   157,    32,    59,    62,   139,   151,
     169,   211,   165,   256,   114,   140,   203,   203,    75,    36,
      36,    75,    36,    36,    36,    36,   188,   256,   256,   256,
     256,   256,   256,   252,    11,   141,    27,    71,    74,   142,
     215,   233,   242,   116,   217,   228,    86,   218,   256,   231,
     242,   152,   167,   141,   256,   256,   152,   256,   152,   139,
     169,   230,   141,   165,   201,   202,   205,   256,   256,   256,
     256,   256,   256,   139,    75,    75,    75,    75,   256,   234,
     143,   235,   215,   216,   256,   256,   156,   168,   214,   143,
     156,    75,    75,    77,   212,   203,   203,   141,    75,    75,
      75,    75,    75,    75,   256,   256,   256,   256,   217,   228,
     215,   232,   233,   242,    37,   143,   242,   256,   256,   218,
     256,   232,   233,   131,   213,   214,   143,   232
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
#line 440 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 445 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 446 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 453 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 458 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 459 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 478 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 479 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 483 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 489 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 491 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 493 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 498 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (6)].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[(1) - (6)].b), (yyvsp[(2) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType, (yyvsp[(6) - (7)].pblockstmt), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 525 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 526 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 547 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 551 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 558 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 567 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 589 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 591 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 593 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 595 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 597 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 599 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 601 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 603 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 605 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 607 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 609 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 611 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 613 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 615 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 617 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 621 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 626 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 645 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 648 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 661 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 662 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 663 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 664 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 669 "chapel.ypp"
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
#line 683 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 684 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 686 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 700 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 706 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 712 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 718 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 724 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 731 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 775 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 779 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 793 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 795 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 797 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 804 "chapel.ypp"
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
#line 814 "chapel.ypp"
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
#line 826 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 831 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 836 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 844 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 845 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 850 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 852 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 854 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 859 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 869 "chapel.ypp"
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
#line 878 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 882 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 895 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 900 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 905 "chapel.ypp"
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
#line 925 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 931 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 938 "chapel.ypp"
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
#line 946 "chapel.ypp"
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
#line 958 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 964 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 971 "chapel.ypp"
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
#line 993 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 997 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 1002 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 1006 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 1035 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 1036 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 1037 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 1039 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 1041 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1042 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1043 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1047 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1048 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1049 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1050 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1051 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1052 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1053 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1054 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1055 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1056 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1057 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1058 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1062 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1063 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1067 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1071 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1072 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1073 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1078 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1080 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1082 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1084 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1089 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1101 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1123 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1129 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1143 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1149 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1155 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1159 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1160 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1169 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1174 "chapel.ypp"
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
#line 1187 "chapel.ypp"
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
#line 1203 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1205 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1207 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1212 "chapel.ypp"
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
#line 1220 "chapel.ypp"
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
#line 1228 "chapel.ypp"
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
#line 1236 "chapel.ypp"
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
#line 1245 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1254 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1255 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1256 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1262 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1270 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1272 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      USR_WARN(var, "support for '=>' to alias arrays is deprecated as of chpl version 1.15.  Use a 'ref' declaration instead.");
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1318 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1347 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1354 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1358 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1386 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1394 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1395 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1396 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1400 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1402 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1406 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1411 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1416 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1417 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1421 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1423 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1445 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1447 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1449 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1451 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1453 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 354:

/* Line 1806 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1464 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1472 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1480 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1486 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1493 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1499 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1505 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1511 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1562 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1566 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1574 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1579 "chapel.ypp"
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[(1) - (2)].pt); (yyval.pIntentExpr).iVar = (yyvsp[(2) - (2)].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1582 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1615 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1617 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1619 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1621 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1625 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1653 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1654 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1659 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1660 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1661 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1670 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1671 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1672 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1676 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1680 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1682 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1686 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1690 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 1698 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 1703 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 1705 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 1709 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 452:

/* Line 1806 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 453:

/* Line 1806 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 454:

/* Line 1806 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 455:

/* Line 1806 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 456:

/* Line 1806 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:

/* Line 1806 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 458:

/* Line 1806 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 459:

/* Line 1806 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 460:

/* Line 1806 of yacc.c  */
#line 1720 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 461:

/* Line 1806 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 462:

/* Line 1806 of yacc.c  */
#line 1722 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 463:

/* Line 1806 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 464:

/* Line 1806 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 465:

/* Line 1806 of yacc.c  */
#line 1728 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 466:

/* Line 1806 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 467:

/* Line 1806 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 468:

/* Line 1806 of yacc.c  */
#line 1731 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 469:

/* Line 1806 of yacc.c  */
#line 1732 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 470:

/* Line 1806 of yacc.c  */
#line 1733 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 471:

/* Line 1806 of yacc.c  */
#line 1737 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 472:

/* Line 1806 of yacc.c  */
#line 1738 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 473:

/* Line 1806 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 474:

/* Line 1806 of yacc.c  */
#line 1740 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 475:

/* Line 1806 of yacc.c  */
#line 1744 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 476:

/* Line 1806 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 477:

/* Line 1806 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 478:

/* Line 1806 of yacc.c  */
#line 1747 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 479:

/* Line 1806 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 480:

/* Line 1806 of yacc.c  */
#line 1753 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 481:

/* Line 1806 of yacc.c  */
#line 1754 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 482:

/* Line 1806 of yacc.c  */
#line 1755 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 483:

/* Line 1806 of yacc.c  */
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 484:

/* Line 1806 of yacc.c  */
#line 1757 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 485:

/* Line 1806 of yacc.c  */
#line 1758 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 8526 "bison-chapel.cpp"
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



