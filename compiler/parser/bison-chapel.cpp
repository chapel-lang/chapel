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
#define YYLAST   13307

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  147
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  122
/* YYNRULES -- Number of rules.  */
#define YYNRULES  482
/* YYNRULES -- Number of states.  */
#define YYNSTATES  911

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
    1046,  1049,  1051,  1053,  1057,  1061,  1063,  1065,  1067,  1071,
    1075,  1076,  1078,  1080,  1084,  1088,  1092,  1094,  1096,  1098,
    1100,  1102,  1104,  1106,  1108,  1111,  1116,  1121,  1126,  1132,
    1135,  1138,  1145,  1152,  1157,  1167,  1177,  1185,  1192,  1199,
    1204,  1214,  1224,  1232,  1237,  1244,  1251,  1261,  1271,  1278,
    1280,  1282,  1284,  1286,  1288,  1290,  1292,  1294,  1298,  1299,
    1301,  1306,  1308,  1312,  1317,  1319,  1323,  1326,  1330,  1334,
    1336,  1340,  1343,  1348,  1350,  1352,  1354,  1356,  1358,  1360,
    1362,  1364,  1369,  1373,  1377,  1380,  1383,  1385,  1388,  1391,
    1393,  1395,  1397,  1399,  1401,  1403,  1405,  1410,  1415,  1420,
    1424,  1428,  1432,  1436,  1441,  1445,  1450,  1452,  1454,  1456,
    1458,  1460,  1464,  1469,  1473,  1478,  1482,  1487,  1491,  1497,
    1501,  1505,  1509,  1513,  1517,  1521,  1525,  1529,  1533,  1537,
    1541,  1545,  1549,  1553,  1557,  1561,  1565,  1569,  1573,  1577,
    1581,  1585,  1589,  1592,  1595,  1598,  1601,  1604,  1607,  1611,
    1615,  1619,  1623,  1627,  1631,  1635,  1639,  1641,  1643,  1645,
    1647,  1649,  1651
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     148,     0,    -1,   149,    -1,    -1,   149,   150,    -1,   152,
      -1,   151,   152,    -1,    57,     7,    -1,   151,    57,     7,
      -1,   153,    -1,   156,    -1,   161,    -1,   162,    -1,   169,
      -1,   163,    -1,   172,    -1,   175,    -1,   173,    -1,   182,
      -1,   176,    -1,   177,    -1,   181,    -1,   168,    -1,   245,
     131,    -1,    12,   152,    -1,    13,   246,   152,    -1,    14,
     164,   131,    -1,    18,   246,   156,    -1,    22,   164,   131,
      -1,    24,   255,   131,    -1,    42,   165,   152,    -1,    45,
     255,   167,    -1,    45,   167,    -1,    51,   255,   167,    -1,
      70,   255,   167,    -1,    70,   167,    -1,    74,   152,    -1,
      89,   255,   131,    -1,     1,    -1,   154,   155,    47,   165,
     138,   139,    -1,   154,   155,    47,   165,   138,   157,   139,
      -1,    -1,    62,    -1,    59,    -1,    -1,    61,    -1,   138,
     139,    -1,   138,   157,   139,    -1,   150,    -1,   157,   150,
      -1,   255,    -1,   255,    11,   255,    -1,   158,   112,   255,
      -1,   158,   112,   255,    11,   255,    -1,    -1,   158,    -1,
     134,    -1,   158,    -1,    83,   234,   131,    -1,    83,   255,
      30,   160,   131,    -1,    83,   255,    52,   159,   131,    -1,
      66,   234,   131,    -1,   257,    93,   256,   131,    -1,   257,
     104,   256,   131,    -1,   257,   101,   256,   131,    -1,   257,
     103,   256,   131,    -1,   257,    97,   256,   131,    -1,   257,
     102,   256,   131,    -1,   257,    98,   256,   131,    -1,   257,
      94,   256,   131,    -1,   257,    95,   256,   131,    -1,   257,
      96,   256,   131,    -1,   257,   106,   256,   131,    -1,   257,
     105,   256,   131,    -1,   257,   135,   256,   131,    -1,   257,
     136,   256,   131,    -1,   257,    99,   256,   131,    -1,   257,
     100,   256,   131,    -1,    -1,   165,    -1,     3,    -1,   164,
      -1,     7,    -1,    26,   152,    -1,   156,    -1,    67,   131,
      -1,    67,   256,   131,    -1,   131,    -1,   170,    -1,    62,
     170,    -1,    59,   170,    -1,   189,    -1,   219,    -1,   185,
      -1,   171,    -1,   196,    -1,   222,    -1,    35,   255,   131,
      -1,    35,   255,    30,   160,   131,    -1,    35,   255,    52,
     159,   131,    -1,    35,   222,    -1,    32,     9,    -1,    26,
     152,    87,   255,   131,    -1,    87,   255,   167,    -1,    19,
     255,    37,   255,   246,   167,    -1,    19,   255,    37,   174,
     246,   167,    -1,    19,   255,   246,   167,    -1,    33,   255,
      37,   255,   167,    -1,    33,   255,    37,   174,   167,    -1,
      33,   255,   167,    -1,    33,   174,   167,    -1,    33,    55,
     165,    37,   255,   167,    -1,    34,   255,    37,   255,   167,
      -1,    34,   255,    37,   255,   249,   167,    -1,    34,   255,
      37,   174,   167,    -1,    34,   255,    37,   174,   249,   167,
      -1,    34,   255,   167,    -1,    34,   255,   249,   167,    -1,
     142,   234,    37,   255,   143,   152,    -1,   142,   234,    37,
     255,   249,   143,   152,    -1,   142,   234,    37,   174,   143,
     152,    -1,   142,   234,    37,   174,   249,   143,   152,    -1,
     142,   234,   143,   152,    -1,   142,   234,   249,   143,   152,
      -1,    90,   140,   234,   141,    -1,    36,   255,    75,   152,
      -1,    36,   255,   156,    -1,    36,   255,    75,   152,    28,
     152,    -1,    36,   255,   156,    28,   152,    -1,    23,   152,
      -1,    78,   255,   131,    -1,    79,   255,   131,    -1,    78,
     163,    -1,    79,   163,    -1,    78,   156,   178,    -1,    79,
     156,   178,    -1,    -1,   178,   179,    -1,    16,   156,    -1,
      16,   180,   156,    -1,    16,   140,   180,   141,   156,    -1,
     165,    -1,   165,   111,   255,    -1,    76,   255,   131,    -1,
      69,   255,   138,   183,   139,    -1,    -1,   183,   184,    -1,
      85,   234,   167,    -1,    53,   152,    -1,    53,    26,   152,
      -1,   186,   165,   187,   138,   188,   139,    -1,    32,   166,
     186,   165,   187,   138,   188,   139,    -1,    17,    -1,    63,
      -1,    82,    -1,    -1,   111,   234,    -1,    -1,   188,   169,
      -1,   188,   151,   169,    -1,    29,   165,   138,   190,   139,
      -1,   191,    -1,   190,   112,    -1,   190,   112,   191,    -1,
     165,    -1,   165,    93,   255,    -1,    -1,    -1,    43,   193,
     204,   194,   211,   229,   218,   214,    -1,    -1,    39,    -1,
      31,   166,    -1,    32,   166,    -1,    -1,    -1,   195,   210,
     197,   199,   198,   211,   229,   212,   218,   213,    -1,   209,
     201,   203,    -1,   209,   202,   203,    -1,   209,   200,   114,
     201,   203,    -1,   209,   200,   114,   202,   203,    -1,   240,
      -1,   140,   255,   141,    -1,   165,    -1,   108,   165,    -1,
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
     215,    -1,   116,   255,    -1,   116,   216,    -1,    -1,    86,
     255,    -1,    80,   220,   131,    -1,    20,    80,   220,   131,
      -1,    32,    80,   220,   131,    -1,   165,   221,    -1,   165,
     221,   112,   220,    -1,    -1,    93,   241,    -1,    93,   230,
      -1,   223,    55,   224,   131,    -1,   223,    21,   224,   131,
      -1,   223,    65,   224,   131,    -1,   223,    21,    65,   224,
     131,    -1,   223,    84,   224,   131,    -1,    -1,    20,    -1,
      32,    -1,   225,    -1,   224,   112,   225,    -1,   165,   229,
     228,    -1,   140,   227,   141,   229,   228,    -1,    81,    -1,
     165,    -1,   140,   227,   141,    -1,   226,    -1,   226,   112,
      -1,   226,   112,   227,    -1,    -1,    93,    50,    -1,    93,
     256,    -1,    -1,   111,   241,    -1,   111,   230,    -1,     1,
      -1,   142,   234,   143,   241,    -1,   142,   234,   143,   230,
      -1,   142,   234,    37,   255,   143,   241,    -1,     1,    -1,
      -1,   241,    -1,   215,    -1,   142,   143,   231,    -1,   142,
     234,   143,   231,    -1,   142,   143,   232,    -1,   142,   234,
     143,   232,    -1,   142,   234,    37,   255,   143,   231,    -1,
      -1,   111,   241,    -1,   111,   215,    -1,   111,    27,    -1,
     111,    71,    -1,   111,    74,    -1,   111,   232,    -1,   255,
      -1,   215,    -1,   234,   112,   255,    -1,   234,   112,   215,
      -1,    81,    -1,   256,    -1,   215,    -1,   235,   112,   235,
      -1,   236,   112,   235,    -1,    -1,   238,    -1,   239,    -1,
     238,   112,   239,    -1,   165,    93,   215,    -1,   165,    93,
     256,    -1,   215,    -1,   256,    -1,   165,    -1,   244,    -1,
     257,    -1,   243,    -1,   265,    -1,   264,    -1,    71,   255,
      -1,    38,   140,   237,   141,    -1,    27,   140,   237,   141,
      -1,    73,   140,   237,   141,    -1,    72,    73,   140,   237,
     141,    -1,    12,   255,    -1,    74,   255,    -1,    33,   255,
      37,   255,    26,   255,    -1,    33,   255,    37,   174,    26,
     255,    -1,    33,   255,    26,   255,    -1,    33,   255,    37,
     255,    26,    36,   255,    75,   255,    -1,    33,   255,    37,
     174,    26,    36,   255,    75,   255,    -1,    33,   255,    26,
      36,   255,    75,   255,    -1,    34,   255,    37,   255,    26,
     255,    -1,    34,   255,    37,   174,    26,   255,    -1,    34,
     255,    26,   255,    -1,    34,   255,    37,   255,    26,    36,
     255,    75,   255,    -1,    34,   255,    37,   174,    26,    36,
     255,    75,   255,    -1,    34,   255,    26,    36,   255,    75,
     255,    -1,   142,   234,   143,   255,    -1,   142,   234,    37,
     255,   143,   255,    -1,   142,   234,    37,   174,   143,   255,
      -1,   142,   234,    37,   255,   143,    36,   255,    75,   255,
      -1,   142,   234,    37,   174,   143,    36,   255,    75,   255,
      -1,    36,   255,    75,   255,    28,   255,    -1,    49,    -1,
     244,    -1,   240,    -1,   260,    -1,   259,    -1,   192,    -1,
     253,    -1,   254,    -1,   252,   137,   255,    -1,    -1,   247,
      -1,    88,   140,   248,   141,    -1,   251,    -1,   248,   112,
     251,    -1,    88,   140,   250,   141,    -1,   251,    -1,   250,
     112,   251,    -1,   208,   240,    -1,   255,    64,   240,    -1,
     268,    64,   240,    -1,   257,    -1,   252,   137,   255,    -1,
      48,   255,    -1,    44,   224,    37,   255,    -1,   262,    -1,
     241,    -1,   242,    -1,   266,    -1,   267,    -1,   192,    -1,
     253,    -1,   254,    -1,   140,   116,   255,   141,    -1,   255,
     111,   255,    -1,   255,   115,   255,    -1,   255,   115,    -1,
     115,   255,    -1,   115,    -1,    78,   255,    -1,    79,   255,
      -1,   255,    -1,   240,    -1,   259,    -1,   260,    -1,   261,
      -1,   257,    -1,   192,    -1,   258,   140,   237,   141,    -1,
     258,   142,   237,   143,    -1,    58,   140,   237,   141,    -1,
     255,   114,   165,    -1,   255,   114,    80,    -1,   255,   114,
      27,    -1,   140,   235,   141,    -1,   140,   235,   112,   141,
      -1,   140,   236,   141,    -1,   140,   236,   112,   141,    -1,
       4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,
     138,   234,   139,    -1,   138,   234,   112,   139,    -1,   142,
     234,   143,    -1,   142,   234,   112,   143,    -1,   142,   263,
     143,    -1,   142,   263,   112,   143,    -1,   255,    91,   255,
      -1,   263,   112,   255,    91,   255,    -1,   255,   129,   255,
      -1,   255,   124,   255,    -1,   255,   134,   255,    -1,   255,
     113,   255,    -1,   255,   132,   255,    -1,   255,   133,   255,
      -1,   255,   125,   255,    -1,   255,   117,   255,    -1,   255,
     127,   255,    -1,   255,   123,   255,    -1,   255,   120,   255,
      -1,   255,   122,   255,    -1,   255,   119,   255,    -1,   255,
     107,   255,    -1,   255,   109,   255,    -1,   255,   110,   255,
      -1,   255,    92,   255,    -1,   255,   128,   255,    -1,   255,
     118,   255,    -1,   255,    15,   255,    -1,   255,    10,   255,
      -1,   255,   121,   255,    -1,   255,    25,   255,    -1,   129,
     255,    -1,   124,   255,    -1,    46,   255,    -1,    56,   255,
      -1,   126,   255,    -1,   108,   255,    -1,   255,    64,   255,
      -1,   255,    64,   174,    -1,   268,    64,   255,    -1,   268,
      64,   174,    -1,   255,    68,   255,    -1,   255,    68,   174,
      -1,   268,    68,   255,    -1,   268,    68,   174,    -1,   129,
      -1,   134,    -1,    92,    -1,   128,    -1,   107,    -1,   109,
      -1,   110,    -1
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
    1373,  1379,  1380,  1381,  1382,  1386,  1387,  1388,  1392,  1393,
    1397,  1398,  1402,  1403,  1407,  1408,  1409,  1410,  1414,  1425,
    1426,  1427,  1428,  1429,  1430,  1432,  1434,  1436,  1438,  1440,
    1442,  1447,  1449,  1451,  1453,  1455,  1457,  1459,  1461,  1463,
    1465,  1467,  1469,  1471,  1478,  1484,  1490,  1496,  1505,  1515,
    1523,  1524,  1525,  1526,  1527,  1528,  1529,  1530,  1535,  1536,
    1540,  1544,  1547,  1552,  1556,  1559,  1564,  1568,  1569,  1573,
    1574,  1579,  1584,  1592,  1593,  1594,  1595,  1596,  1597,  1598,
    1599,  1600,  1602,  1604,  1606,  1608,  1610,  1615,  1616,  1617,
    1620,  1621,  1622,  1623,  1634,  1635,  1639,  1640,  1641,  1645,
    1646,  1647,  1655,  1656,  1657,  1658,  1662,  1663,  1664,  1665,
    1666,  1667,  1668,  1669,  1670,  1671,  1675,  1683,  1684,  1688,
    1689,  1690,  1691,  1692,  1693,  1694,  1695,  1696,  1697,  1698,
    1699,  1700,  1701,  1702,  1703,  1704,  1705,  1706,  1707,  1708,
    1709,  1710,  1714,  1715,  1716,  1717,  1718,  1719,  1723,  1724,
    1725,  1726,  1730,  1731,  1732,  1733,  1738,  1739,  1740,  1741,
    1742,  1743,  1744
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
  "opt_formal_type", "expr_ls", "tuple_component", "tuple_expr_ls",
  "opt_actual_ls", "actual_ls", "actual_expr", "ident_expr",
  "type_level_expr", "for_expr", "cond_expr", "nil_expr",
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
     233,   234,   234,   234,   234,   235,   235,   235,   236,   236,
     237,   237,   238,   238,   239,   239,   239,   239,   240,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   243,   244,
     245,   245,   245,   245,   245,   245,   245,   245,   246,   246,
     247,   248,   248,   249,   250,   250,   251,   251,   251,   252,
     252,   253,   254,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   256,   256,   256,
     257,   257,   257,   257,   258,   258,   259,   259,   259,   260,
     260,   260,   261,   261,   261,   261,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   263,   263,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   265,   265,   265,   265,   265,   265,   266,   266,
     266,   266,   267,   267,   267,   267,   268,   268,   268,   268,
     268,   268,   268
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
       2,     1,     1,     3,     3,     1,     1,     1,     3,     3,
       0,     1,     1,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     4,     4,     5,     2,
       2,     6,     6,     4,     9,     9,     7,     6,     6,     4,
       9,     9,     7,     4,     6,     6,     9,     9,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       4,     1,     3,     4,     1,     3,     2,     3,     3,     1,
       3,     2,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     3,     3,     2,     2,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     4,     3,
       3,     3,     3,     4,     3,     4,     1,     1,     1,     1,
       1,     3,     4,     3,     4,     3,     4,     3,     5,     3,
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
       3,     0,     0,     1,    38,    80,   426,   427,   428,   429,
     430,     0,   378,    78,   152,   378,     0,   283,    78,     0,
       0,     0,     0,     0,    78,    78,     0,     0,   282,     0,
       0,   170,     0,   166,     0,     0,     0,     0,   369,     0,
       0,     0,     0,   282,   282,   153,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   154,     0,
       0,     0,   478,   480,     0,   481,   482,   406,     0,     0,
     479,   476,    87,   477,     0,     0,     0,     4,     0,     5,
       9,    44,    10,    11,    12,    14,   338,    22,    13,    88,
      94,    15,    17,    16,    19,    20,    21,    18,    93,     0,
      91,   398,     0,    95,    92,    96,     0,   410,   394,   395,
     341,   339,     0,     0,   399,   400,     0,   340,     0,   411,
     412,   413,   393,   343,   342,   396,   397,     0,    24,   349,
       0,     0,   379,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,   398,   410,   339,   399,   400,   378,   340,
     411,   412,     0,     0,   129,     0,     0,   330,     0,    82,
      81,   171,   101,     0,   172,     0,     0,     0,     0,     0,
     283,   284,   100,     0,     0,   330,     0,     0,     0,     0,
       0,   285,     0,    84,    32,     0,   464,   391,     0,   465,
       7,   330,   284,    90,    89,   262,   322,     0,   321,     0,
       0,    85,   409,     0,     0,    35,     0,   344,     0,   330,
      36,   350,     0,   136,   132,     0,   340,   136,   133,     0,
     274,     0,     0,   321,     0,     0,   467,   405,   463,   466,
     462,    46,    48,     0,     0,   325,     0,   327,     0,     0,
     326,     0,   321,     0,     0,     6,    45,     0,   155,   248,
     247,   173,     0,     0,     0,     0,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   330,   330,     0,     0,     0,    25,    26,     0,
      27,     0,     0,     0,     0,     0,     0,     0,    28,    29,
       0,   338,   336,     0,   331,   332,   337,     0,     0,     0,
       0,     0,   110,     0,     0,   109,     0,     0,     0,   116,
       0,     0,    54,    97,     0,   126,     0,    30,   223,   167,
     289,     0,   290,   292,     0,   301,     0,   295,     0,     0,
      83,    31,    33,     0,   172,   261,     0,    61,   407,   408,
      86,   145,    34,   330,     0,   143,   134,   130,   135,   131,
       0,   272,   269,    58,     0,    54,   103,    37,    47,    49,
       0,   431,     0,     0,   422,     0,   424,     0,     0,     0,
       0,     0,     0,   435,     8,     0,     0,     0,   241,     0,
       0,     0,     0,     0,   377,   459,   458,   461,   469,   468,
     473,   472,   455,   452,   453,   454,   402,   442,   421,   420,
     419,   403,   446,   457,   451,   449,   460,   450,   448,   440,
     445,   447,   456,   439,   443,   444,   441,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   471,   470,   475,   474,   235,
     232,   233,   234,   238,   239,   240,     0,     0,   381,     0,
       0,     0,     0,     0,     0,     0,     0,   433,   378,   378,
     106,   270,     0,     0,   346,     0,   164,     0,   161,   271,
     155,     0,     0,     0,   353,     0,     0,     0,   359,     0,
       0,     0,   117,   477,    57,     0,    50,    55,     0,   125,
       0,     0,   345,     0,   224,     0,   231,   249,     0,   293,
       0,   305,     0,   300,   394,     0,   287,   392,   286,   418,
     324,   323,     0,     0,   347,     0,   137,   276,   394,     0,
       0,     0,   432,   401,   423,   328,   425,   329,     0,     0,
     434,   122,   363,     0,   437,   436,     0,     0,   156,   157,
     245,   242,   243,   246,   174,     0,     0,   278,   277,   279,
     281,    62,    69,    70,    71,    66,    68,    76,    77,    64,
      67,    65,    63,    73,    72,    74,    75,   416,   417,   236,
     237,   386,     0,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   102,   334,   335,   333,
       0,   162,   160,     0,     0,   124,     0,     0,   108,     0,
     107,     0,     0,   114,     0,     0,   112,     0,     0,   384,
       0,    98,     0,    99,     0,     0,   128,   230,   222,     0,
     314,   250,   253,   252,   254,     0,   291,   294,   295,     0,
     296,   297,     0,     0,   144,   146,   348,     0,   138,   141,
       0,   273,    59,    60,     0,     0,     0,     0,   123,     0,
       0,   282,   244,   249,   205,   203,   208,   215,   216,   217,
     212,   214,   210,   213,   211,   209,   219,   218,   184,   187,
     185,   186,   197,   188,   201,   193,   191,   204,   192,   190,
     195,   200,   202,   189,   194,   198,   199,   196,   206,   207,
       0,   182,     0,   220,   220,   180,   280,   382,   410,   410,
       0,     0,     0,     0,     0,     0,     0,     0,   105,   104,
     165,   163,   157,   111,     0,     0,   352,     0,   351,     0,
       0,   358,   115,     0,   357,   113,     0,   383,    52,    51,
     127,   368,   225,     0,     0,   295,   251,   267,   288,     0,
       0,     0,   148,     0,     0,     0,   139,     0,   120,   365,
       0,     0,   118,   364,     0,   438,    39,     0,    78,   282,
     282,   150,   282,   158,     0,   183,     0,     0,   223,   176,
     177,     0,     0,     0,     0,     0,     0,     0,     0,   282,
     356,     0,     0,   362,     0,     0,   385,     0,   314,   317,
     318,   319,     0,   316,   320,   394,   263,   227,   226,     0,
       0,     0,   303,   394,   149,   147,     0,   142,     0,   121,
       0,   119,    40,   159,   255,   181,   182,   220,   220,     0,
       0,     0,     0,     0,     0,     0,   151,     0,     0,     0,
       0,    53,   295,   306,     0,   264,   266,   265,   268,   259,
     260,   168,     0,   140,     0,     0,   256,   267,   178,   179,
     221,     0,     0,     0,     0,     0,     0,   355,   354,   361,
     360,   229,   228,   308,   309,   311,   394,     0,   433,   394,
     367,   366,     0,     0,   310,   312,   257,   175,   258,   306,
     313
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   232,    78,   519,    80,    81,   247,    82,
     233,   514,   518,   515,    83,    84,    85,   160,    86,   164,
     184,    87,    88,    89,    90,    91,    92,   612,    93,    94,
      95,   376,   546,   670,    96,    97,   542,   665,    98,    99,
     407,   681,   100,   497,   498,   143,   177,   527,   102,   103,
     408,   683,   574,   722,   723,   724,   799,   349,   523,   524,
     525,   476,   575,   251,   655,   877,   907,   871,   196,   866,
     827,   830,   104,   221,   381,   105,   106,   180,   181,   353,
     354,   536,   357,   533,   894,   824,   765,   234,   238,   239,
     323,   324,   325,   144,   108,   109,   110,   145,   112,   131,
     132,   477,   340,   638,   478,   113,   146,   147,   202,   326,
     149,   118,   150,   151,   121,   122,   243,   123,   124,   125,
     126,   127
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -814
static const yytype_int16 yypact[] =
{
    -814,   114,  2640,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  3890,    61,   174,  -814,    61,  8373,   131,   174,  3890,
    8373,  3890,    96,   174,   104,   533,  6973,  8373,  7113,  8373,
     108,  -814,   174,  -814,    46,  7533,  8373,  8373,  -814,  8373,
    8373,   277,   149,   790,   967,  -814,  7673,  6553,  8373,  7533,
    8373,   255,   159,  3890,  8373,  8513,  8513,   174,  -814,  7673,
    8373,  8373,  -814,  -814,  8373,  -814,  -814,  1433,  8373,  8373,
    -814,  8373,  -814,  -814,  3050,  6133,  7673,  -814,  3750,  -814,
    -814,   272,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,   174,
    -814,   219,    68,  -814,  -814,  -814,   271,   206,  -814,  -814,
    -814,   207,   212,   220,   234,   237, 13111,  2157,   185,   242,
     243,  -814,  -814,  -814,  -814,  -814,  -814,   226,  -814, 13111,
     240,  3890,  -814,   258,  -814,   222,  8373,  8373,  8373,  8373,
    8373,  7673,  7673,   229,  -814,  -814,  -814,  -814, 11308,   245,
    -814,  -814,   174,   259,  -814, 11577,   315,  6693,   269,  -814,
    -814,  -814,  -814,   174,   126,   174,   270,    17, 10258, 10224,
    -814,  -814,  -814, 11246, 10436,  6693,  3890,   273,    10,    89,
      44,  -814,  3890,  -814,  -814, 10507,   280,   301, 10507,   280,
    -814,  6693,   246,  -814,  -814,   174,  -814,   -15, 13111,  8373,
    8373,  -814, 13111,   285, 10296,  -814, 10507, 13111,   282,  6693,
    -814, 13111, 11648,  -814,  -814, 11710,  5553,  -814,  -814, 11781,
     327,   294,    32, 11382, 10507, 11843,   127,  1822,   280,   127,
     280,  -814,  -814,  3190,   -12,  -814,  8373,  -814,   -29,   -23,
    -814,    66, 11914,   -42,   419,  -814,  -814,   380,   318,  -814,
    -814,  -814,    19,    46,    46,    46,  -814,  8373,  8373,  8373,
    8373,  7813,  7813,  8373,  8373,  8373,  8373,  8373,  8373,    77,
    1433,  8373,  8373,  8373,  8373,  8373,  8373,  8373,  8373,  8373,
    8373,  8373,  8373,  8373,  8373,  8373,  7253,  7253,  7253,  7253,
    7253,  7253,  7253,  7253,  7253,  7253,  7253,  7253,  7253,  7253,
    7253,  7253,  6693,  6693,  7813,  7813,  5993,  -814,  -814,  3330,
    -814, 11444, 11515, 11976,    41,  7813,    17,   300,  -814,  -814,
    8373,   339,  -814,   292,   323,  -814,  -814,   174,   306,   174,
     402,  7673,  -814,  4030,  7813,  -814,  4170,  7813,   302,  -814,
      17,  8653,  8373,  -814,  3890,   412,   304,  -814,    65,  -814,
    -814,    10,  -814,   329,   324,  -814,  5430,   350,  8373,    46,
    -814,  -814,  -814,   326,  -814,  -814,  7673,  -814, 13111, 13111,
    -814,  -814,  -814,  6693,   328,  -814,   448,  -814,   448,  -814,
    5430,   359,  -814,  -814,  8653,  8373,  -814,  -814,  -814,  -814,
    7393,  -814, 10066,  6273,  -814,  6413,  -814,  7813,  5570,  2910,
     331,  8373,  5852,  -814,  -814,   174,  7673,   337,   147,    46,
     102,   106,   179,   183, 11172,  5261,  5261,   252,  -814,   252,
    -814,   252,  1220,  1046,  1581,   951,   301,   127,  -814,  -814,
    -814,  1822,  1671,   252,  1794,  1794,  5261,  1794,  1794,  1881,
     127,  1671,  5401,  1881,   280,   280,   127,   345,   346,   355,
     361,   362,   363,   365,   366,   367,   369,   371,   373,   376,
     379,   381,   382,   349,   375,  -814,   252,  -814,   252,    58,
    -814,  -814,  -814,  -814,  -814,  -814,   174,   -20,  -814, 13173,
     253,  8793,  7813,  8933,  7813,  8373,  7813,  2046,    61, 12047,
    -814,  -814, 12109,  6693,  -814,  6693,   423,   112,  -814,  -814,
     318,  8373,    78,  8373, 13111,    56, 10569,  8373, 13111,    45,
   10374,  5993,  -814,   392,   413,   395, 12180,   413,   396,   500,
   12242,  3890,  -814,   101,  -814,    52,  -814,   265,   393,    10,
      89,  -814,  7673,  -814,   555,  6833,  -814, 13111,  -814,  -814,
    -814, 13111,    40,   394,  -814,    27,  -814,  -814,   188,   174,
     406,   408,  -814,  -814,  -814,  -814,  -814,  -814,   -28,   453,
    -814,  -814, 13111,  3890, 13111,  -814, 12313,   409,   421,  -814,
     479,  -814,  -814,  -814,  -814,  2322,   210,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  5993,  -814,  7813,  7813,  8373,   523, 12375,  8373,
     525, 12446,   415,  2460,    17,    17,  -814,  -814,  -814,  -814,
    8373,   174,  -814,   414, 10507,  -814, 10640,  4310,  -814,  4450,
    -814, 10702,  4590,  -814,    17,  4730,  -814,    17,   103,  -814,
    8373,  -814,  8373,  -814,  3890,  8373,  -814,   642,  -814,    10,
     445,   494,  -814,  -814,  -814,    55,  -814,  -814,   350,    73,
    -814,  -814,  4870,  7673,  -814,  -814,  -814,   174,  -814,   454,
     222,  -814,  -814,  -814,  5010,   426,  5150,   436,  -814,  8373,
    3470,   873,  -814,   265,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,   174,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,  -814,
    8373,   469,   470,   450,   450,  -814,  -814,  -814,   105,   111,
   12508,  9073,  9213, 12579,  9353,  9493,  9633,  9773,  -814,  -814,
   13111,  -814,  -814,  -814,  3890,  8373, 13111,  8373, 13111,  3890,
    8373, 13111,  -814,  8373, 13111,  -814,  5993,  -814, 12641, 13111,
    -814, 13111,  -814,   456,  7953,   110,  -814,   505,  -814,  7813,
    5290,  3890,  -814,    49,   460,  8373,  -814,  8373,  -814, 13111,
    3890,  8373,  -814, 13111,  3890, 13111,  -814,  3610,   603,   483,
     483,  -814,  1392,  -814,    31,  -814, 10104,  2718,    65,  -814,
    -814,  8373,  8373,  8373,  8373,  8373,  8373,  8373,  8373,  1087,
   12242, 10773, 10835, 12242, 10906, 10968,  -814,  8373,   445,    96,
    8373,  8373,  5711,  -814,  -814,   113,  7673,  -814,  -814,  8373,
      20,  9950,  -814,   608,   315,  -814,   222, 13111, 11039,  -814,
   11101,  -814,  -814,  -814,   517,  -814,  -814,   450,   450,   155,
   12712, 12774, 12845, 12907, 12978, 13040,  -814,  3890,  3890,  3890,
    3890, 13111,   110,  8093,    87,  -814,  -814, 13111, 13111,  -814,
    -814,  -814,  9773,  -814,  3890,  3890,  -814,   505,  -814,  -814,
    -814,  8373,  8373,  8373,  8373,  8373,  8373, 12242, 12242, 12242,
   12242,  -814,  -814,  -814,  -814,  -814,   260,  7813,  9913,   721,
   12242, 12242,   109,  9988,  -814,  -814,  -814,  -814,  -814,  8233,
    -814
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -814,  -814,  -814,    -1,  -658,  1709,  -814,  -814,  -814,  1696,
     -85,  -279,   223,   221,  -814,  -814,   268,   275,  2213,   -17,
     624,  -731,  -645,   -33,  -814,  -814,  -814,    47,  -814,  -814,
    -814,   397,  -814,   -58,  -814,  -814,  -814,  -814,  -814,   447,
     115,  -126,  -814,  -814,    -9,   635,  -814,  -814,  -814,  -814,
    -814,  -814,  -814,  -814,  -178,  -177,  -716,  -814,  -175,   -26,
    -814,  -336,  -814,  -814,   -57,  -814,  -814,  -274,    13,  -814,
    -233,  -246,  -814,  -132,  -814,   606,  -814,  -213,   276,  -814,
    -348,  -652,  -509,  -378,  -670,  -813,  -182,    18,    28,  -814,
    -130,  -814,   144,   356,  -352,  -814,  -814,   963,  -814,     0,
    -814,  -814,  -212,  -814,  -506,  -814,   992,  1181,    -2,   160,
     417,  -814,  1354,  1500,  -814,  -814,  -814,  -814,  -814,  -814,
    -814,  -290
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -434
static const yytype_int16 yytable[] =
{
     116,    77,   547,   528,   534,   639,   768,   161,   800,   129,
     193,   194,   526,     5,   148,   135,   480,   116,   155,   116,
     317,   658,     5,   792,   168,   169,   173,   174,   548,   400,
       5,   328,   355,   185,   186,   187,   793,   188,   189,   410,
     411,   412,   413,   182,   198,   346,   204,   206,   207,     5,
     895,   211,   212,   215,   219,     5,   355,   223,   224,   225,
     338,   363,   226,   517,   197,   227,   228,   229,  -230,   230,
     402,   632,   198,   167,   242,   182,   116,   222,   486,   374,
       5,   358,   627,   393,   409,   905,   469,    47,   237,   395,
     355,   350,   602,   662,   241,   599,   727,   366,  -298,   870,
     390,   403,   470,   397,   428,   471,   517,     5,  -298,   249,
     769,   159,   394,   828,     3,   674,   367,  -298,   396,   472,
     473,   603,  -298,   600,   897,   663,  -298,   391,   250,   116,
     474,   878,   879,   338,   129,   311,   312,   313,   211,   198,
     242,  -298,   356,    14,   366,   475,   767,   843,   316,   130,
     351,   792,   260,   398,   338,   309,   359,   429,   309,   178,
     314,   366,  -298,   383,   793,   309,   356,   667,   570,  -298,
     322,   870,   463,   464,   116,   364,    47,     5,   398,   664,
     116,   657,  -298,   309,   487,   398,   178,   309,   322,    45,
     366,   261,   649,  -298,   309,   262,   576,   368,   369,   398,
     356,  -298,   571,   535,   322,  -230,  -315,   203,    58,   399,
     892,   152,   572,   647,   359,   756,   770,  -387,   359,   625,
    -298,   480,   322,  -388,   621,  -315,   826,   573,   904,  -315,
     898,   116,   389,   577,   392,   240,   157,   578,   267,   910,
     906,   269,   648,   543,   757,   272,  -387,   309,   175,     5,
     816,   622,  -388,   159,  -315,   414,   415,   416,   417,   419,
     421,   422,   423,   424,   425,   426,   427,   647,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,   442,
     443,   444,   445,   446,   190,   844,   651,   -78,   133,   191,
     304,   359,   252,   153,   305,   359,   880,   634,   637,   209,
    -275,   763,   466,   468,   479,   260,   -78,   116,   418,   420,
     579,   526,   480,   489,   580,   322,   322,   605,   492,  -275,
     652,   305,   359,   214,   218,   302,   253,   303,   208,   198,
     653,   504,   506,   246,   508,   510,   254,  -371,  -370,   516,
     516,   726,   520,   256,   261,   654,   675,   677,   262,   502,
    -374,   465,   467,  -307,   116,   255,   537,   257,   107,  -415,
     309,  -415,   488,   267,   541,  -375,   269,   107,  -376,  -415,
     272,  -415,  -307,  -373,  -372,   107,  -307,   107,   116,   540,
     306,   505,   516,   516,   509,  -414,   322,  -414,   541,   308,
     318,   267,   832,   268,   269,   559,   541,   562,   272,   564,
     566,  -307,   320,   540,   198,   279,   237,   327,   237,   107,
     331,   540,   825,   348,   285,   269,   370,   671,   833,   117,
     380,   555,   373,   557,   568,   382,   404,   405,   117,   406,
     107,   491,   493,   494,   107,   495,   117,   499,   117,   501,
     521,   529,   511,   535,   558,   522,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   526,   258,   545,   530,   480,   539,   259,   544,
     117,   549,   216,   216,   563,   569,   581,   582,   260,   504,
     608,   508,   611,   520,   613,   562,   583,   107,   614,   615,
     597,   117,   584,   585,   586,   117,   587,   588,   589,   624,
     590,   626,   591,   170,   592,   631,   617,   593,   322,   479,
     594,   896,   595,   596,    24,   192,   620,   261,   598,   116,
     899,   262,    31,   -56,  -169,   640,   641,   643,   644,   607,
     242,   610,   107,   366,   656,   666,     5,   672,   107,   673,
     159,   338,   162,  -169,   682,   263,   896,   680,   117,   731,
     659,   734,   742,   240,  -284,   240,   764,   896,   736,   766,
     264,   116,   265,   266,   267,   775,   268,   269,   270,   780,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   784,
     280,   281,   282,  -338,   797,   283,   284,   285,  -284,   107,
     798,   829,  -299,   117,   876,   787,   676,   818,  -284,   117,
     479,   836,   419,   466,   730,   550,     5,   733,   551,   774,
     159,   329,   741,   163,   378,   623,   809,  -284,   740,   847,
     848,   762,  -299,   849,  -284,   746,   794,   748,   908,   891,
     751,   902,  -299,   754,   172,   538,   862,   101,   758,   619,
     759,  -299,   116,   761,     0,  -302,   101,     0,  -299,     0,
     117,   418,   465,   618,   101,     0,   101,     0,  -284,     0,
     116,   198,     0,   469,     0,   107,     0,  -299,  -284,     0,
       0,     0,   779,   205,   783,  -302,     0,   785,   116,   470,
       0,   773,   471,   163,     0,  -302,  -299,  -284,   101,   107,
       0,     0,   107,  -299,  -302,   661,   472,   473,     0,     0,
     107,  -302,     0,     0,     0,     0,     0,   474,     0,   101,
       0,     0,     0,   101,     0,     0,     0,     0,   796,     0,
    -302,     0,   475,     0,     0,     0,   117,     0,     0,   746,
     748,     0,   751,   754,   779,   783,     0,     0,     0,  -302,
       0,     0,   810,   811,     0,   812,  -302,   813,   814,     0,
     117,   815,     0,   117,   479,   107,   193,   194,  -304,     0,
       0,   117,   116,     0,     0,     0,   101,   831,   562,   116,
       0,     0,     0,   837,     0,   838,     0,   823,   116,   840,
       0,     0,   116,     0,     0,   116,   389,     0,  -304,     0,
       0,   332,   335,   339,     0,     0,     0,     0,  -304,   810,
     850,   851,   813,   852,   853,   854,   855,  -304,     0,   361,
     170,   101,   362,     0,  -304,   861,   117,   101,   207,   211,
     242,    24,   192,     0,   867,     0,     0,   868,     0,    31,
     372,  -169,   601,  -304,     0,     0,     0,   -43,     0,   865,
     864,     0,     0,     0,     0,     0,     0,     0,   386,     0,
    -169,   -43,  -304,     0,     0,   887,   888,   889,   890,  -304,
       0,   116,     0,     0,     0,     0,     0,     0,   101,     0,
     783,     0,   900,   901,     0,     0,   893,   107,     0,   887,
     888,   889,   890,   900,   901,     0,     0,     0,     0,     0,
      14,     0,     0,    17,     0,   903,   562,     0,     0,     0,
       0,     0,    23,     0,    24,   788,     0,   783,    28,     0,
       0,   893,    31,     0,  -169,     0,     0,     0,     0,   107,
       0,     0,   893,     0,     0,     0,     0,     0,     0,     0,
      41,   725,   789,  -169,     0,   790,    45,     0,   117,     0,
     490,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,    58,     0,     0,     0,     0,
     728,   729,     0,     0,   512,   111,     0,     0,   101,     0,
       0,   101,     0,     0,   111,     0,   260,     0,     0,   101,
     117,     0,   111,   107,   111,   107,     0,   170,   107,     0,
       0,   107,     0,     0,   114,     0,     0,     0,    24,   192,
     107,     0,     0,   114,    72,     0,    31,     0,  -169,     0,
       0,   114,   791,   114,   -42,   261,   111,     0,   107,   262,
       0,     0,     0,     0,     0,     0,     0,  -169,   -42,     0,
     107,     0,   107,     0,   101,     0,   107,   111,     0,     0,
       0,   111,     0,     0,   117,   114,   117,     0,     0,   117,
       0,     0,   117,     0,     0,     0,     0,     0,   264,     0,
       0,   117,   267,     0,   268,   269,   114,     0,     0,   272,
     114,   260,     0,     0,     0,     0,   279,     0,     0,   117,
       0,     0,     0,   283,   284,   285,     0,     0,     0,     0,
       0,   117,     0,   117,   111,     0,     0,   117,     0,     0,
     107,     0,     0,     0,    14,   107,     0,    17,     0,     0,
     261,     0,     0,     0,   262,     0,    23,     0,    24,   788,
       0,     0,    28,   114,     0,     0,    31,   107,  -169,   628,
     630,     0,     0,   633,   636,     0,   107,     0,     0,   111,
     107,     0,     0,   107,    41,   111,   789,  -169,     0,   790,
      45,     0,     0,     0,     0,     0,   101,   267,     0,   268,
     269,   117,     0,     0,   272,     0,   117,    57,   114,    58,
       0,   279,     0,     0,   114,     0,     0,     0,   283,   284,
     285,     0,     0,   115,     0,     0,     0,     0,   117,     0,
       0,     0,   115,     0,     0,     0,   111,   117,   101,     0,
     115,   117,   115,     0,   117,     0,     0,     0,     0,     0,
       0,     0,     0,   107,   107,   107,   107,     0,    72,     0,
       0,     0,     0,     0,     0,   114,   856,     0,     0,     0,
     107,   107,     0,     0,   115,     0,     0,     0,   738,   739,
       0,     0,     0,     0,     0,   260,     0,     0,   743,     0,
       0,     0,     0,     0,     0,   115,     0,     0,   752,   115,
       0,   755,   101,     0,   101,     0,     0,   101,     0,     0,
     101,     0,   111,     0,   117,   117,   117,   117,     0,   101,
       0,     0,     0,     0,   261,     0,     0,     0,   262,     0,
       0,   117,   117,     0,     0,     0,   111,   101,     0,   111,
       0,   114,     0,     0,     0,     0,     0,   111,     0,   101,
       0,   101,   115,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   114,     0,   264,   114,   265,
     266,   267,     0,   268,   269,   270,   114,   271,   272,   273,
     274,     0,   276,   277,   278,   279,     0,   280,     0,   282,
       0,     0,   283,   284,   285,     0,   119,   115,     0,     0,
       0,     0,   111,   115,     0,   119,     0,     0,     0,     0,
       0,     0,     0,   119,     0,   119,     0,     0,     0,   101,
       0,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,   114,     0,     0,     0,     0,     0,   835,     0,     0,
       0,     0,     0,     0,     0,     0,   101,   119,     0,    14,
       0,     0,    17,     0,   115,   101,     0,     0,     0,   101,
       0,    23,   101,    24,   788,     0,     0,    28,   119,     0,
       0,    31,   119,  -169,     0,     0,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,   244,
       0,   789,  -169,     0,   790,    45,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,    57,     0,    58,     0,    33,    34,     0,    36,
       0,    37,    38,     0,   111,   119,     0,     0,     0,    40,
     115,    42,   101,   101,   101,   101,     0,     0,     0,     0,
       0,     0,   120,     0,    50,    51,    52,   140,     0,   101,
     101,   120,     0,   114,   115,     0,     0,   115,     0,   120,
       0,   120,     0,    72,     0,   115,   111,     0,     0,     0,
     119,     0,     0,     0,     0,     0,   119,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   120,     0,   114,     0,    68,     0,    69,
       0,     0,    71,     0,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,   120,   142,     0,     0,   120,     0,
     115,     0,     0,     0,     0,     0,     0,   119,     0,     0,
     111,     0,   111,     0,     0,   111,     0,     0,   111,     0,
       0,     0,     0,     0,     0,     0,   260,   111,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114,
       0,   114,     0,     0,   114,   111,     0,   114,     0,     0,
       0,   120,     0,     0,     0,     0,   114,   111,     0,   111,
       0,     0,     0,   111,     0,   261,     0,     0,     0,   262,
       0,     0,     0,     0,   114,     0,     0,     0,     0,     0,
       0,     0,     0,   119,     0,     0,   114,     0,   114,     0,
       0,     0,   114,     0,     0,     0,   120,     0,     0,     0,
       0,     0,   120,     0,     0,     0,     0,   119,   264,     0,
     119,   266,   267,     0,   268,   269,   260,     0,   119,   272,
       0,     0,   115,     0,     0,     0,   279,   111,     0,     0,
       0,    79,   111,   283,   284,   285,     0,     0,     0,     0,
     128,     0,     0,     0,     0,     0,     0,     0,   154,     0,
     156,   183,     0,   120,   111,   261,   114,     0,     0,   262,
       0,   114,     0,   111,   115,   183,     0,   111,     0,     0,
     111,   213,   217,   119,     0,     0,     0,     0,     0,     0,
       0,     0,   210,   114,     0,     0,     0,     0,     0,     0,
       0,     0,   114,     0,     0,     0,   114,     0,   264,   114,
     265,   266,   267,    79,   268,   269,   270,   245,     0,   272,
     273,   274,     0,   276,   277,   278,   279,     0,     0,     0,
     282,     0,     0,   283,   284,   285,     0,     0,   115,   120,
     115,     0,     0,   115,     0,     0,   115,     0,     0,   260,
     111,   111,   111,   111,     0,   115,     0,     0,     0,     0,
       0,   310,     0,   120,     0,     0,   120,   111,   111,     0,
     307,     0,     0,   115,   120,     0,     0,   260,     0,   114,
     114,   114,   114,     0,     0,   115,     0,   115,   261,     0,
       0,   115,   262,   183,   183,   183,   114,   114,     0,     0,
     345,     0,     0,     0,     0,   119,     0,     0,     0,     0,
       0,   183,     0,     0,   183,   347,   261,     0,     0,     0,
     262,   360,     0,     0,     0,     0,     0,     0,     0,   120,
       0,   264,   183,   265,   266,   267,   260,   268,   269,   270,
       0,     0,   272,     0,     0,     0,     0,   119,   278,   279,
     183,     0,     0,   282,     0,   115,   283,   284,   285,   264,
     115,   265,   266,   267,     0,   268,   269,     0,     0,     0,
     272,     0,    79,     0,     0,   261,   278,   279,     0,   262,
       0,   282,   115,     0,   283,   284,   285,     0,     0,     0,
       0,   115,     0,     0,     0,   115,     0,     0,   115,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   119,     0,   119,     0,     0,   119,     0,   264,   119,
     265,   266,   267,     0,   268,   269,     0,     0,   119,   272,
       0,     0,     0,     0,     0,     0,   279,     0,     0,     0,
       0,     0,   183,   283,   284,   285,   119,     0,    79,     0,
       0,   120,     0,     0,     0,     0,     0,     0,   119,     0,
     119,     0,     0,     0,   119,     0,   183,     0,   115,   115,
     115,   115,   360,     0,     0,   360,     0,     0,     0,     5,
       6,     7,     8,     9,    10,   115,   115,     0,   136,     0,
       0,     0,     0,   120,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   137,
     138,     0,   139,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,   119,     0,
       0,     0,    40,   119,    42,     0,     0,     0,   561,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,    52,
     140,     0,     0,     0,     0,   119,     0,   120,     0,   120,
       0,     0,   120,     0,   119,   120,     0,     0,   119,     0,
       0,   119,     0,     0,   120,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,   120,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,     0,   120,     0,   120,     0,     0,     0,
     120,     0,     0,     0,     0,     0,    75,     0,   142,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   183,   183,     0,     0,   183,   183,     0,     0,     0,
       0,   119,   119,   119,   119,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   134,     0,   119,   119,
     646,   134,     0,     0,     0,     0,   158,   134,   134,     0,
       0,   668,     0,     0,   120,   176,     0,   179,     0,   120,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,     0,     0,     0,     0,     0,     0,
     220,   120,   678,     0,     0,     0,     0,     0,     0,     0,
     120,     0,     0,     0,   120,     0,     0,   120,     0,     0,
       0,     0,   300,   301,  -389,     0,     0,  -414,     0,  -414,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     183,   183,   248,     0,     0,     0,     0,     0,     0,     0,
     183,     0,   345,     0,     0,     5,     0,   345,     0,     0,
     183,     0,   684,   183,     0,     0,   360,   685,   360,     0,
       0,   360,     0,     0,   360,     0,     0,     0,     0,     0,
       0,     0,     0,   760,     0,     0,     0,   120,   120,   120,
     120,     0,     0,     0,     0,   220,   776,     0,     0,     0,
     321,   772,     0,     0,   120,   120,   220,     0,   330,     0,
       0,     0,     0,   778,     0,   782,     0,     0,   321,    79,
       0,   352,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   321,   134,     0,     0,   365,     0,
       0,     0,     0,     0,     0,   686,   687,   688,   689,   690,
     691,     0,   321,   692,   693,   694,   695,   696,   697,   698,
     699,   700,   701,     0,     0,   702,     0,     0,     0,   703,
     704,   705,   706,   707,   708,   709,   710,   711,   712,   713,
       0,   714,     0,     0,   715,   716,   717,   718,     0,   719,
       0,     0,   720,     0,     0,   179,   179,   179,   179,   183,
     258,     0,     0,     0,     0,   259,     0,     0,     0,     0,
     834,     0,   430,     0,     0,   260,     0,     0,     0,   839,
       0,     0,     0,   841,     0,     0,    79,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   345,   345,     0,
     345,   345,     0,     0,     0,   321,   321,     0,     0,     0,
       0,     0,     0,     0,   261,     0,   869,     0,   262,     0,
       0,     0,   873,     0,   345,     0,   345,     0,     0,     0,
     496,     0,   500,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   352,     0,     0,   264,     0,   265,
     266,   267,   179,   268,   269,   270,     0,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   321,   280,   281,   282,
       0,     0,   283,   284,   285,     0,     0,     0,   869,     0,
       0,     0,     0,   737,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   567,     0,
       0,     0,   179,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      -2,     4,     0,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,    41,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,   321,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     5,    58,    59,  -282,     0,     0,    60,   684,    61,
       0,     0,    62,   685,     0,     0,     0,     0,   650,     0,
       0,     0,   352,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,   669,     0,
       0,     0,   220,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     0,    76,     0,     0,     0,     0,     0,   721,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   686,   687,   688,   689,   690,   691,     0,     0,   692,
     693,   694,   695,   696,   697,   698,   699,   700,   701,     0,
       0,   702,     0,     0,   496,   703,   704,   705,   706,   707,
     708,   709,   710,   711,   712,   713,     0,   714,     0,     0,
     715,   716,   717,   718,     0,   719,     0,     0,     0,     0,
       0,     0,   352,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     669,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,   795,     5,     6,     7,     8,     9,    10,     0,
    -433,     0,    11,    12,    13,  -433,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,  -433,    21,    22,  -433,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,     0,    42,    43,
    -169,   -41,    44,    45,  -433,  -282,    46,    47,  -433,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,   134,  -433,     0,     0,     0,     0,     0,     0,     0,
     846,     0,     0,     0,     0,     0,     0,  -433,    64,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,     0,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,    69,  -433,  -433,  -433,
       0,    72,  -433,  -433,  -433,     0,     0,     0,    74,  -433,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,    41,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
     195,    72,     0,     0,    73,     0,     0,     0,    74,   231,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,    41,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,   388,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,    41,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,   231,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,    41,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,   786,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,    41,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,   842,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,   244,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,     0,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,   503,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,     0,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,   507,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,     0,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,   745,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,     0,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,   747,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,     0,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,   750,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,     0,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,   753,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,     0,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,   771,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,     0,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,   777,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,     0,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     4,    76,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -282,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,   781,     0,    30,    31,
       0,  -169,    32,    33,    34,    35,    36,   -41,    37,    38,
       0,    39,     0,     0,     0,  -282,    40,     0,    42,    43,
    -169,   -41,    44,    45,     0,  -282,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,     0,    58,    59,  -282,     0,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,     0,   260,     0,    74,     0,
      75,   531,    76,     5,     6,     7,     8,     9,    10,     0,
    -433,     0,   136,     0,     0,  -433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -433,     0,    22,     0,     0,
       0,     0,     0,   137,   138,   261,   139,     0,    30,   262,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,     0,    40,     0,    42,     0,
       0,     0,     0,   263,  -433,     0,     0,     0,  -433,     0,
       0,    50,    51,    52,   140,     0,     0,     0,   264,     0,
     265,   266,   267,     0,   268,   269,   270,     0,   271,   272,
     273,   274,  -433,   276,   277,   278,   279,     0,   280,   281,
     282,     0,     0,   283,   284,   285,     0,  -433,    64,  -433,
    -433,  -433,     0,  -433,  -433,  -433,     0,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,    69,  -433,  -433,  -433,
       0,     0,  -433,  -433,  -433,     0,   260,     0,   141,     0,
      75,   531,   532,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   137,   138,   261,   139,     0,    30,   262,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,     0,    40,     0,    42,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,    50,    51,    52,   140,     0,     0,     0,   264,     0,
     265,   266,   267,     0,   268,   269,   270,     0,   271,   272,
     273,   274,    62,   276,   277,   278,   279,     0,   280,     0,
     282,     0,     0,   283,   284,   285,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,     0,     0,     0,    73,     0,     0,     0,   141,     0,
      75,     0,   532,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   137,   138,     0,   139,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,     0,    40,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,    52,   140,     0,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,   300,   301,
       0,     0,     0,  -414,    68,  -414,    69,     0,    70,    71,
     195,     0,     0,     0,    73,     0,     0,     0,   141,     0,
      75,     0,   142,   560,     5,     6,     7,     8,     9,    10,
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
       0,    75,     0,   142,   863,     5,     6,     7,     8,     9,
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
      70,    71,     0,     0,     0,     0,    73,     0,     0,     0,
     141,     0,    75,     0,   142,   565,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   139,
     470,    30,     0,   471,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,   472,   473,    40,
       0,    42,     0,     0,     0,     0,     0,     0,   474,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,   199,   200,     0,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,   195,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,   554,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,   199,   200,     0,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,   195,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,   556,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,   199,   200,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,   201,     0,     0,    73,     0,     0,
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,   199,   200,     0,     0,     0,     0,     0,     0,     0,
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
       0,    37,    38,   660,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,   199,   200,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,   165,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
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
      22,     0,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,   199,   200,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
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
       0,    70,    71,   195,     0,     0,     0,    73,     0,     0,
       0,   141,   552,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   182,
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
       0,    74,     0,    75,     0,   142,     5,     6,     7,     8,
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
       0,    70,    71,   195,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   166,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,     0,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     819,     0,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   820,    51,    52,   821,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    69,
       0,    70,    71,   195,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,     0,   822,     5,     6,     7,     8,
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
       0,    70,    71,   195,     0,     0,     0,    73,     0,     0,
       0,   141,     0,    75,     0,   822,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   808,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,     0,    40,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,    74,     0,    75,     0,   142,     5,     6,     7,     8,
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
       0,    70,    71,     0,     0,     0,     0,   513,     0,     0,
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   606,
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
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   609,
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
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   802,
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
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   803,
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
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   805,
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
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   806,
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
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   807,
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
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   808,
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
       0,   141,     0,    75,     0,   142,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   137,   138,     0,   139,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
     258,    37,    38,     0,     0,   259,     0,     0,     0,    40,
       0,    42,     0,     0,     0,   260,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    52,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   258,     0,
       0,     0,     0,   259,     0,     0,  -306,     0,     0,     0,
       0,     0,     0,   260,   261,     0,     0,     0,   262,     0,
       0,    64,     0,     0,     0,  -306,     0,     0,     0,  -306,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
       0,     0,   263,   195,     0,     0,     0,     0,     0,     0,
       0,   141,   261,    75,  -306,   822,   262,   264,     0,   265,
     266,   267,     0,   268,   269,   270,     0,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   258,   280,   281,   282,
     263,   259,   283,   284,   285,     0,     0,     0,     0,     0,
       0,   260,     0,   872,     0,   264,     0,   265,   266,   267,
       0,   268,   269,   270,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   258,   280,   281,   282,     0,   259,
     283,   284,   285,     0,     0,     0,     0,     0,     0,   260,
     261,   909,     0,     0,   262,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   261,     0,
       0,     0,   262,   264,     0,   265,   266,   267,     0,   268,
     269,   270,     0,   271,   272,   273,   274,   275,   276,   277,
     278,   279,     0,   280,   281,   282,   263,     0,   283,   284,
     285,     0,     0,     0,     0,     0,     0,   553,     0,     0,
       0,   264,     0,   265,   266,   267,     0,   268,   269,   270,
       0,   271,   272,   273,   274,   275,   276,   277,   278,   279,
       0,   280,   281,   282,   258,     0,   283,   284,   285,   259,
       0,     0,     0,     0,     0,   845,     0,     0,     0,   260,
     336,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   337,     0,     0,     0,     0,     0,     0,   258,     0,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,   333,     0,     0,     0,   261,     0,
       0,     0,   262,     0,     0,   334,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,     0,     0,     0,
       0,   259,   338,     0,     0,     0,   263,     0,     0,     0,
       0,   260,   261,     0,     0,     0,   262,     0,     0,     0,
       0,   264,     0,   265,   266,   267,     0,   268,   269,   270,
       0,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     263,   280,   281,   282,     0,     0,   283,   284,   285,     0,
     261,     0,   309,     0,   262,   264,     0,   265,   266,   267,
       0,   268,   269,   270,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   258,   280,   281,   282,   263,   259,
     283,   284,   285,     0,     0,     0,   309,     0,     0,   260,
     635,     0,     0,   264,     0,   265,   266,   267,     0,   268,
     269,   270,     0,   271,   272,   273,   274,   275,   276,   277,
     278,   279,     0,   280,   281,   282,     0,     0,   283,   284,
     285,     0,     0,     0,   371,     0,     0,     0,   261,     0,
       0,     0,   262,     0,     0,     0,   258,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   260,   338,     0,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,   265,   266,   267,     0,   268,   269,   270,
       0,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     261,   280,   281,   282,   262,     0,   283,   284,   285,     0,
       0,   344,   309,     0,     0,     0,     0,   258,     0,     0,
       0,     0,   259,     0,     0,     0,     0,     0,   263,     0,
       0,     0,   260,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,   265,   266,   267,     0,   268,
     269,   270,     0,   271,   272,   273,   274,   275,   276,   277,
     278,   279,     0,   280,   281,   282,     0,     0,   283,   284,
     285,   261,     0,     0,   309,   262,     0,     0,     0,   258,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,   629,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,   265,   266,   267,     0,
     268,   269,   270,     0,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   261,   280,   281,   282,   262,     0,   283,
     284,   285,     0,     0,     0,   309,     0,     0,     0,     0,
     258,     0,     0,     0,     0,   259,     0,     0,     0,     0,
       0,   263,     0,     0,     0,   260,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,   265,   266,
     267,     0,   268,   269,   270,     0,   271,   272,   273,   274,
     275,   276,   277,   278,   279,     0,   280,   281,   282,     0,
       0,   283,   284,   285,   261,     0,     0,   309,   262,     0,
       0,     0,   258,     0,     0,   744,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,     0,     0,
       0,     0,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,   265,
     266,   267,     0,   268,   269,   270,     0,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   261,   280,   281,   282,
     262,     0,   283,   284,   285,     0,     0,   749,   309,     0,
       0,     0,     0,   258,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,   263,     0,     0,     0,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,   265,   266,   267,     0,   268,   269,   270,     0,   271,
     272,   273,   274,   275,   276,   277,   278,   279,     0,   280,
     281,   282,     0,     0,   283,   284,   285,   261,     0,     0,
     309,   262,     0,     0,     0,   258,     0,     0,   857,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,     0,     0,     0,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,   265,   266,   267,     0,   268,   269,   270,     0,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   261,
     280,   281,   282,   262,     0,   283,   284,   285,     0,     0,
     858,   309,     0,     0,     0,     0,   258,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,   263,     0,     0,
       0,   260,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,   265,   266,   267,     0,   268,   269,
     270,     0,   271,   272,   273,   274,   275,   276,   277,   278,
     279,     0,   280,   281,   282,     0,     0,   283,   284,   285,
     261,     0,     0,   309,   262,     0,     0,     0,   258,     0,
       0,   859,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,     0,     0,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,   265,   266,   267,     0,   268,
     269,   270,     0,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   261,   280,   281,   282,   262,     0,   283,   284,
     285,     0,     0,   860,   309,     0,     0,     0,     0,   258,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
     263,     0,     0,     0,   260,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,     0,   265,   266,   267,
       0,   268,   269,   270,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,     0,   280,   281,   282,     0,     0,
     283,   284,   285,   261,     0,     0,   309,   262,     0,     0,
       0,   258,     0,     0,   874,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,   265,   266,
     267,     0,   268,   269,   270,     0,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   261,   280,   281,   282,   262,
       0,   283,   284,   285,     0,     0,   875,   309,     0,     0,
       0,     0,   258,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,   263,     0,     0,     0,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
     265,   266,   267,     0,   268,   269,   270,     0,   271,   272,
     273,   274,   275,   276,   277,   278,   279,     0,   280,   281,
     282,     0,     0,   283,   284,   285,   261,     0,     0,   309,
     262,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,     0,     0,     0,
       0,   259,     0,     0,   263,     0,     0,     0,     0,     0,
       0,   260,     0,     0,     0,     0,   341,     0,     0,   264,
       0,   265,   266,   267,     0,   268,   269,   270,     0,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   342,   280,
     281,   282,     0,     0,   283,   284,   285,     0,     0,  -390,
     261,     0,     0,     0,   262,     0,     0,     0,   258,     0,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,     0,     0,     0,     0,   263,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,     0,   264,     0,   265,   266,   267,     0,   268,
     269,   270,     0,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   261,   280,   281,   282,   262,   343,   283,   284,
     285,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   258,     0,     0,     0,   130,   259,     0,     0,
     263,     0,     0,     0,     0,     0,     0,   260,     0,     0,
       0,     0,   384,     0,     0,   264,     0,   265,   266,   267,
       0,   268,   269,   270,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   385,   280,   281,   282,     0,     0,
     283,   284,   285,     0,     0,     0,   261,     0,     0,     0,
     262,     0,     0,     0,   258,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
     481,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,   264,
       0,   265,   266,   267,     0,   268,   269,   270,     0,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   261,   280,
     281,   282,   262,     0,   283,   284,   285,     0,     0,     0,
       0,     0,     0,     0,     0,   258,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,   263,     0,     0,     0,
     260,   483,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,   484,   265,   266,   267,     0,   268,   269,   270,
       0,   271,   272,   273,   274,   275,   276,   277,   278,   279,
       0,   280,   281,   282,     0,     0,   283,   284,   285,   261,
       0,     0,     0,   262,     0,     0,     0,   258,     0,     0,
       0,     0,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,   265,   266,   267,     0,   268,   269,
     270,     0,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   261,   280,   281,   282,   262,     0,   283,   284,   285,
       0,     0,     0,     0,     0,     0,     0,     0,   258,     0,
       0,     0,     0,   259,     0,     0,     0,     0,     0,   263,
       0,     0,     0,   260,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,   265,   266,   267,     0,
     268,   269,   270,     0,   271,   272,   273,   274,   275,   276,
     277,   278,   279,     0,   280,   281,   282,     0,   319,   283,
     284,   285,   261,     0,     0,     0,   262,     0,     0,     0,
     258,     0,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,     0,   265,   266,   267,
       0,   268,   269,   270,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   261,   280,   281,   282,   262,   375,
     283,   284,   285,     0,     0,     0,     0,     0,     0,     0,
       0,   258,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,   263,     0,     0,     0,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,   265,
     266,   267,     0,   268,   269,   270,     0,   271,   272,   273,
     274,   275,   276,   277,   278,   279,     0,   280,   281,   282,
       0,   377,   283,   284,   285,   261,     0,     0,     0,   262,
       0,     0,     0,   258,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
     265,   266,   267,     0,   268,   269,   270,     0,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   261,   280,   281,
     282,   262,   379,   283,   284,   285,     0,     0,     0,     0,
       0,     0,     0,     0,   258,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,   263,     0,     0,     0,   260,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,   265,   266,   267,     0,   268,   269,   270,     0,
     271,   272,   273,   274,   275,   276,   277,   278,   279,     0,
     280,   281,   282,     0,   387,   283,   284,   285,   261,     0,
       0,     0,   262,     0,     0,     0,   258,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   260,     0,     0,     0,   401,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,   265,   266,   267,     0,   268,   269,   270,
       0,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     261,   280,   281,   282,   262,     0,   283,   284,   285,     0,
       0,   485,     0,     0,     0,     0,     0,   258,     0,     0,
       0,     0,   259,     0,     0,     0,     0,     0,   263,     0,
       0,     0,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,   265,   266,   267,     0,   268,
     269,   270,     0,   271,   272,   273,   274,   275,   276,   277,
     278,   279,     0,   280,   281,   282,     0,     0,   283,   284,
     285,   261,     0,     0,     0,   262,     0,     0,     0,   258,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,   130,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,   265,   266,   267,     0,
     268,   269,   270,     0,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   261,   280,   281,   282,   262,     0,   283,
     284,   285,     0,     0,     0,     0,     0,     0,     0,     0,
     258,   642,     0,     0,     0,   259,     0,     0,     0,     0,
       0,   263,     0,     0,     0,   260,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,   265,   266,
     267,     0,   268,   269,   270,     0,   271,   272,   273,   274,
     275,   276,   277,   278,   279,     0,   280,   281,   282,     0,
     616,   283,   284,   285,   261,     0,     0,     0,   262,     0,
       0,     0,   258,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,     0,     0,
     645,     0,   263,     0,     0,     0,     0,     0,     0,     0,
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
       0,   262,     0,     0,     0,   258,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,   732,     0,     0,   679,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,   265,   266,   267,     0,   268,   269,   270,     0,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   261,
     280,   281,   282,   262,     0,   283,   284,   285,     0,     0,
       0,     0,     0,     0,     0,     0,   258,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,   263,     0,     0,
       0,   260,   735,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,   265,   266,   267,     0,   268,   269,
     270,     0,   271,   272,   273,   274,   275,   276,   277,   278,
     279,     0,   280,   281,   282,     0,     0,   283,   284,   285,
     261,     0,     0,     0,   262,     0,     0,     0,   258,     0,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,     0,     0,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,   265,   266,   267,     0,   268,
     269,   270,     0,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   261,   280,   281,   282,   262,     0,   283,   284,
     285,     0,     0,   801,     0,     0,     0,     0,     0,   258,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
     263,     0,     0,     0,   260,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,     0,   265,   266,   267,
       0,   268,   269,   270,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,     0,   280,   281,   282,     0,     0,
     283,   284,   285,   261,     0,     0,     0,   262,     0,     0,
       0,   258,   817,     0,   804,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,   265,   266,
     267,     0,   268,   269,   270,     0,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   261,   280,   281,   282,   262,
       0,   283,   284,   285,     0,     0,     0,     0,     0,     0,
       0,     0,   258,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,   263,     0,     0,     0,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
     265,   266,   267,     0,   268,   269,   270,     0,   271,   272,
     273,   274,   275,   276,   277,   278,   279,     0,   280,   281,
     282,     0,     0,   283,   284,   285,   261,     0,     0,     0,
     262,     0,     0,     0,   258,     0,     0,   881,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,   265,   266,   267,     0,   268,   269,   270,     0,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   261,   280,
     281,   282,   262,     0,   283,   284,   285,     0,     0,   882,
       0,     0,     0,     0,     0,   258,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,   263,     0,     0,     0,
     260,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,   265,   266,   267,     0,   268,   269,   270,
       0,   271,   272,   273,   274,   275,   276,   277,   278,   279,
       0,   280,   281,   282,     0,     0,   283,   284,   285,   261,
       0,     0,     0,   262,     0,     0,     0,   258,     0,     0,
     883,     0,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,   265,   266,   267,     0,   268,   269,
     270,     0,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   261,   280,   281,   282,   262,     0,   283,   284,   285,
       0,     0,   884,     0,     0,     0,     0,     0,   258,     0,
       0,     0,     0,   259,     0,     0,     0,     0,     0,   263,
       0,     0,     0,   260,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,   265,   266,   267,     0,
     268,   269,   270,     0,   271,   272,   273,   274,   275,   276,
     277,   278,   279,     0,   280,   281,   282,     0,     0,   283,
     284,   285,   261,     0,     0,     0,   262,     0,     0,     0,
     258,     0,     0,   885,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,     0,   265,   266,   267,
       0,   268,   269,   270,     0,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   261,   280,   281,   282,   262,     0,
     283,   284,   285,     0,     0,   886,     0,     0,     0,     0,
       0,   258,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,   263,     0,     0,     0,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,   265,
     266,   267,     0,   268,   269,   270,     0,   271,   272,   273,
     274,   275,   276,   277,   278,   279,     0,   280,   281,   282,
       0,     0,   283,   284,   285,   261,     0,     0,     0,   262,
       0,     0,     0,   258,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
     265,   266,   267,     0,   268,   269,   270,     0,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   604,   280,   281,
     282,   262,     0,   283,   284,   285,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,   265,   266,   267,     0,   268,   269,   270,     0,
     271,   272,   273,   274,   275,   276,   277,   278,   279,     0,
     280,   281,   282,     0,     0,   283,   284,   285
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-814))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,     2,   380,   351,   356,   511,   658,    24,   724,    11,
      43,    44,   348,     3,    16,    15,   306,    19,    20,    21,
     152,   530,     3,   681,    26,    27,    28,    29,   380,   241,
       3,   163,     1,    35,    36,    37,   681,    39,    40,   252,
     253,   254,   255,    26,    46,   175,    48,    49,    50,     3,
     863,    53,    54,    55,    56,     3,     1,    59,    60,    61,
      88,   191,    64,   342,    46,    67,    68,    69,     3,    71,
     112,    26,    74,    26,    76,    26,    78,    59,    37,   209,
       3,    37,    26,   112,    65,   898,    21,    67,    75,   112,
       1,    81,   112,    53,    76,    37,   602,   112,    67,   830,
     112,   143,    37,    37,    27,    40,   385,     3,    77,    41,
      37,     7,   141,   765,     0,   143,   131,    86,   141,    54,
      55,   141,    67,    65,    37,    85,    37,   139,    60,   131,
      65,   847,   848,    88,   136,   137,   138,   139,   140,   141,
     142,    86,   111,    17,   112,    80,   655,   792,   148,    88,
     140,   809,    25,   112,    88,   138,   112,    80,   138,   140,
     142,   112,   131,   131,   809,   138,   111,   140,    21,   138,
     157,   902,   302,   303,   176,   192,    67,     3,   112,   139,
     182,   529,    93,   138,   143,   112,   140,   138,   175,    63,
     112,    64,   140,   138,   138,    68,   409,   199,   200,   112,
     111,   112,    55,    93,   191,   140,    93,    47,    82,   143,
     862,    80,    65,   112,   112,   112,   143,   112,   112,   141,
     131,   511,   209,   112,   112,   112,   116,    80,   898,   116,
     143,   233,   233,   131,   236,    75,   140,   131,   111,   909,
     131,   114,   141,   373,   141,   118,   141,   138,   140,     3,
     756,   139,   141,     7,   141,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   112,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,     7,   794,    21,    41,    13,   140,
      64,   112,    21,    18,    68,   112,   141,   509,   510,   140,
     112,   649,   304,   305,   306,    25,    60,   309,   261,   262,
     131,   647,   602,   315,   131,   302,   303,    64,   320,   131,
      55,    68,   112,    55,    56,   140,    55,   142,    73,   331,
      65,   333,   334,    61,   336,   337,    65,   131,   131,   341,
     342,   131,   344,   131,    64,    80,   558,   559,    68,   331,
     131,   304,   305,    93,   356,    84,   358,   137,     2,   140,
     138,   142,   315,   111,   366,   131,   114,    11,   131,   140,
     118,   142,   112,   131,   131,    19,   116,    21,   380,   366,
     140,   334,   384,   385,   337,   140,   373,   142,   390,   131,
     131,   111,   770,   113,   114,   397,   398,   399,   118,   401,
     402,   141,    87,   390,   406,   125,   393,   138,   395,    53,
     140,   398,   764,   140,   134,   114,   131,   549,   770,     2,
      93,   393,   140,   395,   406,   131,     7,    47,    11,   111,
      74,   131,    93,   141,    78,   112,    19,   131,    21,    37,
      28,   112,   140,    93,   397,   141,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   798,    10,    16,   141,   756,   141,    15,   141,
      53,   112,    55,    56,   143,   138,   131,   131,    25,   481,
     482,   483,   484,   485,   486,   487,   131,   131,   488,   489,
     141,    74,   131,   131,   131,    78,   131,   131,   131,   501,
     131,   503,   131,    20,   131,   507,   493,   131,   495,   511,
     131,   863,   131,   131,    31,    32,    93,    64,   143,   521,
     872,    68,    39,   131,    41,   112,   131,   131,    28,   482,
     532,   484,   176,   112,   141,   141,     3,   131,   182,   131,
       7,    88,     9,    60,    65,    92,   898,   138,   131,    26,
     532,    26,   138,   393,    21,   395,   111,   909,   143,    65,
     107,   563,   109,   110,   111,   111,   113,   114,   115,   143,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   143,
     127,   128,   129,   114,   114,   132,   133,   134,    55,   233,
     140,    86,    37,   176,    77,   680,   143,   141,    65,   182,
     602,   141,   604,   605,   606,   384,     3,   609,   385,   667,
       7,   164,   621,    80,   217,   500,   742,    84,   620,   797,
     797,   647,    67,   798,    21,   627,   683,   629,   902,   862,
     632,   877,    77,   635,    28,   359,   818,     2,   640,   495,
     642,    86,   644,   645,    -1,    37,    11,    -1,    93,    -1,
     233,   604,   605,   493,    19,    -1,    21,    -1,    55,    -1,
     662,   663,    -1,    21,    -1,   309,    -1,   112,    65,    -1,
      -1,    -1,   674,    49,   676,    67,    -1,   679,   680,    37,
      -1,   663,    40,    80,    -1,    77,   131,    84,    53,   333,
      -1,    -1,   336,   138,    86,   535,    54,    55,    -1,    -1,
     344,    93,    -1,    -1,    -1,    -1,    -1,    65,    -1,    74,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,   720,    -1,
     112,    -1,    80,    -1,    -1,    -1,   309,    -1,    -1,   731,
     732,    -1,   734,   735,   736,   737,    -1,    -1,    -1,   131,
      -1,    -1,   744,   745,    -1,   747,   138,   749,   750,    -1,
     333,   753,    -1,   336,   756,   399,   789,   790,    37,    -1,
      -1,   344,   764,    -1,    -1,    -1,   131,   769,   770,   771,
      -1,    -1,    -1,   775,    -1,   777,    -1,   764,   780,   781,
      -1,    -1,   784,    -1,    -1,   787,   787,    -1,    67,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,    -1,    77,   801,
     802,   803,   804,   805,   806,   807,   808,    86,    -1,   185,
      20,   176,   188,    -1,    93,   817,   399,   182,   820,   821,
     822,    31,    32,    -1,   826,    -1,    -1,   829,    -1,    39,
     206,    41,   476,   112,    -1,    -1,    -1,    47,    -1,   826,
     822,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,    -1,
      60,    61,   131,    -1,    -1,   857,   858,   859,   860,   138,
      -1,   863,    -1,    -1,    -1,    -1,    -1,    -1,   233,    -1,
     872,    -1,   874,   875,    -1,    -1,   863,   521,    -1,   881,
     882,   883,   884,   885,   886,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    20,    -1,   897,   898,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    31,    32,    -1,   909,    35,    -1,
      -1,   898,    39,    -1,    41,    -1,    -1,    -1,    -1,   563,
      -1,    -1,   909,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,   575,    59,    60,    -1,    62,    63,    -1,   521,    -1,
     316,    -1,    -1,    -1,   309,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
     604,   605,    -1,    -1,   340,     2,    -1,    -1,   333,    -1,
      -1,   336,    -1,    -1,    11,    -1,    25,    -1,    -1,   344,
     563,    -1,    19,   627,    21,   629,    -1,    20,   632,    -1,
      -1,   635,    -1,    -1,     2,    -1,    -1,    -1,    31,    32,
     644,    -1,    -1,    11,   131,    -1,    39,    -1,    41,    -1,
      -1,    19,   139,    21,    47,    64,    53,    -1,   662,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,
     674,    -1,   676,    -1,   399,    -1,   680,    74,    -1,    -1,
      -1,    78,    -1,    -1,   627,    53,   629,    -1,    -1,   632,
      -1,    -1,   635,    -1,    -1,    -1,    -1,    -1,   107,    -1,
      -1,   644,   111,    -1,   113,   114,    74,    -1,    -1,   118,
      78,    25,    -1,    -1,    -1,    -1,   125,    -1,    -1,   662,
      -1,    -1,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,   674,    -1,   676,   131,    -1,    -1,   680,    -1,    -1,
     744,    -1,    -1,    -1,    17,   749,    -1,    20,    -1,    -1,
      64,    -1,    -1,    -1,    68,    -1,    29,    -1,    31,    32,
      -1,    -1,    35,   131,    -1,    -1,    39,   771,    41,   505,
     506,    -1,    -1,   509,   510,    -1,   780,    -1,    -1,   176,
     784,    -1,    -1,   787,    57,   182,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,   521,   111,    -1,   113,
     114,   744,    -1,    -1,   118,    -1,   749,    80,   176,    82,
      -1,   125,    -1,    -1,   182,    -1,    -1,    -1,   132,   133,
     134,    -1,    -1,     2,    -1,    -1,    -1,    -1,   771,    -1,
      -1,    -1,    11,    -1,    -1,    -1,   233,   780,   563,    -1,
      19,   784,    21,    -1,   787,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   857,   858,   859,   860,    -1,   131,    -1,
      -1,    -1,    -1,    -1,    -1,   233,   139,    -1,    -1,    -1,
     874,   875,    -1,    -1,    53,    -1,    -1,    -1,   614,   615,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,   624,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,   634,    78,
      -1,   637,   627,    -1,   629,    -1,    -1,   632,    -1,    -1,
     635,    -1,   309,    -1,   857,   858,   859,   860,    -1,   644,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,
      -1,   874,   875,    -1,    -1,    -1,   333,   662,    -1,   336,
      -1,   309,    -1,    -1,    -1,    -1,    -1,   344,    -1,   674,
      -1,   676,   131,    -1,    -1,   680,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   333,    -1,   107,   336,   109,
     110,   111,    -1,   113,   114,   115,   344,   117,   118,   119,
     120,    -1,   122,   123,   124,   125,    -1,   127,    -1,   129,
      -1,    -1,   132,   133,   134,    -1,     2,   176,    -1,    -1,
      -1,    -1,   399,   182,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    21,    -1,    -1,    -1,   744,
      -1,    -1,    -1,    -1,   749,    -1,    -1,    -1,    -1,    -1,
      -1,   399,    -1,    -1,    -1,    -1,    -1,   773,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   771,    53,    -1,    17,
      -1,    -1,    20,    -1,   233,   780,    -1,    -1,    -1,   784,
      -1,    29,   787,    31,    32,    -1,    -1,    35,    74,    -1,
      -1,    39,    78,    41,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    80,    -1,    82,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,   521,   131,    -1,    -1,    -1,    56,
     309,    58,   857,   858,   859,   860,    -1,    -1,    -1,    -1,
      -1,    -1,     2,    -1,    71,    72,    73,    74,    -1,   874,
     875,    11,    -1,   521,   333,    -1,    -1,   336,    -1,    19,
      -1,    21,    -1,   131,    -1,   344,   563,    -1,    -1,    -1,
     176,    -1,    -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,   563,    -1,   124,    -1,   126,
      -1,    -1,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    74,   142,    -1,    -1,    78,    -1,
     399,    -1,    -1,    -1,    -1,    -1,    -1,   233,    -1,    -1,
     627,    -1,   629,    -1,    -1,   632,    -1,    -1,   635,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,   644,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   627,
      -1,   629,    -1,    -1,   632,   662,    -1,   635,    -1,    -1,
      -1,   131,    -1,    -1,    -1,    -1,   644,   674,    -1,   676,
      -1,    -1,    -1,   680,    -1,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,   662,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   309,    -1,    -1,   674,    -1,   676,    -1,
      -1,    -1,   680,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,   182,    -1,    -1,    -1,    -1,   333,   107,    -1,
     336,   110,   111,    -1,   113,   114,    25,    -1,   344,   118,
      -1,    -1,   521,    -1,    -1,    -1,   125,   744,    -1,    -1,
      -1,     2,   749,   132,   133,   134,    -1,    -1,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      21,    35,    -1,   233,   771,    64,   744,    -1,    -1,    68,
      -1,   749,    -1,   780,   563,    49,    -1,   784,    -1,    -1,
     787,    55,    56,   399,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    53,   771,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   780,    -1,    -1,    -1,   784,    -1,   107,   787,
     109,   110,   111,    74,   113,   114,   115,    78,    -1,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
     129,    -1,    -1,   132,   133,   134,    -1,    -1,   627,   309,
     629,    -1,    -1,   632,    -1,    -1,   635,    -1,    -1,    25,
     857,   858,   859,   860,    -1,   644,    -1,    -1,    -1,    -1,
      -1,   135,    -1,   333,    -1,    -1,   336,   874,   875,    -1,
     131,    -1,    -1,   662,   344,    -1,    -1,    25,    -1,   857,
     858,   859,   860,    -1,    -1,   674,    -1,   676,    64,    -1,
      -1,   680,    68,   167,   168,   169,   874,   875,    -1,    -1,
     174,    -1,    -1,    -1,    -1,   521,    -1,    -1,    -1,    -1,
      -1,   185,    -1,    -1,   188,   176,    64,    -1,    -1,    -1,
      68,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   399,
      -1,   107,   206,   109,   110,   111,    25,   113,   114,   115,
      -1,    -1,   118,    -1,    -1,    -1,    -1,   563,   124,   125,
     224,    -1,    -1,   129,    -1,   744,   132,   133,   134,   107,
     749,   109,   110,   111,    -1,   113,   114,    -1,    -1,    -1,
     118,    -1,   233,    -1,    -1,    64,   124,   125,    -1,    68,
      -1,   129,   771,    -1,   132,   133,   134,    -1,    -1,    -1,
      -1,   780,    -1,    -1,    -1,   784,    -1,    -1,   787,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   627,    -1,   629,    -1,    -1,   632,    -1,   107,   635,
     109,   110,   111,    -1,   113,   114,    -1,    -1,   644,   118,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,   316,   132,   133,   134,   662,    -1,   309,    -1,
      -1,   521,    -1,    -1,    -1,    -1,    -1,    -1,   674,    -1,
     676,    -1,    -1,    -1,   680,    -1,   340,    -1,   857,   858,
     859,   860,   333,    -1,    -1,   336,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   874,   875,    -1,    12,    -1,
      -1,    -1,    -1,   563,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,   744,    -1,
      -1,    -1,    56,   749,    58,    -1,    -1,    -1,   399,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,   771,    -1,   627,    -1,   629,
      -1,    -1,   632,    -1,   780,   635,    -1,    -1,   784,    -1,
      -1,   787,    -1,    -1,   644,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   662,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,   674,    -1,   676,    -1,    -1,    -1,
     680,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   505,   506,    -1,    -1,   509,   510,    -1,    -1,    -1,
      -1,   857,   858,   859,   860,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,   874,   875,
     521,    18,    -1,    -1,    -1,    -1,    23,    24,    25,    -1,
      -1,   545,    -1,    -1,   744,    32,    -1,    34,    -1,   749,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      57,   771,   563,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     780,    -1,    -1,    -1,   784,    -1,    -1,   787,    -1,    -1,
      -1,    -1,   135,   136,   137,    -1,    -1,   140,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     614,   615,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     624,    -1,   626,    -1,    -1,     3,    -1,   631,    -1,    -1,
     634,    -1,    10,   637,    -1,    -1,   627,    15,   629,    -1,
      -1,   632,    -1,    -1,   635,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   644,    -1,    -1,    -1,   857,   858,   859,
     860,    -1,    -1,    -1,    -1,   152,   670,    -1,    -1,    -1,
     157,   662,    -1,    -1,   874,   875,   163,    -1,   165,    -1,
      -1,    -1,    -1,   674,    -1,   676,    -1,    -1,   175,   680,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   209,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,   129,    -1,    -1,   132,   133,   134,   135,    -1,   137,
      -1,    -1,   140,    -1,    -1,   252,   253,   254,   255,   773,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
     771,    -1,   269,    -1,    -1,    25,    -1,    -1,    -1,   780,
      -1,    -1,    -1,   784,    -1,    -1,   787,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   811,   812,    -1,
     814,   815,    -1,    -1,    -1,   302,   303,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,   830,    -1,    68,    -1,
      -1,    -1,   836,    -1,   838,    -1,   840,    -1,    -1,    -1,
     327,    -1,   329,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   351,    -1,    -1,   107,    -1,   109,
     110,   111,   359,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   373,   127,   128,   129,
      -1,    -1,   132,   133,   134,    -1,    -1,    -1,   902,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   405,    -1,
      -1,    -1,   409,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,   495,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,     3,    82,    83,    84,    -1,    -1,    87,    10,    89,
      -1,    -1,    92,    15,    -1,    -1,    -1,    -1,   525,    -1,
      -1,    -1,   529,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,   545,    -1,
      -1,    -1,   549,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,    -1,   142,    -1,    -1,    -1,    -1,    -1,   575,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,    -1,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,    -1,
      -1,   113,    -1,    -1,   621,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,   129,    -1,    -1,
     132,   133,   134,   135,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,   649,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     667,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,   699,     3,     4,     5,     6,     7,     8,    -1,
      10,    -1,    12,    13,    14,    15,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,   788,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     797,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,    -1,    -1,   138,   139,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
     130,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,   139,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,   139,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,   139,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,   139,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,   139,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,    -1,    25,    -1,   138,    -1,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      10,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    64,    36,    -1,    38,    68,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,
      -1,    -1,    -1,    92,    64,    -1,    -1,    -1,    68,    -1,
      -1,    71,    72,    73,    74,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,    92,   122,   123,   124,   125,    -1,   127,   128,
     129,    -1,    -1,   132,   133,   134,    -1,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,    -1,    25,    -1,   138,    -1,
     140,     1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    64,    36,    -1,    38,    68,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,    92,   122,   123,   124,   125,    -1,   127,    -1,
     129,    -1,    -1,   132,   133,   134,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,    -1,   142,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,   135,   136,
      -1,    -1,    -1,   140,   124,   142,   126,    -1,   128,   129,
     130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   138,    -1,
     140,    -1,   142,   143,     3,     4,     5,     6,     7,     8,
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
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,
     138,    -1,   140,    -1,   142,   143,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      37,    38,    -1,    40,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    54,    55,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,   141,   142,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,   141,   142,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,   129,    -1,   131,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    50,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    32,    33,    34,    -1,    36,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,   139,   140,    -1,   142,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
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
      -1,   138,    -1,   140,    -1,   142,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      10,    48,    49,    -1,    -1,    15,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    64,    -1,    -1,    -1,    68,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    92,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    64,   140,   141,   142,    68,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    10,   127,   128,   129,
      92,    15,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,   143,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    10,   127,   128,   129,    -1,    15,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      64,   143,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    68,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,    92,    -1,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    10,    -1,   132,   133,   134,    15,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    68,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    25,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      92,   127,   128,   129,    -1,    -1,   132,   133,   134,    -1,
      64,    -1,   138,    -1,    68,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    10,   127,   128,   129,    92,    15,
     132,   133,   134,    -1,    -1,    -1,   138,    -1,    -1,    25,
      26,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,    -1,    -1,   132,   133,
     134,    -1,    -1,    -1,   138,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      64,   127,   128,   129,    68,    -1,   132,   133,   134,    -1,
      -1,    75,   138,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,    -1,    -1,   132,   133,
     134,    64,    -1,    -1,   138,    68,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    64,   127,   128,   129,    68,    -1,   132,
     133,   134,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,    -1,
      -1,   132,   133,   134,    64,    -1,    -1,   138,    68,    -1,
      -1,    -1,    10,    -1,    -1,    75,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,    -1,   132,   133,   134,    -1,    -1,    75,   138,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,
     138,    68,    -1,    -1,    -1,    10,    -1,    -1,    75,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    64,
     127,   128,   129,    68,    -1,   132,   133,   134,    -1,    -1,
      75,   138,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      64,    -1,    -1,   138,    68,    -1,    -1,    -1,    10,    -1,
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
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    52,   127,
     128,   129,    -1,    -1,   132,   133,   134,    -1,    -1,   137,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    64,   127,   128,   129,    68,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    88,    15,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    30,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    52,   127,   128,   129,    -1,    -1,
     132,   133,   134,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    64,   127,
     128,   129,    68,    -1,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    37,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,    -1,   132,   133,   134,    64,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    64,   127,   128,   129,    68,    -1,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,    -1,   131,   132,
     133,   134,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    64,   127,   128,   129,    68,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
      -1,   131,   132,   133,   134,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    64,   127,   128,
     129,    68,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,   132,   133,   134,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      64,   127,   128,   129,    68,    -1,   132,   133,   134,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,    -1,    -1,   132,   133,
     134,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    88,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    64,   127,   128,   129,    68,    -1,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,    -1,
     131,   132,   133,   134,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      28,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    64,
     127,   128,   129,    68,    -1,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    10,    11,    -1,    75,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    64,   127,   128,   129,    68,
      -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,
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
     128,   129,    68,    -1,   132,   133,   134,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,    -1,   132,   133,   134,    64,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,
      75,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,
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
      10,    -1,    -1,    75,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    64,   127,   128,   129,    68,    -1,
     132,   133,   134,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
      -1,    -1,   132,   133,   134,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    64,   127,   128,
     129,    68,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,    -1,   132,   133,   134
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
     189,   192,   195,   196,   219,   222,   223,   240,   241,   242,
     243,   244,   245,   252,   253,   254,   255,   257,   258,   259,
     260,   261,   262,   264,   265,   266,   267,   268,   152,   255,
      88,   246,   247,   164,   165,   246,    12,    33,    34,    36,
      74,   138,   142,   192,   240,   244,   253,   254,   255,   257,
     259,   260,    80,   164,   152,   255,   152,   140,   165,     7,
     164,   166,     9,    80,   166,    55,    90,   174,   255,   255,
      20,    32,   222,   255,   255,   140,   165,   193,   140,   165,
     224,   225,    26,   156,   167,   255,   255,   255,   255,   255,
       7,   140,    32,   170,   170,   130,   215,   234,   255,    78,
      79,   131,   255,   256,   255,   167,   255,   255,    73,   140,
     152,   255,   255,   156,   163,   255,   257,   156,   163,   255,
     165,   220,   234,   255,   255,   255,   255,   255,   255,   255,
     255,   139,   150,   157,   234,    81,   116,   215,   235,   236,
     256,   234,   255,   263,    57,   152,    61,   155,   165,    41,
      60,   210,    21,    55,    65,    84,   131,   137,    10,    15,
      25,    64,    68,    92,   107,   109,   110,   111,   113,   114,
     115,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     127,   128,   129,   132,   133,   134,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     135,   136,   140,   142,    64,    68,   140,   152,   131,   138,
     156,   255,   255,   255,   234,    37,   246,   220,   131,   131,
      87,   165,   215,   237,   238,   239,   256,   138,   220,   186,
     165,   140,   167,    26,    37,   167,    26,    37,    88,   167,
     249,    30,    52,   131,    75,   156,   237,   152,   140,   204,
      81,   140,   165,   226,   227,     1,   111,   229,    37,   112,
     152,   167,   167,   237,   166,   165,   112,   131,   255,   255,
     131,   138,   167,   140,   237,   131,   178,   131,   178,   131,
      93,   221,   131,   131,    30,    52,   167,   131,   139,   150,
     112,   139,   255,   112,   141,   112,   141,    37,   112,   143,
     249,    91,   112,   143,     7,    47,   111,   187,   197,    65,
     224,   224,   224,   224,   255,   255,   255,   255,   174,   255,
     174,   255,   255,   255,   255,   255,   255,   255,    27,    80,
     165,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   237,   237,   174,   255,   174,   255,    21,
      37,    40,    54,    55,    65,    80,   208,   248,   251,   255,
     268,    26,    37,    26,    37,    75,    37,   143,   174,   255,
     167,   131,   255,    93,   141,   112,   165,   190,   191,   131,
     165,    37,   234,    36,   255,   174,   255,    36,   255,   174,
     255,   140,   167,   134,   158,   160,   255,   158,   159,   152,
     255,    28,   141,   205,   206,   207,   208,   194,   227,   112,
     141,     1,   142,   230,   241,    93,   228,   255,   225,   141,
     215,   255,   183,   237,   141,    16,   179,   230,   241,   112,
     160,   159,   139,   141,   141,   235,   141,   235,   174,   255,
     143,   152,   255,   143,   255,   143,   255,   165,   234,   138,
      21,    55,    65,    80,   199,   209,   224,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   141,   143,    37,
      65,   240,   112,   141,    64,    64,    36,   174,   255,    36,
     174,   255,   174,   255,   246,   246,   131,   215,   256,   239,
      93,   112,   139,   187,   255,   141,   255,    26,   167,    26,
     167,   255,    26,   167,   249,    26,   167,   249,   250,   251,
     112,   131,    11,   131,    28,    28,   152,   112,   141,   140,
     165,    21,    55,    65,    80,   211,   141,   227,   229,   234,
      50,   256,    53,    85,   139,   184,   141,   140,   156,   165,
     180,   220,   131,   131,   143,   249,   143,   249,   152,    91,
     138,   188,    65,   198,    10,    15,    93,    94,    95,    96,
      97,    98,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   113,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   129,   132,   133,   134,   135,   137,
     140,   165,   200,   201,   202,   240,   131,   251,   240,   240,
     255,    26,    26,   255,    26,    26,   143,   143,   167,   167,
     255,   191,   138,   167,    75,    36,   255,    36,   255,    75,
      36,   255,   167,    36,   255,   167,   112,   141,   255,   255,
     152,   255,   206,   227,   111,   233,    65,   229,   228,    37,
     143,    26,   152,   234,   180,   111,   156,    36,   152,   255,
     143,    36,   152,   255,   143,   255,   139,   157,    32,    59,
      62,   139,   151,   169,   211,   165,   255,   114,   140,   203,
     203,    75,    36,    36,    75,    36,    36,    36,    36,   188,
     255,   255,   255,   255,   255,   255,   251,    11,   141,    27,
      71,    74,   142,   215,   232,   241,   116,   217,   228,    86,
     218,   255,   230,   241,   152,   167,   141,   255,   255,   152,
     255,   152,   139,   169,   229,   141,   165,   201,   202,   205,
     255,   255,   255,   255,   255,   255,   139,    75,    75,    75,
      75,   255,   233,   143,   234,   215,   216,   255,   255,   156,
     168,   214,   143,   156,    75,    75,    77,   212,   203,   203,
     141,    75,    75,    75,    75,    75,    75,   255,   255,   255,
     255,   217,   228,   215,   231,   232,   241,    37,   143,   241,
     255,   255,   218,   255,   231,   232,   131,   213,   214,   143,
     231
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

/* Line 1821 of yacc.c  */
#line 439 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1821 of yacc.c  */
#line 444 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1821 of yacc.c  */
#line 445 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1821 of yacc.c  */
#line 452 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1821 of yacc.c  */
#line 457 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1821 of yacc.c  */
#line 458 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:

/* Line 1821 of yacc.c  */
#line 477 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:

/* Line 1821 of yacc.c  */
#line 478 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:

/* Line 1821 of yacc.c  */
#line 479 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:

/* Line 1821 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:

/* Line 1821 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:

/* Line 1821 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:

/* Line 1821 of yacc.c  */
#line 483 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 30:

/* Line 1821 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1821 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:

/* Line 1821 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1821 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:

/* Line 1821 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:

/* Line 1821 of yacc.c  */
#line 489 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:

/* Line 1821 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:

/* Line 1821 of yacc.c  */
#line 491 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:

/* Line 1821 of yacc.c  */
#line 492 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 39:

/* Line 1821 of yacc.c  */
#line 497 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (6)].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[(1) - (6)].b), (yyvsp[(2) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:

/* Line 1821 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType, (yyvsp[(6) - (7)].pblockstmt), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 41:

/* Line 1821 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 42:

/* Line 1821 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:

/* Line 1821 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:

/* Line 1821 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 45:

/* Line 1821 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 46:

/* Line 1821 of yacc.c  */
#line 524 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 47:

/* Line 1821 of yacc.c  */
#line 525 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 48:

/* Line 1821 of yacc.c  */
#line 530 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 49:

/* Line 1821 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 50:

/* Line 1821 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 51:

/* Line 1821 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 52:

/* Line 1821 of yacc.c  */
#line 546 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 53:

/* Line 1821 of yacc.c  */
#line 550 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 54:

/* Line 1821 of yacc.c  */
#line 557 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 56:

/* Line 1821 of yacc.c  */
#line 566 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 58:

/* Line 1821 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 59:

/* Line 1821 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 60:

/* Line 1821 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 61:

/* Line 1821 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 62:

/* Line 1821 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 63:

/* Line 1821 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 64:

/* Line 1821 of yacc.c  */
#line 590 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 65:

/* Line 1821 of yacc.c  */
#line 592 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 66:

/* Line 1821 of yacc.c  */
#line 594 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 67:

/* Line 1821 of yacc.c  */
#line 596 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 68:

/* Line 1821 of yacc.c  */
#line 598 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 69:

/* Line 1821 of yacc.c  */
#line 600 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 70:

/* Line 1821 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 71:

/* Line 1821 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 72:

/* Line 1821 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 73:

/* Line 1821 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 74:

/* Line 1821 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 75:

/* Line 1821 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 76:

/* Line 1821 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 77:

/* Line 1821 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 78:

/* Line 1821 of yacc.c  */
#line 620 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 80:

/* Line 1821 of yacc.c  */
#line 625 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 81:

/* Line 1821 of yacc.c  */
#line 629 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 82:

/* Line 1821 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 83:

/* Line 1821 of yacc.c  */
#line 634 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 84:

/* Line 1821 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 85:

/* Line 1821 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 86:

/* Line 1821 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 87:

/* Line 1821 of yacc.c  */
#line 644 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 89:

/* Line 1821 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 90:

/* Line 1821 of yacc.c  */
#line 647 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 97:

/* Line 1821 of yacc.c  */
#line 660 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 98:

/* Line 1821 of yacc.c  */
#line 661 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 99:

/* Line 1821 of yacc.c  */
#line 662 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 100:

/* Line 1821 of yacc.c  */
#line 663 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 101:

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
#line 682 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 103:

/* Line 1821 of yacc.c  */
#line 683 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 104:

/* Line 1821 of yacc.c  */
#line 684 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 105:

/* Line 1821 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 106:

/* Line 1821 of yacc.c  */
#line 686 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 107:

/* Line 1821 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 108:

/* Line 1821 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 109:

/* Line 1821 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 110:

/* Line 1821 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 111:

/* Line 1821 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 112:

/* Line 1821 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 113:

/* Line 1821 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 114:

/* Line 1821 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 115:

/* Line 1821 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 116:

/* Line 1821 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 117:

/* Line 1821 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 118:

/* Line 1821 of yacc.c  */
#line 699 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 119:

/* Line 1821 of yacc.c  */
#line 705 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 120:

/* Line 1821 of yacc.c  */
#line 711 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 121:

/* Line 1821 of yacc.c  */
#line 717 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 122:

/* Line 1821 of yacc.c  */
#line 723 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 123:

/* Line 1821 of yacc.c  */
#line 730 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 124:

/* Line 1821 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 125:

/* Line 1821 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 126:

/* Line 1821 of yacc.c  */
#line 744 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 127:

/* Line 1821 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 128:

/* Line 1821 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 129:

/* Line 1821 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 130:

/* Line 1821 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 131:

/* Line 1821 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 132:

/* Line 1821 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 133:

/* Line 1821 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 134:

/* Line 1821 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 135:

/* Line 1821 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 136:

/* Line 1821 of yacc.c  */
#line 762 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 137:

/* Line 1821 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 138:

/* Line 1821 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 139:

/* Line 1821 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 140:

/* Line 1821 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 141:

/* Line 1821 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 142:

/* Line 1821 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 143:

/* Line 1821 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 144:

/* Line 1821 of yacc.c  */
#line 782 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 145:

/* Line 1821 of yacc.c  */
#line 786 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 146:

/* Line 1821 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 147:

/* Line 1821 of yacc.c  */
#line 792 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 148:

/* Line 1821 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 149:

/* Line 1821 of yacc.c  */
#line 796 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 150:

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
#line 825 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 153:

/* Line 1821 of yacc.c  */
#line 830 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 154:

/* Line 1821 of yacc.c  */
#line 835 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 155:

/* Line 1821 of yacc.c  */
#line 843 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 156:

/* Line 1821 of yacc.c  */
#line 844 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 157:

/* Line 1821 of yacc.c  */
#line 849 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 158:

/* Line 1821 of yacc.c  */
#line 851 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 159:

/* Line 1821 of yacc.c  */
#line 853 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 160:

/* Line 1821 of yacc.c  */
#line 858 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 161:

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
#line 877 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 163:

/* Line 1821 of yacc.c  */
#line 881 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 164:

/* Line 1821 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 165:

/* Line 1821 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 166:

/* Line 1821 of yacc.c  */
#line 894 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 167:

/* Line 1821 of yacc.c  */
#line 899 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 168:

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
#line 924 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 170:

/* Line 1821 of yacc.c  */
#line 930 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 171:

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
#line 957 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 174:

/* Line 1821 of yacc.c  */
#line 963 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 175:

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
#line 992 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 177:

/* Line 1821 of yacc.c  */
#line 996 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 178:

/* Line 1821 of yacc.c  */
#line 1001 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 179:

/* Line 1821 of yacc.c  */
#line 1005 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 181:

/* Line 1821 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 183:

/* Line 1821 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 184:

/* Line 1821 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 185:

/* Line 1821 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 186:

/* Line 1821 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 187:

/* Line 1821 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 188:

/* Line 1821 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 189:

/* Line 1821 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 190:

/* Line 1821 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 191:

/* Line 1821 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 192:

/* Line 1821 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 193:

/* Line 1821 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 194:

/* Line 1821 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 195:

/* Line 1821 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 196:

/* Line 1821 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 197:

/* Line 1821 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 198:

/* Line 1821 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 199:

/* Line 1821 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 200:

/* Line 1821 of yacc.c  */
#line 1035 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 201:

/* Line 1821 of yacc.c  */
#line 1036 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 202:

/* Line 1821 of yacc.c  */
#line 1037 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 203:

/* Line 1821 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 204:

/* Line 1821 of yacc.c  */
#line 1039 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 205:

/* Line 1821 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 206:

/* Line 1821 of yacc.c  */
#line 1041 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 207:

/* Line 1821 of yacc.c  */
#line 1042 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 208:

/* Line 1821 of yacc.c  */
#line 1046 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 209:

/* Line 1821 of yacc.c  */
#line 1047 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 210:

/* Line 1821 of yacc.c  */
#line 1048 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 211:

/* Line 1821 of yacc.c  */
#line 1049 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 212:

/* Line 1821 of yacc.c  */
#line 1050 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 213:

/* Line 1821 of yacc.c  */
#line 1051 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 214:

/* Line 1821 of yacc.c  */
#line 1052 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 215:

/* Line 1821 of yacc.c  */
#line 1053 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 216:

/* Line 1821 of yacc.c  */
#line 1054 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 217:

/* Line 1821 of yacc.c  */
#line 1055 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 218:

/* Line 1821 of yacc.c  */
#line 1056 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 219:

/* Line 1821 of yacc.c  */
#line 1057 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 220:

/* Line 1821 of yacc.c  */
#line 1061 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 221:

/* Line 1821 of yacc.c  */
#line 1062 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 222:

/* Line 1821 of yacc.c  */
#line 1066 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 223:

/* Line 1821 of yacc.c  */
#line 1070 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 224:

/* Line 1821 of yacc.c  */
#line 1071 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 225:

/* Line 1821 of yacc.c  */
#line 1072 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 226:

/* Line 1821 of yacc.c  */
#line 1077 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 227:

/* Line 1821 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 228:

/* Line 1821 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 229:

/* Line 1821 of yacc.c  */
#line 1083 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 230:

/* Line 1821 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 231:

/* Line 1821 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 232:

/* Line 1821 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 233:

/* Line 1821 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 234:

/* Line 1821 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 235:

/* Line 1821 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 236:

/* Line 1821 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 237:

/* Line 1821 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 238:

/* Line 1821 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 239:

/* Line 1821 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 240:

/* Line 1821 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 241:

/* Line 1821 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 242:

/* Line 1821 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 243:

/* Line 1821 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 244:

/* Line 1821 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 245:

/* Line 1821 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 246:

/* Line 1821 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 247:

/* Line 1821 of yacc.c  */
#line 1113 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 248:

/* Line 1821 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 249:

/* Line 1821 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 250:

/* Line 1821 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 251:

/* Line 1821 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 252:

/* Line 1821 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 253:

/* Line 1821 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 254:

/* Line 1821 of yacc.c  */
#line 1123 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 255:

/* Line 1821 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 256:

/* Line 1821 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 257:

/* Line 1821 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 260:

/* Line 1821 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 261:

/* Line 1821 of yacc.c  */
#line 1142 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 262:

/* Line 1821 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 263:

/* Line 1821 of yacc.c  */
#line 1148 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 265:

/* Line 1821 of yacc.c  */
#line 1153 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 266:

/* Line 1821 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 267:

/* Line 1821 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 268:

/* Line 1821 of yacc.c  */
#line 1159 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 269:

/* Line 1821 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 270:

/* Line 1821 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 271:

/* Line 1821 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 272:

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 275:

/* Line 1821 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 276:

/* Line 1821 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 277:

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
#line 1244 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 282:

/* Line 1821 of yacc.c  */
#line 1253 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 283:

/* Line 1821 of yacc.c  */
#line 1254 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 284:

/* Line 1821 of yacc.c  */
#line 1255 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 286:

/* Line 1821 of yacc.c  */
#line 1261 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 287:

/* Line 1821 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 288:

/* Line 1821 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 289:

/* Line 1821 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 290:

/* Line 1821 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 291:

/* Line 1821 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 292:

/* Line 1821 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 293:

/* Line 1821 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 294:

/* Line 1821 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 295:

/* Line 1821 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 296:

/* Line 1821 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 297:

/* Line 1821 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 298:

/* Line 1821 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 299:

/* Line 1821 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 300:

/* Line 1821 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 301:

/* Line 1821 of yacc.c  */
#line 1304 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 302:

/* Line 1821 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 303:

/* Line 1821 of yacc.c  */
#line 1329 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 304:

/* Line 1821 of yacc.c  */
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

/* Line 1821 of yacc.c  */
#line 1340 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 306:

/* Line 1821 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 307:

/* Line 1821 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 308:

/* Line 1821 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 309:

/* Line 1821 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 310:

/* Line 1821 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 311:

/* Line 1821 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 312:

/* Line 1821 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 313:

/* Line 1821 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 314:

/* Line 1821 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 315:

/* Line 1821 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 316:

/* Line 1821 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 317:

/* Line 1821 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 318:

/* Line 1821 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 319:

/* Line 1821 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 320:

/* Line 1821 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 321:

/* Line 1821 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 322:

/* Line 1821 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 323:

/* Line 1821 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 324:

/* Line 1821 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 325:

/* Line 1821 of yacc.c  */
#line 1386 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 326:

/* Line 1821 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 327:

/* Line 1821 of yacc.c  */
#line 1388 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 328:

/* Line 1821 of yacc.c  */
#line 1392 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 329:

/* Line 1821 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 330:

/* Line 1821 of yacc.c  */
#line 1397 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 332:

/* Line 1821 of yacc.c  */
#line 1402 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 333:

/* Line 1821 of yacc.c  */
#line 1403 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 334:

/* Line 1821 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 335:

/* Line 1821 of yacc.c  */
#line 1408 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 336:

/* Line 1821 of yacc.c  */
#line 1409 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 337:

/* Line 1821 of yacc.c  */
#line 1410 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 338:

/* Line 1821 of yacc.c  */
#line 1414 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 344:

/* Line 1821 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 345:

/* Line 1821 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 346:

/* Line 1821 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 347:

/* Line 1821 of yacc.c  */
#line 1437 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 348:

/* Line 1821 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 349:

/* Line 1821 of yacc.c  */
#line 1441 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 350:

/* Line 1821 of yacc.c  */
#line 1443 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 351:

/* Line 1821 of yacc.c  */
#line 1448 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 352:

/* Line 1821 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 353:

/* Line 1821 of yacc.c  */
#line 1452 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 354:

/* Line 1821 of yacc.c  */
#line 1454 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 355:

/* Line 1821 of yacc.c  */
#line 1456 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 356:

/* Line 1821 of yacc.c  */
#line 1458 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 357:

/* Line 1821 of yacc.c  */
#line 1460 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 358:

/* Line 1821 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 359:

/* Line 1821 of yacc.c  */
#line 1464 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 360:

/* Line 1821 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 361:

/* Line 1821 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 362:

/* Line 1821 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 363:

/* Line 1821 of yacc.c  */
#line 1472 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 364:

/* Line 1821 of yacc.c  */
#line 1479 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 365:

/* Line 1821 of yacc.c  */
#line 1485 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 366:

/* Line 1821 of yacc.c  */
#line 1491 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 367:

/* Line 1821 of yacc.c  */
#line 1497 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 368:

/* Line 1821 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 369:

/* Line 1821 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 377:

/* Line 1821 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 378:

/* Line 1821 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 380:

/* Line 1821 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 381:

/* Line 1821 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 382:

/* Line 1821 of yacc.c  */
#line 1548 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 383:

/* Line 1821 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 384:

/* Line 1821 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 385:

/* Line 1821 of yacc.c  */
#line 1560 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 386:

/* Line 1821 of yacc.c  */
#line 1565 "chapel.ypp"
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[(1) - (2)].pt); (yyval.pIntentExpr).iVar = (yyvsp[(2) - (2)].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
    break;

  case 387:

/* Line 1821 of yacc.c  */
#line 1568 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 388:

/* Line 1821 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 390:

/* Line 1821 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1821 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 392:

/* Line 1821 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 401:

/* Line 1821 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 402:

/* Line 1821 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:

/* Line 1821 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1821 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 405:

/* Line 1821 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 406:

/* Line 1821 of yacc.c  */
#line 1611 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 407:

/* Line 1821 of yacc.c  */
#line 1615 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 408:

/* Line 1821 of yacc.c  */
#line 1616 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 409:

/* Line 1821 of yacc.c  */
#line 1617 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 416:

/* Line 1821 of yacc.c  */
#line 1639 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 417:

/* Line 1821 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 418:

/* Line 1821 of yacc.c  */
#line 1641 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 419:

/* Line 1821 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 420:

/* Line 1821 of yacc.c  */
#line 1646 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 421:

/* Line 1821 of yacc.c  */
#line 1647 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 422:

/* Line 1821 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 423:

/* Line 1821 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 424:

/* Line 1821 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 425:

/* Line 1821 of yacc.c  */
#line 1658 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 426:

/* Line 1821 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 427:

/* Line 1821 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 428:

/* Line 1821 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 429:

/* Line 1821 of yacc.c  */
#line 1665 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 430:

/* Line 1821 of yacc.c  */
#line 1666 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 431:

/* Line 1821 of yacc.c  */
#line 1667 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 432:

/* Line 1821 of yacc.c  */
#line 1668 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 433:

/* Line 1821 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 434:

/* Line 1821 of yacc.c  */
#line 1670 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 435:

/* Line 1821 of yacc.c  */
#line 1672 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 436:

/* Line 1821 of yacc.c  */
#line 1676 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 437:

/* Line 1821 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 438:

/* Line 1821 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 439:

/* Line 1821 of yacc.c  */
#line 1688 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 440:

/* Line 1821 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 441:

/* Line 1821 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 442:

/* Line 1821 of yacc.c  */
#line 1691 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 443:

/* Line 1821 of yacc.c  */
#line 1692 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 444:

/* Line 1821 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 445:

/* Line 1821 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 446:

/* Line 1821 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 447:

/* Line 1821 of yacc.c  */
#line 1696 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 448:

/* Line 1821 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 449:

/* Line 1821 of yacc.c  */
#line 1698 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 450:

/* Line 1821 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 451:

/* Line 1821 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 452:

/* Line 1821 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 453:

/* Line 1821 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 454:

/* Line 1821 of yacc.c  */
#line 1703 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 455:

/* Line 1821 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 456:

/* Line 1821 of yacc.c  */
#line 1705 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:

/* Line 1821 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 458:

/* Line 1821 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 459:

/* Line 1821 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 460:

/* Line 1821 of yacc.c  */
#line 1709 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 461:

/* Line 1821 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 462:

/* Line 1821 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 463:

/* Line 1821 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 464:

/* Line 1821 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 465:

/* Line 1821 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 466:

/* Line 1821 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 467:

/* Line 1821 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 468:

/* Line 1821 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 469:

/* Line 1821 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 470:

/* Line 1821 of yacc.c  */
#line 1725 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 471:

/* Line 1821 of yacc.c  */
#line 1726 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 472:

/* Line 1821 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 473:

/* Line 1821 of yacc.c  */
#line 1731 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 474:

/* Line 1821 of yacc.c  */
#line 1732 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 475:

/* Line 1821 of yacc.c  */
#line 1733 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 476:

/* Line 1821 of yacc.c  */
#line 1738 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 477:

/* Line 1821 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 478:

/* Line 1821 of yacc.c  */
#line 1740 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 479:

/* Line 1821 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 480:

/* Line 1821 of yacc.c  */
#line 1742 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 481:

/* Line 1821 of yacc.c  */
#line 1743 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 482:

/* Line 1821 of yacc.c  */
#line 1744 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1821 of yacc.c  */
#line 8451 "bison-chapel.cpp"
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



