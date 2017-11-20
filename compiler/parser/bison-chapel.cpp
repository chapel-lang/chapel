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
    std::vector<DefExpr*>*    pvecOfDefs;
    FnSymbol*                 pfnsymbol;
    Flag                      flag;
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif


/* Line 288 of yacc.c  */
#line 130 "chapel.ypp"

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
#line 152 "chapel.ypp"

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
#line 234 "bison-chapel.cpp"

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
#line 183 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 340 of yacc.c  */
#line 448 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 454 "bison-chapel.cpp"
/* Unqualified %code blocks.  */

/* Line 344 of yacc.c  */
#line 38 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;


/* Line 344 of yacc.c  */
#line 191 "chapel.ypp"

  #include "build.h"
  #include "CatchStmt.h"
  #include "DeferStmt.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForallStmt.h"
  #include "ForLoop.h"
  #include "misc.h"
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

    // like USR_FATAL_CONT
    setupError(__FILE__, __LINE__, 3);

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
  }



/* Line 344 of yacc.c  */
#line 535 "bison-chapel.cpp"

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
#define YYLAST   13609

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  147
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  124
/* YYNRULES -- Number of rules.  */
#define YYNRULES  495
/* YYNRULES -- Number of states.  */
#define YYNSTATES  938

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
      75,    79,    83,    86,    90,    94,    97,   100,   104,   107,
     114,   122,   130,   131,   133,   135,   136,   138,   141,   145,
     149,   151,   154,   156,   160,   164,   170,   171,   173,   175,
     177,   181,   187,   193,   197,   202,   207,   212,   217,   222,
     227,   232,   237,   242,   247,   252,   257,   262,   267,   272,
     277,   278,   280,   282,   284,   286,   289,   291,   294,   298,
     300,   302,   305,   308,   310,   312,   314,   316,   318,   320,
     324,   330,   336,   339,   342,   348,   352,   359,   366,   371,
     377,   383,   387,   391,   398,   404,   411,   417,   424,   428,
     433,   440,   448,   455,   463,   468,   474,   479,   484,   488,
     495,   501,   508,   514,   523,   531,   534,   538,   542,   545,
     548,   552,   556,   557,   560,   563,   567,   573,   575,   579,
     583,   589,   595,   596,   599,   603,   606,   610,   617,   626,
     633,   642,   644,   646,   648,   649,   652,   653,   656,   660,
     666,   672,   674,   676,   679,   683,   685,   689,   690,   691,
     700,   701,   703,   706,   709,   710,   711,   722,   726,   730,
     736,   742,   744,   748,   750,   753,   755,   757,   759,   761,
     763,   765,   767,   769,   771,   773,   775,   777,   779,   781,
     783,   785,   787,   789,   791,   793,   795,   797,   799,   801,
     803,   805,   807,   809,   811,   813,   815,   817,   819,   821,
     823,   825,   826,   830,   834,   835,   837,   841,   846,   851,
     858,   865,   866,   868,   870,   872,   874,   876,   879,   882,
     884,   886,   888,   889,   891,   893,   896,   898,   900,   902,
     904,   905,   907,   910,   912,   914,   916,   917,   919,   921,
     923,   925,   927,   930,   932,   933,   935,   938,   941,   942,
     945,   949,   954,   959,   962,   967,   968,   971,   974,   979,
     984,   989,   995,  1000,  1001,  1003,  1005,  1007,  1011,  1015,
    1021,  1023,  1025,  1029,  1031,  1034,  1038,  1039,  1042,  1045,
    1046,  1049,  1052,  1054,  1059,  1064,  1071,  1075,  1076,  1078,
    1080,  1084,  1089,  1093,  1098,  1105,  1106,  1109,  1112,  1115,
    1118,  1121,  1124,  1126,  1128,  1132,  1136,  1138,  1142,  1144,
    1146,  1148,  1152,  1156,  1157,  1159,  1161,  1165,  1169,  1173,
    1175,  1177,  1179,  1181,  1183,  1185,  1187,  1189,  1192,  1197,
    1202,  1207,  1213,  1216,  1219,  1226,  1233,  1238,  1248,  1258,
    1266,  1273,  1280,  1285,  1295,  1305,  1313,  1318,  1325,  1332,
    1342,  1352,  1359,  1361,  1363,  1365,  1367,  1369,  1371,  1373,
    1375,  1379,  1380,  1382,  1387,  1389,  1393,  1398,  1400,  1404,
    1407,  1411,  1415,  1417,  1421,  1424,  1429,  1431,  1433,  1435,
    1437,  1439,  1441,  1443,  1445,  1450,  1454,  1458,  1461,  1464,
    1466,  1469,  1472,  1474,  1476,  1478,  1480,  1482,  1484,  1486,
    1491,  1496,  1501,  1505,  1509,  1513,  1517,  1522,  1526,  1531,
    1533,  1535,  1537,  1539,  1541,  1545,  1550,  1554,  1559,  1563,
    1568,  1572,  1578,  1582,  1586,  1590,  1594,  1598,  1602,  1606,
    1610,  1614,  1618,  1622,  1626,  1630,  1634,  1638,  1642,  1646,
    1650,  1654,  1658,  1662,  1666,  1670,  1673,  1676,  1679,  1682,
    1685,  1688,  1692,  1696,  1700,  1704,  1708,  1712,  1716,  1720,
    1722,  1724,  1726,  1728,  1730,  1732
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     148,     0,    -1,   149,    -1,    -1,   149,   150,    -1,   152,
      -1,   151,   152,    -1,    57,     7,    -1,   151,    57,     7,
      -1,   153,    -1,   156,    -1,   161,    -1,   162,    -1,   169,
      -1,   163,    -1,   172,    -1,   175,    -1,   173,    -1,   182,
      -1,   176,    -1,   177,    -1,   181,    -1,   168,    -1,   247,
     131,    -1,    12,   152,    -1,    13,   248,   152,    -1,    14,
     164,   131,    -1,    18,   248,   156,    -1,    22,   164,   131,
      -1,    24,   236,   131,    -1,    42,   165,   152,    -1,    45,
     257,   167,    -1,    45,   167,    -1,    51,   257,   167,    -1,
      70,   257,   167,    -1,    70,   167,    -1,    74,   152,    -1,
      89,   257,   131,    -1,     1,   131,    -1,   154,   155,    47,
     165,   138,   139,    -1,   154,   155,    47,   165,   138,   157,
     139,    -1,   154,   155,    47,   165,   138,     1,   139,    -1,
      -1,    62,    -1,    59,    -1,    -1,    61,    -1,   138,   139,
      -1,   138,   157,   139,    -1,   138,     1,   139,    -1,   150,
      -1,   157,   150,    -1,   257,    -1,   257,    11,   257,    -1,
     158,   112,   257,    -1,   158,   112,   257,    11,   257,    -1,
      -1,   158,    -1,   134,    -1,   158,    -1,    83,   235,   131,
      -1,    83,   257,    30,   160,   131,    -1,    83,   257,    52,
     159,   131,    -1,    66,   235,   131,    -1,   259,    93,   258,
     131,    -1,   259,   104,   258,   131,    -1,   259,   101,   258,
     131,    -1,   259,   103,   258,   131,    -1,   259,    97,   258,
     131,    -1,   259,   102,   258,   131,    -1,   259,    98,   258,
     131,    -1,   259,    94,   258,   131,    -1,   259,    95,   258,
     131,    -1,   259,    96,   258,   131,    -1,   259,   106,   258,
     131,    -1,   259,   105,   258,   131,    -1,   259,   135,   258,
     131,    -1,   259,   136,   258,   131,    -1,   259,    99,   258,
     131,    -1,   259,   100,   258,   131,    -1,    -1,   165,    -1,
       3,    -1,   164,    -1,     7,    -1,    26,   152,    -1,   156,
      -1,    67,   131,    -1,    67,   258,   131,    -1,   131,    -1,
     170,    -1,    62,   170,    -1,    59,   170,    -1,   189,    -1,
     220,    -1,   185,    -1,   171,    -1,   197,    -1,   223,    -1,
      35,   257,   131,    -1,    35,   257,    30,   160,   131,    -1,
      35,   257,    52,   159,   131,    -1,    35,   223,    -1,    32,
       9,    -1,    26,   152,    87,   257,   131,    -1,    87,   257,
     167,    -1,    19,   257,    37,   257,   248,   167,    -1,    19,
     257,    37,   174,   248,   167,    -1,    19,   257,   248,   167,
      -1,    33,   257,    37,   257,   167,    -1,    33,   257,    37,
     174,   167,    -1,    33,   257,   167,    -1,    33,   174,   167,
      -1,    33,    55,   165,    37,   257,   167,    -1,    34,   257,
      37,   257,   167,    -1,    34,   257,    37,   257,   251,   167,
      -1,    34,   257,    37,   174,   167,    -1,    34,   257,    37,
     174,   251,   167,    -1,    34,   257,   167,    -1,    34,   257,
     251,   167,    -1,   142,   235,    37,   257,   143,   152,    -1,
     142,   235,    37,   257,   251,   143,   152,    -1,   142,   235,
      37,   174,   143,   152,    -1,   142,   235,    37,   174,   251,
     143,   152,    -1,   142,   235,   143,   152,    -1,   142,   235,
     251,   143,   152,    -1,    90,   140,   235,   141,    -1,    36,
     257,    75,   152,    -1,    36,   257,   156,    -1,    36,   257,
      75,   152,    28,   152,    -1,    36,   257,   156,    28,   152,
      -1,    36,   257,   203,   257,    75,   152,    -1,    36,   257,
     203,   257,   156,    -1,    36,   257,   203,   257,    75,   152,
      28,   152,    -1,    36,   257,   203,   257,   156,    28,   152,
      -1,    23,   152,    -1,    78,   257,   131,    -1,    79,   257,
     131,    -1,    78,   163,    -1,    79,   163,    -1,    78,   156,
     178,    -1,    79,   156,   178,    -1,    -1,   178,   179,    -1,
      16,   156,    -1,    16,   180,   156,    -1,    16,   140,   180,
     141,   156,    -1,   165,    -1,   165,   111,   257,    -1,    76,
     257,   131,    -1,    69,   257,   138,   183,   139,    -1,    69,
     257,   138,     1,   139,    -1,    -1,   183,   184,    -1,    85,
     235,   167,    -1,    53,   152,    -1,    53,    26,   152,    -1,
     186,   165,   187,   138,   188,   139,    -1,    32,   166,   186,
     165,   187,   138,   188,   139,    -1,   186,   165,   187,   138,
       1,   139,    -1,    32,   166,   186,   165,   187,   138,     1,
     139,    -1,    17,    -1,    63,    -1,    82,    -1,    -1,   111,
     235,    -1,    -1,   188,   169,    -1,   188,   151,   169,    -1,
     190,   165,   138,   191,   139,    -1,   190,   165,   138,     1,
     139,    -1,    29,    -1,   192,    -1,   191,   112,    -1,   191,
     112,   192,    -1,   165,    -1,   165,    93,   257,    -1,    -1,
      -1,    43,   194,   205,   195,   212,   230,   219,   215,    -1,
      -1,    39,    -1,    31,   166,    -1,    32,   166,    -1,    -1,
      -1,   196,   211,   198,   200,   199,   212,   230,   213,   219,
     214,    -1,   210,   202,   204,    -1,   210,   203,   204,    -1,
     210,   201,   114,   202,   204,    -1,   210,   201,   114,   203,
     204,    -1,   242,    -1,   140,   257,   141,    -1,   165,    -1,
     108,   165,    -1,   107,    -1,   109,    -1,   110,    -1,   108,
      -1,   117,    -1,   127,    -1,   123,    -1,   120,    -1,   122,
      -1,   119,    -1,   129,    -1,   124,    -1,   134,    -1,   113,
      -1,   132,    -1,   133,    -1,   125,    -1,   118,    -1,   126,
      -1,    15,    -1,   121,    -1,    10,    -1,   135,    -1,   137,
      -1,    93,    -1,   104,    -1,   101,    -1,   103,    -1,    97,
      -1,   102,    -1,    98,    -1,    94,    -1,    95,    -1,    96,
      -1,   106,    -1,   105,    -1,    -1,   140,   206,   141,    -1,
     140,   206,   141,    -1,    -1,   207,    -1,   206,   112,   207,
      -1,   208,   165,   234,   229,    -1,   208,   165,   234,   218,
      -1,   208,   140,   228,   141,   234,   229,    -1,   208,   140,
     228,   141,   234,   218,    -1,    -1,   209,    -1,    37,    -1,
      40,    -1,    54,    -1,    21,    -1,    21,    37,    -1,    21,
      65,    -1,    55,    -1,    65,    -1,    80,    -1,    -1,    55,
      -1,    65,    -1,    21,    65,    -1,    21,    -1,    80,    -1,
      60,    -1,    41,    -1,    -1,    21,    -1,    21,    65,    -1,
      65,    -1,    55,    -1,    80,    -1,    -1,    77,    -1,   131,
      -1,   215,    -1,   156,    -1,   168,    -1,   130,   165,    -1,
     130,    -1,    -1,   216,    -1,   116,   257,    -1,   116,   217,
      -1,    -1,    86,   257,    -1,    80,   221,   131,    -1,    20,
      80,   221,   131,    -1,    32,    80,   221,   131,    -1,   165,
     222,    -1,   165,   222,   112,   221,    -1,    -1,    93,   243,
      -1,    93,   231,    -1,   224,    55,   225,   131,    -1,   224,
      21,   225,   131,    -1,   224,    65,   225,   131,    -1,   224,
      21,    65,   225,   131,    -1,   224,    84,   225,   131,    -1,
      -1,    20,    -1,    32,    -1,   226,    -1,   225,   112,   226,
      -1,   165,   230,   229,    -1,   140,   228,   141,   230,   229,
      -1,    81,    -1,   165,    -1,   140,   228,   141,    -1,   227,
      -1,   227,   112,    -1,   227,   112,   228,    -1,    -1,    93,
      50,    -1,    93,   258,    -1,    -1,   111,   243,    -1,   111,
     231,    -1,     1,    -1,   142,   235,   143,   243,    -1,   142,
     235,   143,   231,    -1,   142,   235,    37,   257,   143,   243,
      -1,   142,     1,   143,    -1,    -1,   243,    -1,   216,    -1,
     142,   143,   232,    -1,   142,   235,   143,   232,    -1,   142,
     143,   233,    -1,   142,   235,   143,   233,    -1,   142,   235,
      37,   257,   143,   232,    -1,    -1,   111,   243,    -1,   111,
     216,    -1,   111,    27,    -1,   111,    71,    -1,   111,    74,
      -1,   111,   233,    -1,   257,    -1,   216,    -1,   235,   112,
     257,    -1,   235,   112,   216,    -1,   257,    -1,   236,   112,
     257,    -1,    81,    -1,   258,    -1,   216,    -1,   237,   112,
     237,    -1,   238,   112,   237,    -1,    -1,   240,    -1,   241,
      -1,   240,   112,   241,    -1,   165,    93,   216,    -1,   165,
      93,   258,    -1,   216,    -1,   258,    -1,   165,    -1,   246,
      -1,   259,    -1,   245,    -1,   267,    -1,   266,    -1,    71,
     257,    -1,    38,   140,   239,   141,    -1,    27,   140,   239,
     141,    -1,    73,   140,   239,   141,    -1,    72,    73,   140,
     239,   141,    -1,    12,   257,    -1,    74,   257,    -1,    33,
     257,    37,   257,    26,   257,    -1,    33,   257,    37,   174,
      26,   257,    -1,    33,   257,    26,   257,    -1,    33,   257,
      37,   257,    26,    36,   257,    75,   257,    -1,    33,   257,
      37,   174,    26,    36,   257,    75,   257,    -1,    33,   257,
      26,    36,   257,    75,   257,    -1,    34,   257,    37,   257,
      26,   257,    -1,    34,   257,    37,   174,    26,   257,    -1,
      34,   257,    26,   257,    -1,    34,   257,    37,   257,    26,
      36,   257,    75,   257,    -1,    34,   257,    37,   174,    26,
      36,   257,    75,   257,    -1,    34,   257,    26,    36,   257,
      75,   257,    -1,   142,   235,   143,   257,    -1,   142,   235,
      37,   257,   143,   257,    -1,   142,   235,    37,   174,   143,
     257,    -1,   142,   235,    37,   257,   143,    36,   257,    75,
     257,    -1,   142,   235,    37,   174,   143,    36,   257,    75,
     257,    -1,    36,   257,    75,   257,    28,   257,    -1,    49,
      -1,   246,    -1,   242,    -1,   262,    -1,   261,    -1,   193,
      -1,   255,    -1,   256,    -1,   254,   137,   257,    -1,    -1,
     249,    -1,    88,   140,   250,   141,    -1,   253,    -1,   250,
     112,   253,    -1,    88,   140,   252,   141,    -1,   253,    -1,
     252,   112,   253,    -1,   209,   242,    -1,   257,    64,   242,
      -1,   270,    64,   242,    -1,   259,    -1,   254,   137,   257,
      -1,    48,   257,    -1,    44,   225,    37,   257,    -1,   264,
      -1,   243,    -1,   244,    -1,   268,    -1,   269,    -1,   193,
      -1,   255,    -1,   256,    -1,   140,   116,   257,   141,    -1,
     257,   111,   257,    -1,   257,   115,   257,    -1,   257,   115,
      -1,   115,   257,    -1,   115,    -1,    78,   257,    -1,    79,
     257,    -1,   257,    -1,   242,    -1,   261,    -1,   262,    -1,
     263,    -1,   259,    -1,   193,    -1,   260,   140,   239,   141,
      -1,   260,   142,   239,   143,    -1,    58,   140,   239,   141,
      -1,   257,   114,   165,    -1,   257,   114,    80,    -1,   257,
     114,    27,    -1,   140,   237,   141,    -1,   140,   237,   112,
     141,    -1,   140,   238,   141,    -1,   140,   238,   112,   141,
      -1,     4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,
      -1,   138,   235,   139,    -1,   138,   235,   112,   139,    -1,
     142,   235,   143,    -1,   142,   235,   112,   143,    -1,   142,
     265,   143,    -1,   142,   265,   112,   143,    -1,   257,    91,
     257,    -1,   265,   112,   257,    91,   257,    -1,   257,   129,
     257,    -1,   257,   124,   257,    -1,   257,   134,   257,    -1,
     257,   113,   257,    -1,   257,   132,   257,    -1,   257,   133,
     257,    -1,   257,   125,   257,    -1,   257,   117,   257,    -1,
     257,   127,   257,    -1,   257,   123,   257,    -1,   257,   120,
     257,    -1,   257,   122,   257,    -1,   257,   119,   257,    -1,
     257,   107,   257,    -1,   257,   109,   257,    -1,   257,   110,
     257,    -1,   257,    92,   257,    -1,   257,   128,   257,    -1,
     257,   118,   257,    -1,   257,    15,   257,    -1,   257,    10,
     257,    -1,   257,   121,   257,    -1,   257,    25,   257,    -1,
     129,   257,    -1,   124,   257,    -1,    46,   257,    -1,    56,
     257,    -1,   126,   257,    -1,   108,   257,    -1,   257,    64,
     257,    -1,   257,    64,   174,    -1,   270,    64,   257,    -1,
     270,    64,   174,    -1,   257,    68,   257,    -1,   257,    68,
     174,    -1,   270,    68,   257,    -1,   270,    68,   174,    -1,
     129,    -1,   134,    -1,    92,    -1,   128,    -1,   107,    -1,
     109,    -1,   110,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   441,   441,   446,   447,   453,   454,   459,   460,   465,
     466,   467,   468,   469,   470,   471,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   498,
     501,   503,   509,   510,   511,   515,   516,   529,   530,   531,
     536,   537,   542,   547,   552,   556,   563,   568,   572,   577,
     581,   582,   583,   587,   591,   593,   595,   597,   599,   601,
     603,   605,   607,   609,   611,   613,   615,   617,   619,   621,
     626,   627,   631,   635,   636,   640,   641,   645,   646,   650,
     651,   652,   653,   654,   655,   656,   657,   661,   662,   666,
     667,   668,   669,   673,   688,   689,   690,   691,   692,   693,
     694,   695,   696,   697,   698,   699,   700,   701,   702,   703,
     704,   710,   716,   722,   728,   735,   745,   749,   750,   751,
     752,   753,   755,   757,   759,   764,   767,   768,   769,   770,
     771,   772,   776,   777,   781,   782,   783,   787,   788,   792,
     795,   797,   802,   803,   807,   809,   811,   818,   828,   838,
     848,   861,   866,   871,   879,   880,   885,   886,   888,   893,
     909,   916,   925,   933,   937,   944,   945,   950,   955,   949,
     980,   986,   993,  1001,  1013,  1019,  1012,  1047,  1051,  1056,
    1060,  1068,  1069,  1073,  1074,  1075,  1076,  1077,  1078,  1079,
    1080,  1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,
    1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,  1102,
    1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,
    1113,  1117,  1118,  1122,  1126,  1127,  1128,  1132,  1134,  1136,
    1138,  1143,  1144,  1148,  1149,  1150,  1151,  1152,  1153,  1154,
    1155,  1156,  1160,  1161,  1162,  1163,  1164,  1165,  1169,  1170,
    1174,  1175,  1176,  1177,  1178,  1179,  1183,  1184,  1187,  1188,
    1192,  1193,  1197,  1199,  1204,  1205,  1209,  1210,  1214,  1215,
    1219,  1221,  1223,  1228,  1241,  1258,  1259,  1261,  1266,  1274,
    1282,  1290,  1299,  1309,  1310,  1311,  1315,  1316,  1324,  1326,
    1331,  1333,  1335,  1340,  1342,  1344,  1351,  1352,  1353,  1357,
    1358,  1359,  1360,  1380,  1384,  1388,  1396,  1403,  1404,  1405,
    1409,  1411,  1417,  1419,  1421,  1426,  1427,  1428,  1429,  1430,
    1431,  1432,  1438,  1439,  1440,  1441,  1445,  1446,  1450,  1451,
    1452,  1456,  1457,  1461,  1462,  1466,  1467,  1471,  1472,  1473,
    1474,  1478,  1489,  1490,  1491,  1492,  1493,  1494,  1496,  1498,
    1500,  1502,  1504,  1506,  1511,  1513,  1515,  1517,  1519,  1521,
    1523,  1525,  1527,  1529,  1531,  1533,  1535,  1542,  1548,  1554,
    1560,  1569,  1579,  1587,  1588,  1589,  1590,  1591,  1592,  1593,
    1594,  1599,  1600,  1604,  1608,  1611,  1616,  1620,  1623,  1628,
    1631,  1634,  1640,  1641,  1646,  1651,  1659,  1660,  1661,  1662,
    1663,  1664,  1665,  1666,  1667,  1669,  1671,  1673,  1675,  1677,
    1682,  1683,  1684,  1687,  1688,  1689,  1690,  1701,  1702,  1706,
    1707,  1708,  1712,  1713,  1714,  1722,  1723,  1724,  1725,  1729,
    1730,  1731,  1732,  1733,  1734,  1735,  1736,  1737,  1738,  1742,
    1750,  1751,  1755,  1756,  1757,  1758,  1759,  1760,  1761,  1762,
    1763,  1764,  1765,  1766,  1767,  1768,  1769,  1770,  1771,  1772,
    1773,  1774,  1775,  1776,  1777,  1781,  1782,  1783,  1784,  1785,
    1786,  1790,  1791,  1792,  1793,  1797,  1798,  1799,  1800,  1805,
    1806,  1807,  1808,  1809,  1810,  1811
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
  "enum_decl_stmt", "enum_header", "enum_ls", "enum_item",
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
     153,   153,   154,   154,   154,   155,   155,   156,   156,   156,
     157,   157,   158,   158,   158,   158,   159,   159,   160,   160,
     161,   161,   161,   162,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     164,   164,   165,   166,   166,   167,   167,   168,   168,   169,
     169,   169,   169,   169,   169,   169,   169,   170,   170,   171,
     171,   171,   171,   172,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   174,   175,   175,   175,
     175,   175,   175,   175,   175,   176,   177,   177,   177,   177,
     177,   177,   178,   178,   179,   179,   179,   180,   180,   181,
     182,   182,   183,   183,   184,   184,   184,   185,   185,   185,
     185,   186,   186,   186,   187,   187,   188,   188,   188,   189,
     189,   190,   191,   191,   191,   192,   192,   194,   195,   193,
     196,   196,   196,   196,   198,   199,   197,   200,   200,   200,
     200,   201,   201,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   204,   204,   205,   206,   206,   206,   207,   207,   207,
     207,   208,   208,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   210,   210,   210,   210,   210,   210,   211,   211,
     212,   212,   212,   212,   212,   212,   213,   213,   214,   214,
     215,   215,   216,   216,   217,   217,   218,   218,   219,   219,
     220,   220,   220,   221,   221,   222,   222,   222,   223,   223,
     223,   223,   223,   224,   224,   224,   225,   225,   226,   226,
     227,   227,   227,   228,   228,   228,   229,   229,   229,   230,
     230,   230,   230,   231,   231,   231,   231,   232,   232,   232,
     233,   233,   233,   233,   233,   234,   234,   234,   234,   234,
     234,   234,   235,   235,   235,   235,   236,   236,   237,   237,
     237,   238,   238,   239,   239,   240,   240,   241,   241,   241,
     241,   242,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   245,   246,   247,   247,   247,   247,   247,   247,   247,
     247,   248,   248,   249,   250,   250,   251,   252,   252,   253,
     253,   253,   254,   254,   255,   256,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     258,   258,   258,   259,   259,   259,   259,   260,   260,   261,
     261,   261,   262,   262,   262,   263,   263,   263,   263,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     265,   265,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   267,   267,   267,   267,   267,
     267,   268,   268,   268,   268,   269,   269,   269,   269,   270,
     270,   270,   270,   270,   270,   270
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     2,     2,     3,     2,     6,
       7,     7,     0,     1,     1,     0,     1,     2,     3,     3,
       1,     2,     1,     3,     3,     5,     0,     1,     1,     1,
       3,     5,     5,     3,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       0,     1,     1,     1,     1,     2,     1,     2,     3,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     3,
       5,     5,     2,     2,     5,     3,     6,     6,     4,     5,
       5,     3,     3,     6,     5,     6,     5,     6,     3,     4,
       6,     7,     6,     7,     4,     5,     4,     4,     3,     6,
       5,     6,     5,     8,     7,     2,     3,     3,     2,     2,
       3,     3,     0,     2,     2,     3,     5,     1,     3,     3,
       5,     5,     0,     2,     3,     2,     3,     6,     8,     6,
       8,     1,     1,     1,     0,     2,     0,     2,     3,     5,
       5,     1,     1,     2,     3,     1,     3,     0,     0,     8,
       0,     1,     2,     2,     0,     0,    10,     3,     3,     5,
       5,     1,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     0,     1,     3,     4,     4,     6,
       6,     0,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       0,     1,     2,     1,     1,     1,     0,     1,     1,     1,
       1,     1,     2,     1,     0,     1,     2,     2,     0,     2,
       3,     4,     4,     2,     4,     0,     2,     2,     4,     4,
       4,     5,     4,     0,     1,     1,     1,     3,     3,     5,
       1,     1,     3,     1,     2,     3,     0,     2,     2,     0,
       2,     2,     1,     4,     4,     6,     3,     0,     1,     1,
       3,     4,     3,     4,     6,     0,     2,     2,     2,     2,
       2,     2,     1,     1,     3,     3,     1,     3,     1,     1,
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
       3,     0,     0,     1,     0,    82,   439,   440,   441,   442,
     443,     0,   391,    80,   161,   391,     0,   294,    80,     0,
       0,     0,     0,   171,    80,    80,     0,     0,   293,     0,
       0,   181,     0,   177,     0,     0,     0,     0,   382,     0,
       0,     0,     0,   293,   293,   162,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   163,     0,
       0,     0,   491,   493,     0,   494,   495,   419,     0,     0,
     492,   489,    89,   490,     0,     0,     0,     4,     0,     5,
       9,    45,    10,    11,    12,    14,   351,    22,    13,    90,
      96,    15,    17,    16,    19,    20,    21,    18,    95,     0,
      93,     0,   411,     0,    97,    94,    98,     0,   423,   407,
     408,   354,   352,     0,     0,   412,   413,     0,   353,     0,
     424,   425,   426,   406,   356,   355,   409,   410,     0,    38,
      24,   362,     0,     0,   392,     0,    81,     0,     0,     0,
       0,     0,     0,     0,     0,   411,   423,   352,   412,   413,
     391,   353,   424,   425,     0,     0,   135,     0,   336,     0,
     343,    84,    83,   182,   103,     0,   183,     0,     0,     0,
       0,     0,   294,   295,   102,     0,     0,   343,     0,     0,
       0,     0,     0,   296,     0,    86,    32,     0,   477,   404,
       0,   478,     7,   343,   295,    92,    91,   273,   333,     0,
     332,     0,     0,    87,   422,     0,     0,    35,     0,   357,
       0,   343,    36,   363,     0,   142,   138,     0,   353,   142,
     139,     0,   285,     0,     0,   332,     0,     0,   480,   418,
     476,   479,   475,     0,    47,    50,     0,     0,   338,     0,
     340,     0,     0,   339,     0,   332,     0,     0,     6,    46,
       0,   164,     0,   259,   258,   184,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   417,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   343,     0,     0,
       0,    25,    26,     0,    27,     0,     0,     0,     0,     0,
       0,     0,    28,     0,    29,     0,   351,   349,     0,   344,
     345,   350,     0,     0,     0,     0,   112,     0,     0,   111,
       0,     0,     0,   118,     0,     0,    56,    99,     0,   219,
     226,   227,   228,   223,   225,   221,   224,   222,   220,   230,
     229,   128,     0,     0,    30,   234,   178,   300,     0,   301,
     303,     0,   312,     0,   306,     0,     0,    85,    31,    33,
       0,   183,   272,     0,    63,   420,   421,    88,     0,    34,
     343,     0,   149,   140,   136,   141,   137,     0,   283,   280,
      60,     0,    56,   105,    37,    49,    48,    51,     0,   444,
       0,     0,   435,     0,   437,     0,     0,     0,     0,     0,
       0,   448,     8,     0,     0,     0,     0,   252,     0,     0,
       0,     0,     0,   390,   472,   471,   474,   482,   481,   486,
     485,   468,   465,   466,   467,   415,   455,   434,   433,   432,
     416,   459,   470,   464,   462,   473,   463,   461,   453,   458,
     460,   469,   452,   456,   457,   454,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   484,   483,   488,   487,   246,   243,
     244,   245,   249,   250,   251,     0,     0,   394,     0,     0,
       0,     0,     0,     0,     0,     0,   446,   391,   391,   108,
     281,   337,     0,     0,   359,     0,   282,   164,     0,     0,
       0,   366,     0,     0,     0,   372,     0,     0,     0,   119,
     490,    59,     0,    52,    57,     0,   127,     0,     0,     0,
     358,     0,   235,     0,   242,   260,     0,   304,     0,     0,
     311,   407,     0,   298,   405,   297,   431,   335,   334,     0,
       0,     0,   360,     0,   143,   287,   407,     0,     0,     0,
     445,   414,   436,   341,   438,   342,     0,     0,   447,   124,
     376,     0,   450,   449,     0,     0,   165,     0,     0,   175,
       0,   172,   256,   253,   254,   257,   185,     0,     0,   289,
     288,   290,   292,    64,    71,    72,    73,    68,    70,    78,
      79,    66,    69,    67,    65,    75,    74,    76,    77,   429,
     430,   247,   248,   399,     0,   393,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,   347,
     348,   346,     0,     0,   126,     0,     0,   110,     0,   109,
       0,     0,   116,     0,     0,   114,     0,     0,   397,     0,
     100,     0,   101,     0,     0,   130,     0,   132,   241,   233,
       0,   325,   261,   264,   263,   265,     0,   302,   305,   306,
       0,     0,   307,   308,   151,     0,     0,   150,   153,   361,
       0,   144,   147,     0,   284,    61,    62,     0,     0,     0,
       0,   125,     0,     0,     0,   293,   170,     0,   173,   169,
     255,   260,   216,   214,   195,   198,   196,   197,   208,   199,
     212,   204,   202,   215,   203,   201,   206,   211,   213,   200,
     205,   209,   210,   207,   217,   218,     0,   193,     0,   231,
     231,   191,   291,   395,   423,   423,     0,     0,     0,     0,
       0,     0,     0,     0,   107,   106,     0,   113,     0,     0,
     365,     0,   364,     0,     0,   371,   117,     0,   370,   115,
       0,   396,    54,    53,   129,   381,   131,     0,   236,     0,
       0,   306,   262,   278,   299,   316,     0,   446,     0,   155,
       0,     0,     0,   145,     0,   122,   378,     0,     0,   120,
     377,     0,   451,     0,    39,     0,   159,    80,   293,   293,
     157,   293,   167,   176,   174,     0,   194,     0,     0,   234,
     187,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   293,   369,     0,     0,   375,     0,     0,   398,     0,
       0,   134,   325,   328,   329,   330,     0,   327,   331,   407,
     274,   238,   237,     0,     0,     0,   314,   407,   156,   154,
       0,   148,     0,   123,     0,   121,    41,    40,   168,   266,
     192,   193,   231,   231,     0,     0,     0,     0,     0,     0,
       0,   160,   158,     0,     0,     0,     0,    55,   133,   306,
     317,     0,   275,   277,   276,   279,   270,   271,   179,     0,
     146,     0,     0,   267,   278,   189,   190,   232,     0,     0,
       0,     0,     0,     0,   368,   367,   374,   373,   240,   239,
     319,   320,   322,   407,     0,   446,   407,   380,   379,     0,
       0,   321,   323,   268,   186,   269,   317,   324
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   235,    78,   536,    80,    81,   250,    82,
     236,   531,   535,   532,    83,    84,    85,   162,    86,   166,
     186,    87,    88,    89,    90,    91,    92,   634,    93,    94,
      95,   393,   564,   693,    96,    97,   560,   688,    98,    99,
     425,   705,   100,   101,   590,   591,   145,   179,   545,   103,
     104,   427,   711,   596,   738,   739,   362,   820,   366,   541,
     542,   543,   495,   597,   255,   676,   904,   934,   898,   198,
     893,   851,   854,   105,   223,   398,   106,   107,   182,   183,
     370,   371,   553,   374,   550,   921,   848,   781,   237,   157,
     241,   242,   328,   329,   330,   146,   109,   110,   111,   147,
     113,   133,   134,   496,   344,   657,   497,   114,   148,   149,
     204,   331,   151,   119,   152,   153,   122,   123,   246,   124,
     125,   126,   127,   128
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -775
static const yytype_int16 yypact[] =
{
    -775,   100,  2520,  -775,   -37,  -775,  -775,  -775,  -775,  -775,
    -775,  3705,    75,   167,  -775,    75,  8048,   108,   167,  3705,
    8048,  3705,    37,  -775,   297,   923,  6648,  8048,  6788,  8048,
      78,  -775,   167,  -775,    61,  7208,  8048,  8048,  -775,  8048,
    8048,   236,   148,  1191,  1234,  -775,  7348,  6228,  8048,  7208,
    8048,   224,   163,  3705,  8048,  8188,  8188,   167,  -775,  7348,
    8048,  8048,  -775,  -775,  8048,  -775,  -775,  9728,  8048,  8048,
    -775,  8048,  -775,  -775,  2865,  5808,  7348,  -775,  3565,  -775,
    -775,   301,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,
    -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,   167,
    -775,   167,   256,    45,  -775,  -775,  -775,    49,   239,  -775,
    -775,  -775,   244,   248,   175,   249,   261, 13404,  2079,   290,
     276,   279,  -775,  -775,  -775,  -775,  -775,  -775,    48,  -775,
    -775, 13404,   227,  3705,  -775,   326,  -775,   314,  8048,  8048,
    8048,  8048,  8048,  7348,  7348,   306,  -775,  -775,  -775,  -775,
   11672,   313,  -775,  -775,   167,   328,  -775,   124, 13404,   357,
    6368,  -775,  -775,  -775,  -775,   167,   149,   167,   323,    47,
   11137, 11066,  -775,  -775,  -775, 11610, 10456,  6368,  3705,   334,
      29,   293,    13,  -775,  3705,  -775,  -775, 11270,   613,   355,
   11270,   613,  -775,  6368,   172,  -775,  -775,   167,  -775,   126,
   13404,  8048,  8048,  -775, 13404,   346, 11465,  -775, 11270, 13404,
     340,  6368,  -775, 13404, 11941,  -775,  -775, 12012,  2372,  -775,
    -775, 12074,   389,   353,   214, 11746, 11270, 12145,   365,  1337,
     613,   365,   613,   -36,  -775,  -775,  3005,   107,  -775,  8048,
    -775,   -21,   110,  -775,    98, 12207,    97,   478,  -775,  -775,
     440,   375,   351,  -775,  -775,  -775,    52,    61,    61,    61,
    -775,  8048,  8048,  8048,  8048,  7488,  7488,  8048,  8048,  8048,
    8048,  8048,  8048,    94,  9728,  8048,  8048,  8048,  8048,  8048,
    8048,  8048,  8048,  8048,  8048,  8048,  8048,  8048,  8048,  8048,
    6928,  6928,  6928,  6928,  6928,  6928,  6928,  6928,  6928,  6928,
    6928,  6928,  6928,  6928,  6928,  6928,  6368,  6368,  7488,  7488,
    5668,  -775,  -775,  3145,  -775, 11808, 11879, 12278,    38,  7488,
      47,   360,  -775,  8048,  -775,  8048,   400,  -775,   364,   384,
    -775,  -775,   366,   167,   469,  7348,  -775,  3845,  7488,  -775,
    3985,  7488,   367,  -775,    47,  8328,  8048,  -775,  3705,  -775,
    -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,
    -775,   481,  8048,   369,  -775,   311,  -775,  -775,    29,  -775,
     407,   380,  -775,  8468,   430,  8048,    61,  -775,  -775,  -775,
     386,  -775,  -775,  7348,  -775, 13404, 13404,  -775,    28,  -775,
    6368,   387,  -775,   515,  -775,   515,  -775,  8468,   413,  -775,
    -775,  8328,  8048,  -775,  -775,  -775,  -775,  -775,  7068,  -775,
   10335,  5948,  -775,  6088,  -775,  7488,  5245,  2671,   391,  8048,
    5527,  -775,  -775,   167,  7348,   394,   463,   329,    61,   222,
     225,   230,   251, 11536,  2215,  2215,   324,  -775,   324,  -775,
     324,  5216,  1515,   997,   486,   355,   365,  -775,  -775,  -775,
    1337,  5357,   324,   456,   456,  2215,   456,   456,  1484,   365,
    5357,  5076,  1484,   613,   613,   365,   404,   406,   409,   410,
     412,   414,   417,   418,   420,   422,   424,   425,   426,   427,
     428,   429,   403,   419,  -775,   324,  -775,   324,   255,  -775,
    -775,  -775,  -775,  -775,  -775,   167,   113,  -775, 13475,   321,
    8608,  7488,  8748,  7488,  8048,  7488,  2389,    75, 12340,  -775,
    -775, 13404, 12411,  6368,  -775,  6368,  -775,   375,  8048,   117,
    8048, 13404,    59, 11332,  8048, 13404,    35, 11199,  5668,  -775,
     442,   465,   447, 12473,   465,   451,   555, 12544,  3705, 11403,
    -775,   130,  -775,    68,  -775,   356,   443,    29,   293,  5105,
    -775,   475,  6508,  -775, 13404,  -775,  -775,  -775, 13404,   452,
      26,   457,  -775,    53,  -775,  -775,   252,   167,   461,   470,
    -775,  -775,  -775,  -775,  -775,  -775,    21, 10143,  -775,  -775,
   13404,  3705, 13404,  -775, 12606,   467,   491,    67,   471,   516,
     190,  -775,   543,  -775,  -775,  -775,  -775,  9973,   257,  -775,
    -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,
    -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,
    -775,  -775,  -775,  -775,  5668,  -775,  7488,  7488,  8048,   586,
   12677,  8048,   588, 12739,   472, 10181,    47,    47,  -775,  -775,
    -775,  -775,   489, 11270,  -775, 10518,  4125,  -775,  4265,  -775,
   10593,  4405,  -775,    47,  4545,  -775,    47,   177,  -775,  8048,
    -775,  8048,  -775,  3705,  8048,  -775,  3705,   600,   611,  -775,
      29,   505,   565,  -775,  -775,  -775,    92,  -775,  -775,   430,
     490,    46,  -775,  -775,  -775,  4685,  7348,  -775,  -775,  -775,
     167,  -775,   524,   314,  -775,  -775,  -775,  4825,   494,  4965,
     496,  -775,  8048,  3285,   502,   833,  -775,  8048,   167,  -775,
    -775,   356,  -775,  -775,  -775,   167,  -775,  -775,  -775,  -775,
    -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,
    -775,  -775,  -775,  -775,  -775,  -775,  8048,   529,   536,   514,
     514,  -775,  -775,  -775,   181,   186, 12810,  8888,  9028, 12872,
    9168,  9308,  9448,  9588,  -775,  -775,  1328,  -775,  3705,  8048,
   13404,  8048, 13404,  3705,  8048, 13404,  -775,  8048, 13404,  -775,
    5668,  -775, 12943, 13404,  -775, 13404,   628,  3705,  -775,   517,
    7628,   140,  -775,   574,  -775,  -775,  7488,  9923,  3705,  -775,
      56,   522,  8048,  -775,  8048,  -775, 13404,  3705,  8048,  -775,
   13404,  3705, 13404,   170,  -775,  3425,  -775,   995,   185,   185,
    -775,   408,  -775, 13404,  -775,    76,  -775, 10373, 10022,   311,
    -775,  -775,  8048,  8048,  8048,  8048,  8048,  8048,  8048,  8048,
     528,  1124, 12544, 10655, 10730, 12544, 10792, 10867,  -775,  8048,
    3705,  -775,   505,    37,  8048,  8048,  5386,  -775,  -775,   262,
    7348,  -775,  -775,  8048,    22, 10219,  -775,   509,   357,  -775,
     314, 13404, 10929,  -775, 11004,  -775,  -775,  -775,  -775,   591,
    -775,  -775,   514,   514,   187, 13005, 13076, 13138, 13209, 13271,
   13342,  -775,  -775,  3705,  3705,  3705,  3705, 13404,  -775,   140,
    7768,   109,  -775,  -775, 13404, 13404,  -775,  -775,  -775,  9588,
    -775,  3705,  3705,  -775,   574,  -775,  -775,  -775,  8048,  8048,
    8048,  8048,  8048,  8048, 12544, 12544, 12544, 12544,  -775,  -775,
    -775,  -775,  -775,   304,  7488,  9807,   721, 12544, 12544,    23,
   10297,  -775,  -775,  -775,  -775,  -775,  7908,  -775
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -775,  -775,  -775,    -1,  -660,  1705,  -775,  -775,  -775,  1444,
     -34,  -254,   269,   271,  -775,  -775,   194,   226,  2058,    -4,
    1551,  -774,  -627,   -39,  -775,  -775,  -775,    93,  -775,  -775,
    -775,   454,  -775,   -12,  -775,  -775,  -775,  -775,  -775,   508,
     162,   -76,  -775,  -775,  -775,   -26,   634,  -775,  -775,  -775,
    -775,  -775,  -775,  -775,  -775,  -133,  -591,  -677,  -775,  -131,
      18,  -775,  -358,  -775,  -775,   -22,  -775,  -775,  -239,   -59,
    -775,  -197,  -211,  -775,  -144,  -775,   666,  -775,  -217,   320,
    -775,  -345,  -666,  -520,  -394,  -753,  -710,  -143,   -16,  -775,
      62,  -775,  -162,  -775,   183,   294,  -361,  -775,  -775,   922,
    -775,     7,  -775,  -775,  -236,  -775,  -517,  -775,   990,  1150,
      -2,   734,   439,  -775,  1218,  1363,  -775,  -775,  -775,  -775,
    -775,  -775,  -775,  -308
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -447
static const yytype_int16 yytable[] =
{
     117,    77,   499,   565,   195,   196,   740,   544,   418,   131,
     321,   658,   551,   784,   150,   363,   240,   117,   158,   117,
     163,   332,   137,   546,   170,   171,   175,   176,   679,   559,
     199,   380,     5,   187,   188,   189,   566,   190,   191,   429,
     430,   431,   432,   224,   200,   811,   206,   208,   209,   391,
     375,   213,   214,   217,   221,     5,     5,   225,   226,   227,
     244,   651,   228,   821,     5,   229,   230,   231,   704,   232,
     256,     5,   200,   184,   245,   505,   117,   372,   812,   685,
     897,  -152,   184,   786,  -166,   646,   253,  -166,  -166,    47,
      47,   411,   534,   372,   129,   129,  -166,     5,  -166,  -166,
       3,   327,  -166,   405,   257,   254,  -166,   743,  -166,   342,
     367,   686,   308,  -152,   258,   852,   309,   428,   327,   169,
     412,   447,  -166,   342,  -166,   376,  -166,  -166,   318,  -166,
    -166,   117,  -166,   259,   327,   415,   131,   315,   316,   317,
     213,   200,   245,  -309,   482,   483,   924,  -166,   534,  -166,
     416,  -166,   327,  -309,   933,   897,   783,   320,   416,  -309,
     313,   313,  -309,   132,   697,   687,    14,  -152,   383,   368,
       5,   811,   931,   313,   448,     5,   117,   160,  -309,   161,
     922,   506,   117,   937,   868,   313,   342,   373,   154,   787,
     381,   313,   180,   690,   313,   905,   906,   313,  -166,   385,
     386,   180,   678,   373,   812,   172,  -166,  -309,   670,   420,
     416,   598,    45,   -80,  -309,   932,    24,   194,   177,   408,
     499,   416,   413,   919,    31,   624,  -180,   873,   561,   383,
    -309,    58,   -80,   552,   117,   407,   323,   410,   383,   135,
     421,   417,   668,   192,   155,  -180,   409,   327,   327,   216,
     220,   414,   925,   838,   625,   324,   850,   384,   644,   433,
     434,   435,   436,   438,   440,   441,   442,   443,   444,   445,
     446,   669,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   193,   770,
     653,   656,   621,  -400,   372,   869,   108,   210,  -401,   668,
       5,   129,   708,   211,   161,   108,   485,   487,   498,   866,
     544,   117,   261,   108,  -241,   108,   499,   508,   771,   519,
     622,   511,  -400,   512,   557,   779,   383,  -401,   907,   709,
    -309,   327,   488,   200,   376,   521,   523,   376,   525,   527,
     698,   700,   376,   533,   533,   400,   537,   108,   489,   557,
     592,   490,   240,   599,   240,  -326,   600,   557,   437,   439,
     539,   601,   249,   376,  -286,   491,   492,   310,   108,   376,
    -384,   117,   108,   554,  -326,  -383,   493,   672,  -326,   260,
    -388,   558,   602,  -286,   593,   627,  -309,  -387,   742,   309,
     264,   494,  -389,   856,   594,   117,  -428,  -318,  -428,   533,
     533,   484,   486,  -326,   373,  -309,   558,  -386,   586,   595,
    -385,   673,   507,   577,   558,   580,  -318,   582,   584,   849,
    -318,   674,   200,   694,  -309,    14,   857,   108,    17,   265,
     306,   522,   307,   266,   526,   271,   675,    23,   273,    24,
     807,   118,   276,    28,   325,  -318,  -428,    31,  -428,  -180,
     118,  -241,   313,  -427,   639,  -427,   327,   312,   118,   322,
     118,   544,   499,   335,   588,   247,     5,   808,  -180,   273,
     809,    45,   108,   573,   365,   575,   271,   387,   108,   273,
     390,   264,   397,   276,   399,   422,   424,   423,    57,   426,
      58,   510,   118,   513,   218,   218,   515,   516,   521,   630,
     525,   633,   537,   635,   580,   514,   518,   528,   576,   538,
     540,   264,  -310,   118,   636,   637,   643,   118,   645,   547,
     265,   548,   650,   552,   266,   567,   498,   556,   562,   923,
     108,   563,   587,   681,   581,   603,   117,   604,   926,    72,
     605,   606,  -310,   607,   619,   608,  -313,   245,   609,   610,
     265,   611,  -310,   612,   266,   613,   614,   615,   616,   617,
     618,  -310,   620,   268,   923,   269,   270,   271,  -310,   272,
     273,   274,   118,   -58,   276,   923,  -313,   659,   660,   117,
     282,   283,   662,   663,   677,   286,  -313,  -310,   287,   288,
     289,   684,   695,   268,   629,  -313,   632,   271,   689,   272,
     273,   696,  -313,   383,   276,   703,  -310,   108,   710,   707,
     706,   283,   747,  -310,   750,   752,   780,   118,   287,   288,
     289,  -313,   498,   118,   438,   485,   746,   756,   777,   749,
     782,   108,   488,   785,   108,   792,   102,   797,   264,   801,
    -313,   806,   108,  -351,   760,   102,   762,  -313,   489,   765,
     818,   490,   768,   102,   819,   102,   840,   772,   842,   773,
     853,   117,   775,   860,   117,   491,   492,   881,   903,   805,
     790,   569,   568,   395,   333,   118,   493,   265,   791,   642,
     831,   266,   814,   117,   200,   872,   778,   102,   874,   815,
     935,   494,   918,   929,   174,   796,   555,   800,   641,   889,
     802,   117,     0,     0,     0,   813,     0,     0,   102,     0,
       0,   108,   102,     0,     0,     0,     0,     0,     0,   437,
     484,   847,     0,     0,   271,     0,   272,   273,     0,     0,
       0,   276,     0,     0,   817,     0,     0,     0,   283,     0,
       0,     0,     0,     0,     0,   760,   762,   289,   765,   768,
     796,   800,   118,     0,     0,     0,   832,   833,  -315,   834,
       0,   835,   836,     0,     0,   837,     0,   102,   498,   195,
     196,     0,     0,     0,     0,   117,   118,     0,   117,   118,
       0,   205,     0,     0,   855,   580,   117,   118,  -315,   623,
     861,   892,   862,     0,     0,   117,   864,     0,  -315,   117,
       0,     0,     0,   117,   407,     0,     0,  -315,     0,   243,
       0,     0,   102,     0,  -315,     0,     0,     0,   102,     0,
     832,   875,   876,   835,   877,   878,   879,   880,     0,     0,
     891,   920,   108,  -315,     0,     0,     0,   887,   117,     0,
       0,     0,   209,   213,   245,     0,     0,     0,   894,     0,
      14,   895,  -315,    17,     0,     0,   118,     0,     0,  -315,
       0,     0,    23,     0,    24,   807,   920,     0,    28,     0,
     102,     0,    31,     0,  -180,   108,     0,   920,     0,     0,
       0,   914,   915,   916,   917,     0,     0,     0,   117,     0,
      41,   741,   808,  -180,     0,   809,    45,   800,     0,   927,
     928,     0,     0,     0,     0,     0,   914,   915,   916,   917,
     927,   928,     0,    57,     0,    58,     0,     0,     0,     0,
     744,   745,   930,   580,   112,     0,     5,     0,     0,     0,
     161,     0,   164,   112,   800,     0,     0,     0,     0,     0,
     108,   112,   108,   112,  -295,   108,     0,   102,   108,     0,
       0,     0,     0,     0,     0,     0,     0,   108,     0,     0,
     108,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,   102,   810,     0,   102,   112,     0,   118,  -295,   108,
       0,     0,   102,     0,     0,     0,     0,     0,  -295,     0,
       0,   108,   115,   108,     0,     0,   112,   108,     5,     0,
     112,   115,   161,   165,     0,     0,     0,  -295,     0,   115,
       0,   115,     0,     0,     0,     0,  -295,     0,     0,     0,
     118,     0,   264,     0,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
       0,     0,     0,   115,     0,     0,     0,     0,     0,     0,
    -295,   102,   108,     0,     0,   112,     0,   108,     0,     0,
    -295,   265,     0,     0,   115,   266,     0,     0,   115,     0,
       0,   108,     0,     0,     0,   165,     0,     0,     0,  -295,
       0,     0,   108,     0,     0,   118,     0,   118,     0,     0,
     118,   108,     0,   118,     0,   108,     0,     0,     0,   108,
     112,     0,   118,     0,   268,   118,   112,   270,   271,     0,
     272,   273,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,   283,   115,   118,     0,     0,     0,     0,   287,
     288,   289,     0,     0,   108,     0,   118,     0,   118,     0,
       0,    14,   118,     0,    17,   243,     0,   243,     0,     0,
       0,     0,   116,    23,     0,    24,   807,     0,   112,    28,
       0,   116,     0,    31,     0,  -180,     0,     0,   115,   116,
       0,   116,   102,     0,   115,     0,     0,   108,   108,   108,
     108,    41,     0,   808,  -180,     0,   809,    45,     0,     0,
       0,     0,     0,     0,     0,   108,   108,   118,     0,     0,
       0,     0,   118,   116,    57,     0,    58,     0,     0,     0,
       0,   172,     0,     0,     0,   102,   118,     0,     0,     0,
     120,     0,    24,   194,   116,     0,   115,   118,   116,   120,
      31,     0,  -180,     0,     0,   112,   118,   120,   -44,   120,
     118,     0,     0,     0,   118,     0,     0,   640,     0,     0,
       0,  -180,   -44,     0,   172,    72,     0,     0,     0,   112,
       0,     0,   112,   882,     0,    24,   194,     0,     0,     0,
     112,   120,     0,    31,     0,  -180,     0,     0,     0,   118,
     102,   -43,   102,   116,     0,   102,   683,     0,   102,     0,
       0,     0,   120,     0,  -180,   -43,   120,   102,     0,     0,
     102,     0,     0,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   118,   118,   118,   118,     0,   115,   116,   830,
     115,   102,     0,   102,   116,     0,     0,   102,   115,   112,
     118,   118,     0,     0,     0,  -166,     0,     0,  -166,  -166,
       0,   120,     0,     0,     0,     0,     0,  -166,     0,  -166,
    -166,     0,   264,  -166,     0,   121,     0,  -166,     0,  -166,
       0,     0,     0,     0,   121,     0,     0,     0,     0,     0,
       0,     0,   121,  -166,   121,  -166,   116,  -166,  -166,     0,
    -166,  -166,   102,  -166,     0,     0,   120,   102,     0,     0,
       0,   265,   120,     0,     0,   266,     0,   115,  -166,     0,
    -166,   102,  -166,     0,     0,     0,   121,     0,     0,     0,
       0,     0,   102,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,     0,   102,     0,   121,     0,   102,
       0,   121,     0,     0,   268,     0,   269,   270,   271,     0,
     272,   273,     0,     0,   120,   276,     0,     0,     0,  -166,
     112,   282,   283,   116,     0,     0,   286,  -166,     0,   287,
     288,   289,     0,     0,   102,     0,     0,     0,     0,   185,
       0,     0,     0,     0,     0,     0,     0,   116,     0,     0,
     116,     0,     0,   185,     0,     0,   121,     0,   116,   215,
     219,     0,     0,   112,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,     0,   102,   102,   102,
     102,     0,     0,     0,     0,     0,     0,     0,   115,     0,
       0,   120,     0,     0,     0,   102,   102,     0,     0,     0,
     264,   121,     0,     0,     0,     0,     0,   121,   265,     0,
       0,     0,   266,     0,     0,   120,     0,     0,   120,     0,
       0,     0,     0,     0,     0,     0,   120,   116,   112,     0,
     112,   115,     0,   112,     0,     0,   112,     0,     0,   265,
       0,   314,     0,   266,     0,   112,     0,     0,   112,     0,
       0,   268,     0,   269,   270,   271,     0,   272,   273,   121,
     207,     0,   276,     0,     0,     0,     0,   112,     0,   283,
       0,     0,     0,   185,   185,   185,   287,   288,   289,   112,
     361,   112,     0,     0,     0,   112,   271,     0,   272,   273,
       0,   185,     0,   276,   185,   120,   115,     0,   115,     0,
     283,   115,     0,     0,   115,     0,     0,   287,   288,   289,
       0,     0,   185,   115,     0,     0,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     185,     0,     0,     0,     0,   115,   121,     0,     0,     0,
     112,     0,     0,     0,     0,   112,     0,   115,   116,   115,
       0,     0,     0,   115,     0,     0,     0,     0,     0,   112,
     121,     0,     0,   121,     0,     0,     0,    79,     0,     0,
     112,   121,     0,     0,     0,     0,   130,     0,     0,   112,
     336,   339,   343,   112,   156,     0,   159,   112,     0,     0,
       0,   116,     0,     0,     0,     0,     0,     0,   378,     0,
       0,   379,     0,     0,     0,     0,     0,     0,   115,     0,
       0,     0,     0,   115,     0,     0,   120,     0,   212,   389,
       0,     0,   112,     0,   185,     0,     0,   115,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,   115,    79,
     121,     0,     0,   248,     0,     0,     0,   115,   185,     0,
       0,   115,     0,     0,     0,   115,   116,     0,   116,   120,
       0,   116,     0,     0,   116,   112,   112,   112,   112,     0,
       0,     0,     0,   116,     0,     0,   116,     0,     0,     0,
       0,     0,     0,   112,   112,     0,     0,     0,     0,     0,
     115,     0,     0,     0,     0,   116,     0,     0,   311,     0,
       0,     0,     0,     0,     0,     0,     0,   116,     0,   116,
       0,     0,     0,   116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   120,     0,   120,     0,     0,   120,
       0,   509,   120,   115,   115,   115,   115,     0,     0,     0,
       0,   120,     0,   364,   120,     0,     0,     0,     0,   377,
       0,   115,   115,     0,     0,   529,     0,     0,     0,     0,
       0,   121,     0,   120,     0,     0,     0,     0,   116,     0,
       0,     0,     0,   116,     0,   120,     0,   120,     0,     0,
       0,   120,     0,     0,     0,     0,     0,   116,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,     0,
       0,    79,     0,     0,   121,     0,     0,   116,     0,     0,
       0,   116,     0,     0,     0,   116,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   185,   185,     0,     0,
     185,   185,     0,     0,     0,     0,   120,     0,     0,     0,
       0,   120,     0,   667,     0,     0,     0,     0,     0,     0,
     116,     0,     0,     0,     0,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   120,   691,     0,   121,
       0,   121,     0,     0,   121,   120,     0,   121,    79,   120,
       0,     0,     0,   120,     0,     0,   121,     0,     0,   121,
       0,     0,     0,   116,   116,   116,   116,     0,     0,     0,
       0,     0,   377,     0,     0,   377,     0,     0,   121,     0,
       0,   116,   116,     0,     0,     0,     0,     0,   120,     0,
     121,     0,   121,     0,     0,     0,   121,     0,     0,     0,
       0,   136,     0,   647,   649,     0,   136,   652,   655,     0,
     185,   185,   136,   136,     0,     0,     0,   185,     0,   361,
     178,     0,   181,     0,   361,     0,     0,   185,     0,     0,
     185,   120,   120,   120,   120,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   222,     0,     0,     0,   120,
     120,   121,   579,     0,     0,     0,   121,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   793,     0,     0,
     121,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   121,     0,     0,     0,     0,     0,   251,     0,   252,
     121,     0,     0,     0,   121,     0,     0,     0,   121,     0,
       0,     0,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,     0,   754,   755,     0,
       0,     0,     0,     0,   757,     0,     0,     0,     0,     0,
       0,     0,     0,   121,   766,     0,     0,   769,     0,     0,
       0,     0,   222,     0,   304,   305,  -402,     0,   326,  -427,
       0,  -427,     0,   222,     0,   334,     0,     0,     0,     0,
       0,     0,     0,     0,   185,   326,     0,     0,   369,     0,
     264,     0,     0,   665,     0,     0,   121,   121,   121,   121,
       0,   326,   136,     0,     0,   382,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   121,     0,     0,     0,   326,
       0,     0,     0,     0,     0,     0,     0,   361,   361,   265,
     361,   361,     0,   266,     0,     0,   701,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   896,     0,
       0,     0,     0,     0,   900,     0,   361,   267,   361,     0,
       0,     0,     0,     0,   181,   181,   181,   181,     0,     0,
       0,     0,   268,     0,   269,   270,   271,     0,   272,   273,
     274,   449,   275,   276,   277,   278,     0,   280,   281,   282,
     283,   859,   284,   285,   286,     0,     0,   287,   288,   289,
       0,   377,     0,   377,     0,     0,   377,     0,     0,   377,
       0,     0,     0,     0,   326,   326,     0,     0,   774,     0,
       0,   776,     0,   896,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     789,   517,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   138,   795,     0,   799,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   139,   140,     0,   141,   369,    30,     0,     0,
       0,     0,    33,    34,   181,    36,     0,    37,    38,     0,
       0,     0,     0,     0,     0,    40,     0,    42,   326,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,    52,   142,     0,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,     0,
       0,   585,   841,     0,   589,     0,   181,     0,     0,     0,
       0,     0,     0,   858,     0,     0,     0,    64,     0,     0,
       0,     0,   863,     0,     0,     0,   865,   304,   305,     0,
      79,     0,  -427,     0,  -427,    69,     0,     0,     0,     0,
      -2,     4,     0,     5,     6,     7,     8,     9,    10,    75,
       0,   144,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -293,    18,    19,    20,   888,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -180,    32,    33,    34,    35,    36,   -42,    37,    38,
       0,    39,     0,   326,     0,  -293,    40,    41,    42,    43,
    -180,   -42,    44,    45,     0,  -293,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,   671,    58,    59,  -293,   369,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,   692,     0,     0,     0,   222,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,   737,     0,     0,    74,     0,
      75,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     0,     5,     6,     7,     8,     9,    10,
       0,  -446,     0,    11,    12,    13,  -446,     0,    14,    15,
      16,    17,  -293,    18,    19,    20,  -446,    21,    22,  -446,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -180,    32,    33,    34,    35,    36,   -42,    37,
      38,     0,    39,     0,     0,     0,  -293,    40,   369,    42,
      43,  -180,   -42,    44,    45,  -446,  -293,    46,    47,  -446,
      48,    49,    50,    51,    52,    53,     0,    54,   692,    55,
      56,    57,     0,    58,    59,  -293,     0,     0,    60,     0,
      61,     0,     0,  -446,     0,     0,   589,     0,     0,     0,
       0,     0,     0,   816,     0,     0,     0,     0,  -446,    64,
    -446,  -446,  -446,  -446,  -446,  -446,  -446,     0,  -446,  -446,
    -446,  -446,  -446,  -446,  -446,  -446,  -446,    69,  -446,  -446,
    -446,     0,    72,  -446,  -446,  -446,     0,     0,     0,    74,
    -446,    75,     0,    76,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   136,   233,     0,     5,     6,
       7,     8,     9,    10,     0,     0,   871,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,    41,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,   197,    72,     0,     0,    73,
       0,     0,     0,    74,   234,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,    41,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,   406,    75,   233,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,    41,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,   234,    75,   803,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,    41,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,   804,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,    41,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,   867,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,   247,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   520,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   524,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   759,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   761,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   764,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   767,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,   788,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   794,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   798,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,   264,     0,    74,     0,    75,   680,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   139,   140,
     265,   141,     0,    30,   266,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
       0,    40,     0,    42,     0,     0,     0,     0,   267,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    52,   142,
       0,     0,     0,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,    62,   280,   281,
     282,   283,     0,   284,     0,   286,     0,     0,   287,   288,
     289,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,   197,     0,     0,     0,    73,
       0,   264,     0,   143,     0,    75,     0,   144,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   139,   140,
     265,   141,     0,    30,   266,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
       0,    40,     0,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    52,   142,
       0,     0,     0,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,    62,   280,   281,
     282,   283,     0,   284,     0,   286,     0,     0,   287,   288,
     289,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,   197,     0,     0,     0,    73,
       0,     0,   264,   143,     0,    75,     0,   144,   578,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   139,
     140,   265,   141,     0,    30,   266,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,     0,    40,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,    52,
     142,     0,     0,     0,   268,     0,   269,   270,   271,     0,
     272,   273,   274,     0,     0,   276,   277,   278,    62,   280,
     281,   282,   283,     0,     0,     0,   286,     0,     0,   287,
     288,   289,     0,    63,    64,    65,    66,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    69,     0,    70,    71,   197,     0,     0,     0,
      73,     0,     0,     0,   143,     0,    75,     0,   144,   890,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     139,   140,     0,   141,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,     0,    40,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,    70,    71,     0,     0,     0,
       0,    73,     0,     0,     0,   143,     0,    75,     0,   144,
     583,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,   488,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,   489,    30,     0,   490,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,   491,   492,    40,     0,    42,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,   239,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,   572,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,   574,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,   203,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,   682,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,   167,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   168,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,   172,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
     173,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,   570,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   168,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   843,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   844,
      51,    52,   845,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     846,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     846,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   829,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,   530,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     549,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   628,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   631,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   823,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   824,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   826,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   827,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   828,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   829,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,    63,    64,    65,    66,     0,
     139,   140,     0,   141,     0,    30,     0,     0,     0,     0,
      33,    34,    68,    36,    69,    37,    38,    71,     0,     0,
       0,     0,    73,    40,     0,    42,   143,     0,    75,     0,
     144,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,  -317,
       0,     0,     0,  -317,     0,     0,     5,     6,     7,     8,
       9,    10,     0,    69,     0,   138,     0,   197,     0,     0,
       0,     0,     0,     0,     0,   143,     0,    75,  -317,   846,
      22,     0,     0,     0,     0,     0,   139,   140,     0,   141,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     5,     0,     0,    40,
       0,    42,     0,   712,     0,     0,     0,     0,   713,     0,
       0,     0,     0,     0,    50,    51,    52,   142,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     0,     0,     0,     0,
       0,    64,   712,     0,     0,     0,     0,   713,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,     0,    75,     0,   549,   349,   350,   351,   352,
     353,   354,     0,     0,   355,   356,   357,   358,   359,   360,
     714,   715,   716,   717,     0,     0,   718,     0,     0,     0,
     719,   720,   721,   722,   723,   724,   725,   726,   727,   728,
     729,     0,   730,     0,     0,   731,   732,   733,   734,     0,
     735,     0,     0,   736,     0,   349,   350,   351,   352,   353,
     354,     0,     0,   355,   356,   357,   358,   359,   360,   714,
     715,   716,   717,     0,     0,   718,     0,     0,     0,   719,
     720,   721,   722,   723,   724,   725,   726,   727,   728,   729,
       0,   730,     0,   262,   731,   732,   733,   734,   263,   735,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,     0,     0,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,   265,     0,     0,
       0,   266,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
       0,   342,     0,     0,   263,   267,     0,     0,     0,     0,
       0,     0,     0,     0,   264,   265,     0,     0,     0,   266,
     268,     0,   269,   270,   271,     0,   272,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,     0,
     284,   285,   286,   267,     0,   287,   288,   289,     0,     0,
       0,     0,     0,   265,     0,     0,   699,   266,   268,     0,
     269,   270,   271,     0,   272,   273,   274,     0,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   262,   284,   285,
     286,   267,   263,   287,   288,   289,     0,     0,     0,     0,
       0,     0,   264,     0,   753,     0,   268,     0,   269,   270,
     271,     0,   272,   273,   274,     0,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   262,   284,   285,   286,     0,
     263,   287,   288,   289,     0,     0,     0,     0,     0,     0,
     264,   265,   899,     0,     0,   266,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   262,     0,     0,     0,     0,   263,   267,
       0,     0,     0,     0,     0,     0,     0,     0,   264,   265,
       0,     0,     0,   266,   268,     0,   269,   270,   271,     0,
     272,   273,   274,     0,   275,   276,   277,   278,   279,   280,
     281,   282,   283,     0,   284,   285,   286,   267,     0,   287,
     288,   289,     0,     0,     0,     0,     0,   265,     0,     0,
     936,   266,   268,     0,   269,   270,   271,     0,   272,   273,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,     0,   284,   285,   286,   267,   262,   287,   288,   289,
       0,   263,     0,     0,     0,     0,   571,     0,     0,     0,
     268,   264,   269,   270,   271,     0,   272,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,     0,
     284,   285,   286,     0,     0,   287,   288,   289,     0,     0,
       0,     0,     0,     0,   870,     0,     0,     0,     0,     0,
     265,     0,     0,     0,   266,     0,     0,     0,   262,     0,
       0,   348,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,     0,     0,     0,   267,   349,
     350,   351,   352,   353,   354,     0,     0,   355,   356,   357,
     358,   359,   360,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   265,   284,   285,   286,   266,     0,   287,   288,
     289,     0,     0,   758,   313,     0,     0,     0,     0,     0,
       0,     0,     0,   262,     0,     0,     0,     0,   263,     0,
     267,   349,   350,   351,   352,   353,   354,     0,   264,   355,
     356,   357,   358,   359,   360,   268,     0,   269,   270,   271,
       0,   272,   273,   274,     0,   275,   276,   277,   278,   279,
     280,   281,   282,   283,     0,   284,   285,   286,     0,     0,
     287,   288,   289,     0,     0,     0,   313,   265,     0,     0,
       0,   266,     0,     0,     0,   262,     0,     0,   763,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,     0,     0,   267,   349,   350,   351,   352,
     353,   354,     0,     0,   355,   356,   357,   358,   359,   360,
     268,     0,   269,   270,   271,     0,   272,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   265,
     284,   285,   286,   266,     0,   287,   288,   289,     0,     0,
     883,   313,     0,     0,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,   263,     0,   267,   349,   350,
     351,   352,   353,   354,     0,   264,   355,   356,   357,   358,
     359,   360,   268,     0,   269,   270,   271,     0,   272,   273,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,     0,   284,   285,   286,     0,     0,   287,   288,   289,
       0,     0,     0,   313,   265,     0,     0,     0,   266,     0,
       0,     0,   262,     0,     0,   884,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,     0,
       0,     0,   267,   349,   350,   351,   352,   353,   354,     0,
       0,   355,   356,   357,   358,   359,   360,   268,     0,   269,
     270,   271,     0,   272,   273,   274,     0,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   265,   284,   285,   286,
     266,     0,   287,   288,   289,     0,     0,   885,   313,     0,
       0,     0,     0,     0,     0,     0,     0,   262,     0,     0,
       0,     0,   263,     0,   267,   349,   350,   351,   352,   353,
     354,     0,   264,   355,   356,   357,   358,   359,   360,   268,
       0,   269,   270,   271,     0,   272,   273,   274,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,     0,   284,
     285,   286,     0,     0,   287,   288,   289,     0,     0,     0,
     313,   265,     0,     0,     0,   266,     0,     0,     0,   262,
       0,     0,   886,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,     0,     0,     0,   267,
     349,   350,   351,   352,   353,   354,     0,     0,   355,   356,
     357,   358,   359,   360,   268,     0,   269,   270,   271,     0,
     272,   273,   274,     0,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   265,   284,   285,   286,   266,     0,   287,
     288,   289,     0,     0,   901,   313,     0,     0,     0,     0,
       0,     0,     0,     0,   262,     0,     0,     0,     0,   263,
       0,   267,   349,   350,   351,   352,   353,   354,     0,   264,
     355,   356,   357,   358,   359,   360,   268,     0,   269,   270,
     271,     0,   272,   273,   274,     0,   275,   276,   277,   278,
     279,   280,   281,   282,   283,     0,   284,   285,   286,     0,
       0,   287,   288,   289,     0,     0,     0,   313,   265,     0,
       0,     0,   266,     0,     0,     0,   262,     0,     0,   902,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,   340,     0,     0,     0,   267,   349,   350,   351,
     352,   353,   354,   341,     0,   355,   356,   357,   358,   359,
     360,   268,     0,   269,   270,   271,     0,   272,   273,   274,
       0,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     265,   284,   285,   286,   266,     0,   287,   288,   289,     0,
       0,     0,   313,     0,     0,     0,     0,   262,     0,     0,
       0,     0,   263,     0,   342,     0,     0,     0,   267,     0,
       0,     0,   264,   337,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,   338,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,     0,   284,   285,   286,     0,     0,   287,   288,
     289,   265,     0,     0,   313,   266,     0,     0,     0,   262,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,   654,     0,     0,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,     0,   269,   270,   271,     0,
     272,   273,   274,     0,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   265,   284,   285,   286,   266,     0,   287,
     288,   289,     0,     0,     0,   313,     0,     0,     0,     0,
     262,     0,     0,     0,     0,   263,     0,   342,     0,     0,
       0,   267,     0,     0,     0,   264,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,   269,   270,
     271,     0,   272,   273,   274,     0,   275,   276,   277,   278,
     279,   280,   281,   282,   283,     0,   284,   285,   286,     0,
       0,   287,   288,   289,   265,     0,     0,   313,   266,     0,
       0,     0,   262,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,   648,     0,
       0,     0,   267,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,     0,   269,
     270,   271,     0,   272,   273,   274,     0,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   265,   284,   285,   286,
     266,     0,   287,   288,   289,     0,     0,     0,   313,     0,
       0,     0,     0,   262,     0,     0,     0,     0,   263,     0,
       0,     0,     0,     0,   267,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
       0,   269,   270,   271,     0,   272,   273,   274,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,     0,   284,
     285,   286,     0,     0,   287,   288,   289,   265,     0,     0,
     313,   266,     0,     0,     0,   262,     0,     0,   666,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     268,     0,   269,   270,   271,     0,   272,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   265,
     284,   285,   286,   266,     0,   287,   288,   289,     0,     0,
       0,   313,     0,     0,     0,     0,   262,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,   267,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,   269,   270,   271,     0,   272,   273,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,     0,   284,   285,   286,     0,     0,   287,   288,   289,
     265,     0,     0,   388,   266,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,   263,     0,     0,   267,     0,
       0,     0,     0,     0,     0,   264,     0,     0,     0,     0,
     345,     0,     0,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   346,   284,   285,   286,     0,     0,   287,   288,
     289,     0,     0,  -403,   265,     0,     0,     0,   266,     0,
       0,     0,   262,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,     0,
       0,     0,   267,     0,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,   268,     0,   269,
     270,   271,     0,   272,   273,   274,     0,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   265,   284,   285,   286,
     266,   347,   287,   288,   289,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,     0,     0,     0,
     132,   263,     0,     0,   267,     0,     0,     0,     0,     0,
       0,   264,     0,     0,     0,     0,   401,     0,     0,   268,
       0,   269,   270,   271,     0,   272,   273,   274,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   402,   284,
     285,   286,     0,     0,   287,   288,   289,     0,     0,     0,
     265,     0,     0,     0,   266,     0,     0,     0,   262,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,   500,     0,     0,     0,   267,     0,
       0,     0,     0,     0,     0,   501,     0,     0,     0,     0,
       0,     0,     0,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   265,   284,   285,   286,   266,     0,   287,   288,
     289,     0,     0,     0,     0,     0,     0,     0,     0,   262,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
     267,     0,     0,     0,   264,   502,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,   503,   269,   270,   271,
       0,   272,   273,   274,     0,   275,   276,   277,   278,   279,
     280,   281,   282,   283,     0,   284,   285,   286,     0,     0,
     287,   288,   289,   265,     0,     0,     0,   266,     0,     0,
       0,   262,     0,     0,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,   269,   270,
     271,     0,   272,   273,   274,     0,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   265,   284,   285,   286,   266,
       0,   287,   288,   289,     0,     0,     0,     0,     0,     0,
       0,     0,   262,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,   267,     0,     0,     0,   264,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,     0,
     269,   270,   271,     0,   272,   273,   274,     0,   275,   276,
     277,   278,   279,   280,   281,   282,   283,     0,   284,   285,
     286,     0,   392,   287,   288,   289,   265,     0,     0,     0,
     266,     0,     0,     0,   262,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
       0,   269,   270,   271,     0,   272,   273,   274,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   265,   284,
     285,   286,   266,   394,   287,   288,   289,     0,     0,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,   267,     0,     0,     0,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   268,     0,   269,   270,   271,     0,   272,   273,   274,
       0,   275,   276,   277,   278,   279,   280,   281,   282,   283,
       0,   284,   285,   286,     0,   396,   287,   288,   289,   265,
       0,     0,     0,   266,     0,     0,     0,   262,     0,     0,
       0,     0,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,     0,     0,     0,   267,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,   269,   270,   271,     0,   272,   273,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   265,   284,   285,   286,   266,   404,   287,   288,   289,
       0,     0,     0,     0,     0,     0,     0,     0,   262,     0,
       0,     0,     0,   263,     0,     0,     0,     0,   419,   267,
       0,     0,     0,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,     0,   269,   270,   271,     0,
     272,   273,   274,     0,   275,   276,   277,   278,   279,   280,
     281,   282,   283,     0,   284,   285,   286,     0,     0,   287,
     288,   289,   265,     0,     0,     0,   266,     0,     0,     0,
     262,     0,     0,   504,     0,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,     0,     0,     0,     0,
     267,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,     0,   269,   270,   271,
       0,   272,   273,   274,     0,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   265,   284,   285,   286,   266,     0,
     287,   288,   289,     0,     0,     0,     0,     0,     0,     0,
       0,   262,     0,     0,     0,     0,   263,     0,   132,     0,
       0,     0,   267,     0,     0,     0,   264,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,     0,   269,
     270,   271,     0,   272,   273,   274,     0,   275,   276,   277,
     278,   279,   280,   281,   282,   283,     0,   284,   285,   286,
       0,     0,   287,   288,   289,   265,     0,     0,     0,   266,
       0,     0,     0,   262,   661,     0,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,     0,   267,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,     0,
     269,   270,   271,     0,   272,   273,   274,     0,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   265,   284,   285,
     286,   266,   638,   287,   288,   289,     0,     0,     0,     0,
       0,     0,     0,     0,   262,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,   267,     0,     0,     0,   264,
       0,     0,   664,     0,     0,     0,     0,     0,     0,     0,
     268,     0,   269,   270,   271,     0,   272,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,     0,
     284,   285,   286,     0,     0,   287,   288,   289,   265,     0,
       0,     0,   266,     0,     0,     0,   262,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,     0,     0,     0,   267,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   268,     0,   269,   270,   271,     0,   272,   273,   274,
       0,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     265,   284,   285,   286,   266,     0,   287,   288,   289,     0,
       0,     0,     0,     0,     0,     0,     0,   262,     0,     0,
       0,     0,   263,     0,     0,     0,     0,   702,   267,     0,
       0,     0,   264,   748,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,     0,   284,   285,   286,     0,     0,   287,   288,
     289,   265,     0,     0,     0,   266,     0,     0,     0,   262,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,   751,     0,     0,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,     0,   269,   270,   271,     0,
     272,   273,   274,     0,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   265,   284,   285,   286,   266,     0,   287,
     288,   289,     0,     0,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,   263,     0,     0,     0,     0,
       0,   267,     0,     0,     0,   264,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,   269,   270,
     271,     0,   272,   273,   274,     0,   275,   276,   277,   278,
     279,   280,   281,   282,   283,     0,   284,   285,   286,     0,
       0,   287,   288,   289,   265,     0,     0,     0,   266,     0,
       0,     0,   262,     0,     0,   822,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,     0,
       0,     0,   267,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,     0,   269,
     270,   271,     0,   272,   273,   274,     0,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   265,   284,   285,   286,
     266,     0,   287,   288,   289,     0,     0,   825,     0,     0,
       0,     0,     0,   262,   839,     0,     0,     0,   263,     0,
       0,     0,     0,     0,   267,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
       0,   269,   270,   271,     0,   272,   273,   274,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,     0,   284,
     285,   286,     0,     0,   287,   288,   289,   265,     0,     0,
       0,   266,     0,     0,     0,   262,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     268,     0,   269,   270,   271,     0,   272,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   265,
     284,   285,   286,   266,     0,   287,   288,   289,     0,     0,
     908,     0,     0,     0,     0,     0,   262,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,   267,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,   269,   270,   271,     0,   272,   273,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,     0,   284,   285,   286,     0,     0,   287,   288,   289,
     265,     0,     0,     0,   266,     0,     0,     0,   262,     0,
       0,   909,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,     0,     0,     0,   267,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   265,   284,   285,   286,   266,     0,   287,   288,
     289,     0,     0,   910,     0,     0,     0,     0,     0,   262,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
     267,     0,     0,     0,   264,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,     0,   269,   270,   271,
       0,   272,   273,   274,     0,   275,   276,   277,   278,   279,
     280,   281,   282,   283,     0,   284,   285,   286,     0,     0,
     287,   288,   289,   265,     0,     0,     0,   266,     0,     0,
       0,   262,     0,     0,   911,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,   269,   270,
     271,     0,   272,   273,   274,     0,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   265,   284,   285,   286,   266,
       0,   287,   288,   289,     0,     0,   912,     0,     0,     0,
       0,     0,   262,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,   267,     0,     0,     0,   264,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,     0,
     269,   270,   271,     0,   272,   273,   274,     0,   275,   276,
     277,   278,   279,   280,   281,   282,   283,     0,   284,   285,
     286,     0,     0,   287,   288,   289,   265,     0,     0,     0,
     266,     0,     0,     0,   262,     0,     0,   913,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
       0,   269,   270,   271,     0,   272,   273,   274,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   265,   284,
     285,   286,   266,     0,   287,   288,   289,     0,     0,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,   267,     0,     0,     0,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   268,     0,   269,   270,   271,     0,   272,   273,   274,
       0,   275,   276,   277,   278,   279,   280,   281,   282,   283,
       0,   284,   285,   286,     0,     0,   287,   288,   289,   626,
       0,     0,     0,   266,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   267,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,   269,   270,   271,     0,   272,   273,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,     0,   284,   285,   286,     0,     0,   287,   288,   289
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-775))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,     2,   310,   397,    43,    44,   597,   365,   244,    11,
     154,   528,   373,   679,    16,   177,    75,    19,    20,    21,
      24,   165,    15,   368,    26,    27,    28,    29,   548,     1,
      46,   193,     3,    35,    36,    37,   397,    39,    40,   256,
     257,   258,   259,    59,    46,   705,    48,    49,    50,   211,
      37,    53,    54,    55,    56,     3,     3,    59,    60,    61,
      76,    26,    64,   740,     3,    67,    68,    69,     1,    71,
      21,     3,    74,    26,    76,    37,    78,     1,   705,    53,
     854,    53,    26,    37,    17,    26,    41,    20,    21,    67,
      67,   112,   346,     1,   131,   131,    29,     3,    31,    32,
       0,   160,    35,   139,    55,    60,    39,   624,    41,    88,
      81,    85,    64,    85,    65,   781,    68,    65,   177,    26,
     141,    27,    55,    88,    57,   112,    59,    60,   144,    62,
      63,   133,    65,    84,   193,    37,   138,   139,   140,   141,
     142,   143,   144,    67,   306,   307,    37,    80,   402,    82,
     112,    84,   211,    77,   131,   929,   676,   150,   112,    67,
     138,   138,    86,    88,   143,   139,    17,   139,   112,   140,
       3,   831,   925,   138,    80,     3,   178,   140,    86,     7,
     890,   143,   184,   936,   811,   138,    88,   111,    80,   143,
     194,   138,   140,   140,   138,   872,   873,   138,   131,   201,
     202,   140,   547,   111,   831,    20,   139,   131,   140,   112,
     112,   428,    63,    41,   138,   925,    31,    32,   140,   112,
     528,   112,   112,   889,    39,   112,    41,   818,   390,   112,
     138,    82,    60,    93,   236,   236,   112,   239,   112,    13,
     143,   143,   112,     7,    18,    60,   139,   306,   307,    55,
      56,   141,   143,   770,   141,   131,   116,   131,   141,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   141,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   140,   112,
     526,   527,    37,   112,     1,   815,     2,    73,   112,   112,
       3,   131,   112,   140,     7,    11,   308,   309,   310,   139,
     668,   313,   137,    19,     3,    21,   624,   319,   141,   335,
      65,   323,   141,   325,   383,   670,   112,   141,   141,   139,
      37,   390,    21,   335,   112,   337,   338,   112,   340,   341,
     576,   577,   112,   345,   346,   131,   348,    53,    37,   408,
      21,    40,   411,   131,   413,    93,   131,   416,   265,   266,
     362,   131,    61,   112,   112,    54,    55,   140,    74,   112,
     131,   373,    78,   375,   112,   131,    65,    21,   116,   131,
     131,   383,   131,   131,    55,    64,    93,   131,   131,    68,
      25,    80,   131,   787,    65,   397,   140,    93,   142,   401,
     402,   308,   309,   141,   111,   112,   408,   131,   424,    80,
     131,    55,   319,   415,   416,   417,   112,   419,   420,   780,
     116,    65,   424,   567,   131,    17,   787,   133,    20,    64,
     140,   338,   142,    68,   341,   111,    80,    29,   114,    31,
      32,     2,   118,    35,    87,   141,   140,    39,   142,    41,
      11,   140,   138,   140,   513,   142,   515,   131,    19,   131,
      21,   819,   770,   140,     1,    57,     3,    59,    60,   114,
      62,    63,   178,   411,   140,   413,   111,   131,   184,   114,
     140,    25,    93,   118,   131,     7,   111,    47,    80,   138,
      82,   131,    53,    93,    55,    56,   112,   131,   500,   501,
     502,   503,   504,   505,   506,   141,    37,   140,   415,    28,
     141,    25,    37,    74,   507,   508,   518,    78,   520,   112,
      64,   141,   524,    93,    68,   112,   528,   141,   141,   890,
     236,    16,   138,   549,   143,   131,   538,   131,   899,   131,
     131,   131,    67,   131,   141,   131,    37,   549,   131,   131,
      64,   131,    77,   131,    68,   131,   131,   131,   131,   131,
     131,    86,   143,   107,   925,   109,   110,   111,    93,   113,
     114,   115,   133,   131,   118,   936,    67,   112,   131,   581,
     124,   125,   131,    28,   141,   129,    77,   112,   132,   133,
     134,   139,   131,   107,   501,    86,   503,   111,   141,   113,
     114,   131,    93,   112,   118,   138,   131,   313,    65,    93,
     139,   125,    26,   138,    26,   143,   111,   178,   132,   133,
     134,   112,   624,   184,   626,   627,   628,   138,    28,   631,
      65,   337,    21,   143,   340,   111,     2,   143,    25,   143,
     131,   139,   348,   114,   646,    11,   648,   138,    37,   651,
     114,    40,   654,    19,   140,    21,    28,   659,   141,   661,
      86,   663,   664,   141,   666,    54,    55,   139,    77,   703,
     686,   402,   401,   219,   166,   236,    65,    64,   690,   517,
     756,    68,   708,   685,   686,   818,   668,    53,   819,   711,
     929,    80,   889,   904,    28,   697,   376,   699,   515,   842,
     702,   703,    -1,    -1,    -1,   707,    -1,    -1,    74,    -1,
      -1,   417,    78,    -1,    -1,    -1,    -1,    -1,    -1,   626,
     627,   780,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,
      -1,   118,    -1,    -1,   736,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,   747,   748,   134,   750,   751,
     752,   753,   313,    -1,    -1,    -1,   758,   759,    37,   761,
      -1,   763,   764,    -1,    -1,   767,    -1,   133,   770,   808,
     809,    -1,    -1,    -1,    -1,   777,   337,    -1,   780,   340,
      -1,    47,    -1,    -1,   786,   787,   788,   348,    67,   495,
     792,   850,   794,    -1,    -1,   797,   798,    -1,    77,   801,
      -1,    -1,    -1,   805,   805,    -1,    -1,    86,    -1,    75,
      -1,    -1,   178,    -1,    93,    -1,    -1,    -1,   184,    -1,
     822,   823,   824,   825,   826,   827,   828,   829,    -1,    -1,
     846,   890,   538,   112,    -1,    -1,    -1,   839,   840,    -1,
      -1,    -1,   844,   845,   846,    -1,    -1,    -1,   850,    -1,
      17,   853,   131,    20,    -1,    -1,   417,    -1,    -1,   138,
      -1,    -1,    29,    -1,    31,    32,   925,    -1,    35,    -1,
     236,    -1,    39,    -1,    41,   581,    -1,   936,    -1,    -1,
      -1,   883,   884,   885,   886,    -1,    -1,    -1,   890,    -1,
      57,   597,    59,    60,    -1,    62,    63,   899,    -1,   901,
     902,    -1,    -1,    -1,    -1,    -1,   908,   909,   910,   911,
     912,   913,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
     626,   627,   924,   925,     2,    -1,     3,    -1,    -1,    -1,
       7,    -1,     9,    11,   936,    -1,    -1,    -1,    -1,    -1,
     646,    19,   648,    21,    21,   651,    -1,   313,   654,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   663,    -1,    -1,
     666,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,
      -1,   337,   139,    -1,   340,    53,    -1,   538,    55,   685,
      -1,    -1,   348,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,   697,     2,   699,    -1,    -1,    74,   703,     3,    -1,
      78,    11,     7,    80,    -1,    -1,    -1,    84,    -1,    19,
      -1,    21,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
     581,    -1,    25,    -1,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      55,   417,   758,    -1,    -1,   133,    -1,   763,    -1,    -1,
      65,    64,    -1,    -1,    74,    68,    -1,    -1,    78,    -1,
      -1,   777,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,
      -1,    -1,   788,    -1,    -1,   646,    -1,   648,    -1,    -1,
     651,   797,    -1,   654,    -1,   801,    -1,    -1,    -1,   805,
     178,    -1,   663,    -1,   107,   666,   184,   110,   111,    -1,
     113,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   133,   685,    -1,    -1,    -1,    -1,   132,
     133,   134,    -1,    -1,   840,    -1,   697,    -1,   699,    -1,
      -1,    17,   703,    -1,    20,   411,    -1,   413,    -1,    -1,
      -1,    -1,     2,    29,    -1,    31,    32,    -1,   236,    35,
      -1,    11,    -1,    39,    -1,    41,    -1,    -1,   178,    19,
      -1,    21,   538,    -1,   184,    -1,    -1,   883,   884,   885,
     886,    57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   901,   902,   758,    -1,    -1,
      -1,    -1,   763,    53,    80,    -1,    82,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,   581,   777,    -1,    -1,    -1,
       2,    -1,    31,    32,    74,    -1,   236,   788,    78,    11,
      39,    -1,    41,    -1,    -1,   313,   797,    19,    47,    21,
     801,    -1,    -1,    -1,   805,    -1,    -1,   513,    -1,    -1,
      -1,    60,    61,    -1,    20,   131,    -1,    -1,    -1,   337,
      -1,    -1,   340,   139,    -1,    31,    32,    -1,    -1,    -1,
     348,    53,    -1,    39,    -1,    41,    -1,    -1,    -1,   840,
     646,    47,   648,   133,    -1,   651,   552,    -1,   654,    -1,
      -1,    -1,    74,    -1,    60,    61,    78,   663,    -1,    -1,
     666,    -1,    -1,   313,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   685,
      -1,    -1,   883,   884,   885,   886,    -1,   337,   178,     1,
     340,   697,    -1,   699,   184,    -1,    -1,   703,   348,   417,
     901,   902,    -1,    -1,    -1,    17,    -1,    -1,    20,    21,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    29,    -1,    31,
      32,    -1,    25,    35,    -1,     2,    -1,    39,    -1,    41,
      -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    55,    21,    57,   236,    59,    60,    -1,
      62,    63,   758,    65,    -1,    -1,   178,   763,    -1,    -1,
      -1,    64,   184,    -1,    -1,    68,    -1,   417,    80,    -1,
      82,   777,    84,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,   788,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   797,    -1,    -1,    -1,   801,    -1,    74,    -1,   805,
      -1,    78,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,    -1,    -1,   236,   118,    -1,    -1,    -1,   131,
     538,   124,   125,   313,    -1,    -1,   129,   139,    -1,   132,
     133,   134,    -1,    -1,   840,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,    -1,    -1,
     340,    -1,    -1,    49,    -1,    -1,   133,    -1,   348,    55,
      56,    -1,    -1,   581,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   883,   884,   885,
     886,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   538,    -1,
      -1,   313,    -1,    -1,    -1,   901,   902,    -1,    -1,    -1,
      25,   178,    -1,    -1,    -1,    -1,    -1,   184,    64,    -1,
      -1,    -1,    68,    -1,    -1,   337,    -1,    -1,   340,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   348,   417,   646,    -1,
     648,   581,    -1,   651,    -1,    -1,   654,    -1,    -1,    64,
      -1,   137,    -1,    68,    -1,   663,    -1,    -1,   666,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   236,
      49,    -1,   118,    -1,    -1,    -1,    -1,   685,    -1,   125,
      -1,    -1,    -1,   169,   170,   171,   132,   133,   134,   697,
     176,   699,    -1,    -1,    -1,   703,   111,    -1,   113,   114,
      -1,   187,    -1,   118,   190,   417,   646,    -1,   648,    -1,
     125,   651,    -1,    -1,   654,    -1,    -1,   132,   133,   134,
      -1,    -1,   208,   663,    -1,    -1,   666,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,    -1,    -1,    -1,    -1,   685,   313,    -1,    -1,    -1,
     758,    -1,    -1,    -1,    -1,   763,    -1,   697,   538,   699,
      -1,    -1,    -1,   703,    -1,    -1,    -1,    -1,    -1,   777,
     337,    -1,    -1,   340,    -1,    -1,    -1,     2,    -1,    -1,
     788,   348,    -1,    -1,    -1,    -1,    11,    -1,    -1,   797,
     169,   170,   171,   801,    19,    -1,    21,   805,    -1,    -1,
      -1,   581,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,   190,    -1,    -1,    -1,    -1,    -1,    -1,   758,    -1,
      -1,    -1,    -1,   763,    -1,    -1,   538,    -1,    53,   208,
      -1,    -1,   840,    -1,   320,    -1,    -1,   777,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   788,    74,
     417,    -1,    -1,    78,    -1,    -1,    -1,   797,   344,    -1,
      -1,   801,    -1,    -1,    -1,   805,   646,    -1,   648,   581,
      -1,   651,    -1,    -1,   654,   883,   884,   885,   886,    -1,
      -1,    -1,    -1,   663,    -1,    -1,   666,    -1,    -1,    -1,
      -1,    -1,    -1,   901,   902,    -1,    -1,    -1,    -1,    -1,
     840,    -1,    -1,    -1,    -1,   685,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   697,    -1,   699,
      -1,    -1,    -1,   703,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   646,    -1,   648,    -1,    -1,   651,
      -1,   320,   654,   883,   884,   885,   886,    -1,    -1,    -1,
      -1,   663,    -1,   178,   666,    -1,    -1,    -1,    -1,   184,
      -1,   901,   902,    -1,    -1,   344,    -1,    -1,    -1,    -1,
      -1,   538,    -1,   685,    -1,    -1,    -1,    -1,   758,    -1,
      -1,    -1,    -1,   763,    -1,   697,    -1,   699,    -1,    -1,
      -1,   703,    -1,    -1,    -1,    -1,    -1,   777,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   788,    -1,
      -1,   236,    -1,    -1,   581,    -1,    -1,   797,    -1,    -1,
      -1,   801,    -1,    -1,    -1,   805,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   522,   523,    -1,    -1,
     526,   527,    -1,    -1,    -1,    -1,   758,    -1,    -1,    -1,
      -1,   763,    -1,   539,    -1,    -1,    -1,    -1,    -1,    -1,
     840,    -1,    -1,    -1,    -1,   777,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   788,   563,    -1,   646,
      -1,   648,    -1,    -1,   651,   797,    -1,   654,   313,   801,
      -1,    -1,    -1,   805,    -1,    -1,   663,    -1,    -1,   666,
      -1,    -1,    -1,   883,   884,   885,   886,    -1,    -1,    -1,
      -1,    -1,   337,    -1,    -1,   340,    -1,    -1,   685,    -1,
      -1,   901,   902,    -1,    -1,    -1,    -1,    -1,   840,    -1,
     697,    -1,   699,    -1,    -1,    -1,   703,    -1,    -1,    -1,
      -1,    13,    -1,   522,   523,    -1,    18,   526,   527,    -1,
     636,   637,    24,    25,    -1,    -1,    -1,   643,    -1,   645,
      32,    -1,    34,    -1,   650,    -1,    -1,   653,    -1,    -1,
     656,   883,   884,   885,   886,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,   901,
     902,   758,   417,    -1,    -1,    -1,   763,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   693,    -1,    -1,
     777,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   788,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     797,    -1,    -1,    -1,   801,    -1,    -1,    -1,   805,    -1,
      -1,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,    -1,   636,   637,    -1,
      -1,    -1,    -1,    -1,   643,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   840,   653,    -1,    -1,   656,    -1,    -1,
      -1,    -1,   154,    -1,   135,   136,   137,    -1,   160,   140,
      -1,   142,    -1,   165,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   790,   177,    -1,    -1,   180,    -1,
      25,    -1,    -1,   538,    -1,    -1,   883,   884,   885,   886,
      -1,   193,   194,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   901,   902,    -1,    -1,    -1,   211,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   833,   834,    64,
     836,   837,    -1,    68,    -1,    -1,   581,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   854,    -1,
      -1,    -1,    -1,    -1,   860,    -1,   862,    92,   864,    -1,
      -1,    -1,    -1,    -1,   256,   257,   258,   259,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,   273,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,   790,   127,   128,   129,    -1,    -1,   132,   133,   134,
      -1,   646,    -1,   648,    -1,    -1,   651,    -1,    -1,   654,
      -1,    -1,    -1,    -1,   306,   307,    -1,    -1,   663,    -1,
      -1,   666,    -1,   929,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     685,   333,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,   697,    -1,   699,    -1,    -1,    -1,   703,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,   368,    38,    -1,    -1,
      -1,    -1,    43,    44,   376,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,   390,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,    -1,
      -1,   423,   777,    -1,   426,    -1,   428,    -1,    -1,    -1,
      -1,    -1,    -1,   788,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,   797,    -1,    -1,    -1,   801,   135,   136,    -1,
     805,    -1,   140,    -1,   142,   126,    -1,    -1,    -1,    -1,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,   140,
      -1,   142,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,   840,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,   515,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,   543,    82,    83,    84,   547,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   563,    -1,    -1,    -1,   567,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,   597,    -1,    -1,   138,    -1,
     140,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    12,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,   670,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,   690,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,   708,    -1,    -1,    -1,
      -1,    -1,    -1,   715,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,    -1,    -1,    -1,   138,
     139,   140,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   807,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,   818,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,   130,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,   139,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,   139,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,   139,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,   139,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,   139,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    25,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      64,    36,    -1,    38,    68,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,    92,   122,   123,
     124,   125,    -1,   127,    -1,   129,    -1,    -1,   132,   133,
     134,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,
      -1,    25,    -1,   138,    -1,   140,    -1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      64,    36,    -1,    38,    68,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,    92,   122,   123,
     124,   125,    -1,   127,    -1,   129,    -1,    -1,   132,   133,
     134,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,
      -1,    -1,    25,   138,    -1,   140,    -1,   142,   143,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    64,    36,    -1,    38,    68,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,    -1,   118,   119,   120,    92,   122,
     123,   124,   125,    -1,    -1,    -1,   129,    -1,    -1,   132,
     133,   134,    -1,   107,   108,   109,   110,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,    -1,   128,   129,   130,    -1,    -1,    -1,
     134,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,   143,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,
     143,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    37,    38,    -1,    40,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    54,    55,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    80,    -1,
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
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,   141,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,   141,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    50,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    -1,
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
      -1,    -1,    -1,    55,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      32,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
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
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    -1,
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
      -1,    -1,   134,    -1,    -1,    -1,   138,   139,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,
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
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
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
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,   107,   108,   109,   110,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,   124,    46,   126,    48,    49,   129,    -1,    -1,
      -1,    -1,   134,    56,    -1,    58,   138,    -1,   140,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,   126,    -1,    12,    -1,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,   140,   141,   142,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,     3,    -1,    -1,    56,
      -1,    58,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,   108,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,    93,    94,    95,    96,
      97,    98,    -1,    -1,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,    -1,   113,    -1,    -1,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    -1,   129,    -1,    -1,   132,   133,   134,   135,    -1,
     137,    -1,    -1,   140,    -1,    93,    94,    95,    96,    97,
      98,    -1,    -1,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,   129,    -1,    10,   132,   133,   134,   135,    15,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    88,    -1,    -1,    15,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    64,    -1,    -1,    -1,    68,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    92,    -1,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,   143,    68,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    10,   127,   128,
     129,    92,    15,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,   143,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    10,   127,   128,   129,    -1,
      15,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    64,   143,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    64,
      -1,    -1,    -1,    68,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,    92,    -1,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
     143,    68,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    92,    10,   132,   133,   134,
      -1,    15,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
     107,    25,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,    -1,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,
      -1,    75,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    98,    -1,    -1,   101,   102,   103,
     104,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    64,   127,   128,   129,    68,    -1,   132,   133,
     134,    -1,    -1,    75,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      92,    93,    94,    95,    96,    97,    98,    -1,    25,   101,
     102,   103,   104,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,    -1,
     132,   133,   134,    -1,    -1,    -1,   138,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    75,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    98,    -1,    -1,   101,   102,   103,   104,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    64,
     127,   128,   129,    68,    -1,   132,   133,   134,    -1,    -1,
      75,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    92,    93,    94,
      95,    96,    97,    98,    -1,    25,   101,   102,   103,   104,
     105,   106,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      -1,    -1,    -1,   138,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    10,    -1,    -1,    75,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    98,    -1,
      -1,   101,   102,   103,   104,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,    -1,   132,   133,   134,    -1,    -1,    75,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    92,    93,    94,    95,    96,    97,
      98,    -1,    25,   101,   102,   103,   104,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,    -1,    -1,   132,   133,   134,    -1,    -1,    -1,
     138,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,
      -1,    -1,    75,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    98,    -1,    -1,   101,   102,
     103,   104,   105,   106,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    64,   127,   128,   129,    68,    -1,   132,
     133,   134,    -1,    -1,    75,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    92,    93,    94,    95,    96,    97,    98,    -1,    25,
     101,   102,   103,   104,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,    -1,
      -1,   132,   133,   134,    -1,    -1,    -1,   138,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    75,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    98,    37,    -1,   101,   102,   103,   104,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      64,   127,   128,   129,    68,    -1,   132,   133,   134,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    88,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    37,   109,   110,   111,    -1,   113,
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
      10,    -1,    -1,    -1,    -1,    15,    -1,    88,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,    -1,
      -1,   132,   133,   134,    64,    -1,    -1,   138,    68,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,    -1,   132,   133,   134,    -1,    -1,    -1,   138,    -1,
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
      -1,   138,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      64,    -1,    -1,   138,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      30,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    52,   127,   128,   129,    -1,    -1,   132,   133,
     134,    -1,    -1,   137,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      88,    15,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    52,   127,
     128,   129,    -1,    -1,   132,   133,   134,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    64,   127,   128,   129,    68,    -1,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    37,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,    -1,
     132,   133,   134,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
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
      -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    64,   127,   128,   129,    68,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    91,    92,
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
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    88,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
      -1,    -1,   132,   133,   134,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    64,   127,   128,
     129,    68,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    25,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,    -1,   132,   133,   134,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      64,   127,   128,   129,    68,    -1,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    91,    92,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,    -1,    -1,   132,   133,
     134,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    64,   127,   128,   129,    68,    -1,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
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
      68,    -1,    -1,    -1,    10,    -1,    -1,    75,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    64,   127,
     128,   129,    68,    -1,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,    -1,   132,   133,   134,    64,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134
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
     189,   190,   193,   196,   197,   220,   223,   224,   242,   243,
     244,   245,   246,   247,   254,   255,   256,   257,   259,   260,
     261,   262,   263,   264,   266,   267,   268,   269,   270,   131,
     152,   257,    88,   248,   249,   164,   165,   248,    12,    33,
      34,    36,    74,   138,   142,   193,   242,   246,   255,   256,
     257,   259,   261,   262,    80,   164,   152,   236,   257,   152,
     140,     7,   164,   166,     9,    80,   166,    55,    90,   174,
     257,   257,    20,    32,   223,   257,   257,   140,   165,   194,
     140,   165,   225,   226,    26,   156,   167,   257,   257,   257,
     257,   257,     7,   140,    32,   170,   170,   130,   216,   235,
     257,    78,    79,   131,   257,   258,   257,   167,   257,   257,
      73,   140,   152,   257,   257,   156,   163,   257,   259,   156,
     163,   257,   165,   221,   235,   257,   257,   257,   257,   257,
     257,   257,   257,     1,   139,   150,   157,   235,    81,   116,
     216,   237,   238,   258,   235,   257,   265,    57,   152,    61,
     155,   165,   165,    41,    60,   211,    21,    55,    65,    84,
     131,   137,    10,    15,    25,    64,    68,    92,   107,   109,
     110,   111,   113,   114,   115,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   127,   128,   129,   132,   133,   134,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   135,   136,   140,   142,    64,    68,
     140,   152,   131,   138,   156,   257,   257,   257,   235,    37,
     248,   221,   131,   112,   131,    87,   165,   216,   239,   240,
     241,   258,   221,   186,   165,   140,   167,    26,    37,   167,
      26,    37,    88,   167,   251,    30,    52,   131,    75,    93,
      94,    95,    96,    97,    98,   101,   102,   103,   104,   105,
     106,   156,   203,   239,   152,   140,   205,    81,   140,   165,
     227,   228,     1,   111,   230,    37,   112,   152,   167,   167,
     239,   166,   165,   112,   131,   257,   257,   131,   138,   167,
     140,   239,   131,   178,   131,   178,   131,    93,   222,   131,
     131,    30,    52,   167,   131,   139,   139,   150,   112,   139,
     257,   112,   141,   112,   141,    37,   112,   143,   251,    91,
     112,   143,     7,    47,   111,   187,   138,   198,    65,   225,
     225,   225,   225,   257,   257,   257,   257,   174,   257,   174,
     257,   257,   257,   257,   257,   257,   257,    27,    80,   165,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   239,   239,   174,   257,   174,   257,    21,    37,
      40,    54,    55,    65,    80,   209,   250,   253,   257,   270,
      26,    37,    26,    37,    75,    37,   143,   174,   257,   167,
     131,   257,   257,    93,   141,   112,   131,   165,    37,   235,
      36,   257,   174,   257,    36,   257,   174,   257,   140,   167,
     134,   158,   160,   257,   158,   159,   152,   257,    28,   257,
     141,   206,   207,   208,   209,   195,   228,   112,   141,   142,
     231,   243,    93,   229,   257,   226,   141,   216,   257,     1,
     183,   239,   141,    16,   179,   231,   243,   112,   160,   159,
     139,   141,   141,   237,   141,   237,   174,   257,   143,   152,
     257,   143,   257,   143,   257,   165,   235,   138,     1,   165,
     191,   192,    21,    55,    65,    80,   200,   210,   225,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   141,
     143,    37,    65,   242,   112,   141,    64,    64,    36,   174,
     257,    36,   174,   257,   174,   257,   248,   248,   131,   216,
     258,   241,   187,   257,   141,   257,    26,   167,    26,   167,
     257,    26,   167,   251,    26,   167,   251,   252,   253,   112,
     131,    11,   131,    28,    28,   152,    75,   156,   112,   141,
     140,   165,    21,    55,    65,    80,   212,   141,   228,   230,
       1,   235,    50,   258,   139,    53,    85,   139,   184,   141,
     140,   156,   165,   180,   221,   131,   131,   143,   251,   143,
     251,   152,    91,   138,     1,   188,   139,    93,   112,   139,
      65,   199,    10,    15,   107,   108,   109,   110,   113,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     129,   132,   133,   134,   135,   137,   140,   165,   201,   202,
     203,   242,   131,   253,   242,   242,   257,    26,    26,   257,
      26,    26,   143,   143,   167,   167,   138,   167,    75,    36,
     257,    36,   257,    75,    36,   257,   167,    36,   257,   167,
     112,   141,   257,   257,   152,   257,   152,    28,   207,   228,
     111,   234,    65,   230,   229,   143,    37,   143,    26,   152,
     235,   180,   111,   156,    36,   152,   257,   143,    36,   152,
     257,   143,   257,     1,   139,   157,   139,    32,    59,    62,
     139,   151,   169,   257,   192,   212,   165,   257,   114,   140,
     204,   204,    75,    36,    36,    75,    36,    36,    36,    36,
       1,   188,   257,   257,   257,   257,   257,   257,   253,    11,
      28,   152,   141,    27,    71,    74,   142,   216,   233,   243,
     116,   218,   229,    86,   219,   257,   231,   243,   152,   167,
     141,   257,   257,   152,   257,   152,   139,   139,   169,   230,
     141,   165,   202,   203,   206,   257,   257,   257,   257,   257,
     257,   139,   139,    75,    75,    75,    75,   257,   152,   234,
     143,   235,   216,   217,   257,   257,   156,   168,   215,   143,
     156,    75,    75,    77,   213,   204,   204,   141,    75,    75,
      75,    75,    75,    75,   257,   257,   257,   257,   218,   229,
     216,   232,   233,   243,    37,   143,   243,   257,   257,   219,
     257,   232,   233,   131,   214,   215,   143,   232
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
#line 441 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 446 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 447 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 454 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 459 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 460 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 479 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 483 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 489 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 491 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 494 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (6)].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[(1) - (6)].b), (yyvsp[(2) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType, (yyvsp[(6) - (7)].pblockstmt), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 516 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 529 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 530 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 547 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 552 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 556 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 563 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 592 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 594 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 596 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 598 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 600 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 618 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 620 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 622 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 626 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 645 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 650 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 653 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 666 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 667 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 668 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 669 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 674 "chapel.ypp"
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

  case 104:

/* Line 1806 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 703 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 705 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 711 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 717 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 723 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 729 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 736 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 745 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 749 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 753 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 755 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 757 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 759 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 776 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 777 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 781 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 782 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 792 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 796 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 798 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 802 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 803 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 808 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 810 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 812 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 819 "chapel.ypp"
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

  case 158:

/* Line 1806 of yacc.c  */
#line 829 "chapel.ypp"
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

  case 159:

/* Line 1806 of yacc.c  */
#line 839 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch),
                                             NULL,
                                             (yyvsp[(1) - (6)].aggrTag),
                                             (yyvsp[(3) - (6)].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 849 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch),
                                             (yyvsp[(2) - (8)].pch),
                                             (yyvsp[(3) - (8)].aggrTag),
                                             (yyvsp[(5) - (8)].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 861 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 866 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 871 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 880 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 887 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 894 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(1) - (5)].penumtype);
      for_vector(DefExpr, ec, *(yyvsp[(4) - (5)].pvecOfDefs)) {
        ec->sym->type = pdt;
        pdt->constants.insertAtTail(ec);
        if (pdt->defaultValue == NULL) {
          pdt->defaultValue = ec->sym;
        }
      }
      delete (yyvsp[(4) - (5)].pvecOfDefs);
      pdt->doc = (yylsp[(1) - (5)]).comment;
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(1) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 910 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 917 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 926 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 934 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 938 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 944 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 945 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 950 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 955 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 960 "chapel.ypp"
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

  case 180:

/* Line 1806 of yacc.c  */
#line 980 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 986 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 993 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 1001 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 1013 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 1019 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 1026 "chapel.ypp"
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

  case 187:

/* Line 1806 of yacc.c  */
#line 1048 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 1052 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 1057 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 1061 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 1069 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 1074 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 1075 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 1076 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 1077 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 1078 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 1080 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 1082 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 1083 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 1085 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1089 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1090 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1091 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1102 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1103 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1112 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1113 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1126 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1135 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1139 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1143 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1148 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1149 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1150 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1151 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1152 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1153 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1155 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1156 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1160 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1161 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1162 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1163 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1169 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1170 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1174 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1177 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1178 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1179 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1183 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1187 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1193 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1214 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1229 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1242 "chapel.ypp"
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

  case 285:

/* Line 1806 of yacc.c  */
#line 1258 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1260 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1262 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1267 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1275 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1283 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1291 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1300 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1317 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1327 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1332 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1336 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1351 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1352 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1358 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1389 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1397 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1403 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1404 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1405 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1410 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1412 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1418 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1420 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1426 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1427 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1429 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1432 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1441 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1445 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1446 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1451 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1452 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1456 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 342:

/* Line 1806 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 343:

/* Line 1806 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1471 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1472 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1536 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1543 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1549 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1555 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1561 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1570 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1612 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1616 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1621 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1624 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1628 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromArgIntent((yyvsp[(1) - (2)].pt), (yyvsp[(2) - (2)].pexpr));
    }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1631 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1634 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1642 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1647 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1652 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1668 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1670 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1672 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1676 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1682 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1684 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1722 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1725 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 1731 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 1732 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 1733 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 1734 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 1735 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 1736 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 1737 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 1739 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 1743 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 1750 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 1751 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 452:

/* Line 1806 of yacc.c  */
#line 1755 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 453:

/* Line 1806 of yacc.c  */
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 454:

/* Line 1806 of yacc.c  */
#line 1757 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 455:

/* Line 1806 of yacc.c  */
#line 1758 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 456:

/* Line 1806 of yacc.c  */
#line 1759 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:

/* Line 1806 of yacc.c  */
#line 1760 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 458:

/* Line 1806 of yacc.c  */
#line 1761 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 459:

/* Line 1806 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 460:

/* Line 1806 of yacc.c  */
#line 1763 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 461:

/* Line 1806 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 462:

/* Line 1806 of yacc.c  */
#line 1765 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 463:

/* Line 1806 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 464:

/* Line 1806 of yacc.c  */
#line 1767 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 465:

/* Line 1806 of yacc.c  */
#line 1768 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 466:

/* Line 1806 of yacc.c  */
#line 1769 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 467:

/* Line 1806 of yacc.c  */
#line 1770 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 468:

/* Line 1806 of yacc.c  */
#line 1771 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 469:

/* Line 1806 of yacc.c  */
#line 1772 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 470:

/* Line 1806 of yacc.c  */
#line 1773 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 471:

/* Line 1806 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 472:

/* Line 1806 of yacc.c  */
#line 1775 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 473:

/* Line 1806 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 474:

/* Line 1806 of yacc.c  */
#line 1777 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 475:

/* Line 1806 of yacc.c  */
#line 1781 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 476:

/* Line 1806 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 477:

/* Line 1806 of yacc.c  */
#line 1783 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 478:

/* Line 1806 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 479:

/* Line 1806 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 480:

/* Line 1806 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 481:

/* Line 1806 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 482:

/* Line 1806 of yacc.c  */
#line 1791 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 483:

/* Line 1806 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 484:

/* Line 1806 of yacc.c  */
#line 1793 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 485:

/* Line 1806 of yacc.c  */
#line 1797 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 486:

/* Line 1806 of yacc.c  */
#line 1798 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 487:

/* Line 1806 of yacc.c  */
#line 1799 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 488:

/* Line 1806 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 489:

/* Line 1806 of yacc.c  */
#line 1805 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 490:

/* Line 1806 of yacc.c  */
#line 1806 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 491:

/* Line 1806 of yacc.c  */
#line 1807 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 492:

/* Line 1806 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 493:

/* Line 1806 of yacc.c  */
#line 1809 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 494:

/* Line 1806 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 495:

/* Line 1806 of yacc.c  */
#line 1811 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 8657 "bison-chapel.cpp"
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



