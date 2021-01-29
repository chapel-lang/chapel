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

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;


/* Line 288 of yacc.c  */
#line 47 "chapel.ypp"

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
#line 67 "chapel.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_1_
  #define _BISON_CHAPEL_DEFINES_1_

  #include "symbol.h"

  #include "ImportStmt.h"

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
  class NamedExpr;
  class Type;

  enum   ProcIter {
    ProcIter_PROC,
    ProcIter_ITER
  };

  struct PotentialRename {
    enum{SINGLE, DOUBLE} tag;

    union {
      Expr*  elem;
      std::pair<Expr*, Expr*>* renamed;
    };
  };

  struct WhereAndLifetime {
    Expr* where;
    Expr* lifetime;
  };
  static inline
  WhereAndLifetime makeWhereAndLifetime(Expr* w, Expr* lt) {
    WhereAndLifetime ret;
    ret.where = w;
    ret.lifetime = lt;
    return ret;
  }

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
    ModuleSymbol*             pmodsymbol;
    ImportStmt*               pimportstmt;
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<PotentialRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif


/* Line 288 of yacc.c  */
#line 151 "chapel.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_2_
  #define _BISON_CHAPEL_DEFINES_2_

  struct YYLTYPE {
    int         first_line;
    int         first_column;
    int         last_line;
    int         last_column;
    const char* comment;
    const char* prevModule;
  };

  #define YYLTYPE_IS_DECLARED 1
  #define YYLTYPE_IS_TRIVIAL  1

  #endif


/* Line 288 of yacc.c  */
#line 174 "chapel.ypp"

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
#line 254 "bison-chapel.cpp"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TIDENT = 258,
     TQUERIEDIDENT = 259,
     INTLITERAL = 260,
     REALLITERAL = 261,
     IMAGLITERAL = 262,
     STRINGLITERAL = 263,
     BYTESLITERAL = 264,
     CSTRINGLITERAL = 265,
     EXTERNCODE = 266,
     TALIGN = 267,
     TAS = 268,
     TATOMIC = 269,
     TBEGIN = 270,
     TBREAK = 271,
     TBOOL = 272,
     TBORROWED = 273,
     TBY = 274,
     TBYTES = 275,
     TCATCH = 276,
     TCLASS = 277,
     TCOBEGIN = 278,
     TCOFORALL = 279,
     TCOMPLEX = 280,
     TCONFIG = 281,
     TCONST = 282,
     TCONTINUE = 283,
     TDEFER = 284,
     TDELETE = 285,
     TDMAPPED = 286,
     TDO = 287,
     TDOMAIN = 288,
     TELSE = 289,
     TENUM = 290,
     TEXCEPT = 291,
     TEXPORT = 292,
     TEXTERN = 293,
     TFALSE = 294,
     TFOR = 295,
     TFORALL = 296,
     TFORWARDING = 297,
     TIF = 298,
     TIMAG = 299,
     TIMPORT = 300,
     TIN = 301,
     TINCLUDE = 302,
     TINDEX = 303,
     TINLINE = 304,
     TINOUT = 305,
     TINT = 306,
     TITER = 307,
     TINITEQUALS = 308,
     TIMPLEMENTS = 309,
     TINTERFACE = 310,
     TLABEL = 311,
     TLAMBDA = 312,
     TLET = 313,
     TLIFETIME = 314,
     TLOCAL = 315,
     TLOCALE = 316,
     TMINUSMINUS = 317,
     TMODULE = 318,
     TNEW = 319,
     TNIL = 320,
     TNOINIT = 321,
     TNONE = 322,
     TNOTHING = 323,
     TON = 324,
     TONLY = 325,
     TOTHERWISE = 326,
     TOUT = 327,
     TOVERRIDE = 328,
     TOWNED = 329,
     TPARAM = 330,
     TPLUSPLUS = 331,
     TPRAGMA = 332,
     TPRIMITIVE = 333,
     TPRIVATE = 334,
     TPROC = 335,
     TPROTOTYPE = 336,
     TPUBLIC = 337,
     TREAL = 338,
     TRECORD = 339,
     TREDUCE = 340,
     TREF = 341,
     TREQUIRE = 342,
     TRETURN = 343,
     TSCAN = 344,
     TSELECT = 345,
     TSERIAL = 346,
     TSHARED = 347,
     TSINGLE = 348,
     TSPARSE = 349,
     TSTRING = 350,
     TSUBDOMAIN = 351,
     TSYNC = 352,
     TTHEN = 353,
     TTHIS = 354,
     TTHROW = 355,
     TTHROWS = 356,
     TTRUE = 357,
     TTRY = 358,
     TTRYBANG = 359,
     TTYPE = 360,
     TUINT = 361,
     TUNDERSCORE = 362,
     TUNION = 363,
     TUNMANAGED = 364,
     TUSE = 365,
     TVAR = 366,
     TVOID = 367,
     TWHEN = 368,
     TWHERE = 369,
     TWHILE = 370,
     TWITH = 371,
     TYIELD = 372,
     TZIP = 373,
     TALIAS = 374,
     TAND = 375,
     TASSIGN = 376,
     TASSIGNBAND = 377,
     TASSIGNBOR = 378,
     TASSIGNBXOR = 379,
     TASSIGNDIVIDE = 380,
     TASSIGNEXP = 381,
     TASSIGNLAND = 382,
     TASSIGNLOR = 383,
     TASSIGNMINUS = 384,
     TASSIGNMOD = 385,
     TASSIGNMULTIPLY = 386,
     TASSIGNPLUS = 387,
     TASSIGNREDUCE = 388,
     TASSIGNSL = 389,
     TASSIGNSR = 390,
     TBANG = 391,
     TBAND = 392,
     TBNOT = 393,
     TBOR = 394,
     TBXOR = 395,
     TCOLON = 396,
     TCOMMA = 397,
     TDIVIDE = 398,
     TDOT = 399,
     TDOTDOT = 400,
     TDOTDOTDOT = 401,
     TEQUAL = 402,
     TEXP = 403,
     TGREATER = 404,
     TGREATEREQUAL = 405,
     THASH = 406,
     TIO = 407,
     TLESS = 408,
     TLESSEQUAL = 409,
     TMINUS = 410,
     TMOD = 411,
     TNOTEQUAL = 412,
     TOR = 413,
     TPLUS = 414,
     TQUESTION = 415,
     TSEMI = 416,
     TSHIFTLEFT = 417,
     TSHIFTRIGHT = 418,
     TSTAR = 419,
     TSWAP = 420,
     TLCBR = 421,
     TRCBR = 422,
     TLP = 423,
     TRP = 424,
     TLSBR = 425,
     TRSBR = 426,
     TNOELSE = 427,
     TDOTDOTOPENHIGH = 428,
     TUMINUS = 429,
     TUPLUS = 430,
     TLNOT = 431
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
#line 205 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 340 of yacc.c  */
#line 498 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 504 "bison-chapel.cpp"
/* Unqualified %code blocks.  */

/* Line 344 of yacc.c  */
#line 39 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;
  bool        parsingPrivate=false;


/* Line 344 of yacc.c  */
#line 213 "chapel.ypp"

  #include "build.h"
  #include "CatchStmt.h"
  #include "DeferStmt.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForallStmt.h"
  #include "ForLoop.h"
  #include "IfExpr.h"
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
    setupError("parser", __FILE__, __LINE__, 3);

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
#line 585 "bison-chapel.cpp"

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
#define YYLAST   20453

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  177
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  161
/* YYNRULES -- Number of rules.  */
#define YYNRULES  683
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1224

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   431

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
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    17,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    45,    47,    49,    51,    53,    55,    57,    60,    63,
      67,    71,    75,    79,    83,    87,    91,    94,    98,   102,
     105,   108,   112,   115,   120,   124,   129,   134,   135,   137,
     139,   140,   142,   143,   145,   147,   148,   156,   159,   163,
     167,   169,   172,   174,   178,   182,   188,   190,   194,   198,
     202,   208,   214,   215,   217,   219,   221,   222,   224,   226,
     231,   238,   247,   256,   263,   272,   281,   286,   288,   292,
     298,   300,   304,   308,   313,   318,   323,   328,   333,   338,
     339,   341,   343,   345,   347,   349,   351,   353,   355,   357,
     359,   361,   363,   365,   367,   369,   371,   373,   375,   377,
     379,   381,   383,   385,   387,   389,   391,   393,   395,   397,
     399,   401,   403,   405,   407,   409,   411,   413,   415,   417,
     419,   421,   423,   425,   427,   429,   431,   433,   435,   437,
     439,   442,   444,   447,   451,   453,   455,   458,   459,   463,
     465,   467,   469,   471,   473,   475,   477,   481,   487,   493,
     496,   497,   506,   507,   517,   518,   527,   528,   538,   542,
     546,   552,   558,   561,   567,   571,   578,   585,   590,   596,
     602,   606,   610,   617,   623,   630,   636,   643,   647,   652,
     656,   661,   668,   676,   683,   691,   696,   702,   707,   713,
     718,   723,   727,   734,   740,   745,   749,   756,   762,   769,
     775,   784,   792,   797,   802,   809,   813,   815,   819,   821,
     823,   825,   827,   829,   831,   833,   835,   837,   839,   841,
     843,   845,   847,   849,   851,   853,   855,   857,   859,   866,
     871,   879,   885,   889,   896,   899,   903,   907,   910,   913,
     917,   921,   922,   925,   928,   932,   938,   940,   944,   948,
     954,   960,   961,   964,   968,   971,   975,   982,   989,   991,
     993,   995,   996,   999,  1000,  1003,  1007,  1013,  1019,  1021,
    1023,  1026,  1030,  1032,  1036,  1037,  1038,  1047,  1048,  1050,
    1052,  1053,  1054,  1065,  1069,  1073,  1079,  1085,  1089,  1091,
    1095,  1097,  1099,  1101,  1103,  1105,  1107,  1109,  1111,  1113,
    1115,  1117,  1119,  1121,  1123,  1125,  1127,  1129,  1131,  1133,
    1135,  1137,  1139,  1141,  1143,  1145,  1147,  1150,  1152,  1154,
    1156,  1158,  1160,  1162,  1164,  1166,  1168,  1170,  1172,  1174,
    1175,  1179,  1183,  1184,  1186,  1190,  1195,  1201,  1206,  1212,
    1219,  1226,  1227,  1229,  1231,  1233,  1235,  1237,  1240,  1243,
    1245,  1247,  1249,  1250,  1252,  1254,  1257,  1259,  1261,  1263,
    1265,  1266,  1268,  1271,  1273,  1275,  1277,  1278,  1280,  1282,
    1284,  1286,  1288,  1290,  1292,  1295,  1298,  1299,  1302,  1305,
    1310,  1315,  1317,  1321,  1325,  1329,  1333,  1337,  1341,  1345,
    1348,  1350,  1352,  1356,  1361,  1366,  1369,  1374,  1375,  1378,
    1381,  1383,  1385,  1387,  1390,  1392,  1397,  1401,  1403,  1407,
    1411,  1417,  1419,  1421,  1425,  1427,  1430,  1434,  1435,  1438,
    1441,  1445,  1448,  1453,  1457,  1461,  1466,  1470,  1471,  1474,
    1477,  1480,  1482,  1483,  1486,  1489,  1492,  1494,  1499,  1504,
    1511,  1515,  1516,  1518,  1520,  1524,  1529,  1533,  1538,  1545,
    1546,  1549,  1552,  1555,  1558,  1560,  1562,  1566,  1570,  1572,
    1576,  1578,  1580,  1582,  1586,  1590,  1591,  1593,  1595,  1599,
    1603,  1607,  1609,  1611,  1613,  1615,  1617,  1620,  1622,  1624,
    1626,  1628,  1630,  1632,  1635,  1640,  1645,  1650,  1656,  1659,
    1662,  1664,  1667,  1669,  1672,  1674,  1677,  1679,  1682,  1684,
    1686,  1693,  1700,  1705,  1715,  1725,  1733,  1740,  1747,  1752,
    1762,  1772,  1780,  1785,  1792,  1799,  1809,  1819,  1826,  1828,
    1830,  1832,  1834,  1836,  1838,  1840,  1842,  1846,  1847,  1849,
    1854,  1856,  1860,  1865,  1867,  1871,  1876,  1880,  1884,  1886,
    1888,  1891,  1893,  1896,  1898,  1900,  1904,  1906,  1909,  1912,
    1915,  1918,  1921,  1930,  1939,  1949,  1959,  1964,  1966,  1968,
    1970,  1972,  1974,  1976,  1978,  1980,  1982,  1987,  1991,  1995,
    1999,  2002,  2005,  2008,  2010,  2011,  2013,  2016,  2019,  2021,
    2023,  2025,  2027,  2029,  2031,  2034,  2037,  2039,  2041,  2046,
    2051,  2056,  2060,  2064,  2068,  2072,  2078,  2082,  2087,  2091,
    2096,  2098,  2100,  2102,  2104,  2106,  2108,  2110,  2112,  2114,
    2116,  2118,  2122,  2127,  2131,  2136,  2140,  2145,  2149,  2155,
    2159,  2163,  2167,  2171,  2175,  2179,  2183,  2187,  2191,  2195,
    2199,  2203,  2207,  2211,  2215,  2219,  2223,  2227,  2231,  2235,
    2239,  2243,  2247,  2250,  2253,  2256,  2259,  2262,  2265,  2268,
    2272,  2276,  2280,  2284,  2288,  2292,  2296,  2300,  2302,  2304,
    2306,  2308,  2310,  2312
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     178,     0,    -1,   179,    -1,    -1,   179,   180,    -1,   182,
      -1,   181,   182,    -1,    77,     8,    -1,   181,    77,     8,
      -1,   184,    -1,   188,    -1,   190,    -1,   197,    -1,   198,
      -1,   201,    -1,   212,    -1,   202,    -1,   221,    -1,   224,
      -1,   231,    -1,   226,    -1,   222,    -1,   239,    -1,   233,
      -1,   234,    -1,   238,    -1,   211,    -1,   309,   161,    -1,
      14,   182,    -1,    15,   310,   182,    -1,    16,   203,   161,
      -1,    23,   310,   190,    -1,    28,   203,   161,    -1,    30,
     297,   161,    -1,    56,   205,   182,    -1,    60,   321,   210,
      -1,    60,   210,    -1,    69,   321,   210,    -1,    91,   321,
     210,    -1,    91,   210,    -1,    97,   182,    -1,   117,   321,
     161,    -1,     1,   161,    -1,   185,   186,    63,   205,    -1,
     183,   166,   167,    -1,   183,   166,   191,   167,    -1,   183,
     166,     1,   167,    -1,    -1,    82,    -1,    79,    -1,    -1,
      81,    -1,    -1,    82,    -1,    79,    -1,    -1,    47,   189,
     187,   186,    63,   205,   161,    -1,   166,   167,    -1,   166,
     191,   167,    -1,   166,     1,   167,    -1,   180,    -1,   191,
     180,    -1,   321,    -1,   321,    13,   321,    -1,   192,   142,
     321,    -1,   192,   142,   321,    13,   321,    -1,   321,    -1,
     321,    13,   321,    -1,   321,    13,   107,    -1,   193,   142,
     321,    -1,   193,   142,   321,    13,   321,    -1,   193,   142,
     321,    13,   107,    -1,    -1,   192,    -1,   164,    -1,   192,
      -1,    -1,    82,    -1,    79,    -1,   196,   110,   193,   161,
      -1,   196,   110,   321,    36,   195,   161,    -1,   196,   110,
     321,    13,   321,    36,   195,   161,    -1,   196,   110,   321,
      13,   107,    36,   195,   161,    -1,   196,   110,   321,    70,
     194,   161,    -1,   196,   110,   321,    13,   321,    70,   194,
     161,    -1,   196,   110,   321,    13,   107,    70,   194,   161,
      -1,   196,    45,   200,   161,    -1,   321,    -1,   321,    13,
     206,    -1,   321,   144,   166,   192,   167,    -1,   199,    -1,
     200,   142,   199,    -1,    87,   296,   161,    -1,   324,   260,
     323,   161,    -1,   324,   165,   323,   161,    -1,   324,   133,
     323,   161,    -1,   324,   127,   323,   161,    -1,   324,   128,
     323,   161,    -1,   324,   121,    66,   161,    -1,    -1,     3,
      -1,     3,    -1,    67,    -1,    99,    -1,    39,    -1,   102,
      -1,   207,    -1,     3,    -1,    67,    -1,    99,    -1,    39,
      -1,   102,    -1,   207,    -1,     3,    -1,    99,    -1,    17,
      -1,    51,    -1,   106,    -1,    83,    -1,    44,    -1,    25,
      -1,    20,    -1,    95,    -1,    97,    -1,    93,    -1,    74,
      -1,    92,    -1,    18,    -1,   109,    -1,    33,    -1,    48,
      -1,    61,    -1,    68,    -1,   112,    -1,    17,    -1,    35,
      -1,    51,    -1,   106,    -1,    83,    -1,    44,    -1,    25,
      -1,    20,    -1,    95,    -1,    61,    -1,    68,    -1,   112,
      -1,    97,    -1,    93,    -1,    33,    -1,    48,    -1,    32,
     182,    -1,   190,    -1,    88,   161,    -1,    88,   323,   161,
      -1,   161,    -1,   214,    -1,    82,   214,    -1,    -1,    79,
     213,   214,    -1,   254,    -1,   283,    -1,   246,    -1,   279,
      -1,   242,    -1,   215,    -1,   216,    -1,    42,   321,   161,
      -1,    42,   321,    36,   195,   161,    -1,    42,   321,    70,
     194,   161,    -1,    42,   283,    -1,    -1,    38,    84,   217,
     205,   244,   166,   245,   167,    -1,    -1,    38,     8,    84,
     218,   205,   244,   166,   245,   167,    -1,    -1,    37,    84,
     219,   205,   244,   166,   245,   167,    -1,    -1,    37,     8,
      84,   220,   205,   244,   166,   245,   167,    -1,    38,   322,
     254,    -1,    37,   322,   254,    -1,    38,   322,   282,   284,
     161,    -1,    37,   322,   282,   284,   161,    -1,    38,    11,
      -1,    32,   182,   115,   321,   161,    -1,   115,   321,   210,
      -1,    24,   321,    46,   321,   310,   210,    -1,    24,   321,
      46,   223,   310,   210,    -1,    24,   321,   310,   210,    -1,
      40,   321,    46,   321,   210,    -1,    40,   321,    46,   223,
     210,    -1,    40,   321,   210,    -1,    40,   223,   210,    -1,
      40,    75,   205,    46,   321,   210,    -1,    41,   321,    46,
     321,   210,    -1,    41,   321,    46,   321,   313,   210,    -1,
      41,   321,    46,   223,   210,    -1,    41,   321,    46,   223,
     313,   210,    -1,    41,   321,   210,    -1,    41,   321,   313,
     210,    -1,    41,   223,   210,    -1,    41,   223,   313,   210,
      -1,   170,   296,    46,   321,   171,   182,    -1,   170,   296,
      46,   321,   313,   171,   182,    -1,   170,   296,    46,   223,
     171,   182,    -1,   170,   296,    46,   223,   313,   171,   182,
      -1,   170,   296,   171,   182,    -1,   170,   296,   313,   171,
     182,    -1,   170,   223,   171,   182,    -1,   170,   223,   313,
     171,   182,    -1,   118,   168,   296,   169,    -1,    43,   321,
      98,   182,    -1,    43,   321,   190,    -1,    43,   321,    98,
     182,    34,   182,    -1,    43,   321,   190,    34,   182,    -1,
      43,   225,    98,   182,    -1,    43,   225,   190,    -1,    43,
     225,    98,   182,    34,   182,    -1,    43,   225,   190,    34,
     182,    -1,    43,   321,   260,   321,    98,   182,    -1,    43,
     321,   260,   321,   190,    -1,    43,   321,   260,   321,    98,
     182,    34,   182,    -1,    43,   321,   260,   321,   190,    34,
     182,    -1,   111,   205,   121,   321,    -1,    27,   205,   121,
     321,    -1,    55,   205,   168,   227,   169,   190,    -1,    55,
     205,   190,    -1,   228,    -1,   227,   142,   228,    -1,   205,
      -1,     3,    -1,    17,    -1,    51,    -1,   106,    -1,    83,
      -1,    44,    -1,    25,    -1,    20,    -1,    95,    -1,    61,
      -1,    68,    -1,   112,    -1,   230,    -1,    67,    -1,    99,
      -1,    39,    -1,   102,    -1,    33,    -1,    48,    -1,    54,
     205,   168,   301,   169,   161,    -1,   229,    54,   205,   161,
      -1,   229,    54,   205,   168,   301,   169,   161,    -1,    54,
     205,   168,   301,   169,    -1,   229,    54,   205,    -1,   229,
      54,   205,   168,   301,   169,    -1,    29,   182,    -1,   103,
     321,   161,    -1,   104,   321,   161,    -1,   103,   202,    -1,
     104,   202,    -1,   103,   190,   235,    -1,   104,   190,   235,
      -1,    -1,   235,   236,    -1,    21,   190,    -1,    21,   237,
     190,    -1,    21,   168,   237,   169,   190,    -1,   205,    -1,
     205,   141,   321,    -1,   100,   321,   161,    -1,    90,   321,
     166,   240,   167,    -1,    90,   321,   166,     1,   167,    -1,
      -1,   240,   241,    -1,   113,   296,   210,    -1,    71,   182,
      -1,    71,    32,   182,    -1,   243,   205,   244,   166,   245,
     167,    -1,   243,   205,   244,   166,     1,   167,    -1,    22,
      -1,    84,    -1,   108,    -1,    -1,   141,   296,    -1,    -1,
     245,   212,    -1,   245,   181,   212,    -1,   247,   205,   166,
     248,   167,    -1,   247,   205,   166,     1,   167,    -1,    35,
      -1,   249,    -1,   248,   142,    -1,   248,   142,   249,    -1,
     205,    -1,   205,   121,   321,    -1,    -1,    -1,    57,   251,
     262,   252,   269,   291,   275,   272,    -1,    -1,    49,    -1,
      73,    -1,    -1,    -1,   253,   268,   255,   257,   256,   269,
     290,   270,   275,   271,    -1,   267,   259,   261,    -1,   267,
     260,   261,    -1,   267,   258,   144,   259,   261,    -1,   267,
     258,   144,   260,   261,    -1,   267,     1,   261,    -1,   303,
      -1,   168,   321,   169,    -1,   204,    -1,   137,    -1,   139,
      -1,   140,    -1,   138,    -1,   147,    -1,   157,    -1,   154,
      -1,   150,    -1,   153,    -1,   149,    -1,   159,    -1,   155,
      -1,   164,    -1,   143,    -1,   162,    -1,   163,    -1,   156,
      -1,   148,    -1,   136,    -1,    19,    -1,   151,    -1,    12,
      -1,   165,    -1,   152,    -1,    53,    -1,   205,   136,    -1,
     121,    -1,   132,    -1,   129,    -1,   131,    -1,   125,    -1,
     130,    -1,   126,    -1,   122,    -1,   123,    -1,   124,    -1,
     135,    -1,   134,    -1,    -1,   168,   263,   169,    -1,   168,
     263,   169,    -1,    -1,   264,    -1,   263,   142,   264,    -1,
     265,   205,   295,   288,    -1,   181,   265,   205,   295,   288,
      -1,   265,   205,   295,   274,    -1,   181,   265,   205,   295,
     274,    -1,   265,   168,   287,   169,   295,   288,    -1,   265,
     168,   287,   169,   295,   274,    -1,    -1,   266,    -1,    46,
      -1,    50,    -1,    72,    -1,    27,    -1,    27,    46,    -1,
      27,    86,    -1,    75,    -1,    86,    -1,   105,    -1,    -1,
      75,    -1,    86,    -1,    27,    86,    -1,    27,    -1,   105,
      -1,    80,    -1,    52,    -1,    -1,    27,    -1,    27,    86,
      -1,    86,    -1,    75,    -1,   105,    -1,    -1,   101,    -1,
     161,    -1,   272,    -1,   190,    -1,   211,    -1,     4,    -1,
     146,    -1,   146,   321,    -1,   146,   273,    -1,    -1,   114,
     321,    -1,    59,   276,    -1,   114,   321,    59,   276,    -1,
      59,   276,   114,   321,    -1,   277,    -1,   276,   142,   277,
      -1,   278,   121,   278,    -1,   278,   153,   278,    -1,   278,
     154,   278,    -1,   278,   147,   278,    -1,   278,   149,   278,
      -1,   278,   150,   278,    -1,    88,   278,    -1,     3,    -1,
      99,    -1,   105,   280,   161,    -1,    26,   105,   280,   161,
      -1,    38,   105,   280,   161,    -1,   205,   281,    -1,   205,
     281,   142,   280,    -1,    -1,   121,   304,    -1,   121,   292,
      -1,    75,    -1,    27,    -1,    86,    -1,    27,    86,    -1,
     111,    -1,    26,   282,   284,   161,    -1,   282,   284,   161,
      -1,   285,    -1,   284,   142,   285,    -1,   205,   291,   288,
      -1,   168,   287,   169,   291,   288,    -1,   107,    -1,   205,
      -1,   168,   287,   169,    -1,   286,    -1,   286,   142,    -1,
     286,   142,   287,    -1,    -1,   121,    66,    -1,   121,   323,
      -1,   170,   171,   304,    -1,   170,   171,    -1,   170,   296,
     171,   304,    -1,   170,   296,   171,    -1,   170,   171,   289,
      -1,   170,   296,   171,   289,    -1,   170,     1,   171,    -1,
      -1,   141,   304,    -1,   141,   289,    -1,   141,   209,    -1,
       1,    -1,    -1,   141,   304,    -1,   141,   292,    -1,   141,
     209,    -1,     1,    -1,   170,   296,   171,   304,    -1,   170,
     296,   171,   292,    -1,   170,   296,    46,   321,   171,   304,
      -1,   170,     1,   171,    -1,    -1,   304,    -1,   273,    -1,
     170,   171,   293,    -1,   170,   296,   171,   293,    -1,   170,
     171,   294,    -1,   170,   296,   171,   294,    -1,   170,   296,
      46,   321,   171,   293,    -1,    -1,   141,   304,    -1,   141,
     273,    -1,   141,   209,    -1,   141,   294,    -1,   321,    -1,
     273,    -1,   296,   142,   321,    -1,   296,   142,   273,    -1,
     321,    -1,   297,   142,   321,    -1,   107,    -1,   323,    -1,
     273,    -1,   298,   142,   298,    -1,   299,   142,   298,    -1,
      -1,   301,    -1,   302,    -1,   301,   142,   302,    -1,   206,
     121,   273,    -1,   206,   121,   323,    -1,   273,    -1,   323,
      -1,   206,    -1,   208,    -1,   305,    -1,   305,   160,    -1,
     160,    -1,   308,    -1,   324,    -1,   307,    -1,   334,    -1,
     333,    -1,    93,   321,    -1,    48,   168,   300,   169,    -1,
      33,   168,   300,   169,    -1,    96,   168,   300,   169,    -1,
      94,    96,   168,   300,   169,    -1,    14,   321,    -1,    97,
     321,    -1,    74,    -1,    74,   321,    -1,   109,    -1,   109,
     321,    -1,    92,    -1,    92,   321,    -1,    18,    -1,    18,
     321,    -1,    22,    -1,    84,    -1,    40,   321,    46,   321,
      32,   321,    -1,    40,   321,    46,   223,    32,   321,    -1,
      40,   321,    32,   321,    -1,    40,   321,    46,   321,    32,
      43,   321,    98,   321,    -1,    40,   321,    46,   223,    32,
      43,   321,    98,   321,    -1,    40,   321,    32,    43,   321,
      98,   321,    -1,    41,   321,    46,   321,    32,   321,    -1,
      41,   321,    46,   223,    32,   321,    -1,    41,   321,    32,
     321,    -1,    41,   321,    46,   321,    32,    43,   321,    98,
     321,    -1,    41,   321,    46,   223,    32,    43,   321,    98,
     321,    -1,    41,   321,    32,    43,   321,    98,   321,    -1,
     170,   296,   171,   321,    -1,   170,   296,    46,   321,   171,
     321,    -1,   170,   296,    46,   223,   171,   321,    -1,   170,
     296,    46,   321,   171,    43,   321,    98,   321,    -1,   170,
     296,    46,   223,   171,    43,   321,    98,   321,    -1,    43,
     321,    98,   321,    34,   321,    -1,    65,    -1,   308,    -1,
     303,    -1,   327,    -1,   326,    -1,   250,    -1,   319,    -1,
     320,    -1,   317,   152,   321,    -1,    -1,   311,    -1,   116,
     168,   312,   169,    -1,   315,    -1,   312,   142,   315,    -1,
     116,   168,   314,   169,    -1,   315,    -1,   314,   142,   315,
      -1,   316,   303,   291,   288,    -1,   337,    85,   303,    -1,
     321,    85,   303,    -1,    27,    -1,    46,    -1,    27,    46,
      -1,    86,    -1,    27,    86,    -1,   111,    -1,   324,    -1,
     317,   152,   321,    -1,    64,    -1,    64,    74,    -1,    64,
      92,    -1,    64,   109,    -1,    64,    18,    -1,   318,   321,
      -1,    64,    74,   168,   321,   169,   168,   300,   169,    -1,
      64,    92,   168,   321,   169,   168,   300,   169,    -1,    64,
      74,   168,   321,   169,   168,   300,   169,   160,    -1,    64,
      92,   168,   321,   169,   168,   300,   169,   160,    -1,    58,
     284,    46,   321,    -1,   331,    -1,   304,    -1,   306,    -1,
     335,    -1,   336,    -1,   250,    -1,   319,    -1,   320,    -1,
     232,    -1,   168,   146,   321,   169,    -1,   321,   141,   321,
      -1,   321,   145,   321,    -1,   321,   173,   321,    -1,   321,
     145,    -1,   145,   321,    -1,   173,   321,    -1,   145,    -1,
      -1,   321,    -1,   103,   321,    -1,   104,   321,    -1,   321,
      -1,   303,    -1,   326,    -1,   327,    -1,   328,    -1,   324,
      -1,   321,   136,    -1,   305,   160,    -1,   250,    -1,   330,
      -1,   325,   168,   300,   169,    -1,   325,   170,   300,   171,
      -1,    78,   168,   300,   169,    -1,   321,   144,   206,    -1,
     321,   144,   105,    -1,   321,   144,    33,    -1,   321,   144,
      61,    -1,   321,   144,    20,   168,   169,    -1,   168,   298,
     169,    -1,   168,   298,   142,   169,    -1,   168,   299,   169,
      -1,   168,   299,   142,   169,    -1,    39,    -1,   102,    -1,
       8,    -1,     9,    -1,   329,    -1,   330,    -1,     5,    -1,
       6,    -1,     7,    -1,    10,    -1,    67,    -1,   166,   296,
     167,    -1,   166,   296,   142,   167,    -1,   170,   296,   171,
      -1,   170,   296,   142,   171,    -1,   170,   332,   171,    -1,
     170,   332,   142,   171,    -1,   321,   119,   321,    -1,   332,
     142,   321,   119,   321,    -1,   321,   159,   321,    -1,   321,
     155,   321,    -1,   321,   164,   321,    -1,   321,   143,   321,
      -1,   321,   162,   321,    -1,   321,   163,   321,    -1,   321,
     156,   321,    -1,   321,   147,   321,    -1,   321,   157,   321,
      -1,   321,   154,   321,    -1,   321,   150,   321,    -1,   321,
     153,   321,    -1,   321,   149,   321,    -1,   321,   137,   321,
      -1,   321,   139,   321,    -1,   321,   140,   321,    -1,   321,
     120,   321,    -1,   321,   158,   321,    -1,   321,   148,   321,
      -1,   321,    19,   321,    -1,   321,    12,   321,    -1,   321,
     151,   321,    -1,   321,    31,   321,    -1,   159,   321,    -1,
     155,   321,    -1,    62,   321,    -1,    76,   321,    -1,   136,
     321,    -1,   321,   136,    -1,   138,   321,    -1,   321,    85,
     321,    -1,   321,    85,   223,    -1,   337,    85,   321,    -1,
     337,    85,   223,    -1,   321,    89,   321,    -1,   321,    89,
     223,    -1,   337,    89,   321,    -1,   337,    89,   223,    -1,
     159,    -1,   164,    -1,   120,    -1,   158,    -1,   137,    -1,
     139,    -1,   140,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   510,   510,   515,   516,   522,   523,   528,   529,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   571,   584,   589,   594,   602,   603,   604,
     608,   609,   613,   614,   615,   620,   619,   640,   641,   642,
     647,   648,   653,   658,   663,   667,   676,   681,   686,   691,
     695,   699,   707,   712,   716,   721,   725,   726,   727,   731,
     732,   733,   734,   735,   736,   737,   741,   746,   747,   748,
     752,   753,   757,   761,   763,   765,   767,   769,   771,   778,
     779,   783,   784,   785,   786,   787,   788,   791,   792,   793,
     794,   795,   796,   808,   809,   820,   821,   822,   823,   824,
     825,   826,   827,   828,   829,   830,   831,   832,   833,   834,
     835,   836,   837,   838,   842,   843,   844,   845,   846,   847,
     848,   849,   850,   851,   852,   853,   860,   861,   862,   863,
     867,   868,   872,   873,   877,   878,   879,   889,   889,   894,
     895,   896,   897,   898,   899,   900,   904,   905,   906,   907,
     912,   911,   927,   926,   943,   942,   958,   957,   973,   977,
     982,   990,  1001,  1008,  1009,  1010,  1011,  1012,  1013,  1014,
    1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,
    1025,  1026,  1032,  1038,  1044,  1050,  1057,  1064,  1068,  1075,
    1079,  1080,  1081,  1082,  1084,  1085,  1086,  1087,  1089,  1091,
    1093,  1095,  1100,  1101,  1105,  1107,  1115,  1116,  1121,  1126,
    1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,
    1137,  1138,  1145,  1146,  1147,  1148,  1157,  1158,  1162,  1164,
    1167,  1173,  1175,  1178,  1184,  1187,  1188,  1189,  1190,  1191,
    1192,  1196,  1197,  1201,  1202,  1203,  1207,  1208,  1212,  1215,
    1217,  1222,  1223,  1227,  1229,  1231,  1238,  1248,  1262,  1267,
    1272,  1280,  1281,  1286,  1287,  1289,  1294,  1310,  1317,  1326,
    1334,  1338,  1345,  1346,  1351,  1356,  1350,  1383,  1386,  1390,
    1398,  1408,  1397,  1440,  1444,  1449,  1453,  1458,  1465,  1466,
    1470,  1471,  1472,  1473,  1474,  1475,  1476,  1477,  1478,  1479,
    1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,  1488,  1489,
    1490,  1491,  1492,  1493,  1494,  1495,  1496,  1500,  1501,  1502,
    1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,  1515,
    1516,  1520,  1524,  1525,  1526,  1530,  1532,  1534,  1536,  1538,
    1540,  1545,  1546,  1550,  1551,  1552,  1553,  1554,  1555,  1556,
    1557,  1558,  1562,  1563,  1564,  1565,  1566,  1567,  1571,  1572,
    1576,  1577,  1578,  1579,  1580,  1581,  1585,  1586,  1589,  1590,
    1594,  1595,  1599,  1603,  1604,  1605,  1613,  1614,  1616,  1618,
    1620,  1625,  1627,  1632,  1633,  1634,  1635,  1636,  1637,  1638,
    1642,  1644,  1649,  1651,  1653,  1658,  1671,  1688,  1689,  1691,
    1696,  1697,  1698,  1699,  1700,  1704,  1710,  1718,  1719,  1727,
    1729,  1734,  1736,  1738,  1743,  1745,  1747,  1754,  1755,  1756,
    1761,  1763,  1765,  1769,  1773,  1775,  1779,  1787,  1788,  1789,
    1790,  1791,  1796,  1797,  1798,  1799,  1800,  1820,  1824,  1828,
    1836,  1843,  1844,  1845,  1849,  1851,  1857,  1859,  1861,  1866,
    1867,  1868,  1869,  1870,  1876,  1877,  1878,  1879,  1883,  1884,
    1888,  1889,  1890,  1894,  1895,  1899,  1900,  1904,  1905,  1909,
    1910,  1911,  1912,  1916,  1917,  1928,  1930,  1932,  1938,  1939,
    1940,  1941,  1942,  1943,  1945,  1947,  1949,  1951,  1953,  1955,
    1958,  1960,  1962,  1964,  1966,  1968,  1970,  1972,  1975,  1977,
    1982,  1984,  1986,  1988,  1990,  1992,  1994,  1996,  1998,  2000,
    2002,  2004,  2006,  2013,  2019,  2025,  2031,  2040,  2050,  2058,
    2059,  2060,  2061,  2062,  2063,  2064,  2065,  2070,  2071,  2075,
    2079,  2080,  2084,  2088,  2089,  2093,  2097,  2101,  2108,  2109,
    2110,  2111,  2112,  2113,  2117,  2118,  2123,  2125,  2129,  2133,
    2137,  2145,  2150,  2156,  2162,  2169,  2179,  2187,  2188,  2189,
    2190,  2191,  2192,  2193,  2194,  2195,  2196,  2198,  2200,  2202,
    2217,  2219,  2221,  2223,  2228,  2229,  2233,  2234,  2235,  2239,
    2240,  2241,  2242,  2251,  2252,  2253,  2254,  2255,  2259,  2260,
    2261,  2265,  2266,  2267,  2268,  2269,  2277,  2278,  2279,  2280,
    2284,  2285,  2289,  2290,  2294,  2295,  2296,  2297,  2298,  2299,
    2300,  2301,  2303,  2305,  2306,  2307,  2311,  2319,  2320,  2324,
    2325,  2326,  2327,  2328,  2329,  2330,  2331,  2332,  2333,  2334,
    2335,  2336,  2337,  2338,  2339,  2340,  2341,  2342,  2343,  2344,
    2345,  2346,  2351,  2352,  2353,  2354,  2355,  2356,  2357,  2361,
    2362,  2363,  2364,  2368,  2369,  2370,  2371,  2376,  2377,  2378,
    2379,  2380,  2381,  2382
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "TQUERIEDIDENT", "INTLITERAL",
  "REALLITERAL", "IMAGLITERAL", "STRINGLITERAL", "BYTESLITERAL",
  "CSTRINGLITERAL", "EXTERNCODE", "TALIGN", "TAS", "TATOMIC", "TBEGIN",
  "TBREAK", "TBOOL", "TBORROWED", "TBY", "TBYTES", "TCATCH", "TCLASS",
  "TCOBEGIN", "TCOFORALL", "TCOMPLEX", "TCONFIG", "TCONST", "TCONTINUE",
  "TDEFER", "TDELETE", "TDMAPPED", "TDO", "TDOMAIN", "TELSE", "TENUM",
  "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE", "TFOR", "TFORALL",
  "TFORWARDING", "TIF", "TIMAG", "TIMPORT", "TIN", "TINCLUDE", "TINDEX",
  "TINLINE", "TINOUT", "TINT", "TITER", "TINITEQUALS", "TIMPLEMENTS",
  "TINTERFACE", "TLABEL", "TLAMBDA", "TLET", "TLIFETIME", "TLOCAL",
  "TLOCALE", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TNONE",
  "TNOTHING", "TON", "TONLY", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED",
  "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC",
  "TPROTOTYPE", "TPUBLIC", "TREAL", "TRECORD", "TREDUCE", "TREF",
  "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSHARED",
  "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS",
  "TTHROW", "TTHROWS", "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT",
  "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TVOID", "TWHEN",
  "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND",
  "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL",
  "TASSIGNSR", "TBANG", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON",
  "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP",
  "TGREATER", "TGREATEREQUAL", "THASH", "TIO", "TLESS", "TLESSEQUAL",
  "TMINUS", "TMOD", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP",
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TDOTDOTOPENHIGH", "TUMINUS",
  "TUPLUS", "TLNOT", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "module_decl_start",
  "module_decl_stmt", "access_control", "opt_prototype",
  "include_access_control", "include_module_stmt", "$@1", "block_stmt",
  "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls", "except_ls",
  "use_access_control", "use_stmt", "import_stmt", "import_expr",
  "import_ls", "require_stmt", "assignment_stmt", "opt_label_ident",
  "ident_fn_def", "ident_def", "ident_use", "internal_type_ident_def",
  "scalar_type", "reserved_type_ident_use", "do_stmt", "return_stmt",
  "class_level_stmt", "@2", "private_decl", "forwarding_stmt",
  "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@7", "$@8", "linkage_spec",
  "fn_decl_stmt", "$@9", "$@10", "fn_decl_stmt_inner",
  "fn_decl_receiver_expr", "fn_ident", "assignop_ident", "opt_formal_ls",
  "req_formal_ls", "formal_ls", "formal", "opt_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_or_iter",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "var_arg_expr", "opt_lifetime_where",
  "lifetime_components_expr", "lifetime_expr", "lifetime_ident",
  "type_alias_decl_stmt", "type_alias_decl_stmt_inner", "opt_init_type",
  "var_decl_type", "var_decl_stmt", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "ret_array_type",
  "opt_ret_type", "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "opt_formal_type", "expr_ls", "simple_expr_ls",
  "tuple_component", "tuple_expr_ls", "opt_actual_ls", "actual_ls",
  "actual_expr", "ident_expr", "type_level_expr", "sub_type_level_expr",
  "for_expr", "cond_expr", "nil_expr", "stmt_level_expr",
  "opt_task_intent_ls", "task_intent_clause", "task_intent_ls",
  "forall_intent_clause", "forall_intent_ls", "intent_expr",
  "shadow_var_prefix", "io_expr", "new_maybe_decorated", "new_expr",
  "let_expr", "expr", "opt_expr", "opt_try_expr", "lhs_expr",
  "call_base_expr", "call_expr", "dot_expr", "parenthesized_expr",
  "bool_literal", "str_bytes_literal", "literal_expr", "assoc_expr_ls",
  "binary_op_expr", "unary_op_expr", "reduce_expr", "scan_expr",
  "reduce_scan_op_expr", 0
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
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   177,   178,   179,   179,   180,   180,   181,   181,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   183,   184,   184,   184,   185,   185,   185,
     186,   186,   187,   187,   187,   189,   188,   190,   190,   190,
     191,   191,   192,   192,   192,   192,   193,   193,   193,   193,
     193,   193,   194,   194,   195,   195,   196,   196,   196,   197,
     197,   197,   197,   197,   197,   197,   198,   199,   199,   199,
     200,   200,   201,   202,   202,   202,   202,   202,   202,   203,
     203,   204,   204,   204,   204,   204,   204,   205,   205,   205,
     205,   205,   205,   206,   206,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   209,   209,   209,   209,
     210,   210,   211,   211,   212,   212,   212,   213,   212,   214,
     214,   214,   214,   214,   214,   214,   215,   215,   215,   215,
     217,   216,   218,   216,   219,   216,   220,   216,   216,   216,
     216,   216,   221,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   223,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   225,   225,   226,   226,   227,   227,   228,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   230,   230,   230,   230,   230,   230,   231,   231,
     231,   232,   232,   232,   233,   234,   234,   234,   234,   234,
     234,   235,   235,   236,   236,   236,   237,   237,   238,   239,
     239,   240,   240,   241,   241,   241,   242,   242,   243,   243,
     243,   244,   244,   245,   245,   245,   246,   246,   247,   248,
     248,   248,   249,   249,   251,   252,   250,   253,   253,   253,
     255,   256,   254,   257,   257,   257,   257,   257,   258,   258,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   261,
     261,   262,   263,   263,   263,   264,   264,   264,   264,   264,
     264,   265,   265,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   267,   267,   267,   267,   267,   267,   268,   268,
     269,   269,   269,   269,   269,   269,   270,   270,   271,   271,
     272,   272,   273,   274,   274,   274,   275,   275,   275,   275,
     275,   276,   276,   277,   277,   277,   277,   277,   277,   277,
     278,   278,   279,   279,   279,   280,   280,   281,   281,   281,
     282,   282,   282,   282,   282,   283,   283,   284,   284,   285,
     285,   286,   286,   286,   287,   287,   287,   288,   288,   288,
     289,   289,   289,   289,   289,   289,   289,   290,   290,   290,
     290,   290,   291,   291,   291,   291,   291,   292,   292,   292,
     292,   293,   293,   293,   294,   294,   294,   294,   294,   295,
     295,   295,   295,   295,   296,   296,   296,   296,   297,   297,
     298,   298,   298,   299,   299,   300,   300,   301,   301,   302,
     302,   302,   302,   303,   303,   304,   304,   304,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   307,   308,   309,
     309,   309,   309,   309,   309,   309,   309,   310,   310,   311,
     312,   312,   313,   314,   314,   315,   315,   315,   316,   316,
     316,   316,   316,   316,   317,   317,   318,   318,   318,   318,
     318,   319,   319,   319,   319,   319,   320,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   322,   322,   323,   323,   323,   324,
     324,   324,   324,   325,   325,   325,   325,   325,   326,   326,
     326,   327,   327,   327,   327,   327,   328,   328,   328,   328,
     329,   329,   330,   330,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   332,   332,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   334,   334,   334,   334,   334,   334,   334,   335,
     335,   335,   335,   336,   336,   336,   336,   337,   337,   337,
     337,   337,   337,   337
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     3,     2,
       2,     3,     2,     4,     3,     4,     4,     0,     1,     1,
       0,     1,     0,     1,     1,     0,     7,     2,     3,     3,
       1,     2,     1,     3,     3,     5,     1,     3,     3,     3,
       5,     5,     0,     1,     1,     1,     0,     1,     1,     4,
       6,     8,     8,     6,     8,     8,     4,     1,     3,     5,
       1,     3,     3,     4,     4,     4,     4,     4,     4,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     3,     1,     1,     2,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     5,     5,     2,
       0,     8,     0,     9,     0,     8,     0,     9,     3,     3,
       5,     5,     2,     5,     3,     6,     6,     4,     5,     5,
       3,     3,     6,     5,     6,     5,     6,     3,     4,     3,
       4,     6,     7,     6,     7,     4,     5,     4,     5,     4,
       4,     3,     6,     5,     4,     3,     6,     5,     6,     5,
       8,     7,     4,     4,     6,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     6,     4,
       7,     5,     3,     6,     2,     3,     3,     2,     2,     3,
       3,     0,     2,     2,     3,     5,     1,     3,     3,     5,
       5,     0,     2,     3,     2,     3,     6,     6,     1,     1,
       1,     0,     2,     0,     2,     3,     5,     5,     1,     1,
       2,     3,     1,     3,     0,     0,     8,     0,     1,     1,
       0,     0,    10,     3,     3,     5,     5,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     3,     0,     1,     3,     4,     5,     4,     5,     6,
       6,     0,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       0,     1,     2,     1,     1,     1,     0,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     0,     2,     2,     4,
       4,     1,     3,     3,     3,     3,     3,     3,     3,     2,
       1,     1,     3,     4,     4,     2,     4,     0,     2,     2,
       1,     1,     1,     2,     1,     4,     3,     1,     3,     3,
       5,     1,     1,     3,     1,     2,     3,     0,     2,     2,
       3,     2,     4,     3,     3,     4,     3,     0,     2,     2,
       2,     1,     0,     2,     2,     2,     1,     4,     4,     6,
       3,     0,     1,     1,     3,     4,     3,     4,     6,     0,
       2,     2,     2,     2,     1,     1,     3,     3,     1,     3,
       1,     1,     1,     3,     3,     0,     1,     1,     3,     3,
       3,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     2,     4,     4,     4,     5,     2,     2,
       1,     2,     1,     2,     1,     2,     1,     2,     1,     1,
       6,     6,     4,     9,     9,     7,     6,     6,     4,     9,
       9,     7,     4,     6,     6,     9,     9,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     1,     4,
       1,     3,     4,     1,     3,     4,     3,     3,     1,     1,
       2,     1,     2,     1,     1,     3,     1,     2,     2,     2,
       2,     2,     8,     8,     9,     9,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     3,     3,
       2,     2,     2,     1,     0,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     4,     4,
       4,     3,     3,     3,     3,     5,     3,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     3,     4,     3,     4,     3,     5,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,   113,   626,   627,   628,   622,
     623,   629,     0,   547,    99,   134,   516,   141,   518,   547,
       0,   140,     0,   421,    99,     0,     0,     0,   246,   135,
     594,   594,   620,     0,     0,     0,     0,   139,    55,   247,
     298,   136,     0,     0,     0,   294,     0,     0,   143,     0,
     566,   538,   630,   144,     0,   299,   510,   420,     0,     0,
       0,   157,    48,   138,   519,   422,     0,     0,     0,     0,
     514,     0,     0,   142,     0,     0,   114,     0,   621,     0,
       0,     0,   137,   280,   512,   424,   145,     0,     0,   679,
       0,   681,     0,   682,   683,   593,     0,   680,   677,   497,
     154,   678,     0,     0,     0,     0,     4,     0,     5,     0,
       9,    50,    10,    11,     0,    12,    13,    14,    16,   493,
     494,    26,    15,   155,   164,   165,    17,    21,    18,    20,
       0,   241,    19,   585,    23,    24,    25,    22,   163,     0,
     161,     0,   582,     0,   159,   162,     0,   160,   599,   578,
     495,   579,   500,   498,     0,     0,     0,   583,   584,     0,
     499,     0,   600,   601,   602,   624,   625,   577,   502,   501,
     580,   581,     0,    42,    28,   508,     0,     0,   548,   100,
       0,     0,   518,   135,     0,     0,     0,     0,   519,     0,
       0,     0,     0,   582,   599,   498,   583,   584,   517,   499,
     600,   601,     0,   547,     0,     0,   423,     0,   254,     0,
     478,     0,   485,   622,   519,   595,   297,   622,   182,   519,
       0,   297,     0,     0,     0,     0,     0,     0,     0,   169,
       0,     0,     0,     0,     0,    52,   485,   107,   115,   127,
     121,   120,   129,   110,   119,   130,   116,   131,   108,   132,
     125,   118,   126,   124,   122,   123,   109,   111,   117,   128,
     133,     0,   112,     0,     0,     0,     0,     0,     0,   427,
       0,   151,    36,     0,   664,   570,   567,   568,   569,     0,
     511,   665,     7,   485,   297,   278,   288,   594,   279,   156,
     392,   475,     0,   474,     0,     0,   152,   598,     0,     0,
      39,     0,   515,   503,     0,   485,    40,   509,     0,   261,
     257,     0,   499,   261,   258,     0,   417,     0,   513,     0,
       0,   666,   668,   591,   663,   662,     0,    57,    60,     0,
       0,   480,     0,   482,     0,     0,   481,     0,     0,   474,
       0,   592,     0,     6,     0,    51,     0,     0,     0,     0,
     281,     0,   379,   378,   300,     0,   496,    27,     0,   571,
       0,     0,     0,     0,     0,     0,   667,     0,     0,     0,
       0,     0,     0,   590,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     337,   344,   345,   346,   341,   343,     0,     0,   339,   342,
     340,   338,     0,   348,   347,     0,     0,   485,   485,     0,
       0,     0,    29,    30,     0,     0,     0,     0,     0,     0,
       0,    31,     0,     0,     0,     0,    32,     0,    33,     0,
     493,   491,     0,   486,   487,   492,   176,     0,   179,     0,
     172,     0,     0,   178,     0,     0,     0,   191,     0,     0,
     190,     0,   199,     0,     0,     0,   197,     0,     0,    72,
     166,     0,     0,     0,   215,     0,   337,   211,     0,    54,
      53,    50,     0,     0,     0,   225,    34,   361,   295,   431,
       0,   432,   434,     0,   456,     0,   437,     0,     0,   150,
      35,     0,     0,    37,     0,   158,     0,    92,   596,   597,
     153,     0,    38,   485,     0,   268,   259,   255,   260,   256,
       0,   415,   412,   184,    41,    59,    58,    61,     0,   631,
       0,     0,   616,     0,   618,     0,     0,     0,     0,     0,
       0,     0,     0,   635,     8,     0,    44,     0,     0,    90,
       0,    87,     0,    66,   252,     0,     0,     0,   372,   426,
     546,   659,   658,   661,   670,   669,   674,   673,   655,   652,
     653,   654,   587,   642,   113,     0,   613,   614,   114,   612,
     611,   588,   646,   657,   651,   649,   660,   650,   648,   640,
     645,   647,   656,   639,   643,   644,   641,   589,     0,     0,
       0,     0,     0,     0,     0,     0,   672,   671,   676,   675,
     558,   559,   561,   563,     0,   550,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   633,   252,   547,   547,
     187,   413,   425,   479,     0,     0,   505,     0,     0,   281,
       0,     0,   281,   414,     0,     0,     0,     0,   522,     0,
       0,     0,   200,     0,   528,     0,     0,   198,   678,    75,
       0,    62,    73,     0,     0,     0,   214,     0,   210,     0,
       0,     0,     0,   504,     0,   228,     0,   226,   366,   363,
     364,   365,   369,   370,   371,   361,     0,   353,     0,   362,
     380,     0,   435,     0,   148,   149,   147,   146,     0,   455,
     454,   578,     0,   429,   576,   428,     0,     0,   610,   477,
     476,     0,     0,     0,   506,     0,   262,   419,   578,     0,
     632,   586,   617,   483,   619,   484,   207,     0,     0,     0,
     634,   205,   532,     0,   637,   636,     0,    46,    45,    43,
       0,    86,     0,     0,     0,    79,     0,     0,    72,   249,
       0,   282,     0,     0,   292,     0,   289,   376,   373,   374,
     377,   301,     0,     0,    98,    96,    97,    95,    94,    93,
     608,   609,   560,   562,     0,   549,   134,   141,   140,   139,
     136,   143,   144,   138,   142,   137,   145,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   183,   489,   490,   488,   281,     0,   181,   281,
       0,   180,     0,   209,     0,     0,   189,     0,   188,     0,
     553,     0,     0,   195,     0,     0,   193,     0,     0,   167,
       0,   168,   223,   222,     0,   217,     0,     0,   213,     0,
     219,     0,   251,     0,     0,   367,   368,     0,   361,   351,
       0,   469,   381,   384,   383,   385,     0,   433,   436,   437,
       0,     0,   438,   439,     0,     0,   270,     0,     0,   269,
     272,   507,     0,   263,   266,     0,   416,   208,     0,     0,
       0,     0,   206,     0,    91,    88,     0,    69,    68,    67,
       0,     0,     0,     0,   297,   287,     0,   290,   286,   375,
     380,   349,   101,   332,   115,   330,   121,   120,   104,   119,
     116,   335,   131,   102,   132,   118,   122,   103,   105,   117,
     133,   329,   311,   314,   312,   313,   324,   315,   328,   320,
     318,   331,   334,   319,   317,   322,   327,   316,   321,   325,
     326,   323,   333,     0,   310,     0,   106,     0,   349,   349,
     308,   615,   551,   437,   599,   599,     0,     0,     0,     0,
       0,     0,   251,     0,     0,     0,   186,   185,     0,   283,
       0,   283,   192,     0,     0,   521,     0,   520,     0,   552,
       0,     0,   527,   196,     0,   526,   194,    64,    63,   216,
     212,   537,   218,     0,     0,   248,   227,   224,   469,   354,
       0,     0,   437,   382,   396,   430,   460,     0,   633,   485,
     485,     0,   274,     0,     0,     0,   264,     0,   203,   534,
       0,     0,   201,   533,     0,   638,     0,     0,     0,    72,
       0,    72,    80,    83,   253,   277,   157,   297,   276,   297,
     284,   293,   291,     0,   361,   307,     0,   336,     0,   303,
     304,   555,     0,     0,     0,     0,     0,     0,     0,     0,
     253,   283,   297,   283,   297,   525,     0,     0,   554,   531,
       0,     0,     0,     0,   221,    56,   437,   469,     0,   472,
     471,   473,   578,   393,   357,   355,     0,     0,     0,     0,
     458,   578,     0,     0,   275,   273,     0,   267,     0,   204,
       0,   202,    89,    71,    70,     0,     0,     0,     0,   250,
     285,   451,     0,   386,     0,   309,   101,   103,   349,   349,
       0,     0,     0,     0,     0,     0,   297,   175,   297,   171,
       0,     0,     0,     0,    65,   220,   358,   356,   437,   461,
       0,   395,   394,   410,     0,   411,   398,   401,     0,   397,
     390,   391,   296,     0,   572,   573,   265,     0,     0,    82,
      85,    81,    84,     0,   450,   449,   578,   387,   396,   350,
     305,   306,     0,     0,     0,     0,     0,     0,   177,   173,
     524,   523,   530,   529,   360,   359,   463,   464,   466,   578,
       0,   633,   409,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   578,   574,   575,   536,   535,     0,   441,     0,
       0,     0,   465,   467,   400,   402,   403,   406,   407,   408,
     404,   405,   399,   446,   444,   578,   633,   388,   302,   389,
     461,   445,   578,   468
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   328,   107,   658,   109,   110,   111,   346,
     471,   112,   235,   113,   329,   649,   542,   653,   650,   114,
     115,   116,   539,   540,   117,   118,   180,   934,   267,   119,
     262,   120,   689,   272,   121,   122,   284,   123,   124,   125,
     441,   631,   437,   628,   126,   127,   787,   128,   233,   129,
     666,   667,   192,   131,   132,   133,   134,   135,   506,   706,
     865,   136,   137,   702,   860,   138,   139,   546,   884,   140,
     141,   745,   746,   193,   265,   680,   143,   144,   548,   890,
     751,   937,   938,   468,  1035,   478,   676,   677,   678,   679,
     752,   354,   846,  1158,  1218,  1142,   431,  1074,  1078,  1136,
    1137,  1138,   145,   317,   511,   146,   147,   268,   269,   482,
     483,   693,  1155,  1103,   486,   690,  1177,  1071,   992,   330,
     209,   334,   335,   432,   433,   434,   194,   149,   150,   151,
     152,   195,   154,   177,   178,   604,   453,   809,   605,   606,
     155,   156,   196,   197,   159,   221,   435,   199,   161,   200,
     201,   164,   165,   166,   167,   340,   168,   169,   170,   171,
     172
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1041
static const yytype_int16 yypact[] =
{
   -1041,    99,  3630, -1041,   -54,   114, -1041, -1041, -1041, -1041,
   -1041, -1041,  4998,     7,   189,   126, 14767,   205, 20018,     7,
   11466,   243,   439,   252,   189,  4998, 11466,  4998,   176, 20101,
   11635,  8408,   291,  8577,  9768,  7048,  8746,   358, -1041,   215,
   -1041,   387, 20138, 20138, 20138, -1041,  2401,  9937,   401, 11466,
     329, -1041,   416,   441, 11466, -1041, 14767, -1041, 11466,   488,
     342,   206,  1785,   481, 20221, -1041, 10108,  7899, 11466,  9937,
   14767, 11466,   465,   508,   398,  4998,   526, 11466,   529, 11804,
   11804, 20138,   530, -1041, 14767, -1041,   537, 11466, 11466, -1041,
   11466, -1041, 11466, -1041, -1041, 14297, 11466, -1041, 11466, -1041,
   -1041, -1041,  3972,  7219,  8917, 11466, -1041,  4827, -1041,   430,
   -1041,   525, -1041, -1041,    61, -1041, -1041, -1041, -1041, -1041,
   -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041,
     553, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041, 20138,
   -1041, 20138,   411,   201, -1041, -1041,  2401, -1041,   460, -1041,
     462, -1041, -1041,   466,   471,   486, 11466,   478,   491, 19536,
    1786,   360,   495,   499, -1041, -1041,   408, -1041, -1041, -1041,
   -1041, -1041,   468, -1041, -1041, 19536,   476,  4998, -1041, -1041,
     506, 11466, -1041, -1041, 11466, 11466, 11466, 20138, -1041, 11466,
   10108, 10108,   607,   419, -1041, -1041, -1041, -1041,   -34,   444,
   -1041, -1041,   502, 16237, 20138,  2401, -1041,   510, -1041,   173,
   19536,   557,  8070,   589, 20258, 19536,   500,   594, -1041, 20341,
   20138,   500, 20138,   517,    39, 15917,    18, 15841,   268, -1041,
   15999, 20138, 20138,   -17,  3349,   492,  8070, -1041, -1041, -1041,
   -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041,
   -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041,
   -1041,   518, -1041,   450,  4998,   521,  1534,    65,   106, -1041,
    4998, -1041, -1041, 16313,   404, -1041,   533,   534, -1041, 16313,
     -34,   404, -1041,  8070,  1819, -1041, -1041, 10277, -1041, -1041,
   -1041, -1041,   259, 19536, 11466, 11466, -1041, 19536,   532, 16861,
   -1041, 16313,   -34, 19536,   535,  8070, -1041, 19536, 16937, -1041,
   -1041, 17017,  2141, -1041, -1041, 17097,   577,   543,   -34, 16313,
   17177,   177,   177,   749,   404,   404,   161, -1041, -1041,  4143,
     -55, -1041, 11466, -1041,   123,   136, -1041,   -47,   -16, 17257,
     131,   749,   697, -1041,  4314, -1041,   643, 11466, 11466, 20138,
     566,   542, -1041, -1041, -1041,   265,   449, -1041, 11466,   565,
   11466, 11466, 11466,  9768,  9768, 11466,   483, 11466, 11466, 11466,
   11466, 11466,   227, 14297, 11466, 11466, 11466, 11466, 11466, 11466,
   11466, 11466, 11466, 11466, 11466, 11466, 11466, 11466, 11466, 11466,
     644, -1041, -1041, -1041, -1041, -1041,  9086,  9086, -1041, -1041,
   -1041, -1041,  9086, -1041, -1041,  9086,  9086,  8070,  8070,  9768,
    9768,  7730, -1041, -1041, 16393, 16469, 17333,   544,   -13, 20138,
    4485, -1041,  9768,    39,   550,   269, -1041, 11466, -1041, 11466,
     593, -1041,   546,   575, -1041, -1041, -1041, 20138, -1041,  2401,
   -1041, 20138,   560, -1041,  2401,   683, 10108, -1041,  5169,  9768,
   -1041,   563, -1041,    39,  5340,  9768, -1041,    39, 11973, 11466,
   -1041,   611,   612,  4998,   700,  4998, -1041,   704, 11466, -1041,
   -1041,   525,   570,  8070, 20138, -1041, -1041,   413, -1041, -1041,
    1534, -1041,   600,   576, -1041, 12142,   623, 11466,  2401, -1041,
   -1041, 11466, 11466, -1041,   578, -1041, 10108, -1041, 19536, 19536,
   -1041,    30, -1041,  8070,   580, -1041,   725, -1041,   725, -1041,
   12311,   608, -1041, -1041, -1041, -1041, -1041, -1041,  9257, -1041,
   17413,  7390, -1041,  7561, -1041,  4998,   582,  9768,  9428,  3801,
     584, 11466, 10446, -1041, -1041,   283, -1041,  4656, 20138, -1041,
     341, 17495,   370, 16081,    21, 10108,   585, 19981,   353, -1041,
   17571, 19783, 19783,   410, -1041,   410, -1041,   410, 17066,  1076,
    1059,  1144,   -34,   177, -1041,   595, -1041, -1041, -1041, -1041,
   -1041,   749, 17146,   410,  1252,  1252, 19783,  1252,  1252,   930,
     177, 17146, 19824,   930,   404,   404,   177,   749,   601,   617,
     618,   620,   626,   627,   603,   598, -1041,   410, -1041,   410,
      73, -1041, -1041, -1041,   138, -1041,  1555, 19612,   474, 12480,
    9768, 12649,  9768, 11466,  8070,  9768, 15035,   605,     7, 17651,
   -1041, -1041, -1041, 19536, 17727,  8070, -1041,  8070, 20138,   566,
     380, 20138,   566, -1041,   381, 11466,   158,  8746, 19536,    63,
   16549,  7730, -1041,  8746, 19536,    29, 16157, -1041,   628,   642,
     637, 17807,   642,   638, 11466, 11466,   768,  4998,   770, 17883,
    4998, 16625,   744, -1041,   170, -1041,   244, -1041,   200, -1041,
   -1041, -1041, -1041, -1041, -1041,   755,   250, -1041,  3117, -1041,
     396,   647,  1534,    65,    44,    68, 11466, 11466,  6879, -1041,
   -1041,   574,  8239, -1041, 19536, -1041, 17963, 18039, -1041, -1041,
   19536,   646,    83,   648, -1041,  1696, -1041, -1041,   382, 20138,
   -1041, -1041, -1041, -1041, -1041, -1041, -1041,  4998,    -8, 16705,
   -1041, -1041, 19536,  4998, 19536, -1041, 18121, -1041, -1041, -1041,
   11466, -1041,   115,   133, 11466, -1041, 10615, 11973, 11466, -1041,
    8070,   669,  1276,   651,   698,   -37, -1041,   734, -1041, -1041,
   -1041, -1041, 14133,   653, -1041, -1041, -1041, -1041, -1041, -1041,
   -1041, -1041, -1041, -1041,  7730, -1041, -1041, -1041, -1041, -1041,
   -1041, -1041, -1041, -1041, -1041, -1041, -1041,    19,  9768,  9768,
   11466,   791, 18197, 11466,   792, 18277,   262,   654, 18353,  8070,
      39,    39, -1041, -1041, -1041, -1041,   566,   665, -1041,   566,
     667, -1041, 16313, -1041, 15215,  5511, -1041,  5682, -1041,   263,
   -1041, 15291,  5853, -1041,    39,  6024, -1041,    39, 11466, -1041,
   11466, -1041, 19536, 19536,  4998, -1041,  4998, 11466, -1041,  4998,
     805, 20138,   674, 20138,   502, -1041, -1041, 20138,   801, -1041,
    1534,   699,   756, -1041, -1041, -1041,    69, -1041, -1041,   623,
     673,    14, -1041, -1041,   677,   681, -1041,  6195, 10108, -1041,
   -1041, -1041, 20138, -1041,   709,   502, -1041, -1041,  6366,   684,
    6537,   691, -1041, 11466, -1041, -1041, 11466, 18433,   234, 16781,
     692,   693,   303,   685,  2687, -1041, 11466, 20138, -1041, -1041,
     396,   695,   272, -1041,   721, -1041,   722,   723,   732,   731,
     733, -1041,   735,   746,   739,   740,   751,   318,   762,   758,
     759, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041,
   -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041,
   -1041, -1041, -1041, 11466, -1041,   764,   771,   765,   695,   695,
   -1041, -1041, -1041,   623,   307,   323, 18509, 12818, 12987, 18589,
   13156, 13325, -1041, 13494, 13663,   325, -1041, -1041,   752, -1041,
     753, -1041, -1041,  4998,  8746, 19536,  8746, 19536,  7730, -1041,
    4998,  8746, 19536, -1041,  8746, 19536, -1041, 18671, 19536, -1041,
   -1041, 19536,   881,  4998,   763, -1041, -1041, -1041,   699, -1041,
     748, 10786,   155, -1041,   175, -1041, -1041,  9768, 14901,  8070,
    8070,  4998, -1041,     4,   754, 11466, -1041,  8746, -1041, 19536,
    4998,  8746, -1041, 19536,  4998, 19536,   156, 10955, 11973, 11466,
   11973, 11466, -1041, -1041,   767, -1041, -1041,  1819, -1041,  2094,
   -1041, 19536, -1041,    50,   413, -1041, 18747, -1041, 15133, -1041,
   -1041, -1041, 11466, 11466, 11466, 11466, 11466, 11466, 11466, 11466,
   -1041, -1041,  2954, -1041,  3278, 17883, 15371, 15447, -1041, 17883,
   15527, 15603, 11466,  4998, -1041, -1041,   155,   699,  9599, -1041,
   -1041, -1041,   121, 10108, -1041, -1041,    49, 11466,   -45, 18828,
   -1041,   811,   757,   760,   557, -1041,   502, 19536, 15683, -1041,
   15759, -1041, -1041, -1041, 19536,   769,   772,   776,   779, -1041,
   -1041, -1041, 13832,   819,   331, -1041,   795,   807,   695,   695,
   18909, 18985, 19065, 19141, 19221, 19297, 19751, -1041, 19768, -1041,
    4998,  4998,  4998,  4998, 19536, -1041, -1041, -1041,   155, 11126,
      85, -1041, 19536, -1041,   159, -1041,   143, -1041,   516, 19380,
   -1041, -1041, -1041, 13663,   785,   786, -1041,  4998,  4998, -1041,
   -1041, -1041, -1041,  6708, -1041, -1041,   599, -1041,   175, -1041,
   -1041, -1041, 11466, 11466, 11466, 11466, 11466, 11466, -1041, -1041,
   17883, 17883, 17883, 17883, -1041, -1041, -1041, -1041, -1041,   260,
    9768, 14465, -1041, 11466,    49,   159,   159,   159,   159,   159,
     159,    49,   813, -1041, -1041, 17883, 17883,   789, 14001,   101,
      88, 19456, -1041, -1041, 19536, -1041, -1041, -1041, -1041, -1041,
   -1041, -1041,   808, -1041, -1041,   660, 14633, -1041, -1041, -1041,
   11297, -1041,   682, -1041
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1041, -1041, -1041,     1,  -424,  2580, -1041, -1041, -1041,   493,
   -1041, -1041, -1041,   457,   613,  -454, -1041,  -713,  -709, -1041,
   -1041, -1041,   217, -1041, -1041,   407,   939, -1041,  2589,  -163,
    -735, -1041,  -952,    -5,  -975,  -839, -1041,   -61, -1041, -1041,
   -1041, -1041, -1041, -1041, -1041, -1041,   -22, -1041, -1041, -1041,
   -1041,   132,  1224, -1041, -1041, -1041, -1041, -1041,   655, -1041,
     105, -1041, -1041, -1041, -1041, -1041, -1041,  -535,  -720, -1041,
   -1041, -1041,    82,  1240, -1041, -1041, -1041,   292, -1041, -1041,
   -1041, -1041,   -68,  -147,  -891, -1041,   -59,   135,   308, -1041,
   -1041, -1041,    94, -1041, -1041,  -222,    11,  -941,  -169,  -201,
    -193,  -540, -1041,  -188, -1041,    12,   957,  -120,   509, -1041,
    -451,  -811,  -807, -1041,  -642,  -495,  -929, -1040,  -900,   -64,
   -1041,   134, -1041,  -209,  -411,   367,   652,  -406, -1041, -1041,
   -1041,  1481, -1041,   -10, -1041, -1041,  -221, -1041,  -631, -1041,
   -1041, -1041,  1864,  1960,   -12,   970,    72,   869, -1041,  2084,
    2453, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041, -1041,
    -404
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -634
static const yytype_int16 yytable[] =
{
     175,   289,   292,   106,   198,   652,   457,   608,   203,   202,
     810,   224,   226,   406,   210,   707,   424,   936,   215,   215,
     484,   225,   227,   230,   234,   881,   355,   472,   880,   681,
     527,   701,   442,   615,   205,   273,   270,   274,   995,  1069,
     338,   849,   279,    67,   280,  1030,   281,  1039,  1040,   430,
     270,  1101,  1133,   675,   293,   297,   299,   301,   302,   303,
     997,   812,   664,   307,   300,   308,   484,   311,   315,   451,
     484,   270,   318,   430,   494,   319,   320,   291,   321,   691,
     322,   463,   337,   323,   324,   425,   325,   518,  1066,  1178,
     293,   297,   339,   341,   797,   805,   504,   800,  -246,     3,
     451,  -271,   366,  1141,   708,   887,   347,   173,   451,  -447,
     372,  -452,   519,   291,   333,   291,   526,   530,   564,   762,
     430,   420,  -247,   176,   525,  1126,   528,   418,  -452,   528,
     888,  1180,  1041,   942,   451,   943,   564,  1134,  -447,   298,
    -452,  1203,   430,  -271,   359,   451,   496,   615,  1135,   420,
    1154,  -447,   487,   565,   857,   529,   528,  -452,   616,   763,
     485,  -452,  1133,   868,  -447,   406,   566,  1128,  -229,   175,
     420,   348,   414,   415,   416,   336,    67,   307,   293,   339,
    -230,  1075,   739,  -452,   420,   998,  -452,  1174,  -452,   740,
    1100,  1102,   179,   423,   567,   420,   858,  -271,   594,   595,
     297,   291,   291,   786,   994,   420,   485,  -452,   362,   570,
     485,  -447,   212,  1030,   568,  1030,  -447,  1160,  1161,   447,
     450,   452,   456,   495,   297,  1141,  -452,   528,   439,   420,
     564,   848,   568,   444,  1076,  -452,   236,   608,   569,  1052,
     205,  1054,  -470,   528,   430,   430,   835,   565,   488,  1217,
     859,   -78,  1202,   352,   420,  1127,  1181,  1183,  1135,  -236,
     566,   958,   363,  -470,   960,   521,   364,  -470,   490,   -49,
    1018,   297,  1216,   532,   493,   215,   692,  1030,   523,  1030,
     764,   353,   498,   499,   652,  1184,   836,   -49,   567,  1077,
    -470,  1223,   522,   297,   703,    23,   502,  -235,   818,   876,
     496,  1073,   533,   936,  1019,   524,  1096,   765,  1098,  1095,
     430,  1097,   627,   366,   513,   427,   -78,  1175,   370,   630,
     520,   372,   173,  1092,   634,   375,   568,   803,   515,   882,
     517,  1116,   569,  1118,   428,   541,   543,  1058,   206,   832,
     430,   554,   556,    57,   212,  -244,   550,   275,   551,   552,
     553,   555,   557,   558,    65,   559,   560,   561,   562,   563,
     608,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   587,   955,    85,
     747,  -462,   636,   236,   297,   297,   833,   596,   598,   990,
     297,  1214,   838,   297,   297,   297,   297,   597,   599,   607,
     618,   496,  -462,   276,   627,   968,  -462,   488,  -107,  1221,
     619,   488,  -234,   834,   675,   623,  -113,   624,   620,   839,
     497,   277,  1016,   842,   814,   817,   549,   639,   748,  -462,
     622,   952,   969,   645,   293,   362,   638,   640,   278,   749,
     668,  -231,   644,   646,   173,   627,   651,   651,   642,  -557,
     727,   430,   647,   659,  -109,  -238,   661,   291,   750,   669,
    1029,   297,  -114,   670,   430,  -556,    23,   627,   589,   590,
    -242,   843,  1024,   838,   591,   694,  -557,   592,   593,   696,
     697,   741,   844,   730,   700,   671,   310,   314,   672,   363,
      59,   297,  -556,   364,  1050,  -239,   282,   869,   871,   673,
    1159,   845,   731,  1080,   271,   718,   700,   699,   438,   297,
     283,   297,   734,   443,    57,   719,   700,   722,   674,   724,
     726,   866,   488,   488,  -418,    65,   271,    23,   407,   699,
     408,   735,   333,   293,   333,  -233,   309,   313,   517,   699,
     366,   798,   801,  -418,   204,   370,   366,   371,   372,    40,
      85,   370,   375,   409,   372,  -352,   291,   410,   375,   779,
     382,   304,  -237,   410,   608,   652,   305,   652,   388,   875,
     570,   469,  -543,    55,   470,    57,  -607,   430,  -607,  -606,
    -243,  -606,  -352,  -245,  -232,  1072,    65,  -606,   781,  -606,
     784,  -240,  1081,   336,  1182,   336,   344,   638,   782,   644,
     785,   659,   297,   788,   722,   939,   345,   349,   790,   791,
     675,    85,  -603,   297,  -603,   297,   420,  -605,   474,  -605,
    -453,  -540,   356,   802,   851,   804,   430,  -539,  1029,   607,
    1029,   811,   357,  -453,   806,   808,   793,  1185,   358,  -544,
     813,   816,   822,   823,   411,  1206,  1207,  1208,  1209,  1210,
    1211,  -604,  -545,  -604,   148,   713,  -542,   715,  -448,   421,
    -541,   419,  -453,  1186,   148,  1187,  1188,   413,   420,  1189,
    1190,   426,   429,   436,   303,   307,   339,   148,   440,   148,
     297,   271,   271,   271,   271,   446,   473,  -448,  -453,   477,
     464,   467,  1029,   500,  1029,  -453,  1156,   794,   510,   291,
    -448,   491,   492,   503,   512,   534,   538,   545,   547,   372,
     588,   621,   614,  -448,   625,   626,  -453,   627,   541,  -440,
     475,   633,   877,  1179,   879,   651,   651,   148,   297,   635,
     271,   641,   654,   655,   657,  -453,   271,  1192,   660,   663,
    -453,  -442,   682,  -453,   692,   683,   705,   698,  -440,   704,
     709,   742,   607,   717,   148,   723,   554,   596,   271,   148,
    -448,  -440,   754,   753,   853,  -448,   555,   597,   946,   761,
    -442,   949,   760,   789,  -440,  1179,   271,   297,   755,   756,
     362,   757,   668,  -442,   818,   956,   957,   758,   759,   -74,
    1082,  1083,  1215,   965,  1003,   967,  -442,   962,   819,   821,
     972,   669,   824,   975,   826,   670,   977,   831,   978,   973,
    1222,   496,   976,   856,  1179,   981,   847,   861,   885,   886,
     889,  -440,   941,   947,   950,   953,  -440,   671,   668,   148,
     672,   959,   342,   961,   363,   985,   430,   430,   364,   983,
     991,   673,   993,  -442,   996,   999,   293,   669,  -442,  1000,
    1005,   670,  1025,  1022,  1023,  1010,  1009,  -457,  1013,  -459,
     674,  1015,  1014,  1034,   651,  -134,  -141,  -140,  -110,   291,
    -457,   160,  -459,   671,  1031,  -139,   672,  -136,    59,  -143,
     271,   160,  -108,  -144,  -138,   366,   367,   673,   368,   369,
     370,  1109,   371,   372,   160,  -142,   160,   375,  -111,  -457,
    1037,  -459,  -137,  -145,   381,   382,   674,  -112,   385,  1038,
     271,   386,   387,   388,   271,  1063,   148,  1067,  1051,  1053,
    1157,  1036,   148,  1086,  1065,  -457,  1144,  -459,  1099,  1145,
    1149,  -107,  -457,  1150,  -459,   965,   967,  1151,   972,   975,
    1152,  1009,  1013,  -109,   160,  1193,  1194,   874,   312,   312,
    1184,  1055,  1056,  -457,  1057,  -459,   607,   537,  1059,  1060,
    1213,   362,  1061,   207,   662,   986,   289,  1004,   508,  1032,
    1108,   160,  -457,   989,  -459,  1104,   160,  -457,  1219,  -459,
    -457,   148,  -459,   837,  1033,  1079,   722,   297,   297,  1200,
    1212,  1205,   229,  1087,   795,  1088,   148,   695,  1085,  1090,
     216,     0,  1070,     0,  1130,  1094,   651,   651,   651,   651,
       0,     0,     0,     0,     0,   363,     0,     0,     0,   364,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1055,  1110,  1111,  1059,  1112,  1113,  1114,  1115,     0,     0,
       0,     0,     0,     0,     0,     0,   160,     0,     0,     0,
    1124,     0,     0,     0,     0,     0,   339,     0,     0,     0,
       0,  1132,     0,     0,     0,  1139,   366,   367,     0,   368,
     369,   370,   148,   371,   372,     0,     0,     0,   375,   291,
       0,     0,     0,     0,  1131,     0,   382,     0,     0,  1199,
     362,     0,   386,   387,   388,     0,   271,   271,     0,     0,
     148,     0,   271,   271,     0,     0,   148,   362,  1170,  1171,
    1172,  1173,     0,     0,     0,   148,     0,   148,   830,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1013,     0,   160,     0,  1195,  1196,     0,     0,   160,
    1176,   339,     0,     0,   363,     0,     0,     0,   364,     0,
    1170,  1171,  1172,  1173,  1195,  1196,     0,     0,     0,     0,
       0,   363,   863,     0,   291,   364,     0,     0,  1201,   722,
       0,  1204,     0,     0,     0,   362,     0,   148,     0,     0,
       0,   148,     0,     0,     0,     0,     0,     0,     0,   148,
       0,     0,  1176,     0,     0,   366,   367,     0,   160,   369,
     370,     0,   371,   372,   722,     0,     0,   375,  1013,     0,
       0,     0,   366,   160,     0,   382,     0,   370,     0,   371,
     372,   386,   387,   388,   375,     0,   130,     0,     0,   363,
       0,  1176,   382,   364,     0,     0,   130,     0,   386,   387,
     388,     0,   142,     0,     0,     0,     0,   271,   271,   130,
       0,   130,   142,     0,     0,     0,     0,     0,   777,   271,
       0,   467,     0,     0,     0,   142,     0,   142,   467,     0,
       0,   271,     0,     0,   271,     0,     0,   883,     0,     0,
     366,   367,     0,   362,     0,   370,     0,   371,   372,   160,
       0,   987,   375,     0,     0,     0,     0,     0,  -283,   130,
     382,     0,  -283,  -283,     0,     0,   386,   387,   388,   148,
       0,  -283,   148,  -283,  -283,   142,     0,   160,  -283,     0,
       0,     0,  1006,   160,     0,  -283,   130,     0,  -283,     0,
       0,   130,   160,     0,   160,     0,     0,   363,     0,     0,
       0,   364,   142,     0,     0,     0,     0,   142,     0,  -283,
       0,  -283,     0,  -283,     0,  -283,  -283,     0,  -283,     0,
    -283,     0,  -283,     0,     0,     0,     0,     0,     0,   148,
       0,     0,     0,     0,     0,   148,     0,     0,     0,     0,
       0,  -283,     0,     0,  -283,     0,     0,  -283,   366,   367,
       0,   368,   369,   370,   160,   371,   372,   373,   160,     0,
     375,   130,     0,     0,   940,     0,   160,   381,   382,     0,
       0,   385,     0,     0,   386,   387,   388,   142,     0,     0,
       0,     0,     0,     0,     0,   389,     0,     0,     0,     0,
     944,   945,     0,     0,     0,     0,     0,  -283,     0,     0,
       0,     0,     0,  -283,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   148,     0,   148,
     271,     0,     0,     0,   148,     0,     0,   148,     0,     0,
       0,     0,     0,     0,     0,     0,   148,     0,   148,     0,
       0,   148,     0,   153,     0,     0,     0,     0,   130,     0,
       0,     0,     0,   153,   130,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   142,     0,   153,     0,   153,   148,
     142,     0,     0,   467,   467,     0,     0,   467,   467,     0,
     148,     0,   148,     0,     0,     0,   160,     0,     0,   160,
       0,     0,     0,     0,     0,  1140,     0,   237,     0,     0,
       0,     0,     0,  1146,     0,   467,     0,   467,     0,     0,
       0,   238,   239,   130,   240,     0,   153,     0,   564,   241,
       0,     0,     0,     0,     0,     0,     0,   242,   130,   142,
       0,     0,   766,   243,     0,   767,     0,     0,   244,     0,
     768,     0,   245,   153,   142,   246,   160,     0,   153,     0,
     183,     0,   160,     0,     0,   247,     0,     0,     0,   769,
       0,   248,   249,     0,     0,     0,   770,     0,   250,     0,
       0,     0,     0,     0,     0,   148,   771,   251,     0,     0,
       0,     0,   148,   772,     0,     0,   252,   253,     0,   254,
       0,   255,     0,   256,     0,   148,   257,     0,   773,     0,
     258,   479,     0,   259,   130,     0,   260,     0,     0,     0,
     774,     0,     0,   148,   568,     0,     0,  1140,   153,     0,
     142,   775,   148,     0,     0,     0,   148,   776,     0,     0,
       0,     0,   130,     0,   160,     0,   160,     0,   130,     0,
       0,   160,     0,     0,   160,     0,     0,   130,   142,   130,
       0,     0,     0,   160,   142,   160,     0,     0,   160,   237,
       0,     0,   480,   142,     0,   142,     0,     0,     0,     0,
       0,     0,     0,   238,   239,   148,   240,     0,     0,     0,
       0,   241,     0,     0,     0,     0,   160,     0,     0,   242,
       0,     0,     0,     0,     0,   243,     0,   160,     0,   160,
     244,     0,     0,     0,   245,   153,     0,   246,     0,   130,
       0,   153,     0,   130,     0,     0,     0,   247,     0,     0,
       0,   130,     0,   248,   249,   142,     0,     0,     0,   142,
     250,     0,   148,   148,   148,   148,     0,   142,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,   252,   253,
       0,   254,     0,   255,     0,   256,     0,     0,   257,   148,
     148,     0,   258,     0,     0,   259,     0,   285,   260,     0,
     153,    22,    23,     0,     0,     0,     0,     0,     0,     0,
     286,     0,    30,   287,     0,   153,     0,    35,     0,     0,
     -77,     0,   160,     0,    40,     0,     0,  -297,     0,   160,
       0,   285,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,   160,     0,   286,     0,    30,   287,    55,     0,
      57,    35,   420,     0,   862,  -297,   157,     0,    40,   288,
     160,    65,     0,     0,     0,     0,   157,     0,     0,   160,
       0,   130,     0,   160,   130,     0,     0,     0,     0,   157,
      81,   157,    55,    83,    57,   -77,    85,   142,     0,     0,
     142,   153,     0,   288,     0,    65,     0,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   404,     0,     0,    81,     0,     0,    83,     0,   153,
      85,     0,   160,     0,     0,   153,     0,     0,  -564,   157,
       0,   130,     0,     0,   153,     0,   153,   130,     0,     0,
       0,   405,     0,     0,  -603,     0,  -603,   142,     0,     0,
       0,     0,   158,   142,     0,     0,   157,     0,     0,     0,
       0,   157,   158,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   158,     0,   158,     0,   160,
     160,   160,   160,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   153,     0,     0,     0,
     153,     0,     0,     0,     0,     0,   160,   160,   153,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   130,
       0,   130,     0,     0,     0,   158,   130,     0,     0,   130,
       0,   157,     0,     0,     0,   142,     0,   142,   130,     0,
     130,     0,   142,   130,     0,   142,     0,     0,     0,     0,
       0,     0,   158,     0,   142,     0,   142,   158,     0,   142,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   130,     0,     0,     0,     0,   162,     0,     0,     0,
       0,     0,   130,     0,   130,     0,   162,   142,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   142,   162,
     142,   162,     0,     0,     0,     0,   285,     0,     0,     0,
      22,    23,     0,     0,     0,     0,     0,     0,   157,   286,
       0,    30,   287,     0,   157,     0,    35,   158,   153,     0,
       0,   153,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   162,
       0,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,   342,     0,  1026,     0,     0,  1027,     0,   288,     0,
      65,     0,     0,     0,     0,     0,   162,   130,     0,     0,
       0,   162,     0,   157,   130,     0,     0,     0,   153,    81,
       0,     0,    83,   142,   153,    85,     0,   130,   157,     0,
     142,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   142,   158,   130,     0,     0,     0,     0,
     158,     0,     0,     0,   130,     0,     0,     0,   130,     0,
       0,   142,     0,     0,     0,     0,     0,     0,     0,     0,
     142,     0,     0,     0,   142,   100,     0,     0,     0,     0,
       0,   162,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   402,   403,   404,     0,     0,     0,
       0,     0,     0,     0,   157,     0,   153,   130,   153,   158,
       0,     0,     0,   153,     0,     0,   153,     0,     0,     0,
       0,     0,     0,   142,   158,   153,   405,   153,     0,  -603,
     153,  -603,   157,     0,     0,     0,     0,     0,   157,     0,
       0,     0,     0,     0,     0,     0,     0,   157,     0,   157,
       0,     0,     0,     0,     0,     0,     0,     0,   153,     0,
       0,     0,     0,     0,   130,   130,   130,   130,   162,   153,
       0,   153,     0,     0,   162,     0,     0,     0,     0,     0,
     142,   142,   142,   142,     0,     0,     0,     0,     0,     0,
       0,   130,   130,     0,     0,     0,     0,     0,     0,     0,
     158,     0,     0,     0,     0,     0,     0,   142,   142,   157,
       0,     0,     0,   157,     0,     0,     0,     0,     0,     0,
       0,   157,     0,     0,   237,     0,     0,     0,   158,     0,
       0,     0,     0,   162,   158,     0,     0,     0,   238,   239,
       0,   240,     0,   158,     0,   158,   241,     0,   162,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
     243,     0,     0,     0,   153,   244,     0,     0,     0,   245,
       0,   153,   246,     0,     0,   163,     0,     0,     0,     0,
       0,     0,   247,     0,   153,   163,     0,     0,   248,   249,
       0,     0,     0,     0,     0,   250,     0,     0,   163,     0,
     163,     0,   153,     0,   251,   158,     0,     0,     0,   158,
       0,   153,     0,   252,   253,   153,   254,   158,   255,     0,
     256,     0,     0,   257,   162,     0,     0,   258,     0,     0,
     259,     0,     0,   260,     0,     0,     0,     0,     0,     0,
       0,   157,     0,     0,   157,     0,     0,     0,   163,     0,
       0,     0,   162,     0,     0,     0,     0,     0,   162,     0,
       0,     0,     0,     0,   153,     0,     0,   162,     0,   162,
       0,     0,     0,     0,     0,   163,     0,     0,     0,     0,
     163,     0,     0,     0,     0,     0,     0,     0,     0,   266,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   157,   108,     0,     0,     0,     0,   157,     0,     0,
       0,     0,   174,     0,     0,     0,     0,     0,     0,     0,
       0,   153,   153,   153,   153,   208,     0,   211,     0,   162,
       0,     0,     0,   162,     0,     0,     0,   158,     0,     0,
     158,   162,     0,     0,     0,     0,     0,     0,   153,   153,
     163,   261,   263,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   306,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   157,
     316,   157,     0,     0,     0,     0,   157,   158,     0,   157,
       0,     0,   108,   158,     0,     0,     0,   343,   157,     0,
     157,     0,     0,   157,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   285,
       0,     0,     0,    22,    23,     0,     0,   163,     0,     0,
       0,   157,   286,   163,    30,   287,     0,     0,   350,    35,
     351,     0,   157,     0,   157,     0,    40,     0,     0,     0,
       0,   162,     0,     0,   162,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   412,     0,     0,
      55,     0,    57,     0,    59,   158,  1026,   158,     0,  1027,
       0,   288,   158,    65,     0,   158,   417,     0,     0,     0,
       0,     0,   163,     0,   158,     0,   158,     0,     0,   158,
       0,     0,    81,   316,     0,    83,     0,   163,    85,     0,
       0,   162,     0,     0,     0,     0,     0,   162,     0,   316,
       0,   445,     0,     0,     0,     0,     0,   158,     0,     0,
     461,   462,     0,     0,     0,     0,     0,   157,   158,     0,
     158,     0,     0,     0,   157,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   476,     0,     0,   157,   100,     0,
     489,     0,     0,     0,  1028,   481,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   157,     0,     0,     0,     0,
       0,     0,     0,   163,   157,     0,     0,     0,   157,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   162,
       0,   162,     0,     0,     0,     0,   162,     0,     0,   162,
       0,   163,     0,     0,     0,     0,     0,   163,   162,   108,
     162,     0,     0,   162,     0,     0,   163,     0,   163,     0,
       0,     0,     0,   158,   108,     0,     0,   157,     0,     0,
     158,     0,     0,     0,     0,     0,     0,     0,   544,     0,
       0,   162,     0,   158,     0,     0,     0,     0,     0,     0,
       0,     0,   162,     0,   162,     0,     0,     0,     0,     0,
       0,   158,     0,     0,     0,     0,     0,     0,     0,     0,
     158,     0,     0,     0,   158,     0,   285,     0,   163,     0,
      22,    23,   163,     0,   157,   157,   157,   157,     0,   286,
     163,    30,   287,     0,     0,     0,    35,     0,     0,     0,
     108,     0,     0,    40,     0,     0,     0,     0,   617,     0,
       0,   157,   157,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   158,     0,     0,   629,    55,   489,    57,
     632,    59,     0,  1026,   489,     0,  1027,     0,   288,     0,
      65,     0,     0,   656,     0,     0,     0,   162,     0,     0,
       0,     0,     0,     0,   162,     0,     0,     0,     0,    81,
       0,     0,    83,   665,     0,    85,     0,   162,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     158,   158,   158,   158,     0,   162,     0,     0,     0,     0,
       0,     0,     0,     0,   162,     0,     0,     0,   162,     0,
       0,     0,     0,     0,     0,   716,     0,   158,   158,   721,
     163,     0,     0,   163,     0,   100,     0,   108,     0,     0,
     237,  1117,     0,     0,     0,     0,     0,   729,     0,     0,
       0,     0,     0,     0,   238,   239,   744,   240,     0,     0,
       0,     0,   241,     0,     0,     0,     0,   162,     0,     0,
     242,     0,     0,     0,     0,     0,   243,     0,     0,     0,
       0,   244,     0,     0,     0,   245,     0,     0,   246,     0,
     163,     0,     0,     0,     0,     0,   163,     0,   247,     0,
       0,     0,     0,     0,   248,   249,     0,     0,     0,     0,
       0,   250,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,     0,     0,   162,   162,   162,   162,     0,   252,
     253,     0,   254,     0,   255,     0,   256,   796,     0,   257,
     799,     0,     0,   258,     0,     0,   259,     0,     0,   260,
       0,   162,   162,     0,     0,     0,     0,   825,     0,     0,
     828,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   163,     0,
     163,     0,     0,     0,     0,   163,     0,   841,   163,     0,
       0,   481,     0,     0,     0,     0,     0,   163,     0,   163,
       0,     0,   163,     0,     0,   840,     0,     0,     0,     0,
       0,     0,     0,     0,   864,     0,     0,   867,   316,     0,
     285,     0,     0,   872,    22,    23,     0,     0,     0,     0,
     163,     0,     0,   286,     0,    30,   287,     0,     0,     0,
      35,   163,     0,   163,     0,     0,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   935,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,  1026,     0,     0,
    1027,   360,   288,     0,    65,     0,     0,     0,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,    81,     0,   489,    83,   489,     0,    85,
       0,     0,   489,     0,     0,   489,     0,     0,     0,     0,
       0,     0,     0,     0,   979,     0,   980,     0,     0,   982,
       0,     0,     0,     0,     0,     0,   163,     0,     0,     0,
     984,     0,   665,   163,     0,     0,   988,     0,     0,   481,
       0,     0,     0,     0,   363,     0,   163,  1002,   364,   100,
       0,     0,     0,     0,     0,  1119,     0,   465,  1008,     0,
    1012,   864,     0,     0,   163,     0,     0,     0,     0,     0,
       0,     0,     0,   163,     0,     0,     0,   163,     0,   365,
     466,   391,   392,   393,   394,   395,   744,     0,   398,   399,
     400,   401,     0,   403,   404,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,     0,
       0,   386,   387,   388,     0,   420,   163,     0,     0,     0,
       0,     0,   389,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1064,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   163,   163,   163,   163,     0,     0,     0,
       0,  1084,     0,     0,     0,     0,     0,     0,     0,     0,
    1089,     0,     0,     0,  1091,     0,     0,     0,     0,     0,
     163,   163,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   935,     0,     0,
      -2,     4,     0,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,  1125,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,   -76,     0,    38,    39,    40,
       0,    41,  -297,     0,    42,    43,    44,    45,    46,     0,
      47,    48,    49,   -47,    50,    51,     0,    52,    53,    54,
       0,     0,     0,    55,    56,    57,    58,    59,    60,    61,
    -297,   -47,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,     0,    83,    84,
     -76,    85,    86,     0,     0,    87,     0,    88,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   100,     0,     0,   101,     0,   102,     0,   103,     0,
     104,     0,     4,   105,     5,     0,     6,     7,     8,     9,
      10,    11,     0,  -633,     0,    12,    13,    14,    15,    16,
    -633,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,  -633,    27,    28,  -633,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,   -76,     0,    38,    39,
      40,     0,    41,  -297,     0,    42,    43,    44,    45,    46,
       0,    47,    48,    49,   -47,    50,    51,     0,    52,    53,
      54,     0,     0,     0,    55,    56,    57,    58,     0,    60,
      61,  -297,   -47,    62,    63,    64,  -633,    65,    66,    67,
    -633,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -76,    85,    86,     0,     0,    87,     0,    88,     0,
       0,  -633,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -633,  -633,    92,
    -633,  -633,  -633,  -633,  -633,  -633,  -633,     0,  -633,  -633,
    -633,  -633,  -633,     0,  -633,  -633,  -633,  -633,  -633,  -633,
    -633,  -633,   100,  -633,  -633,  -633,     0,   102,  -633,   103,
       0,   104,     0,   326,  -633,     5,   290,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -76,     0,    38,
      39,    40,     0,    41,  -297,     0,    42,    43,    44,    45,
      46,     0,    47,    48,    49,   -47,    50,    51,     0,    52,
      53,    54,     0,     0,     0,    55,    56,    57,    58,    59,
      60,    61,  -297,   -47,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,     0,
      83,    84,   -76,    85,    86,     0,     0,    87,     0,    88,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,   100,     0,     0,   101,     0,   102,   327,
     103,     0,   104,     0,     4,   105,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -76,     0,
      38,    39,    40,     0,    41,  -297,     0,    42,    43,    44,
      45,    46,     0,    47,    48,    49,   -47,    50,    51,     0,
      52,    53,    54,     0,     0,     0,    55,    56,    57,    58,
      59,    60,    61,  -297,   -47,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -76,    85,    86,     0,     0,    87,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
     516,   103,     0,   104,     0,   535,   105,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -76,
       0,    38,    39,    40,     0,    41,  -297,     0,    42,    43,
      44,    45,    46,     0,    47,    48,    49,   -47,    50,    51,
       0,    52,    53,    54,     0,     0,     0,    55,    56,    57,
      58,    59,    60,    61,  -297,   -47,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -76,    85,    86,     0,     0,    87,
       0,    88,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,   536,   103,     0,   104,     0,   326,   105,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     -76,     0,    38,    39,    40,     0,    41,  -297,     0,    42,
      43,    44,    45,    46,     0,    47,    48,    49,   -47,    50,
      51,     0,    52,    53,    54,     0,     0,     0,    55,    56,
      57,    58,    59,    60,    61,  -297,   -47,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -76,    85,    86,     0,     0,
      87,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,   327,   103,     0,   104,     0,     4,   105,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -76,     0,    38,    39,    40,     0,    41,  -297,     0,
      42,    43,    44,    45,    46,     0,    47,    48,    49,   -47,
      50,    51,     0,    52,    53,    54,     0,     0,     0,    55,
      56,    57,    58,    59,    60,    61,  -297,   -47,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,     0,    83,    84,   -76,    85,    86,     0,
       0,    87,     0,    88,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,   100,     0,     0,
     101,     0,   102,   728,   103,     0,   104,     0,     4,   105,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,   -76,     0,    38,    39,    40,     0,    41,  -297,
       0,    42,    43,    44,    45,    46,     0,    47,    48,    49,
     -47,    50,    51,     0,    52,    53,    54,     0,     0,     0,
      55,    56,    57,    58,   342,    60,    61,  -297,   -47,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -76,    85,    86,
       0,     0,    87,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,   104,     0,     4,
     105,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -76,     0,    38,    39,    40,     0,    41,
    -297,     0,    42,    43,    44,    45,    46,     0,    47,    48,
      49,   -47,    50,    51,     0,    52,    53,    54,     0,     0,
       0,    55,    56,    57,    58,     0,    60,    61,  -297,   -47,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,    75,     0,    76,    77,     0,
      78,    79,    80,    81,    82,     0,    83,    84,   -76,    85,
      86,     0,     0,    87,     0,    88,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,   100,
       0,     0,   101,     0,   102,     0,   103,     0,   104,     0,
       4,   105,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,   637,    37,   -76,     0,    38,    39,    40,     0,
      41,  -297,     0,    42,    43,    44,    45,    46,     0,    47,
      48,    49,   -47,    50,    51,     0,    52,    53,    54,     0,
       0,     0,    55,    56,    57,    58,     0,    60,    61,  -297,
     -47,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -76,
      85,    86,     0,     0,    87,     0,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,   104,
       0,     4,   105,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   643,    37,   -76,     0,    38,    39,    40,
       0,    41,  -297,     0,    42,    43,    44,    45,    46,     0,
      47,    48,    49,   -47,    50,    51,     0,    52,    53,    54,
       0,     0,     0,    55,    56,    57,    58,     0,    60,    61,
    -297,   -47,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,     0,    83,    84,
     -76,    85,    86,     0,     0,    87,     0,    88,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   100,     0,     0,   101,     0,   102,     0,   103,     0,
     104,     0,     4,   105,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   964,    37,   -76,     0,    38,    39,
      40,     0,    41,  -297,     0,    42,    43,    44,    45,    46,
       0,    47,    48,    49,   -47,    50,    51,     0,    52,    53,
      54,     0,     0,     0,    55,    56,    57,    58,     0,    60,
      61,  -297,   -47,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -76,    85,    86,     0,     0,    87,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,   104,     0,     4,   105,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   966,    37,   -76,     0,    38,
      39,    40,     0,    41,  -297,     0,    42,    43,    44,    45,
      46,     0,    47,    48,    49,   -47,    50,    51,     0,    52,
      53,    54,     0,     0,     0,    55,    56,    57,    58,     0,
      60,    61,  -297,   -47,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,     0,
      83,    84,   -76,    85,    86,     0,     0,    87,     0,    88,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,   100,     0,     0,   101,     0,   102,     0,
     103,     0,   104,     0,     4,   105,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,   971,    37,   -76,     0,
      38,    39,    40,     0,    41,  -297,     0,    42,    43,    44,
      45,    46,     0,    47,    48,    49,   -47,    50,    51,     0,
      52,    53,    54,     0,     0,     0,    55,    56,    57,    58,
       0,    60,    61,  -297,   -47,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -76,    85,    86,     0,     0,    87,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,   104,     0,     4,   105,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   974,    37,   -76,
       0,    38,    39,    40,     0,    41,  -297,     0,    42,    43,
      44,    45,    46,     0,    47,    48,    49,   -47,    50,    51,
       0,    52,    53,    54,     0,     0,     0,    55,    56,    57,
      58,     0,    60,    61,  -297,   -47,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -76,    85,    86,     0,     0,    87,
       0,    88,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,     0,   103,     0,   104,     0,     4,   105,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,  1001,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     -76,     0,    38,    39,    40,     0,    41,  -297,     0,    42,
      43,    44,    45,    46,     0,    47,    48,    49,   -47,    50,
      51,     0,    52,    53,    54,     0,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -297,   -47,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -76,    85,    86,     0,     0,
      87,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,   104,     0,     4,   105,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,  1007,
      37,   -76,     0,    38,    39,    40,     0,    41,  -297,     0,
      42,    43,    44,    45,    46,     0,    47,    48,    49,   -47,
      50,    51,     0,    52,    53,    54,     0,     0,     0,    55,
      56,    57,    58,     0,    60,    61,  -297,   -47,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,     0,    83,    84,   -76,    85,    86,     0,
       0,    87,     0,    88,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,   100,     0,     0,
     101,     0,   102,     0,   103,     0,   104,     0,     4,   105,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
    1011,    37,   -76,     0,    38,    39,    40,     0,    41,  -297,
       0,    42,    43,    44,    45,    46,     0,    47,    48,    49,
     -47,    50,    51,     0,    52,    53,    54,     0,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -297,   -47,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -76,    85,    86,
       0,     0,    87,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,   104,     0,  1197,
     105,     5,   290,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,  1198,
     850,   105,     5,   290,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,   228,    23,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,    57,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,    65,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,    85,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,     0,
       0,   105,     5,   290,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,   294,   295,     0,    82,   331,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,   332,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     0,   105,     5,   290,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,   294,   295,     0,    82,   331,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,   712,
     191,     0,     0,   105,     5,   290,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,   294,   295,     0,    82,   331,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
     714,   191,     0,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,   600,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,   601,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,   602,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,   603,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,     0,
     191,     0,     5,   105,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,   294,   295,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     296,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     0,   105,     5,   290,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,   294,   295,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,     0,
     191,     0,     5,   105,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,   852,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,   294,   295,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     5,   105,     6,     7,     8,   217,    10,    11,   218,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   219,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,   220,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,     0,
       5,   105,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,   222,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,   223,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,   231,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,   232,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,     0,     0,   105,
       5,   290,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,   223,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,   294,
     295,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,     0,     0,   105,
       5,   290,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,   710,   103,     0,   191,     0,     0,
     105,     5,   290,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,   720,
       0,   105,     5,   290,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
    1129,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,   223,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,     0,
       5,   105,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,   270,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   102,     0,   103,     0,   191,     0,     0,
     105,     5,   290,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,     0,
       5,   105,     6,     7,     8,   217,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   219,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,   220,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,   725,     5,   105,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,   878,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,   191,     0,     0,   105,     5,
     290,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   684,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,   685,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,   686,
      72,    73,    74,   687,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,  1068,     0,     5,   105,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,  1093,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,   191,     0,     0,   105,     5,
     290,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,  1068,     0,     0,   105,
       5,   290,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
    1049,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,     0,     5,   105,
       6,     7,     8,   213,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   214,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,   191,     0,     5,   105,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   183,
       0,     0,     0,    32,   184,   185,     0,   186,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     102,     0,   103,     0,   191,     0,     5,   105,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,   186,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   648,     0,   190,
       0,   103,     0,   191,     0,     5,   105,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,   684,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
     685,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,   686,    72,    73,    74,   687,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   688,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   688,     0,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   780,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,     0,
     191,     0,     5,   105,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   783,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,  1043,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,   191,     0,
       5,   105,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
    1044,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,  1046,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,     0,     5,   105,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,  1047,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,   191,     0,     5,   105,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   183,
       0,     0,     0,    32,   184,   185,     0,  1048,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     190,     0,   103,     0,   191,     0,     5,   105,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,  1049,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   190,
       0,   103,     0,   191,     0,     5,   105,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,   684,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
     685,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,   686,    72,    73,    74,   687,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,  1153,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   891,     0,   892,    90,    91,    92,
      93,    94,     0,     0,     0,   893,    95,     0,     0,     0,
     894,   239,   895,   896,     0,     0,    96,     0,   897,    97,
      98,    99,     0,     0,     0,   101,   242,     0,   183,   103,
       0,  1153,   898,     0,   105,     0,     0,   899,     0,     0,
       0,   245,     0,     0,   900,     0,   901,     0,     0,     0,
       0,     0,     0,     0,   902,     0,     0,     0,     0,     0,
     903,   904,     0,     0,     0,     0,     0,   250,     0,     0,
       0,     0,     0,     0,     0,     0,   905,     0,     0,     0,
       0,     0,     0,     0,     0,   252,   253,     0,   906,     0,
     255,     0,   907,     0,     0,   908,     0,     0,     0,   909,
       0,     0,   259,     0,     0,   910,     0,     0,     0,     0,
       0,     0,     0,     0,   466,   391,   392,   393,   394,   395,
       0,     0,   398,   399,   400,   401,     0,   403,   404,   911,
     912,   913,   914,   915,     0,     0,   916,     0,     0,     0,
     917,   918,   919,   920,   921,   922,   923,   924,   925,   926,
     927,     0,   928,     0,     0,   929,   930,   931,   932,     0,
       5,   933,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,     0,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     5,   290,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,  -461,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,  -461,     0,     0,
       0,  -461,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,   190,     0,   103,  -461,  1068,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,   186,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,  -443,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,  -443,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,  -443,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,  -443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,    92,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,    99,  -443,     0,     0,     0,     0,  -443,
      28,   103,   183,  1153,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,     5,    92,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,    99,     0,     0,
       0,     0,     0,   190,    28,   103,   183,   191,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,    92,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,    99,     0,     0,     0,     0,     0,   190,    28,   103,
     183,   688,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,  1106,    78,     0,     0,
       0,    82,     0,     0,    84,   893,     0,    86,     0,     0,
     238,   239,   895,   240,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
       0,     0,   898,    92,     0,     0,     0,   244,     0,     0,
       0,   245,     0,     0,   246,     0,   901,     0,     0,     0,
       0,     0,     0,     0,   247,     0,     0,     0,     0,     0,
     903,   249,     0,   103,     0,   191,     0,   250,     0,     0,
       0,     0,     0,     0,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,   253,   360,   254,     0,
     255,     0,  1107,     0,   361,   908,     0,     0,     0,   258,
       0,     0,   259,     0,     0,   260,   362,     0,     0,     0,
       0,     0,     0,     0,   466,   391,   392,   393,   394,   395,
       0,     0,   398,   399,   400,   401,     0,   403,   404,   911,
     912,   913,   914,   915,     0,     0,   916,     0,     0,     0,
     917,   918,   919,   920,   921,   922,   923,   924,   925,   926,
     927,     0,   928,     0,     0,   929,   930,   931,   932,     0,
     363,     0,     0,   360,   364,     0,     0,     0,     0,     0,
     361,     0,     0,   963,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,   466,   391,   392,   393,
     394,   395,     0,     0,   398,   399,   400,   401,     0,   403,
     404,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,     0,   374,   375,   376,   377,   378,     0,   379,   380,
     381,   382,   383,   384,   385,     0,   363,   386,   387,   388,
     364,   420,     0,   360,     0,     0,     0,     0,   389,   970,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,   365,   466,   391,   392,   393,   394,   395,     0,     0,
     398,   399,   400,   401,     0,   403,   404,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,     0,   374,   375,
     376,   377,   378,     0,   379,   380,   381,   382,   383,   384,
     385,     0,     0,   386,   387,   388,   363,   420,     0,   360,
     364,     0,     0,     0,   389,     0,   361,     0,     0,  1120,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   365,   466,   391,   392,   393,   394,   395,     0,     0,
     398,   399,   400,   401,     0,   403,   404,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,     0,   374,   375,
     376,   377,   378,     0,   379,   380,   381,   382,   383,   384,
     385,     0,   363,   386,   387,   388,   364,   420,     0,   360,
       0,     0,     0,     0,   389,  1121,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,   365,   466,   391,
     392,   393,   394,   395,     0,     0,   398,   399,   400,   401,
       0,   403,   404,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,     0,   386,
     387,   388,   363,   420,     0,   360,   364,     0,     0,     0,
     389,     0,   361,     0,     0,  1122,     0,     0,     0,     0,
       0,     0,     0,     0,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,   466,   391,
     392,   393,   394,   395,     0,     0,   398,   399,   400,   401,
       0,   403,   404,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,   363,   386,
     387,   388,   364,   420,     0,   360,     0,     0,     0,     0,
     389,  1123,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,     0,     0,     0,     0,     0,
       0,     0,     0,   365,   466,   391,   392,   393,   394,   395,
       0,     0,   398,   399,   400,   401,     0,   403,   404,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,     0,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,     0,     0,   386,   387,   388,   363,   420,
       0,   360,   364,     0,     0,     0,   389,     0,   361,     0,
       0,  1147,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   365,   466,   391,   392,   393,   394,   395,
       0,     0,   398,   399,   400,   401,     0,   403,   404,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,     0,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,     0,   363,   386,   387,   388,   364,   420,
       0,     0,     0,   360,     0,     0,   389,  1148,     0,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,   454,     0,     0,     0,     0,     0,   365,
     466,   391,   392,   393,   394,   395,     0,   455,   398,   399,
     400,   401,     0,   403,   404,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,     0,
       0,   386,   387,   388,     0,   420,   363,     0,     0,   360,
     364,     0,   389,     0,     0,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,   448,
       0,     0,     0,     0,     0,     0,     0,   451,     0,     0,
       0,   365,     0,   449,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,     0,   374,   375,
     376,   377,   378,     0,   379,   380,   381,   382,   383,   384,
     385,     0,   363,   386,   387,   388,   364,   420,     0,     0,
       0,   360,     0,     0,   389,     0,     0,     0,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,   458,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,   459,
     379,   380,   381,   382,   383,   384,   385,     0,     0,   386,
     387,   388,     0,   420,   363,     0,     0,     0,   364,     0,
     389,     0,     0,   360,   736,     0,     0,     0,     0,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,   737,     0,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,   738,   379,   380,   381,   382,   383,   384,   385,     0,
     460,   386,   387,   388,     0,     0,   363,     0,     0,   360,
     364,     0,   389,     0,     0,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,   815,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,     0,   374,   375,
     376,   377,   378,     0,   379,   380,   381,   382,   383,   384,
     385,     0,   363,   386,   387,   388,   364,     0,     0,   360,
       0,     0,     0,     0,   389,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,   451,     0,     0,     0,   365,     0,     0,
       0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,     0,   386,
     387,   388,   363,   420,     0,   360,   364,     0,     0,     0,
     389,     0,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,   270,     0,     0,     0,     0,
       0,     0,     0,   176,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,   363,   386,
     387,   388,   364,     0,     0,   360,     0,     0,     0,     0,
     389,     0,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,   609,     0,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,   610,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,     0,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,     0,     0,   386,   387,   388,   363,   420,
       0,   360,   364,     0,     0,     0,   389,     0,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,   611,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   365,     0,   612,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,     0,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,     0,   363,   386,   387,   388,   364,     0,
       0,   360,     0,     0,     0,     0,   389,     0,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,   807,     0,     0,     0,     0,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,     0,
       0,   386,   387,   388,   363,     0,     0,   360,   364,     0,
       0,     0,   389,     0,   361,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,     0,
     363,   386,   387,   388,   364,   420,     0,   360,     0,     0,
       0,     0,   389,   829,   361,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,     0,   374,   375,   376,   377,   378,     0,   379,   380,
     381,   382,   383,   384,   385,     0,     0,   386,   387,   388,
     363,   420,     0,   360,   364,     0,     0,     0,   389,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,  1020,     0,     0,
       0,   451,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,  1021,   374,   375,   376,   377,   378,     0,   379,   380,
     381,   382,   383,   384,   385,     0,   363,   386,   387,   388,
     364,     0,     0,   360,     0,     0,   870,     0,   389,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,     0,   374,   375,
     376,   377,   378,     0,   379,   380,   381,   382,   383,   384,
     385,     0,     0,   386,   387,   388,   363,     0,     0,   360,
     364,     0,     0,     0,   389,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,     0,   374,   375,
     376,   377,   378,     0,   379,   380,   381,   382,   383,   384,
     385,     0,   363,   386,   387,   388,   364,   501,     0,   360,
       0,     0,     0,     0,   389,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,   362,   505,   386,
     387,   388,   363,     0,     0,     0,   364,     0,     0,   360,
     389,     0,     0,     0,     0,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,     0,   366,   367,   364,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,   362,   507,   386,
     387,   388,   363,     0,     0,     0,   364,     0,     0,   360,
     389,     0,     0,     0,     0,     0,   361,     0,     0,     0,
       0,     0,   366,   367,     0,   368,   369,   370,   362,   371,
     372,   373,     0,   374,   375,   376,   377,   365,     0,   379,
     380,   381,   382,   383,     0,   385,     0,     0,   386,   387,
     388,   363,     0,   366,   367,   364,   368,   369,   370,   389,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,   509,   386,
     387,   388,   363,     0,     0,     0,   364,     0,     0,   360,
     389,     0,     0,     0,     0,     0,   361,     0,     0,     0,
       0,     0,   366,   367,     0,   368,   369,   370,   362,   371,
     372,   373,     0,     0,   375,   376,   377,   365,     0,   379,
     380,   381,   382,     0,     0,   385,     0,     0,   386,   387,
     388,     0,     0,   366,   367,     0,   368,   369,   370,   389,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,   514,   386,
     387,   388,   363,     0,     0,   360,   364,     0,     0,     0,
     389,     0,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   531,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,   363,   386,
     387,   388,   364,     0,     0,   360,     0,     0,     0,     0,
     389,   613,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,     0,     0,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,     0,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,     0,     0,   386,   387,   388,   363,     0,
       0,     0,   364,     0,     0,     0,   389,   360,   732,     0,
       0,     0,     0,     0,   361,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,     0,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,     0,     0,   386,   387,   388,     0,     0,
     363,     0,   711,   360,   364,     0,   389,     0,     0,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   367,     0,   368,   369,   370,     0,   371,   733,
     373,     0,   374,   375,   376,   377,   378,     0,   379,   380,
     381,   382,   383,   384,   385,     0,   363,   386,   387,   388,
     364,     0,     0,   360,     0,     0,     0,     0,   389,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,     0,   374,   375,
     376,   377,   378,  -565,   379,   380,   381,   382,   383,   384,
     385,     0,     0,   386,   387,   388,   363,     0,     0,   360,
     364,     0,     0,     0,   389,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,   176,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,     0,   374,   375,
     376,   377,   378,     0,   379,   380,   381,   382,   383,   384,
     385,     0,   363,   386,   387,   388,   364,     0,     0,   360,
     820,     0,     0,     0,   389,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,   792,   386,
     387,   388,   363,     0,     0,   360,   364,     0,     0,     0,
     389,     0,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,     0,     0,   827,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,   363,   386,
     387,   388,   364,     0,     0,   360,     0,     0,     0,     0,
     389,     0,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,     0,     0,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,     0,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,     0,     0,   386,   387,   388,   363,     0,
       0,   360,   364,     0,     0,     0,   389,     0,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,     0,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,     0,   363,   386,   387,   388,   364,     0,
       0,     0,   854,   360,     0,     0,   389,     0,     0,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,     0,
       0,   386,   387,   388,     0,     0,   363,     0,   855,   360,
     364,     0,   389,     0,     0,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,   948,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     873,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,     0,   374,   375,
     376,   377,   378,     0,   379,   380,   381,   382,   383,   384,
     385,     0,   363,   386,   387,   388,   364,     0,     0,   360,
       0,     0,     0,     0,   389,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,   951,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,     0,   386,
     387,   388,   363,     0,     0,   360,   364,     0,     0,     0,
     389,     0,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,   363,   386,
     387,   388,   364,     0,     0,   360,  1017,     0,     0,     0,
     389,     0,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,     0,     0,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,     0,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,     0,     0,   386,   387,   388,   363,     0,
       0,   360,   364,     0,   954,     0,   389,     0,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,     0,
     374,   375,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   384,   385,     0,   363,   386,   387,   388,   364,     0,
       0,   360,     0,     0,     0,     0,   389,  1042,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,     0,
       0,   386,   387,   388,   363,     0,     0,     0,   364,     0,
       0,     0,   389,   360,  1062,     0,     0,  1045,     0,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,     0,
       0,   386,   387,   388,     0,     0,   363,     0,     0,   360,
     364,     0,   389,     0,     0,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,     0,   374,   375,
     376,   377,   378,     0,   379,   380,   381,   382,   383,   384,
     385,     0,   363,   386,   387,   388,   364,     0,     0,     0,
     360,     0,     0,     0,   389,     0,     0,   361,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,     0,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,     0,   386,
     387,   388,     0,   363,     0,     0,  1105,   364,     0,     0,
     389,   360,     0,     0,     0,     0,     0,     0,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,   367,     0,   368,   369,   370,
       0,   371,   372,   373,     0,   374,   375,   376,   377,   378,
       0,   379,   380,   381,   382,   383,   384,   385,     0,     0,
     386,   387,   388,     0,   363,     0,     0,   360,   364,  1143,
       0,   389,     0,     0,   361,     0,     0,  1162,     0,     0,
       0,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   367,     0,   368,   369,
     370,     0,   371,   372,   373,     0,   374,   375,   376,   377,
     378,     0,   379,   380,   381,   382,   383,   384,   385,     0,
     363,   386,   387,   388,   364,     0,     0,   360,     0,     0,
       0,     0,   389,  1163,   361,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,     0,   374,   375,   376,   377,   378,     0,   379,   380,
     381,   382,   383,   384,   385,     0,     0,   386,   387,   388,
     363,     0,     0,   360,   364,     0,     0,     0,   389,     0,
     361,     0,     0,  1164,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   367,     0,   368,   369,   370,     0,   371,   372,
     373,     0,   374,   375,   376,   377,   378,     0,   379,   380,
     381,   382,   383,   384,   385,     0,   363,   386,   387,   388,
     364,     0,     0,   360,     0,     0,     0,     0,   389,  1165,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,     0,   374,   375,
     376,   377,   378,     0,   379,   380,   381,   382,   383,   384,
     385,     0,     0,   386,   387,   388,   363,     0,     0,   360,
     364,     0,     0,     0,   389,     0,   361,     0,     0,  1166,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,     0,
     368,   369,   370,     0,   371,   372,   373,     0,   374,   375,
     376,   377,   378,     0,   379,   380,   381,   382,   383,   384,
     385,     0,   363,   386,   387,   388,   364,     0,     0,     0,
       0,     0,   360,     0,   389,  1167,     0,     0,     0,   361,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,   368,   369,   370,  1191,
     371,   372,   373,     0,   374,   375,   376,   377,   378,     0,
     379,   380,   381,   382,   383,   384,   385,     0,     0,   386,
     387,   388,     0,     0,     0,   363,     0,     0,   360,   364,
     389,     0,     0,     0,     0,   361,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     365,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,   367,     0,   368,
     369,   370,     0,   371,   372,   373,     0,   374,   375,   376,
     377,   378,     0,   379,   380,   381,   382,   383,   384,   385,
       0,   363,   386,   387,   388,   364,     0,     0,   360,     0,
       0,     0,     0,   389,     0,   361,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,   367,     0,   368,   369,   370,     0,   371,
     372,   373,     0,   374,   375,   376,   377,   378,     0,   379,
     380,   381,   382,   383,   384,   385,     0,     0,   386,   387,
     388,   363,     0,     0,   360,   364,     0,  1220,     0,   389,
       0,   361,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,   367,     0,   368,   369,   370,     0,   371,
     372,   373,     0,   374,   375,   376,   377,   378,     0,   379,
     380,   381,   382,   383,   384,   385,     0,   778,   386,   387,
     388,   364,     0,     0,     0,     0,     0,     0,     0,   389,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,   367,
       0,   368,   369,   370,     0,   371,   372,   373,     0,   374,
     375,   376,   377,   378,     0,   379,   380,   381,   382,   383,
     384,   385,     0,   285,   386,   387,   388,    22,    23,     0,
       0,     0,     0,     0,     0,   389,   286,     0,    30,   287,
     285,     0,     0,    35,    22,    23,     0,     0,     0,     0,
      40,     0,     0,   286,     0,    30,   287,     0,     0,     0,
      35,     0,     0,     0,   362,     0,     0,    40,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
    1026,     0,     0,  1027,     0,   288,     0,    65,     0,     0,
       0,    55,     0,    57,     0,    59,     0,  1026,     0,     0,
    1027,     0,   288,     0,    65,   362,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,   363,     0,
       0,     0,   364,    81,     0,     0,    83,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,   363,
       0,     0,   100,   364,     0,     0,     0,     0,  1168,   366,
     367,     0,   368,   369,   370,     0,   371,   372,   373,   100,
     374,   375,   376,   377,     0,  1169,   379,   380,   381,   382,
     383,   384,   385,     0,   365,   386,   387,   388,     0,     0,
       0,     0,     0,     0,     0,     0,   389,     0,     0,     0,
     366,   367,     0,   368,   369,   370,     0,   371,   372,   373,
       0,   374,   375,   376,   377,     0,     0,   379,   380,   381,
     382,   383,   743,   385,   237,     0,   386,   387,   388,     0,
       0,     0,     0,     0,     0,     0,     0,   389,   238,   239,
       0,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
     243,  -278,     0,     0,     0,   244,     0,     0,     0,   245,
       0,     0,   246,     0,     0,  -278,  -278,     0,  -278,     0,
       0,     0,   247,  -278,     0,     0,     0,     0,   248,   249,
       0,  -278,     0,     0,     0,   250,     0,  -278,     0,     0,
       0,     0,  -278,     0,   251,     0,  -278,     0,     0,  -278,
       0,     0,     0,   252,   253,     0,   254,     0,   255,  -278,
     256,     0,     0,   257,     0,  -278,  -278,   258,     0,     0,
     259,     0,  -278,   260,     0,     0,     0,     0,     0,     0,
       0,  -278,     0,     0,  -288,     0,     0,     0,     0,     0,
    -278,  -278,     0,  -278,     0,  -278,     0,  -278,  -288,  -288,
    -278,  -288,     0,     0,  -278,     0,  -288,  -278,     0,     0,
    -278,     0,     0,     0,  -288,     0,     0,     0,     0,     0,
    -288,   237,     0,     0,     0,  -288,     0,     0,     0,  -288,
       0,     0,  -288,     0,     0,   238,   239,     0,   240,     0,
       0,     0,  -288,   241,     0,     0,     0,     0,  -288,  -288,
       0,   242,     0,     0,     0,  -288,     0,   243,     0,     0,
       0,     0,   244,     0,  -288,     0,   245,     0,     0,   246,
       0,     0,     0,  -288,  -288,     0,  -288,     0,  -288,   247,
    -288,     0,     0,  -288,     0,   248,   249,  -288,     0,     0,
    -288,     0,   250,  -288,     0,     0,     0,     0,     0,     0,
       0,   251,     0,     0,  -279,     0,     0,     0,     0,     0,
     252,   253,     0,   254,     0,   255,     0,   256,  -279,  -279,
     257,  -279,     0,     0,   258,     0,  -279,   259,     0,     0,
     260,     0,     0,     0,  -279,     0,     0,     0,     0,     0,
    -279,  -174,     0,     0,     0,  -279,     0,     0,     0,  -279,
       0,     0,  -279,     0,     0,  -174,  -174,     0,  -174,     0,
       0,     0,  -279,  -174,     0,     0,     0,     0,  -279,  -279,
       0,  -174,     0,     0,     0,  -279,     0,  -174,     0,     0,
       0,     0,  -174,     0,  -279,     0,  -174,     0,     0,  -174,
       0,     0,     0,  -279,  -279,     0,  -279,     0,  -279,  -174,
    -279,     0,     0,  -279,     0,  -174,  -174,  -279,     0,     0,
    -279,     0,  -174,  -279,     0,     0,     0,     0,     0,     0,
       0,  -174,     0,     0,  -170,     0,     0,     0,     0,     0,
    -174,  -174,     0,  -174,     0,  -174,     0,  -174,  -170,  -170,
    -174,  -170,     0,     0,  -174,     0,  -170,  -174,     0,     0,
    -174,     0,     0,     0,  -170,     0,     0,     0,     0,     0,
    -170,     0,     0,     0,     0,  -170,     0,     0,     0,  -170,
       0,     0,  -170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -170,     0,     0,     0,     0,     0,  -170,  -170,
       0,     0,     0,     0,     0,  -170,     0,     0,     0,     0,
       0,     0,     0,     0,  -170,     0,     0,     0,     0,     0,
       0,     0,     0,  -170,  -170,     0,  -170,     0,  -170,     0,
    -170,     0,     0,  -170,     0,     0,     0,  -170,     0,     0,
    -170,     0,     0,  -170
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-1041))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,    62,    66,     2,    16,   459,   227,   411,    20,    19,
     641,    33,    34,   160,    26,   510,   204,   752,    30,    31,
       1,    33,    34,    35,    36,   738,   146,   236,   737,   480,
      46,     1,   220,    46,    22,    47,    32,    49,   849,   991,
     104,   683,    54,    88,    56,   884,    58,   938,   939,   212,
      32,     1,     3,   477,    66,    67,    68,    69,    70,    71,
      46,    32,   473,    75,    69,    77,     1,    79,    80,   116,
       1,    32,    84,   236,   283,    87,    88,    66,    90,   485,
      92,    98,   104,    95,    96,   205,    98,   142,   988,  1129,
     102,   103,   104,   105,   629,    32,   305,   632,    54,     0,
     116,    71,   136,  1078,   510,   142,    45,   161,   116,    59,
     144,    46,   167,   102,   103,   104,   337,   338,     3,    46,
     283,   166,    54,   116,   171,  1066,   142,   191,    59,   142,
     167,    46,   943,   764,   116,   777,     3,    88,    88,    67,
     121,  1181,   305,   113,   156,   116,   142,    46,    99,   166,
    1102,   101,    46,    20,    71,   171,   142,    88,   171,    86,
     141,   142,     3,   171,   114,   312,    33,  1067,    54,   181,
     166,   110,   184,   185,   186,   103,    88,   189,   190,   191,
      54,   992,   161,   114,   166,   171,   121,  1128,   169,   168,
    1029,   141,     3,   203,    61,   166,   113,   167,   407,   408,
     212,   190,   191,   614,   846,   166,   141,   142,    31,   372,
     141,   161,   168,  1052,    99,  1054,   166,  1108,  1109,   224,
     225,   226,   227,   284,   236,  1200,   161,   142,   216,   166,
       3,   682,    99,   221,    59,   166,   168,   641,   105,   959,
     228,   961,   121,   142,   407,   408,    46,    20,   142,   161,
     167,    45,  1181,    52,   166,  1066,   171,   114,    99,    54,
      33,   796,    85,   142,   799,   142,    89,   146,   273,    63,
      36,   283,   171,   142,   279,   287,   121,  1116,   142,  1118,
     142,    80,   294,   295,   738,   142,    86,    81,    61,   114,
     169,  1220,   169,   305,   503,    27,   301,    54,   142,   166,
     142,   146,   171,  1038,    70,   169,  1019,   169,  1021,  1018,
     473,  1020,   142,   136,   319,   142,   110,  1128,   141,   439,
     332,   144,   161,   167,   444,   148,    99,   169,   167,   740,
     329,  1051,   105,  1053,   161,   347,   348,   968,    86,   169,
     503,   363,   364,    75,   168,    54,   358,    18,   360,   361,
     362,   363,   364,   365,    86,   367,   368,   369,   370,   371,
     764,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   789,   111,
      27,   121,   446,   168,   396,   397,   142,   409,   410,   840,
     402,  1198,   142,   405,   406,   407,   408,   409,   410,   411,
     422,   142,   142,    74,   142,   142,   146,   142,   136,  1216,
     422,   142,    54,   169,   838,   427,   144,   429,   423,   169,
     161,    92,   876,    27,   645,   646,   161,   449,    75,   169,
     161,   169,   169,   455,   446,    31,   448,   449,   109,    86,
      27,    54,   454,   455,   161,   142,   458,   459,   453,   142,
     167,   614,   457,   465,   136,    54,   468,   446,   105,    46,
     884,   473,   144,    50,   627,   142,    27,   142,   396,   397,
      54,    75,   169,   142,   402,   487,   169,   405,   406,   491,
     492,   545,    86,   142,   496,    72,    79,    80,    75,    85,
      77,   503,   169,    89,   169,    54,     8,   718,   719,    86,
     169,   105,   161,   998,    47,   527,   518,   496,   216,   521,
     168,   523,   142,   221,    75,   527,   528,   529,   105,   531,
     532,   709,   142,   142,   142,    86,    69,    27,   168,   518,
     170,   161,   521,   545,   523,    54,    79,    80,   537,   528,
     136,   161,   161,   161,   105,   141,   136,   143,   144,    49,
     111,   141,   148,    85,   144,   142,   545,    89,   148,    85,
     156,    96,    54,    89,   968,  1019,   168,  1021,   164,   732,
     733,    79,   161,    73,    82,    75,   168,   740,   170,   168,
      54,   170,   169,    54,    54,   991,    86,   168,   610,   170,
     612,    54,   998,   521,  1134,   523,   166,   609,   610,   611,
     612,   613,   614,   615,   616,   752,    81,    54,   618,   619,
    1034,   111,   168,   625,   170,   627,   166,   168,   168,   170,
      46,   161,   160,   635,   688,   637,   789,   161,  1052,   641,
    1054,   643,   161,    59,   639,   640,   625,   121,   152,   161,
     645,   646,   654,   655,   168,  1185,  1186,  1187,  1188,  1189,
    1190,   168,   161,   170,     2,   521,   161,   523,    59,   202,
     161,    54,    88,   147,    12,   149,   150,   161,   166,   153,
     154,   161,   115,    84,   686,   687,   688,    25,    84,    27,
     692,   224,   225,   226,   227,   168,   168,    88,   114,   168,
     233,   234,  1116,   161,  1118,   121,  1102,   625,   121,   688,
     101,   168,   168,   168,   161,     8,    63,   141,   166,   144,
      66,   161,   168,   114,   121,   169,   142,   142,   730,    59,
     263,   161,   734,  1129,   736,   737,   738,    75,   740,    46,
     273,   168,   121,   121,    34,   161,   279,  1143,    34,   169,
     166,    59,   142,   169,   121,   169,    21,   169,    88,   169,
     142,   166,   764,   171,   102,   171,   778,   779,   301,   107,
     161,   101,   161,   168,   692,   166,   778,   779,   780,   171,
      88,   783,   169,   168,   114,  1181,   319,   789,   161,   161,
      31,   161,    27,   101,   142,   790,   791,   161,   161,   161,
     999,  1000,  1198,   805,   858,   807,   114,   802,   161,   161,
     812,    46,    34,   815,    34,    50,   818,    63,   820,   814,
    1216,   142,   817,   167,  1220,   827,   169,   169,   167,   121,
      86,   161,   169,    32,    32,   171,   166,    72,    27,   177,
      75,   166,    77,   166,    85,   161,   999,  1000,    89,    34,
     141,    86,    86,   161,   171,   168,   858,    46,   166,   168,
     141,    50,   167,   161,   161,   171,   868,    46,   870,    46,
     105,   873,   171,   168,   876,   144,   144,   144,   136,   858,
      59,     2,    59,    72,   886,   144,    75,   144,    77,   144,
     423,    12,   136,   144,   144,   136,   137,    86,   139,   140,
     141,  1038,   143,   144,    25,   144,    27,   148,   136,    88,
     136,    88,   144,   144,   155,   156,   105,   136,   159,   144,
     453,   162,   163,   164,   457,    34,   264,   169,   166,   166,
     101,   933,   270,   169,   161,   114,   169,   114,   161,   169,
     161,   136,   121,   161,   121,   947,   948,   161,   950,   951,
     161,   953,   954,   136,    75,   160,   160,   730,    79,    80,
     142,   963,   964,   142,   966,   142,   968,   344,   970,   971,
     171,    31,   974,    24,   471,   833,  1027,   862,   313,   887,
    1038,   102,   161,   838,   161,  1034,   107,   166,  1200,   166,
     169,   329,   169,   675,   890,   997,   998,   999,  1000,  1158,
    1191,  1184,    35,  1005,   627,  1007,   344,   488,  1003,  1011,
      30,    -1,   991,    -1,  1068,  1017,  1018,  1019,  1020,  1021,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1042,  1043,  1044,  1045,  1046,  1047,  1048,  1049,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
    1062,    -1,    -1,    -1,    -1,    -1,  1068,    -1,    -1,    -1,
      -1,  1073,    -1,    -1,    -1,  1077,   136,   137,    -1,   139,
     140,   141,   420,   143,   144,    -1,    -1,    -1,   148,  1068,
      -1,    -1,    -1,    -1,  1073,    -1,   156,    -1,    -1,  1153,
      31,    -1,   162,   163,   164,    -1,   639,   640,    -1,    -1,
     448,    -1,   645,   646,    -1,    -1,   454,    31,  1120,  1121,
    1122,  1123,    -1,    -1,    -1,   463,    -1,   465,   661,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1143,    -1,   264,    -1,  1147,  1148,    -1,    -1,   270,
    1129,  1153,    -1,    -1,    85,    -1,    -1,    -1,    89,    -1,
    1162,  1163,  1164,  1165,  1166,  1167,    -1,    -1,    -1,    -1,
      -1,    85,   705,    -1,  1153,    89,    -1,    -1,  1180,  1181,
      -1,  1183,    -1,    -1,    -1,    31,    -1,   525,    -1,    -1,
      -1,   529,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   537,
      -1,    -1,  1181,    -1,    -1,   136,   137,    -1,   329,   140,
     141,    -1,   143,   144,  1216,    -1,    -1,   148,  1220,    -1,
      -1,    -1,   136,   344,    -1,   156,    -1,   141,    -1,   143,
     144,   162,   163,   164,   148,    -1,     2,    -1,    -1,    85,
      -1,  1220,   156,    89,    -1,    -1,    12,    -1,   162,   163,
     164,    -1,     2,    -1,    -1,    -1,    -1,   790,   791,    25,
      -1,    27,    12,    -1,    -1,    -1,    -1,    -1,   606,   802,
      -1,   804,    -1,    -1,    -1,    25,    -1,    27,   811,    -1,
      -1,   814,    -1,    -1,   817,    -1,    -1,     1,    -1,    -1,
     136,   137,    -1,    31,    -1,   141,    -1,   143,   144,   420,
      -1,   834,   148,    -1,    -1,    -1,    -1,    -1,    22,    75,
     156,    -1,    26,    27,    -1,    -1,   162,   163,   164,   657,
      -1,    35,   660,    37,    38,    75,    -1,   448,    42,    -1,
      -1,    -1,   865,   454,    -1,    49,   102,    -1,    52,    -1,
      -1,   107,   463,    -1,   465,    -1,    -1,    85,    -1,    -1,
      -1,    89,   102,    -1,    -1,    -1,    -1,   107,    -1,    73,
      -1,    75,    -1,    77,    -1,    79,    80,    -1,    82,    -1,
      84,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,   717,
      -1,    -1,    -1,    -1,    -1,   723,    -1,    -1,    -1,    -1,
      -1,   105,    -1,    -1,   108,    -1,    -1,   111,   136,   137,
      -1,   139,   140,   141,   525,   143,   144,   145,   529,    -1,
     148,   177,    -1,    -1,   752,    -1,   537,   155,   156,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
     778,   779,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,
      -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   805,    -1,   807,
    1003,    -1,    -1,    -1,   812,    -1,    -1,   815,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   824,    -1,   826,    -1,
      -1,   829,    -1,     2,    -1,    -1,    -1,    -1,   264,    -1,
      -1,    -1,    -1,    12,   270,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   264,    -1,    25,    -1,    27,   857,
     270,    -1,    -1,  1056,  1057,    -1,    -1,  1060,  1061,    -1,
     868,    -1,   870,    -1,    -1,    -1,   657,    -1,    -1,   660,
      -1,    -1,    -1,    -1,    -1,  1078,    -1,     3,    -1,    -1,
      -1,    -1,    -1,  1086,    -1,  1088,    -1,  1090,    -1,    -1,
      -1,    17,    18,   329,    20,    -1,    75,    -1,     3,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,   344,   329,
      -1,    -1,    17,    39,    -1,    20,    -1,    -1,    44,    -1,
      25,    -1,    48,   102,   344,    51,   717,    -1,   107,    -1,
      35,    -1,   723,    -1,    -1,    61,    -1,    -1,    -1,    44,
      -1,    67,    68,    -1,    -1,    -1,    51,    -1,    74,    -1,
      -1,    -1,    -1,    -1,    -1,   963,    61,    83,    -1,    -1,
      -1,    -1,   970,    68,    -1,    -1,    92,    93,    -1,    95,
      -1,    97,    -1,    99,    -1,   983,   102,    -1,    83,    -1,
     106,   107,    -1,   109,   420,    -1,   112,    -1,    -1,    -1,
      95,    -1,    -1,  1001,    99,    -1,    -1,  1200,   177,    -1,
     420,   106,  1010,    -1,    -1,    -1,  1014,   112,    -1,    -1,
      -1,    -1,   448,    -1,   805,    -1,   807,    -1,   454,    -1,
      -1,   812,    -1,    -1,   815,    -1,    -1,   463,   448,   465,
      -1,    -1,    -1,   824,   454,   826,    -1,    -1,   829,     3,
      -1,    -1,   168,   463,    -1,   465,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,  1063,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,   857,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,   868,    -1,   870,
      44,    -1,    -1,    -1,    48,   264,    -1,    51,    -1,   525,
      -1,   270,    -1,   529,    -1,    -1,    -1,    61,    -1,    -1,
      -1,   537,    -1,    67,    68,   525,    -1,    -1,    -1,   529,
      74,    -1,  1120,  1121,  1122,  1123,    -1,   537,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      -1,    95,    -1,    97,    -1,    99,    -1,    -1,   102,  1147,
    1148,    -1,   106,    -1,    -1,   109,    -1,    22,   112,    -1,
     329,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    37,    38,    -1,   344,    -1,    42,    -1,    -1,
      45,    -1,   963,    -1,    49,    -1,    -1,    52,    -1,   970,
      -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,   983,    -1,    35,    -1,    37,    38,    73,    -1,
      75,    42,   166,    -1,   168,    80,     2,    -1,    49,    84,
    1001,    86,    -1,    -1,    -1,    -1,    12,    -1,    -1,  1010,
      -1,   657,    -1,  1014,   660,    -1,    -1,    -1,    -1,    25,
     105,    27,    73,   108,    75,   110,   111,   657,    -1,    -1,
     660,   420,    -1,    84,    -1,    86,    -1,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,   105,    -1,    -1,   108,    -1,   448,
     111,    -1,  1063,    -1,    -1,   454,    -1,    -1,   152,    75,
      -1,   717,    -1,    -1,   463,    -1,   465,   723,    -1,    -1,
      -1,   165,    -1,    -1,   168,    -1,   170,   717,    -1,    -1,
      -1,    -1,     2,   723,    -1,    -1,   102,    -1,    -1,    -1,
      -1,   107,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,  1120,
    1121,  1122,  1123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   525,    -1,    -1,    -1,
     529,    -1,    -1,    -1,    -1,    -1,  1147,  1148,   537,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   805,
      -1,   807,    -1,    -1,    -1,    75,   812,    -1,    -1,   815,
      -1,   177,    -1,    -1,    -1,   805,    -1,   807,   824,    -1,
     826,    -1,   812,   829,    -1,   815,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,   824,    -1,   826,   107,    -1,   829,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   857,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,
      -1,    -1,   868,    -1,   870,    -1,    12,   857,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   868,    25,
     870,    27,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   264,    35,
      -1,    37,    38,    -1,   270,    -1,    42,   177,   657,    -1,
      -1,   660,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    82,    -1,    84,    -1,
      86,    -1,    -1,    -1,    -1,    -1,   102,   963,    -1,    -1,
      -1,   107,    -1,   329,   970,    -1,    -1,    -1,   717,   105,
      -1,    -1,   108,   963,   723,   111,    -1,   983,   344,    -1,
     970,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   983,   264,  1001,    -1,    -1,    -1,    -1,
     270,    -1,    -1,    -1,  1010,    -1,    -1,    -1,  1014,    -1,
      -1,  1001,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1010,    -1,    -1,    -1,  1014,   161,    -1,    -1,    -1,    -1,
      -1,   177,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   420,    -1,   805,  1063,   807,   329,
      -1,    -1,    -1,   812,    -1,    -1,   815,    -1,    -1,    -1,
      -1,    -1,    -1,  1063,   344,   824,   165,   826,    -1,   168,
     829,   170,   448,    -1,    -1,    -1,    -1,    -1,   454,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   463,    -1,   465,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   857,    -1,
      -1,    -1,    -1,    -1,  1120,  1121,  1122,  1123,   264,   868,
      -1,   870,    -1,    -1,   270,    -1,    -1,    -1,    -1,    -1,
    1120,  1121,  1122,  1123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1147,  1148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     420,    -1,    -1,    -1,    -1,    -1,    -1,  1147,  1148,   525,
      -1,    -1,    -1,   529,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   537,    -1,    -1,     3,    -1,    -1,    -1,   448,    -1,
      -1,    -1,    -1,   329,   454,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,   463,    -1,   465,    25,    -1,   344,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,   963,    44,    -1,    -1,    -1,    48,
      -1,   970,    51,    -1,    -1,     2,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,   983,    12,    -1,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    25,    -1,
      27,    -1,  1001,    -1,    83,   525,    -1,    -1,    -1,   529,
      -1,  1010,    -1,    92,    93,  1014,    95,   537,    97,    -1,
      99,    -1,    -1,   102,   420,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   657,    -1,    -1,   660,    -1,    -1,    -1,    75,    -1,
      -1,    -1,   448,    -1,    -1,    -1,    -1,    -1,   454,    -1,
      -1,    -1,    -1,    -1,  1063,    -1,    -1,   463,    -1,   465,
      -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   717,     2,    -1,    -1,    -1,    -1,   723,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1120,  1121,  1122,  1123,    25,    -1,    27,    -1,   525,
      -1,    -1,    -1,   529,    -1,    -1,    -1,   657,    -1,    -1,
     660,   537,    -1,    -1,    -1,    -1,    -1,    -1,  1147,  1148,
     177,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   805,
      81,   807,    -1,    -1,    -1,    -1,   812,   717,    -1,   815,
      -1,    -1,   102,   723,    -1,    -1,    -1,   107,   824,    -1,
     826,    -1,    -1,   829,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,   264,    -1,    -1,
      -1,   857,    35,   270,    37,    38,    -1,    -1,   139,    42,
     141,    -1,   868,    -1,   870,    -1,    49,    -1,    -1,    -1,
      -1,   657,    -1,    -1,   660,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      73,    -1,    75,    -1,    77,   805,    79,   807,    -1,    82,
      -1,    84,   812,    86,    -1,   815,   187,    -1,    -1,    -1,
      -1,    -1,   329,    -1,   824,    -1,   826,    -1,    -1,   829,
      -1,    -1,   105,   204,    -1,   108,    -1,   344,   111,    -1,
      -1,   717,    -1,    -1,    -1,    -1,    -1,   723,    -1,   220,
      -1,   222,    -1,    -1,    -1,    -1,    -1,   857,    -1,    -1,
     231,   232,    -1,    -1,    -1,    -1,    -1,   963,   868,    -1,
     870,    -1,    -1,    -1,   970,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   264,    -1,    -1,   983,   161,    -1,
     270,    -1,    -1,    -1,   167,   266,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1001,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   420,  1010,    -1,    -1,    -1,  1014,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   805,
      -1,   807,    -1,    -1,    -1,    -1,   812,    -1,    -1,   815,
      -1,   448,    -1,    -1,    -1,    -1,    -1,   454,   824,   329,
     826,    -1,    -1,   829,    -1,    -1,   463,    -1,   465,    -1,
      -1,    -1,    -1,   963,   344,    -1,    -1,  1063,    -1,    -1,
     970,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   349,    -1,
      -1,   857,    -1,   983,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   868,    -1,   870,    -1,    -1,    -1,    -1,    -1,
      -1,  1001,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1010,    -1,    -1,    -1,  1014,    -1,    22,    -1,   525,    -1,
      26,    27,   529,    -1,  1120,  1121,  1122,  1123,    -1,    35,
     537,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,
     420,    -1,    -1,    49,    -1,    -1,    -1,    -1,   419,    -1,
      -1,  1147,  1148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1063,    -1,    -1,   437,    73,   448,    75,
     441,    77,    -1,    79,   454,    -1,    82,    -1,    84,    -1,
      86,    -1,    -1,   463,    -1,    -1,    -1,   963,    -1,    -1,
      -1,    -1,    -1,    -1,   970,    -1,    -1,    -1,    -1,   105,
      -1,    -1,   108,   474,    -1,   111,    -1,   983,    -1,   480,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1120,  1121,  1122,  1123,    -1,  1001,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1010,    -1,    -1,    -1,  1014,    -1,
      -1,    -1,    -1,    -1,    -1,   525,    -1,  1147,  1148,   529,
     657,    -1,    -1,   660,    -1,   161,    -1,   537,    -1,    -1,
       3,   167,    -1,    -1,    -1,    -1,    -1,   538,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,   547,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,  1063,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
     717,    -1,    -1,    -1,    -1,    -1,   723,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    -1,  1120,  1121,  1122,  1123,    -1,    92,
      93,    -1,    95,    -1,    97,    -1,    99,   628,    -1,   102,
     631,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,  1147,  1148,    -1,    -1,    -1,    -1,   657,    -1,    -1,
     660,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   805,    -1,
     807,    -1,    -1,    -1,    -1,   812,    -1,   678,   815,    -1,
      -1,   682,    -1,    -1,    -1,    -1,    -1,   824,    -1,   826,
      -1,    -1,   829,    -1,    -1,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   705,    -1,    -1,   717,   709,    -1,
      22,    -1,    -1,   723,    26,    27,    -1,    -1,    -1,    -1,
     857,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      42,   868,    -1,   870,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   752,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      82,    12,    84,    -1,    86,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,   105,    -1,   805,   108,   807,    -1,   111,
      -1,    -1,   812,    -1,    -1,   815,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   824,    -1,   826,    -1,    -1,   829,
      -1,    -1,    -1,    -1,    -1,    -1,   963,    -1,    -1,    -1,
     831,    -1,   833,   970,    -1,    -1,   837,    -1,    -1,   840,
      -1,    -1,    -1,    -1,    85,    -1,   983,   857,    89,   161,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    98,   868,    -1,
     870,   862,    -1,    -1,  1001,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1010,    -1,    -1,    -1,  1014,    -1,   120,
     121,   122,   123,   124,   125,   126,   887,    -1,   129,   130,
     131,   132,    -1,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    -1,   166,  1063,    -1,    -1,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   983,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1120,  1121,  1122,  1123,    -1,    -1,    -1,
      -1,  1001,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1010,    -1,    -1,    -1,  1014,    -1,    -1,    -1,    -1,    -1,
    1147,  1148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1038,    -1,    -1,
       0,     1,    -1,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,  1063,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    63,    64,    65,    -1,    67,    68,    69,
      -1,    -1,    -1,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    -1,    86,    87,    88,    -1,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    99,
     100,    -1,   102,   103,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     1,   173,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      99,   100,    -1,   102,   103,   104,   105,   106,    -1,   108,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   166,   167,   168,
      -1,   170,    -1,     1,   173,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    -1,    86,    87,
      88,    -1,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    99,   100,    -1,   102,   103,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,    -1,    -1,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,   161,    -1,    -1,   164,    -1,   166,   167,
     168,    -1,   170,    -1,     1,   173,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    -1,    86,
      87,    88,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    99,   100,    -1,   102,   103,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,    -1,    -1,   115,    -1,
     117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,   166,
     167,   168,    -1,   170,    -1,     1,   173,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    -1,
      86,    87,    88,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,
     166,   167,   168,    -1,   170,    -1,     1,   173,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      -1,    86,    87,    88,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    99,   100,    -1,   102,   103,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
      -1,   166,   167,   168,    -1,   170,    -1,     1,   173,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    -1,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    -1,    86,    87,    88,    -1,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,   108,   109,   110,   111,   112,    -1,
      -1,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,   161,    -1,    -1,
     164,    -1,   166,   167,   168,    -1,   170,    -1,     1,   173,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    -1,    86,    87,    88,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,    99,   100,    -1,   102,
     103,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
      -1,    -1,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     1,
     173,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,    -1,    67,    68,    69,    -1,    -1,
      -1,    73,    74,    75,    76,    -1,    78,    79,    80,    81,
      82,    83,    84,    -1,    86,    87,    88,    -1,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    99,   100,    -1,
     102,   103,   104,   105,   106,    -1,   108,   109,   110,   111,
     112,    -1,    -1,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       1,   173,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    69,    -1,
      -1,    -1,    73,    74,    75,    76,    -1,    78,    79,    80,
      81,    82,    83,    84,    -1,    86,    87,    88,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    99,   100,
      -1,   102,   103,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,    -1,    -1,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,     1,   173,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    63,    64,    65,    -1,    67,    68,    69,
      -1,    -1,    -1,    73,    74,    75,    76,    -1,    78,    79,
      80,    81,    82,    83,    84,    -1,    86,    87,    88,    -1,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    99,
     100,    -1,   102,   103,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     1,   173,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,    78,
      79,    80,    81,    82,    83,    84,    -1,    86,    87,    88,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      99,   100,    -1,   102,   103,   104,   105,   106,    -1,   108,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,    -1,   166,    -1,   168,
      -1,   170,    -1,     1,   173,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,
      78,    79,    80,    81,    82,    83,    84,    -1,    86,    87,
      88,    -1,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    99,   100,    -1,   102,   103,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,    -1,    -1,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,   161,    -1,    -1,   164,    -1,   166,    -1,
     168,    -1,   170,    -1,     1,   173,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    -1,    -1,    73,    74,    75,    76,
      -1,    78,    79,    80,    81,    82,    83,    84,    -1,    86,
      87,    88,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    99,   100,    -1,   102,   103,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,    -1,    -1,   115,    -1,
     117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,   166,
      -1,   168,    -1,   170,    -1,     1,   173,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    -1,    78,    79,    80,    81,    82,    83,    84,    -1,
      86,    87,    88,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,    -1,
     166,    -1,   168,    -1,   170,    -1,     1,   173,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    -1,    -1,    73,    74,
      75,    76,    -1,    78,    79,    80,    81,    82,    83,    84,
      -1,    86,    87,    88,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    99,   100,    -1,   102,   103,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,     1,   173,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    -1,    -1,    73,
      74,    75,    76,    -1,    78,    79,    80,    81,    82,    83,
      84,    -1,    86,    87,    88,    -1,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,   108,   109,   110,   111,   112,    -1,
      -1,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,   161,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     1,   173,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    -1,    78,    79,    80,    81,    82,
      83,    84,    -1,    86,    87,    88,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,    99,   100,    -1,   102,
     103,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
      -1,    -1,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     1,
     173,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,   171,
       1,   173,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,     3,   173,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    75,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
      -1,   173,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,   103,   104,    -1,   106,   107,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,    -1,   173,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,   103,   104,    -1,   106,   107,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,   169,
     170,    -1,    -1,   173,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,    -1,   102,   103,   104,    -1,   106,   107,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,
     169,   170,    -1,     3,   173,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    46,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     3,   173,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,   103,   104,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,    -1,   173,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,   103,   104,    -1,   106,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     3,   173,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    66,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,   103,   104,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,     3,   173,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,   173,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,   103,
     104,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,   173,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,   167,   168,    -1,   170,    -1,    -1,
     173,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,   171,
      -1,   173,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
     171,     3,   173,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,
     173,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,   105,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,   171,     3,   173,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,   107,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,    -1,   173,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,   107,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,    -1,   173,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,   173,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,
     166,    -1,   168,    -1,   170,    -1,     3,   173,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,
      -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,   166,
      -1,   168,    -1,   170,    -1,     3,   173,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    97,
      -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,
     168,    -1,   170,    -1,     3,   173,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,
      -1,   170,    -1,     3,   173,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,
     170,    -1,     3,   173,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,
      -1,     3,   173,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,
      -1,   166,    -1,   168,    -1,   170,    -1,     3,   173,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,
     166,    -1,   168,    -1,   170,    -1,     3,   173,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,
      -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,   158,   159,   160,    -1,    -1,    -1,   164,    -1,   166,
      -1,   168,    -1,   170,    -1,     3,   173,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    97,
      -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
     158,   159,   160,    -1,    -1,    -1,   164,    -1,   166,    -1,
     168,    -1,   170,    -1,     3,   173,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,   136,   137,   138,
     139,   140,    -1,    -1,    -1,    12,   145,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,   155,    -1,    25,   158,
     159,   160,    -1,    -1,    -1,   164,    33,    -1,    35,   168,
      -1,   170,    39,    -1,   173,    -1,    -1,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    95,    -1,
      97,    -1,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,   122,   123,   124,   125,   126,
      -1,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,   139,   140,    -1,    -1,   143,    -1,    -1,    -1,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
       3,   168,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,    -1,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,    -1,   168,    -1,   170,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,    -1,
      -1,   166,    -1,   168,   169,   170,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    59,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    74,    -1,    76,
      -1,    78,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    -1,    99,    -1,   101,   102,    -1,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,   138,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,   160,   161,    -1,    -1,    -1,    -1,   166,
      33,   168,    35,   170,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,     3,   138,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,   160,    -1,    -1,
      -1,    -1,    -1,   166,    33,   168,    35,   170,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,   138,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,   160,    -1,    -1,    -1,    -1,    -1,   166,    33,   168,
      35,   170,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,     3,   102,    -1,    -1,
      -1,   106,    -1,    -1,   109,    12,    -1,   112,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,   138,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    -1,   168,    -1,   170,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    12,    95,    -1,
      97,    -1,    99,    -1,    19,   102,    -1,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,   122,   123,   124,   125,   126,
      -1,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,   139,   140,    -1,    -1,   143,    -1,    -1,    -1,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      85,    -1,    -1,    12,    89,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,   124,
     125,   126,    -1,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    85,   162,   163,   164,
      89,   166,    -1,    12,    -1,    -1,    -1,    -1,   173,    98,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,   121,   122,   123,   124,   125,   126,    -1,    -1,
     129,   130,   131,   132,    -1,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    85,   166,    -1,    12,
      89,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,   121,   122,   123,   124,   125,   126,    -1,    -1,
     129,   130,   131,   132,    -1,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,   166,    -1,    12,
      -1,    -1,    -1,    -1,   173,    98,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,   124,   125,   126,    -1,    -1,   129,   130,   131,   132,
      -1,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    85,   166,    -1,    12,    89,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,   124,   125,   126,    -1,    -1,   129,   130,   131,   132,
      -1,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,   166,    -1,    12,    -1,    -1,    -1,    -1,
     173,    98,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,   126,
      -1,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,   166,
      -1,    12,    89,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,   126,
      -1,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,   166,
      -1,    -1,    -1,    12,    -1,    -1,   173,    98,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,   124,   125,   126,    -1,    46,   129,   130,
     131,   132,    -1,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    -1,   166,    85,    -1,    -1,    12,
      89,    -1,   173,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,   120,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,   166,    -1,    -1,
      -1,    12,    -1,    -1,   173,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    70,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    -1,   166,    85,    -1,    -1,    -1,    89,    -1,
     173,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    70,   153,   154,   155,   156,   157,   158,   159,    -1,
     161,   162,   163,   164,    -1,    -1,    85,    -1,    -1,    12,
      89,    -1,   173,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,    -1,    -1,    12,
      -1,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    85,   166,    -1,    12,    89,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,    -1,    -1,    12,    -1,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,   166,
      -1,    12,    89,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    85,    -1,    -1,    12,    89,    -1,
      -1,    -1,   173,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      85,   162,   163,   164,    89,   166,    -1,    12,    -1,    -1,
      -1,    -1,   173,    98,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    -1,   162,   163,   164,
      85,   166,    -1,    12,    89,    -1,    -1,    -1,   173,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,   116,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    70,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    85,   162,   163,   164,
      89,    -1,    -1,    12,    -1,    -1,   171,    -1,   173,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    85,    -1,    -1,    12,
      89,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,   166,    -1,    12,
      -1,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    31,   161,   162,
     163,   164,    85,    -1,    -1,    -1,    89,    -1,    -1,    12,
     173,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,   136,   137,    89,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    31,   161,   162,
     163,   164,    85,    -1,    -1,    -1,    89,    -1,    -1,    12,
     173,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   136,   137,    -1,   139,   140,   141,    31,   143,
     144,   145,    -1,   147,   148,   149,   150,   120,    -1,   153,
     154,   155,   156,   157,    -1,   159,    -1,    -1,   162,   163,
     164,    85,    -1,   136,   137,    89,   139,   140,   141,   173,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,   161,   162,
     163,   164,    85,    -1,    -1,    -1,    89,    -1,    -1,    12,
     173,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   136,   137,    -1,   139,   140,   141,    31,   143,
     144,   145,    -1,    -1,   148,   149,   150,   120,    -1,   153,
     154,   155,   156,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,   136,   137,    -1,   139,   140,   141,   173,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,   161,   162,
     163,   164,    85,    -1,    -1,    12,    89,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,    -1,    -1,    12,    -1,    -1,    -1,    -1,
     173,    98,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,    -1,
      -1,    -1,    89,    -1,    -1,    -1,   173,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      85,    -1,   169,    12,    89,    -1,   173,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    85,   162,   163,   164,
      89,    -1,    -1,    12,    -1,    -1,    -1,    -1,   173,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    85,    -1,    -1,    12,
      89,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,    -1,    -1,    12,
      13,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,   161,   162,
     163,   164,    85,    -1,    -1,    12,    89,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,    -1,    -1,    12,    -1,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,    -1,
      -1,    12,    89,    -1,    -1,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,    -1,
      -1,    -1,   169,    12,    -1,    -1,   173,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    85,    -1,   169,    12,
      89,    -1,   173,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,    -1,    -1,    12,
      -1,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    85,    -1,    -1,    12,    89,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    85,   162,
     163,   164,    89,    -1,    -1,    12,    13,    -1,    -1,    -1,
     173,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,   162,   163,   164,    85,    -1,
      -1,    12,    89,    -1,   171,    -1,   173,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,    -1,    85,   162,   163,   164,    89,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   173,    98,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    85,    -1,    -1,    -1,    89,    -1,
      -1,    -1,   173,    12,    13,    -1,    -1,    98,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    85,    -1,    -1,    12,
      89,    -1,   173,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,    -1,    -1,    -1,
      12,    -1,    -1,    -1,   173,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    -1,    85,    -1,    -1,   169,    89,    -1,    -1,
     173,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,    -1,    -1,
     162,   163,   164,    -1,    85,    -1,    -1,    12,    89,   171,
      -1,   173,    -1,    -1,    19,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,    -1,
      85,   162,   163,   164,    89,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   173,    98,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    -1,   162,   163,   164,
      85,    -1,    -1,    12,    89,    -1,    -1,    -1,   173,    -1,
      19,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,    -1,    85,   162,   163,   164,
      89,    -1,    -1,    12,    -1,    -1,    -1,    -1,   173,    98,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    -1,   162,   163,   164,    85,    -1,    -1,    12,
      89,    -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,    -1,    85,   162,   163,   164,    89,    -1,    -1,    -1,
      -1,    -1,    12,    -1,   173,    98,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,    59,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,    85,    -1,    -1,    12,    89,
     173,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
      -1,    85,   162,   163,   164,    89,    -1,    -1,    12,    -1,
      -1,    -1,    -1,   173,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,    -1,    -1,   162,   163,
     164,    85,    -1,    -1,    12,    89,    -1,   171,    -1,   173,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,    -1,    85,   162,   163,
     164,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,    -1,    22,   162,   163,   164,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    35,    -1,    37,    38,
      22,    -1,    -1,    42,    26,    27,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    31,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    82,    -1,    84,    -1,    86,    -1,    -1,
      -1,    73,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      82,    -1,    84,    -1,    86,    31,   105,    -1,    -1,   108,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    89,   105,    -1,    -1,   108,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,   161,    89,    -1,    -1,    -1,    -1,   167,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   161,
     147,   148,   149,   150,    -1,   167,   153,   154,   155,   156,
     157,   158,   159,    -1,   120,   162,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,   150,    -1,    -1,   153,   154,   155,
     156,   157,     1,   159,     3,    -1,   162,   163,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,     3,    -1,    -1,    -1,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,    61,    25,    -1,    -1,    -1,    -1,    67,    68,
      -1,    33,    -1,    -1,    -1,    74,    -1,    39,    -1,    -1,
      -1,    -1,    44,    -1,    83,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    92,    93,    -1,    95,    -1,    97,    61,
      99,    -1,    -1,   102,    -1,    67,    68,   106,    -1,    -1,
     109,    -1,    74,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      92,    93,    -1,    95,    -1,    97,    -1,    99,    17,    18,
     102,    20,    -1,    -1,   106,    -1,    25,   109,    -1,    -1,
     112,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,     3,    -1,    -1,    -1,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,    61,    25,    -1,    -1,    -1,    -1,    67,    68,
      -1,    33,    -1,    -1,    -1,    74,    -1,    39,    -1,    -1,
      -1,    -1,    44,    -1,    83,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    92,    93,    -1,    95,    -1,    97,    61,
      99,    -1,    -1,   102,    -1,    67,    68,   106,    -1,    -1,
     109,    -1,    74,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      92,    93,    -1,    95,    -1,    97,    -1,    99,    17,    18,
     102,    20,    -1,    -1,   106,    -1,    25,   109,    -1,    -1,
     112,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,     3,    -1,    -1,    -1,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,    61,    25,    -1,    -1,    -1,    -1,    67,    68,
      -1,    33,    -1,    -1,    -1,    74,    -1,    39,    -1,    -1,
      -1,    -1,    44,    -1,    83,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    92,    93,    -1,    95,    -1,    97,    61,
      99,    -1,    -1,   102,    -1,    67,    68,   106,    -1,    -1,
     109,    -1,    74,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      92,    93,    -1,    95,    -1,    97,    -1,    99,    17,    18,
     102,    20,    -1,    -1,   106,    -1,    25,   109,    -1,    -1,
     112,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    -1,    95,    -1,    97,    -1,
      99,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,   112
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   178,   179,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    47,    48,
      49,    51,    54,    55,    56,    57,    58,    60,    61,    62,
      64,    65,    67,    68,    69,    73,    74,    75,    76,    77,
      78,    79,    82,    83,    84,    86,    87,    88,    90,    91,
      92,    93,    94,    95,    96,    97,    99,   100,   102,   103,
     104,   105,   106,   108,   109,   111,   112,   115,   117,   120,
     136,   137,   138,   139,   140,   145,   155,   158,   159,   160,
     161,   164,   166,   168,   170,   173,   180,   181,   182,   183,
     184,   185,   188,   190,   196,   197,   198,   201,   202,   206,
     208,   211,   212,   214,   215,   216,   221,   222,   224,   226,
     229,   230,   231,   232,   233,   234,   238,   239,   242,   243,
     246,   247,   250,   253,   254,   279,   282,   283,   303,   304,
     305,   306,   307,   308,   309,   317,   318,   319,   320,   321,
     324,   325,   326,   327,   328,   329,   330,   331,   333,   334,
     335,   336,   337,   161,   182,   321,   116,   310,   311,     3,
     203,    14,    22,    35,    40,    41,    43,    54,    84,    97,
     166,   170,   229,   250,   303,   308,   319,   320,   321,   324,
     326,   327,   310,   321,   105,   282,    86,   203,   182,   297,
     321,   182,   168,     8,    84,   321,   322,     8,    11,    84,
     105,   322,    75,   118,   223,   321,   223,   321,    26,   283,
     321,    27,   111,   225,   321,   189,   168,     3,    17,    18,
      20,    25,    33,    39,    44,    48,    51,    61,    67,    68,
      74,    83,    92,    93,    95,    97,    99,   102,   106,   109,
     112,   205,   207,   205,   205,   251,   168,   205,   284,   285,
      32,   190,   210,   321,   321,    18,    74,    92,   109,   321,
     321,   321,     8,   168,   213,    22,    35,    38,    84,   214,
       4,   273,   296,   321,   103,   104,   161,   321,   323,   321,
     210,   321,   321,   321,    96,   168,   182,   321,   321,   190,
     202,   321,   324,   190,   202,   321,   205,   280,   321,   321,
     321,   321,   321,   321,   321,   321,     1,   167,   180,   191,
     296,   107,   146,   273,   298,   299,   323,   223,   296,   321,
     332,   321,    77,   182,   166,    81,   186,    45,   110,    54,
     205,   205,    52,    80,   268,   284,   160,   161,   152,   321,
      12,    19,    31,    85,    89,   120,   136,   137,   139,   140,
     141,   143,   144,   145,   147,   148,   149,   150,   151,   153,
     154,   155,   156,   157,   158,   159,   162,   163,   164,   173,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   165,   260,   168,   170,    85,
      89,   168,   182,   161,   321,   321,   321,   205,   296,    54,
     166,   190,    46,   310,   280,   284,   161,   142,   161,   115,
     206,   273,   300,   301,   302,   323,    84,   219,   254,   282,
      84,   217,   280,   254,   282,   205,   168,   210,    32,    46,
     210,   116,   210,   313,    32,    46,   210,   313,    36,    70,
     161,   205,   205,    98,   190,    98,   121,   190,   260,    79,
      82,   187,   300,   168,   168,   190,   182,   168,   262,   107,
     168,   205,   286,   287,     1,   141,   291,    46,   142,   182,
     210,   168,   168,   210,   300,   214,   142,   161,   321,   321,
     161,   166,   210,   168,   300,   161,   235,   161,   235,   161,
     121,   281,   161,   210,   161,   167,   167,   180,   142,   167,
     321,   142,   169,   142,   169,   171,   313,    46,   142,   171,
     313,   119,   142,   171,     8,     1,   167,   191,    63,   199,
     200,   321,   193,   321,   205,   141,   244,   166,   255,   161,
     321,   321,   321,   321,   223,   321,   223,   321,   321,   321,
     321,   321,   321,   321,     3,    20,    33,    61,    99,   105,
     206,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,    66,   323,
     323,   323,   323,   323,   300,   300,   223,   321,   223,   321,
      27,    46,    86,   111,   312,   315,   316,   321,   337,    32,
      46,    32,    46,    98,   168,    46,   171,   205,   223,   321,
     210,   161,   161,   321,   321,   121,   169,   142,   220,   205,
     284,   218,   205,   161,   284,    46,   296,    43,   321,   223,
     321,   168,   210,    43,   321,   223,   321,   210,   164,   192,
     195,   321,   192,   194,   121,   121,   182,    34,   182,   321,
      34,   321,   186,   169,   301,   205,   227,   228,    27,    46,
      50,    72,    75,    86,   105,   181,   263,   264,   265,   266,
     252,   287,   142,   169,    33,    48,    93,    97,   170,   209,
     292,   304,   121,   288,   321,   285,   321,   321,   169,   273,
     321,     1,   240,   300,   169,    21,   236,   292,   304,   142,
     167,   169,   169,   298,   169,   298,   182,   171,   223,   321,
     171,   182,   321,   171,   321,   171,   321,   167,   167,   205,
     142,   161,    13,   144,   142,   161,    13,    36,    70,   161,
     168,   296,   166,     1,   205,   248,   249,    27,    75,    86,
     105,   257,   267,   168,   161,   161,   161,   161,   161,   161,
     169,   171,    46,    86,   142,   169,    17,    20,    25,    44,
      51,    61,    68,    83,    95,   106,   112,   303,    85,    85,
      43,   223,   321,    43,   223,   321,   301,   223,   321,   168,
     310,   310,   161,   273,   323,   302,   205,   244,   161,   205,
     244,   161,   321,   169,   321,    32,   210,    32,   210,   314,
     315,   321,    32,   210,   313,    32,   210,   313,   142,   161,
      13,   161,   321,   321,    34,   182,    34,    34,   182,    98,
     190,    63,   169,   142,   169,    46,    86,   265,   142,   169,
     168,   205,    27,    75,    86,   105,   269,   169,   287,   291,
       1,   296,    66,   323,   169,   169,   167,    71,   113,   167,
     241,   169,   168,   190,   205,   237,   280,   182,   171,   313,
     171,   313,   182,   119,   199,   206,   166,   321,   107,   321,
     195,   194,   301,     1,   245,   167,   121,   142,   167,    86,
     256,     1,     3,    12,    17,    19,    20,    25,    39,    44,
      51,    53,    61,    67,    68,    83,    95,    99,   102,   106,
     112,   136,   137,   138,   139,   140,   143,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   159,   162,
     163,   164,   165,   168,   204,   205,   207,   258,   259,   260,
     303,   169,   315,   291,   303,   303,   321,    32,    32,   321,
      32,    32,   169,   171,   171,   301,   210,   210,   244,   166,
     244,   166,   210,    98,    43,   321,    43,   321,   142,   169,
      98,    43,   321,   210,    43,   321,   210,   321,   321,   182,
     182,   321,   182,    34,   205,   161,   228,   190,   205,   264,
     287,   141,   295,    86,   291,   288,   171,    46,   171,   168,
     168,    32,   182,   296,   237,   141,   190,    43,   182,   321,
     171,    43,   182,   321,   171,   321,   192,    13,    36,    70,
      36,    70,   161,   161,   169,   167,    79,    82,   167,   181,
     212,   321,   249,   269,   168,   261,   321,   136,   144,   261,
     261,   288,    98,    43,    43,    98,    43,    43,    43,    43,
     169,   166,   245,   166,   245,   321,   321,   321,   315,   321,
     321,   321,    13,    34,   182,   161,   295,   169,   170,   209,
     273,   294,   304,   146,   274,   288,    59,   114,   275,   321,
     292,   304,   300,   300,   182,   210,   169,   321,   321,   182,
     321,   182,   167,   107,   321,   195,   194,   195,   194,   161,
     212,     1,   141,   290,   263,   169,     3,    99,   259,   260,
     321,   321,   321,   321,   321,   321,   245,   167,   245,   167,
      98,    98,    98,    98,   321,   182,   274,   288,   295,   171,
     296,   273,   321,     3,    88,    99,   276,   277,   278,   321,
     190,   211,   272,   171,   169,   169,   190,    98,    98,   161,
     161,   161,   161,   170,   209,   289,   304,   101,   270,   169,
     261,   261,    98,    98,    98,    98,    98,    98,   167,   167,
     321,   321,   321,   321,   274,   288,   273,   293,   294,   304,
      46,   171,   278,   114,   142,   121,   147,   149,   150,   153,
     154,    59,   304,   160,   160,   321,   321,     1,   171,   296,
     275,   321,   293,   294,   321,   277,   278,   278,   278,   278,
     278,   278,   276,   171,   289,   304,   171,   161,   271,   272,
     171,   289,   304,   293
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
#line 510 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 516 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 523 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 528 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 529 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 552 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 553 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 554 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 555 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 556 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 557 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 558 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 559 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 560 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 562 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 563 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 564 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 565 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 566 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 567 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[(4) - (4)].pch), currentModuleType, NULL, yyfilename, (yyvsp[(1) - (4)].b), (yyvsp[(2) - (4)].b), (yylsp[(1) - (4)]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[(4) - (4)].pch);
    }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 590 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 595 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 603 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 609 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 613 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 615 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 620 "chapel.ypp"
    {
    (yylsp[(1) - (1)]).comment = context->latestComment;
    context->latestComment = NULL;
  }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 625 "chapel.ypp"
    {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[(6) - (7)].pch), (yyvsp[(3) - (7)].b), (yyvsp[(4) - (7)].b), (yylsp[(1) - (7)]).comment);
 }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 653 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 658 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 663 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 667 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 676 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 681 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 686 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 691 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 695 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 699 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 707 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 716 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 725 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 726 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 731 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].ponlylist), (yyvsp[(1) - (4)].b)); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), true, (yyvsp[(1) - (8)].b)); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), new UnresolvedSymExpr("_"), (yyvsp[(7) - (8)].ponlylist), true, (yyvsp[(1) - (8)].b)); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), false, (yyvsp[(1) - (8)].b)); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), new UnresolvedSymExpr("_"), (yyvsp[(7) - (8)].ponlylist), false, (yyvsp[(1) - (8)].b)); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(3) - (4)].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[(1) - (4)].b));}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist)); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pimportstmt)); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (3)].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[(3) - (3)].pimportstmt)); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 762 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), new SymExpr(gNoInit), "="); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 779 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 784 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 785 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 786 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 791 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 792 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 793 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 795 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 796 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 808 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 809 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 820 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 821 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 822 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 823 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 824 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 825 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 826 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 827 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 828 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 829 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 830 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 831 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 832 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 833 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 834 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 835 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 836 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 837 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 838 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 842 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 843 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 844 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 845 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 846 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 847 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 848 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 849 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 850 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 851 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 852 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 853 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 860 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 861 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 862 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 863 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 872 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 873 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 877 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 890 "chapel.ypp"
    { parsingPrivate=(yyvsp[(2) - (3)].b); applyPrivateToBlock((yyvsp[(3) - (3)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(3) - (3)].pblockstmt); }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 904 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 905 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 907 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 912 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 917 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[(5) - (8)].pcallexpr),
                                             (yyvsp[(7) - (8)].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[(2) - (8)]).comment));
    }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 927 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 932 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(5) - (9)].pch),
                                             (yyvsp[(2) - (9)].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[(6) - (9)].pcallexpr),
                                             (yyvsp[(8) - (9)].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[(3) - (9)]).comment));
    }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 943 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 948 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[(5) - (8)].pcallexpr),
                                             (yyvsp[(7) - (8)].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[(2) - (8)]).comment));
    }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 958 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 963 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(5) - (9)].pch),
                                             (yyvsp[(2) - (9)].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[(6) - (9)].pcallexpr),
                                             (yyvsp[(8) - (9)].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[(3) - (9)]).comment));
    }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 974 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 978 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 983 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 991 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 1002 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 1008 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 1010 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 1011 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 1012 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), true, false); }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), true, false); }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 1027 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 1033 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 1039 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 1045 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 1051 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1058 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1065 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1069 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1075 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1080 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1082 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1085 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1089 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1091 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1093 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1095 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pexpr) = buildIfVar((yyvsp[(2) - (4)].pch), (yyvsp[(4) - (4)].pexpr), false); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1101 "chapel.ypp"
    { (yyval.pexpr) = buildIfVar((yyvsp[(2) - (4)].pch), (yyvsp[(4) - (4)].pexpr), true);  }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt))); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1108 "chapel.ypp"
    { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (3)].pch), ls, (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[(1) - (1)].pch), INTENT_TYPE); }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1126 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1129 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1134 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1135 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1136 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[(1) - (1)].pch)); }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pch) = "none"; }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1147 "chapel.ypp"
    { (yyval.pch) = "false"; }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1148 "chapel.ypp"
    { (yyval.pch) = "true"; }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1157 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1163 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), NULL)); }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1165 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (4)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (4)].pch), act, NULL)); }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyvsp[(5) - (7)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (7)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (7)].pch), (yyvsp[(5) - (7)].pcallexpr), NULL)); }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1174 "chapel.ypp"
    { (yyval.pexpr) = IfcConstraint::build((yyvsp[(2) - (5)].pch), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1176 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (3)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (3)].pch), act); }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1179 "chapel.ypp"
    { (yyvsp[(5) - (6)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (6)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pcallexpr)); }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1187 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1188 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1190 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1191 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1192 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1196 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1201 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1207 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1208 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1228 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1232 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1239 "chapel.ypp"
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

  case 277:

/* Line 1806 of yacc.c  */
#line 1249 "chapel.ypp"
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

  case 278:

/* Line 1806 of yacc.c  */
#line 1262 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1267 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1272 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1295 "chapel.ypp"
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

  case 287:

/* Line 1806 of yacc.c  */
#line 1311 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1318 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1327 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1335 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1339 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1351 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1356 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1361 "chapel.ypp"
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
      if ((yyvsp[(7) - (8)].lifetimeAndWhere).where)
        (yyvsp[(3) - (8)].pfnsymbol)->where = new BlockStmt((yyvsp[(7) - (8)].lifetimeAndWhere).where);
      if ((yyvsp[(7) - (8)].lifetimeAndWhere).lifetime)
        (yyvsp[(3) - (8)].pfnsymbol)->lifetimeConstraints = new BlockStmt((yyvsp[(7) - (8)].lifetimeAndWhere).lifetime);
      (yyvsp[(3) - (8)].pfnsymbol)->insertAtTail((yyvsp[(8) - (8)].pblockstmt));
      (yyval.pexpr) = new DefExpr(buildLambda((yyvsp[(3) - (8)].pfnsymbol)));
    }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1383 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1386 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1390 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1398 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1408 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1415 "chapel.ypp"
    {
      FnSymbol* fn = (yyvsp[(4) - (10)].pfnsymbol);
      FnSymbol* linkageFn = (yyvsp[(1) - (10)].pfnsymbol);

      fn->copyFlags((yyvsp[(1) - (10)].pfnsymbol));
      if (*linkageFn->name)
        // The user explicitly named this function (controls mangling).
        fn->cname = linkageFn->name;
      else if (linkageFn->numFormals() == 1)
        // cname should be set based upon param
        fn->insertFormalAtTail(linkageFn->getFormal(1));

      if ((yyvsp[(2) - (10)].procIter) == ProcIter_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[(4) - (10)].pfnsymbol), (yyvsp[(6) - (10)].retTag), (yyvsp[(7) - (10)].pexpr), (yyvsp[(8) - (10)].b), (yyvsp[(9) - (10)].lifetimeAndWhere).where, (yyvsp[(9) - (10)].lifetimeAndWhere).lifetime, (yyvsp[(10) - (10)].pblockstmt), (yylsp[(2) - (10)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1441 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1445 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1450 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1454 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1459 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1471 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1472 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1480 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1492 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 342:

/* Line 1806 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 343:

/* Line 1806 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 344:

/* Line 1806 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 354:

/* Line 1806 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pexpr))); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1541 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1550 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1564 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1565 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1566 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1572 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1576 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1577 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1578 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1586 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1605 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1613 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1615 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1617 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1619 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1621 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1626 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1632 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1633 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1634 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1635 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1636 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1637 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1643 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1652 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1654 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1659 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1672 "chapel.ypp"
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

  case 417:

/* Line 1806 of yacc.c  */
#line 1688 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1692 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1696 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1698 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1705 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1711 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1720 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1728 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1735 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1737 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1744 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1748 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1754 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1755 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 1770 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 1780 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 1787 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 1789 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 1791 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 452:

/* Line 1806 of yacc.c  */
#line 1796 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 453:

/* Line 1806 of yacc.c  */
#line 1797 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 454:

/* Line 1806 of yacc.c  */
#line 1798 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 455:

/* Line 1806 of yacc.c  */
#line 1799 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 456:

/* Line 1806 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 457:

/* Line 1806 of yacc.c  */
#line 1821 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 458:

/* Line 1806 of yacc.c  */
#line 1825 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 459:

/* Line 1806 of yacc.c  */
#line 1829 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 460:

/* Line 1806 of yacc.c  */
#line 1837 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 461:

/* Line 1806 of yacc.c  */
#line 1843 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 462:

/* Line 1806 of yacc.c  */
#line 1844 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 463:

/* Line 1806 of yacc.c  */
#line 1845 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 464:

/* Line 1806 of yacc.c  */
#line 1850 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 465:

/* Line 1806 of yacc.c  */
#line 1852 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 466:

/* Line 1806 of yacc.c  */
#line 1858 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 467:

/* Line 1806 of yacc.c  */
#line 1860 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 468:

/* Line 1806 of yacc.c  */
#line 1862 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 469:

/* Line 1806 of yacc.c  */
#line 1866 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 470:

/* Line 1806 of yacc.c  */
#line 1867 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 471:

/* Line 1806 of yacc.c  */
#line 1868 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 472:

/* Line 1806 of yacc.c  */
#line 1869 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 473:

/* Line 1806 of yacc.c  */
#line 1870 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 474:

/* Line 1806 of yacc.c  */
#line 1876 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 475:

/* Line 1806 of yacc.c  */
#line 1877 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 476:

/* Line 1806 of yacc.c  */
#line 1878 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 477:

/* Line 1806 of yacc.c  */
#line 1879 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 478:

/* Line 1806 of yacc.c  */
#line 1883 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 479:

/* Line 1806 of yacc.c  */
#line 1884 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 480:

/* Line 1806 of yacc.c  */
#line 1888 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 481:

/* Line 1806 of yacc.c  */
#line 1889 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 482:

/* Line 1806 of yacc.c  */
#line 1890 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 483:

/* Line 1806 of yacc.c  */
#line 1894 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 484:

/* Line 1806 of yacc.c  */
#line 1895 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 485:

/* Line 1806 of yacc.c  */
#line 1899 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 487:

/* Line 1806 of yacc.c  */
#line 1904 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 488:

/* Line 1806 of yacc.c  */
#line 1905 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 489:

/* Line 1806 of yacc.c  */
#line 1909 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 490:

/* Line 1806 of yacc.c  */
#line 1910 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 491:

/* Line 1806 of yacc.c  */
#line 1911 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 492:

/* Line 1806 of yacc.c  */
#line 1912 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 493:

/* Line 1806 of yacc.c  */
#line 1916 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 494:

/* Line 1806 of yacc.c  */
#line 1917 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 495:

/* Line 1806 of yacc.c  */
#line 1929 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 496:

/* Line 1806 of yacc.c  */
#line 1931 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 497:

/* Line 1806 of yacc.c  */
#line 1933 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 503:

/* Line 1806 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 504:

/* Line 1806 of yacc.c  */
#line 1946 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 505:

/* Line 1806 of yacc.c  */
#line 1948 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 506:

/* Line 1806 of yacc.c  */
#line 1950 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 507:

/* Line 1806 of yacc.c  */
#line 1952 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 508:

/* Line 1806 of yacc.c  */
#line 1954 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 509:

/* Line 1806 of yacc.c  */
#line 1956 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 510:

/* Line 1806 of yacc.c  */
#line 1959 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 511:

/* Line 1806 of yacc.c  */
#line 1961 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 512:

/* Line 1806 of yacc.c  */
#line 1963 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 513:

/* Line 1806 of yacc.c  */
#line 1965 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 514:

/* Line 1806 of yacc.c  */
#line 1967 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 515:

/* Line 1806 of yacc.c  */
#line 1969 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 516:

/* Line 1806 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 517:

/* Line 1806 of yacc.c  */
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 518:

/* Line 1806 of yacc.c  */
#line 1976 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 519:

/* Line 1806 of yacc.c  */
#line 1978 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 520:

/* Line 1806 of yacc.c  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 521:

/* Line 1806 of yacc.c  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 522:

/* Line 1806 of yacc.c  */
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 523:

/* Line 1806 of yacc.c  */
#line 1989 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 524:

/* Line 1806 of yacc.c  */
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 525:

/* Line 1806 of yacc.c  */
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 526:

/* Line 1806 of yacc.c  */
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 527:

/* Line 1806 of yacc.c  */
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 528:

/* Line 1806 of yacc.c  */
#line 1999 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 529:

/* Line 1806 of yacc.c  */
#line 2001 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 530:

/* Line 1806 of yacc.c  */
#line 2003 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 531:

/* Line 1806 of yacc.c  */
#line 2005 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 532:

/* Line 1806 of yacc.c  */
#line 2007 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 533:

/* Line 1806 of yacc.c  */
#line 2014 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 534:

/* Line 1806 of yacc.c  */
#line 2020 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 535:

/* Line 1806 of yacc.c  */
#line 2026 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 536:

/* Line 1806 of yacc.c  */
#line 2032 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 537:

/* Line 1806 of yacc.c  */
#line 2041 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 538:

/* Line 1806 of yacc.c  */
#line 2050 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 546:

/* Line 1806 of yacc.c  */
#line 2066 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 547:

/* Line 1806 of yacc.c  */
#line 2070 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 549:

/* Line 1806 of yacc.c  */
#line 2075 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 550:

/* Line 1806 of yacc.c  */
#line 2079 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 551:

/* Line 1806 of yacc.c  */
#line 2080 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 552:

/* Line 1806 of yacc.c  */
#line 2084 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 553:

/* Line 1806 of yacc.c  */
#line 2088 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 554:

/* Line 1806 of yacc.c  */
#line 2089 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 555:

/* Line 1806 of yacc.c  */
#line 2094 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 556:

/* Line 1806 of yacc.c  */
#line 2098 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 557:

/* Line 1806 of yacc.c  */
#line 2102 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 558:

/* Line 1806 of yacc.c  */
#line 2108 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 559:

/* Line 1806 of yacc.c  */
#line 2109 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 560:

/* Line 1806 of yacc.c  */
#line 2110 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 561:

/* Line 1806 of yacc.c  */
#line 2111 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 562:

/* Line 1806 of yacc.c  */
#line 2112 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 563:

/* Line 1806 of yacc.c  */
#line 2113 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 565:

/* Line 1806 of yacc.c  */
#line 2119 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 566:

/* Line 1806 of yacc.c  */
#line 2124 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 567:

/* Line 1806 of yacc.c  */
#line 2126 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 568:

/* Line 1806 of yacc.c  */
#line 2130 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 569:

/* Line 1806 of yacc.c  */
#line 2134 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 570:

/* Line 1806 of yacc.c  */
#line 2138 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 571:

/* Line 1806 of yacc.c  */
#line 2146 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 572:

/* Line 1806 of yacc.c  */
#line 2151 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 573:

/* Line 1806 of yacc.c  */
#line 2157 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 574:

/* Line 1806 of yacc.c  */
#line 2163 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 575:

/* Line 1806 of yacc.c  */
#line 2170 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 576:

/* Line 1806 of yacc.c  */
#line 2180 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 586:

/* Line 1806 of yacc.c  */
#line 2197 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 587:

/* Line 1806 of yacc.c  */
#line 2199 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 588:

/* Line 1806 of yacc.c  */
#line 2201 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 589:

/* Line 1806 of yacc.c  */
#line 2203 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), false, true); }
    break;

  case 590:

/* Line 1806 of yacc.c  */
#line 2218 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 591:

/* Line 1806 of yacc.c  */
#line 2220 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 592:

/* Line 1806 of yacc.c  */
#line 2222 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr), true); }
    break;

  case 593:

/* Line 1806 of yacc.c  */
#line 2224 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
    break;

  case 594:

/* Line 1806 of yacc.c  */
#line 2228 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 595:

/* Line 1806 of yacc.c  */
#line 2229 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 596:

/* Line 1806 of yacc.c  */
#line 2233 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 597:

/* Line 1806 of yacc.c  */
#line 2234 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 598:

/* Line 1806 of yacc.c  */
#line 2235 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 603:

/* Line 1806 of yacc.c  */
#line 2251 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 604:

/* Line 1806 of yacc.c  */
#line 2252 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 605:

/* Line 1806 of yacc.c  */
#line 2253 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 606:

/* Line 1806 of yacc.c  */
#line 2254 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 608:

/* Line 1806 of yacc.c  */
#line 2259 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 609:

/* Line 1806 of yacc.c  */
#line 2260 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 610:

/* Line 1806 of yacc.c  */
#line 2261 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 611:

/* Line 1806 of yacc.c  */
#line 2265 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 612:

/* Line 1806 of yacc.c  */
#line 2266 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 613:

/* Line 1806 of yacc.c  */
#line 2267 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 614:

/* Line 1806 of yacc.c  */
#line 2268 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 615:

/* Line 1806 of yacc.c  */
#line 2269 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 616:

/* Line 1806 of yacc.c  */
#line 2277 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 617:

/* Line 1806 of yacc.c  */
#line 2278 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 618:

/* Line 1806 of yacc.c  */
#line 2279 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 619:

/* Line 1806 of yacc.c  */
#line 2280 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 620:

/* Line 1806 of yacc.c  */
#line 2284 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 621:

/* Line 1806 of yacc.c  */
#line 2285 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 622:

/* Line 1806 of yacc.c  */
#line 2289 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 623:

/* Line 1806 of yacc.c  */
#line 2290 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 626:

/* Line 1806 of yacc.c  */
#line 2296 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 627:

/* Line 1806 of yacc.c  */
#line 2297 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 628:

/* Line 1806 of yacc.c  */
#line 2298 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 629:

/* Line 1806 of yacc.c  */
#line 2299 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 630:

/* Line 1806 of yacc.c  */
#line 2300 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 631:

/* Line 1806 of yacc.c  */
#line 2301 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr),
                                            new SymExpr(gTrue)); }
    break;

  case 632:

/* Line 1806 of yacc.c  */
#line 2303 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr),
                                                   new SymExpr(gTrue)); }
    break;

  case 633:

/* Line 1806 of yacc.c  */
#line 2305 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 634:

/* Line 1806 of yacc.c  */
#line 2306 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 635:

/* Line 1806 of yacc.c  */
#line 2308 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 636:

/* Line 1806 of yacc.c  */
#line 2312 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 637:

/* Line 1806 of yacc.c  */
#line 2319 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 638:

/* Line 1806 of yacc.c  */
#line 2320 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 639:

/* Line 1806 of yacc.c  */
#line 2324 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 640:

/* Line 1806 of yacc.c  */
#line 2325 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 641:

/* Line 1806 of yacc.c  */
#line 2326 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 642:

/* Line 1806 of yacc.c  */
#line 2327 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 643:

/* Line 1806 of yacc.c  */
#line 2328 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 644:

/* Line 1806 of yacc.c  */
#line 2329 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 645:

/* Line 1806 of yacc.c  */
#line 2330 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 646:

/* Line 1806 of yacc.c  */
#line 2331 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 647:

/* Line 1806 of yacc.c  */
#line 2332 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 648:

/* Line 1806 of yacc.c  */
#line 2333 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 649:

/* Line 1806 of yacc.c  */
#line 2334 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 650:

/* Line 1806 of yacc.c  */
#line 2335 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 651:

/* Line 1806 of yacc.c  */
#line 2336 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 652:

/* Line 1806 of yacc.c  */
#line 2337 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 653:

/* Line 1806 of yacc.c  */
#line 2338 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 654:

/* Line 1806 of yacc.c  */
#line 2339 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 655:

/* Line 1806 of yacc.c  */
#line 2340 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 656:

/* Line 1806 of yacc.c  */
#line 2341 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 657:

/* Line 1806 of yacc.c  */
#line 2342 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 658:

/* Line 1806 of yacc.c  */
#line 2343 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 659:

/* Line 1806 of yacc.c  */
#line 2344 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 660:

/* Line 1806 of yacc.c  */
#line 2345 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 661:

/* Line 1806 of yacc.c  */
#line 2346 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr),
                                               new SymExpr(gTrue)); }
    break;

  case 662:

/* Line 1806 of yacc.c  */
#line 2351 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 663:

/* Line 1806 of yacc.c  */
#line 2352 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 664:

/* Line 1806 of yacc.c  */
#line 2353 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 665:

/* Line 1806 of yacc.c  */
#line 2354 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 666:

/* Line 1806 of yacc.c  */
#line 2355 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 667:

/* Line 1806 of yacc.c  */
#line 2356 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 668:

/* Line 1806 of yacc.c  */
#line 2357 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 669:

/* Line 1806 of yacc.c  */
#line 2361 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 670:

/* Line 1806 of yacc.c  */
#line 2362 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 671:

/* Line 1806 of yacc.c  */
#line 2363 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 672:

/* Line 1806 of yacc.c  */
#line 2364 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 673:

/* Line 1806 of yacc.c  */
#line 2368 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 674:

/* Line 1806 of yacc.c  */
#line 2369 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 675:

/* Line 1806 of yacc.c  */
#line 2370 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 676:

/* Line 1806 of yacc.c  */
#line 2371 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 677:

/* Line 1806 of yacc.c  */
#line 2376 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 678:

/* Line 1806 of yacc.c  */
#line 2377 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 679:

/* Line 1806 of yacc.c  */
#line 2378 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 680:

/* Line 1806 of yacc.c  */
#line 2379 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 681:

/* Line 1806 of yacc.c  */
#line 2380 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 682:

/* Line 1806 of yacc.c  */
#line 2381 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 683:

/* Line 1806 of yacc.c  */
#line 2382 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 11719 "bison-chapel.cpp"
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



