/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0




/* Copy the first part of user declarations.  */

/* Line 371 of yacc.c  */
#line 68 "bison-chapel.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "bison-chapel.h".  */
#ifndef YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED
# define YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
/* Line 387 of yacc.c  */
#line 33 "chapel.ypp"

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;

/* Line 387 of yacc.c  */
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

/* Line 387 of yacc.c  */
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

  enum   ProcIterOp {
    ProcIterOp_PROC,
    ProcIterOp_ITER,
    ProcIterOp_OP
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
    ProcIterOp                procIterOp;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<PotentialRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif

/* Line 387 of yacc.c  */
#line 152 "chapel.ypp"

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

/* Line 387 of yacc.c  */
#line 175 "chapel.ypp"

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


/* Line 387 of yacc.c  */
#line 255 "bison-chapel.cpp"

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
     TFOREACH = 297,
     TFORWARDING = 298,
     TIF = 299,
     TIMAG = 300,
     TIMPORT = 301,
     TIN = 302,
     TINCLUDE = 303,
     TINDEX = 304,
     TINLINE = 305,
     TINOUT = 306,
     TINT = 307,
     TITER = 308,
     TINITEQUALS = 309,
     TIMPLEMENTS = 310,
     TINTERFACE = 311,
     TLABEL = 312,
     TLAMBDA = 313,
     TLET = 314,
     TLIFETIME = 315,
     TLOCAL = 316,
     TLOCALE = 317,
     TMINUSMINUS = 318,
     TMODULE = 319,
     TNEW = 320,
     TNIL = 321,
     TNOINIT = 322,
     TNONE = 323,
     TNOTHING = 324,
     TON = 325,
     TONLY = 326,
     TOPERATOR = 327,
     TOTHERWISE = 328,
     TOUT = 329,
     TOVERRIDE = 330,
     TOWNED = 331,
     TPARAM = 332,
     TPLUSPLUS = 333,
     TPRAGMA = 334,
     TPRIMITIVE = 335,
     TPRIVATE = 336,
     TPROC = 337,
     TPROTOTYPE = 338,
     TPUBLIC = 339,
     TREAL = 340,
     TRECORD = 341,
     TREDUCE = 342,
     TREF = 343,
     TREQUIRE = 344,
     TRETURN = 345,
     TSCAN = 346,
     TSELECT = 347,
     TSERIAL = 348,
     TSHARED = 349,
     TSINGLE = 350,
     TSPARSE = 351,
     TSTRING = 352,
     TSUBDOMAIN = 353,
     TSYNC = 354,
     TTHEN = 355,
     TTHIS = 356,
     TTHROW = 357,
     TTHROWS = 358,
     TTRUE = 359,
     TTRY = 360,
     TTRYBANG = 361,
     TTYPE = 362,
     TUINT = 363,
     TUNDERSCORE = 364,
     TUNION = 365,
     TUNMANAGED = 366,
     TUSE = 367,
     TVAR = 368,
     TVOID = 369,
     TWHEN = 370,
     TWHERE = 371,
     TWHILE = 372,
     TWITH = 373,
     TYIELD = 374,
     TZIP = 375,
     TALIAS = 376,
     TAND = 377,
     TASSIGN = 378,
     TASSIGNBAND = 379,
     TASSIGNBOR = 380,
     TASSIGNBXOR = 381,
     TASSIGNDIVIDE = 382,
     TASSIGNEXP = 383,
     TASSIGNLAND = 384,
     TASSIGNLOR = 385,
     TASSIGNMINUS = 386,
     TASSIGNMOD = 387,
     TASSIGNMULTIPLY = 388,
     TASSIGNPLUS = 389,
     TASSIGNREDUCE = 390,
     TASSIGNSL = 391,
     TASSIGNSR = 392,
     TBANG = 393,
     TBAND = 394,
     TBNOT = 395,
     TBOR = 396,
     TBXOR = 397,
     TCOLON = 398,
     TCOMMA = 399,
     TDIVIDE = 400,
     TDOT = 401,
     TDOTDOT = 402,
     TDOTDOTDOT = 403,
     TEQUAL = 404,
     TEXP = 405,
     TGREATER = 406,
     TGREATEREQUAL = 407,
     THASH = 408,
     TIO = 409,
     TLESS = 410,
     TLESSEQUAL = 411,
     TMINUS = 412,
     TMOD = 413,
     TNOTEQUAL = 414,
     TOR = 415,
     TPLUS = 416,
     TQUESTION = 417,
     TSEMI = 418,
     TSHIFTLEFT = 419,
     TSHIFTRIGHT = 420,
     TSTAR = 421,
     TSWAP = 422,
     TLCBR = 423,
     TRCBR = 424,
     TLP = 425,
     TRP = 426,
     TLSBR = 427,
     TRSBR = 428,
     TNOELSE = 429,
     TDOTDOTOPENHIGH = 430,
     TUMINUS = 431,
     TUPLUS = 432,
     TLNOT = 433
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


#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct yypstate yypstate;

#if defined __STDC__ || defined __cplusplus
int yypush_parse (yypstate *ps, int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc, ParserContext* context);
#else
int yypush_parse ();
#endif

#if defined __STDC__ || defined __cplusplus
yypstate * yypstate_new (void);
#else
yypstate * yypstate_new ();
#endif
#if defined __STDC__ || defined __cplusplus
void yypstate_delete (yypstate *ps);
#else
void yypstate_delete ();
#endif
/* "%code provides" blocks.  */
/* Line 387 of yacc.c  */
#line 206 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 387 of yacc.c  */
#line 498 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 505 "bison-chapel.cpp"
/* Unqualified %code blocks.  */
/* Line 391 of yacc.c  */
#line 39 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;
  bool        parsingPrivate=false;

/* Line 391 of yacc.c  */
#line 214 "chapel.ypp"

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


/* Line 391 of yacc.c  */
#line 583 "bison-chapel.cpp"

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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
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
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   20310

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  179
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  162
/* YYNRULES -- Number of rules.  */
#define YYNRULES  695
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1247

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   433

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
     175,   176,   177,   178
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
     546,   552,   558,   561,   567,   571,   575,   582,   589,   594,
     600,   606,   610,   614,   621,   627,   634,   640,   647,   651,
     656,   660,   665,   671,   678,   684,   691,   695,   700,   704,
     709,   716,   724,   731,   739,   744,   750,   755,   761,   766,
     771,   775,   782,   788,   793,   797,   804,   810,   817,   823,
     832,   840,   845,   850,   857,   861,   863,   867,   869,   871,
     873,   875,   877,   879,   881,   883,   885,   887,   889,   891,
     893,   895,   897,   899,   901,   903,   905,   907,   914,   919,
     927,   933,   937,   944,   947,   951,   955,   958,   961,   965,
     969,   970,   973,   976,   980,   986,   988,   992,   996,  1002,
    1008,  1009,  1012,  1016,  1019,  1023,  1030,  1037,  1039,  1041,
    1043,  1044,  1047,  1048,  1051,  1055,  1061,  1067,  1069,  1071,
    1074,  1078,  1080,  1084,  1085,  1086,  1095,  1096,  1098,  1100,
    1101,  1102,  1113,  1117,  1121,  1127,  1133,  1137,  1139,  1143,
    1145,  1147,  1149,  1151,  1153,  1155,  1157,  1159,  1161,  1163,
    1165,  1167,  1169,  1171,  1173,  1175,  1177,  1179,  1181,  1183,
    1185,  1187,  1189,  1191,  1193,  1195,  1197,  1200,  1202,  1204,
    1206,  1208,  1210,  1212,  1214,  1216,  1218,  1220,  1222,  1224,
    1225,  1229,  1233,  1235,  1239,  1240,  1242,  1247,  1253,  1258,
    1264,  1271,  1278,  1279,  1281,  1283,  1285,  1287,  1289,  1292,
    1295,  1297,  1299,  1301,  1302,  1304,  1306,  1309,  1311,  1313,
    1315,  1317,  1319,  1320,  1322,  1325,  1327,  1329,  1331,  1332,
    1334,  1336,  1338,  1340,  1342,  1344,  1346,  1349,  1352,  1353,
    1356,  1359,  1364,  1369,  1371,  1375,  1379,  1383,  1387,  1391,
    1395,  1399,  1402,  1404,  1406,  1410,  1415,  1420,  1423,  1428,
    1429,  1432,  1435,  1437,  1439,  1441,  1444,  1446,  1451,  1455,
    1457,  1461,  1465,  1471,  1473,  1475,  1479,  1481,  1484,  1488,
    1489,  1492,  1495,  1499,  1502,  1507,  1511,  1515,  1520,  1524,
    1525,  1528,  1531,  1534,  1536,  1537,  1540,  1543,  1546,  1548,
    1553,  1558,  1565,  1569,  1570,  1572,  1574,  1578,  1583,  1587,
    1592,  1599,  1600,  1603,  1606,  1609,  1612,  1614,  1616,  1620,
    1624,  1626,  1630,  1632,  1634,  1636,  1640,  1644,  1645,  1647,
    1649,  1653,  1657,  1661,  1663,  1665,  1667,  1669,  1671,  1674,
    1676,  1678,  1680,  1682,  1684,  1686,  1689,  1694,  1699,  1704,
    1710,  1713,  1716,  1718,  1721,  1723,  1726,  1728,  1731,  1733,
    1736,  1738,  1740,  1747,  1754,  1759,  1769,  1779,  1787,  1794,
    1801,  1806,  1816,  1826,  1834,  1839,  1846,  1853,  1863,  1873,
    1880,  1882,  1884,  1886,  1888,  1890,  1892,  1894,  1896,  1900,
    1901,  1903,  1908,  1910,  1914,  1919,  1921,  1925,  1930,  1934,
    1938,  1940,  1942,  1945,  1947,  1950,  1952,  1954,  1958,  1960,
    1963,  1966,  1969,  1972,  1975,  1984,  1993,  2003,  2013,  2018,
    2020,  2022,  2024,  2026,  2028,  2030,  2032,  2034,  2036,  2041,
    2045,  2049,  2053,  2056,  2059,  2062,  2064,  2065,  2067,  2070,
    2073,  2075,  2077,  2079,  2081,  2083,  2085,  2088,  2091,  2093,
    2095,  2100,  2105,  2110,  2114,  2118,  2122,  2126,  2132,  2136,
    2141,  2145,  2150,  2152,  2154,  2156,  2158,  2160,  2162,  2164,
    2166,  2168,  2170,  2172,  2176,  2181,  2185,  2190,  2194,  2199,
    2203,  2209,  2213,  2217,  2221,  2225,  2229,  2233,  2237,  2241,
    2245,  2249,  2253,  2257,  2261,  2265,  2269,  2273,  2277,  2281,
    2285,  2289,  2293,  2297,  2301,  2304,  2307,  2310,  2313,  2316,
    2319,  2322,  2326,  2330,  2334,  2338,  2342,  2346,  2350,  2354,
    2356,  2358,  2360,  2362,  2364,  2366
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     180,     0,    -1,   181,    -1,    -1,   181,   182,    -1,   184,
      -1,   183,   184,    -1,    79,     8,    -1,   183,    79,     8,
      -1,   186,    -1,   190,    -1,   192,    -1,   199,    -1,   200,
      -1,   203,    -1,   214,    -1,   204,    -1,   223,    -1,   226,
      -1,   233,    -1,   228,    -1,   224,    -1,   241,    -1,   235,
      -1,   236,    -1,   240,    -1,   213,    -1,   312,   163,    -1,
      14,   184,    -1,    15,   313,   184,    -1,    16,   205,   163,
      -1,    23,   313,   192,    -1,    28,   205,   163,    -1,    30,
     300,   163,    -1,    57,   207,   184,    -1,    61,   324,   212,
      -1,    61,   212,    -1,    70,   324,   212,    -1,    93,   324,
     212,    -1,    93,   212,    -1,    99,   184,    -1,   119,   324,
     163,    -1,     1,   163,    -1,   187,   188,    64,   207,    -1,
     185,   168,   169,    -1,   185,   168,   193,   169,    -1,   185,
     168,     1,   169,    -1,    -1,    84,    -1,    81,    -1,    -1,
      83,    -1,    -1,    84,    -1,    81,    -1,    -1,    48,   191,
     189,   188,    64,   207,   163,    -1,   168,   169,    -1,   168,
     193,   169,    -1,   168,     1,   169,    -1,   182,    -1,   193,
     182,    -1,   324,    -1,   324,    13,   324,    -1,   194,   144,
     324,    -1,   194,   144,   324,    13,   324,    -1,   324,    -1,
     324,    13,   324,    -1,   324,    13,   109,    -1,   195,   144,
     324,    -1,   195,   144,   324,    13,   324,    -1,   195,   144,
     324,    13,   109,    -1,    -1,   194,    -1,   166,    -1,   194,
      -1,    -1,    84,    -1,    81,    -1,   198,   112,   195,   163,
      -1,   198,   112,   324,    36,   197,   163,    -1,   198,   112,
     324,    13,   324,    36,   197,   163,    -1,   198,   112,   324,
      13,   109,    36,   197,   163,    -1,   198,   112,   324,    71,
     196,   163,    -1,   198,   112,   324,    13,   324,    71,   196,
     163,    -1,   198,   112,   324,    13,   109,    71,   196,   163,
      -1,   198,    46,   202,   163,    -1,   324,    -1,   324,    13,
     208,    -1,   324,   146,   168,   194,   169,    -1,   201,    -1,
     202,   144,   201,    -1,    89,   299,   163,    -1,   327,   262,
     326,   163,    -1,   327,   167,   326,   163,    -1,   327,   135,
     326,   163,    -1,   327,   129,   326,   163,    -1,   327,   130,
     326,   163,    -1,   327,   123,    67,   163,    -1,    -1,     3,
      -1,     3,    -1,    68,    -1,   101,    -1,    39,    -1,   104,
      -1,   209,    -1,     3,    -1,    68,    -1,   101,    -1,    39,
      -1,   104,    -1,   209,    -1,     3,    -1,   101,    -1,    17,
      -1,    52,    -1,   108,    -1,    85,    -1,    45,    -1,    25,
      -1,    20,    -1,    97,    -1,    99,    -1,    95,    -1,    76,
      -1,    94,    -1,    18,    -1,   111,    -1,    33,    -1,    49,
      -1,    62,    -1,    69,    -1,   114,    -1,    17,    -1,    35,
      -1,    52,    -1,   108,    -1,    85,    -1,    45,    -1,    25,
      -1,    20,    -1,    97,    -1,    62,    -1,    69,    -1,   114,
      -1,    99,    -1,    95,    -1,    33,    -1,    49,    -1,    32,
     184,    -1,   192,    -1,    90,   163,    -1,    90,   326,   163,
      -1,   163,    -1,   216,    -1,    84,   216,    -1,    -1,    81,
     215,   216,    -1,   256,    -1,   286,    -1,   248,    -1,   282,
      -1,   244,    -1,   217,    -1,   218,    -1,    43,   324,   163,
      -1,    43,   324,    36,   197,   163,    -1,    43,   324,    71,
     196,   163,    -1,    43,   286,    -1,    -1,    38,    86,   219,
     207,   246,   168,   247,   169,    -1,    -1,    38,     8,    86,
     220,   207,   246,   168,   247,   169,    -1,    -1,    37,    86,
     221,   207,   246,   168,   247,   169,    -1,    -1,    37,     8,
      86,   222,   207,   246,   168,   247,   169,    -1,    38,   325,
     256,    -1,    37,   325,   256,    -1,    38,   325,   285,   287,
     163,    -1,    37,   325,   285,   287,   163,    -1,    38,    11,
      -1,    32,   184,   117,   324,   163,    -1,   117,   324,   212,
      -1,   117,   227,   212,    -1,    24,   324,    47,   324,   313,
     212,    -1,    24,   324,    47,   225,   313,   212,    -1,    24,
     324,   313,   212,    -1,    40,   324,    47,   324,   212,    -1,
      40,   324,    47,   225,   212,    -1,    40,   324,   212,    -1,
      40,   225,   212,    -1,    40,    77,   207,    47,   324,   212,
      -1,    41,   324,    47,   324,   212,    -1,    41,   324,    47,
     324,   316,   212,    -1,    41,   324,    47,   225,   212,    -1,
      41,   324,    47,   225,   316,   212,    -1,    41,   324,   212,
      -1,    41,   324,   316,   212,    -1,    41,   225,   212,    -1,
      41,   225,   316,   212,    -1,    42,   324,    47,   324,   212,
      -1,    42,   324,    47,   324,   316,   212,    -1,    42,   324,
      47,   225,   212,    -1,    42,   324,    47,   225,   316,   212,
      -1,    42,   324,   212,    -1,    42,   324,   316,   212,    -1,
      42,   225,   212,    -1,    42,   225,   316,   212,    -1,   172,
     299,    47,   324,   173,   184,    -1,   172,   299,    47,   324,
     316,   173,   184,    -1,   172,   299,    47,   225,   173,   184,
      -1,   172,   299,    47,   225,   316,   173,   184,    -1,   172,
     299,   173,   184,    -1,   172,   299,   316,   173,   184,    -1,
     172,   225,   173,   184,    -1,   172,   225,   316,   173,   184,
      -1,   120,   170,   299,   171,    -1,    44,   324,   100,   184,
      -1,    44,   324,   192,    -1,    44,   324,   100,   184,    34,
     184,    -1,    44,   324,   192,    34,   184,    -1,    44,   227,
     100,   184,    -1,    44,   227,   192,    -1,    44,   227,   100,
     184,    34,   184,    -1,    44,   227,   192,    34,   184,    -1,
      44,   324,   262,   324,   100,   184,    -1,    44,   324,   262,
     324,   192,    -1,    44,   324,   262,   324,   100,   184,    34,
     184,    -1,    44,   324,   262,   324,   192,    34,   184,    -1,
     113,   207,   123,   324,    -1,    27,   207,   123,   324,    -1,
      56,   207,   170,   229,   171,   192,    -1,    56,   207,   192,
      -1,   230,    -1,   229,   144,   230,    -1,   207,    -1,     3,
      -1,    17,    -1,    52,    -1,   108,    -1,    85,    -1,    45,
      -1,    25,    -1,    20,    -1,    97,    -1,    62,    -1,    69,
      -1,   114,    -1,   232,    -1,    68,    -1,   101,    -1,    39,
      -1,   104,    -1,    33,    -1,    49,    -1,    55,   207,   170,
     304,   171,   163,    -1,   231,    55,   207,   163,    -1,   231,
      55,   207,   170,   304,   171,   163,    -1,    55,   207,   170,
     304,   171,    -1,   231,    55,   207,    -1,   231,    55,   207,
     170,   304,   171,    -1,    29,   184,    -1,   105,   324,   163,
      -1,   106,   324,   163,    -1,   105,   204,    -1,   106,   204,
      -1,   105,   192,   237,    -1,   106,   192,   237,    -1,    -1,
     237,   238,    -1,    21,   192,    -1,    21,   239,   192,    -1,
      21,   170,   239,   171,   192,    -1,   207,    -1,   207,   143,
     324,    -1,   102,   324,   163,    -1,    92,   324,   168,   242,
     169,    -1,    92,   324,   168,     1,   169,    -1,    -1,   242,
     243,    -1,   115,   299,   212,    -1,    73,   184,    -1,    73,
      32,   184,    -1,   245,   207,   246,   168,   247,   169,    -1,
     245,   207,   246,   168,     1,   169,    -1,    22,    -1,    86,
      -1,   110,    -1,    -1,   143,   299,    -1,    -1,   247,   214,
      -1,   247,   183,   214,    -1,   249,   207,   168,   250,   169,
      -1,   249,   207,   168,     1,   169,    -1,    35,    -1,   251,
      -1,   250,   144,    -1,   250,   144,   251,    -1,   207,    -1,
     207,   123,   324,    -1,    -1,    -1,    58,   253,   264,   254,
     272,   294,   278,   275,    -1,    -1,    50,    -1,    75,    -1,
      -1,    -1,   255,   271,   257,   259,   258,   272,   293,   273,
     278,   274,    -1,   270,   261,   263,    -1,   270,   262,   263,
      -1,   270,   260,   146,   261,   263,    -1,   270,   260,   146,
     262,   263,    -1,   270,     1,   263,    -1,   306,    -1,   170,
     324,   171,    -1,   206,    -1,   139,    -1,   141,    -1,   142,
      -1,   140,    -1,   149,    -1,   159,    -1,   156,    -1,   152,
      -1,   155,    -1,   151,    -1,   161,    -1,   157,    -1,   166,
      -1,   145,    -1,   164,    -1,   165,    -1,   158,    -1,   150,
      -1,   138,    -1,    19,    -1,   153,    -1,    12,    -1,   167,
      -1,   154,    -1,    54,    -1,   143,    -1,   207,   138,    -1,
     123,    -1,   134,    -1,   131,    -1,   133,    -1,   127,    -1,
     132,    -1,   128,    -1,   124,    -1,   125,    -1,   126,    -1,
     137,    -1,   136,    -1,    -1,   170,   266,   171,    -1,   170,
     266,   171,    -1,   267,    -1,   265,   144,   267,    -1,    -1,
     265,    -1,   268,   207,   298,   291,    -1,   183,   268,   207,
     298,   291,    -1,   268,   207,   298,   277,    -1,   183,   268,
     207,   298,   277,    -1,   268,   170,   290,   171,   298,   291,
      -1,   268,   170,   290,   171,   298,   277,    -1,    -1,   269,
      -1,    47,    -1,    51,    -1,    74,    -1,    27,    -1,    27,
      47,    -1,    27,    88,    -1,    77,    -1,    88,    -1,   107,
      -1,    -1,    77,    -1,    88,    -1,    27,    88,    -1,    27,
      -1,   107,    -1,    82,    -1,    53,    -1,    72,    -1,    -1,
      27,    -1,    27,    88,    -1,    88,    -1,    77,    -1,   107,
      -1,    -1,   103,    -1,   163,    -1,   275,    -1,   192,    -1,
     213,    -1,     4,    -1,   148,    -1,   148,   324,    -1,   148,
     276,    -1,    -1,   116,   324,    -1,    60,   279,    -1,   116,
     324,    60,   279,    -1,    60,   279,   116,   324,    -1,   280,
      -1,   279,   144,   280,    -1,   281,   123,   281,    -1,   281,
     155,   281,    -1,   281,   156,   281,    -1,   281,   149,   281,
      -1,   281,   151,   281,    -1,   281,   152,   281,    -1,    90,
     281,    -1,     3,    -1,   101,    -1,   107,   283,   163,    -1,
      26,   107,   283,   163,    -1,    38,   107,   283,   163,    -1,
     207,   284,    -1,   207,   284,   144,   283,    -1,    -1,   123,
     307,    -1,   123,   295,    -1,    77,    -1,    27,    -1,    88,
      -1,    27,    88,    -1,   113,    -1,    26,   285,   287,   163,
      -1,   285,   287,   163,    -1,   288,    -1,   287,   144,   288,
      -1,   207,   294,   291,    -1,   170,   290,   171,   294,   291,
      -1,   109,    -1,   207,    -1,   170,   290,   171,    -1,   289,
      -1,   289,   144,    -1,   289,   144,   290,    -1,    -1,   123,
      67,    -1,   123,   326,    -1,   172,   173,   307,    -1,   172,
     173,    -1,   172,   299,   173,   307,    -1,   172,   299,   173,
      -1,   172,   173,   292,    -1,   172,   299,   173,   292,    -1,
     172,     1,   173,    -1,    -1,   143,   307,    -1,   143,   292,
      -1,   143,   211,    -1,     1,    -1,    -1,   143,   307,    -1,
     143,   295,    -1,   143,   211,    -1,     1,    -1,   172,   299,
     173,   307,    -1,   172,   299,   173,   295,    -1,   172,   299,
      47,   324,   173,   307,    -1,   172,     1,   173,    -1,    -1,
     307,    -1,   276,    -1,   172,   173,   296,    -1,   172,   299,
     173,   296,    -1,   172,   173,   297,    -1,   172,   299,   173,
     297,    -1,   172,   299,    47,   324,   173,   296,    -1,    -1,
     143,   307,    -1,   143,   276,    -1,   143,   211,    -1,   143,
     297,    -1,   324,    -1,   276,    -1,   299,   144,   324,    -1,
     299,   144,   276,    -1,   324,    -1,   300,   144,   324,    -1,
     109,    -1,   326,    -1,   276,    -1,   301,   144,   301,    -1,
     302,   144,   301,    -1,    -1,   304,    -1,   305,    -1,   304,
     144,   305,    -1,   208,   123,   276,    -1,   208,   123,   326,
      -1,   276,    -1,   326,    -1,   208,    -1,   210,    -1,   308,
      -1,   308,   162,    -1,   162,    -1,   311,    -1,   327,    -1,
     310,    -1,   337,    -1,   336,    -1,    95,   324,    -1,    49,
     170,   303,   171,    -1,    33,   170,   303,   171,    -1,    98,
     170,   303,   171,    -1,    96,    98,   170,   305,   171,    -1,
      14,   324,    -1,    99,   324,    -1,    76,    -1,    76,   324,
      -1,   111,    -1,   111,   324,    -1,    94,    -1,    94,   324,
      -1,    18,    -1,    18,   324,    -1,    22,    -1,    86,    -1,
      40,   324,    47,   324,    32,   324,    -1,    40,   324,    47,
     225,    32,   324,    -1,    40,   324,    32,   324,    -1,    40,
     324,    47,   324,    32,    44,   324,   100,   324,    -1,    40,
     324,    47,   225,    32,    44,   324,   100,   324,    -1,    40,
     324,    32,    44,   324,   100,   324,    -1,    41,   324,    47,
     324,    32,   324,    -1,    41,   324,    47,   225,    32,   324,
      -1,    41,   324,    32,   324,    -1,    41,   324,    47,   324,
      32,    44,   324,   100,   324,    -1,    41,   324,    47,   225,
      32,    44,   324,   100,   324,    -1,    41,   324,    32,    44,
     324,   100,   324,    -1,   172,   299,   173,   324,    -1,   172,
     299,    47,   324,   173,   324,    -1,   172,   299,    47,   225,
     173,   324,    -1,   172,   299,    47,   324,   173,    44,   324,
     100,   324,    -1,   172,   299,    47,   225,   173,    44,   324,
     100,   324,    -1,    44,   324,   100,   324,    34,   324,    -1,
      66,    -1,   311,    -1,   306,    -1,   330,    -1,   329,    -1,
     252,    -1,   322,    -1,   323,    -1,   320,   154,   324,    -1,
      -1,   314,    -1,   118,   170,   315,   171,    -1,   318,    -1,
     315,   144,   318,    -1,   118,   170,   317,   171,    -1,   318,
      -1,   317,   144,   318,    -1,   319,   306,   294,   291,    -1,
     340,    87,   306,    -1,   324,    87,   306,    -1,    27,    -1,
      47,    -1,    27,    47,    -1,    88,    -1,    27,    88,    -1,
     113,    -1,   327,    -1,   320,   154,   324,    -1,    65,    -1,
      65,    76,    -1,    65,    94,    -1,    65,   111,    -1,    65,
      18,    -1,   321,   324,    -1,    65,    76,   170,   324,   171,
     170,   303,   171,    -1,    65,    94,   170,   324,   171,   170,
     303,   171,    -1,    65,    76,   170,   324,   171,   170,   303,
     171,   162,    -1,    65,    94,   170,   324,   171,   170,   303,
     171,   162,    -1,    59,   287,    47,   324,    -1,   334,    -1,
     307,    -1,   309,    -1,   338,    -1,   339,    -1,   252,    -1,
     322,    -1,   323,    -1,   234,    -1,   170,   148,   324,   171,
      -1,   324,   143,   324,    -1,   324,   147,   324,    -1,   324,
     175,   324,    -1,   324,   147,    -1,   147,   324,    -1,   175,
     324,    -1,   147,    -1,    -1,   324,    -1,   105,   324,    -1,
     106,   324,    -1,   324,    -1,   306,    -1,   329,    -1,   330,
      -1,   331,    -1,   327,    -1,   324,   138,    -1,   308,   162,
      -1,   252,    -1,   333,    -1,   328,   170,   303,   171,    -1,
     328,   172,   303,   173,    -1,    80,   170,   303,   171,    -1,
     324,   146,   208,    -1,   324,   146,   107,    -1,   324,   146,
      33,    -1,   324,   146,    62,    -1,   324,   146,    20,   170,
     171,    -1,   170,   301,   171,    -1,   170,   301,   144,   171,
      -1,   170,   302,   171,    -1,   170,   302,   144,   171,    -1,
      39,    -1,   104,    -1,     8,    -1,     9,    -1,   332,    -1,
     333,    -1,     5,    -1,     6,    -1,     7,    -1,    10,    -1,
      68,    -1,   168,   299,   169,    -1,   168,   299,   144,   169,
      -1,   172,   299,   173,    -1,   172,   299,   144,   173,    -1,
     172,   335,   173,    -1,   172,   335,   144,   173,    -1,   324,
     121,   324,    -1,   335,   144,   324,   121,   324,    -1,   324,
     161,   324,    -1,   324,   157,   324,    -1,   324,   166,   324,
      -1,   324,   145,   324,    -1,   324,   164,   324,    -1,   324,
     165,   324,    -1,   324,   158,   324,    -1,   324,   149,   324,
      -1,   324,   159,   324,    -1,   324,   156,   324,    -1,   324,
     152,   324,    -1,   324,   155,   324,    -1,   324,   151,   324,
      -1,   324,   139,   324,    -1,   324,   141,   324,    -1,   324,
     142,   324,    -1,   324,   122,   324,    -1,   324,   160,   324,
      -1,   324,   150,   324,    -1,   324,    19,   324,    -1,   324,
      12,   324,    -1,   324,   153,   324,    -1,   324,    31,   324,
      -1,   161,   324,    -1,   157,   324,    -1,    63,   324,    -1,
      78,   324,    -1,   138,   324,    -1,   324,   138,    -1,   140,
     324,    -1,   324,    87,   324,    -1,   324,    87,   225,    -1,
     340,    87,   324,    -1,   340,    87,   225,    -1,   324,    91,
     324,    -1,   324,    91,   225,    -1,   340,    91,   324,    -1,
     340,    91,   225,    -1,   161,    -1,   166,    -1,   122,    -1,
     160,    -1,   139,    -1,   141,    -1,   142,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   512,   512,   517,   518,   524,   525,   530,   531,   536,
     537,   538,   539,   540,   541,   542,   543,   544,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,   573,   586,   591,   596,   604,   605,   606,
     610,   611,   615,   616,   617,   622,   621,   642,   643,   644,
     649,   650,   655,   660,   665,   669,   678,   683,   688,   693,
     697,   701,   709,   714,   718,   723,   727,   728,   729,   733,
     734,   735,   736,   737,   738,   739,   743,   748,   749,   750,
     754,   755,   759,   763,   765,   767,   769,   771,   773,   780,
     781,   785,   786,   787,   788,   789,   790,   793,   794,   795,
     796,   797,   798,   810,   811,   822,   823,   824,   825,   826,
     827,   828,   829,   830,   831,   832,   833,   834,   835,   836,
     837,   838,   839,   840,   844,   845,   846,   847,   848,   849,
     850,   851,   852,   853,   854,   855,   862,   863,   864,   865,
     869,   870,   874,   875,   879,   880,   881,   891,   891,   896,
     897,   898,   899,   900,   901,   902,   906,   907,   908,   909,
     914,   913,   929,   928,   945,   944,   960,   959,   975,   979,
     984,   992,  1003,  1010,  1011,  1012,  1013,  1014,  1015,  1016,
    1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,
    1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,  1035,  1036,
    1037,  1043,  1049,  1055,  1061,  1068,  1075,  1079,  1086,  1090,
    1091,  1092,  1093,  1095,  1096,  1097,  1098,  1100,  1102,  1104,
    1106,  1111,  1112,  1116,  1118,  1126,  1127,  1132,  1137,  1138,
    1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,  1147,  1148,
    1149,  1156,  1157,  1158,  1159,  1168,  1169,  1173,  1175,  1178,
    1184,  1186,  1189,  1195,  1198,  1199,  1200,  1201,  1202,  1203,
    1207,  1208,  1212,  1213,  1214,  1218,  1219,  1223,  1226,  1228,
    1233,  1234,  1238,  1240,  1242,  1249,  1259,  1273,  1278,  1283,
    1291,  1292,  1297,  1298,  1300,  1305,  1321,  1328,  1337,  1345,
    1349,  1356,  1357,  1362,  1367,  1361,  1394,  1397,  1401,  1409,
    1419,  1408,  1458,  1462,  1467,  1471,  1476,  1483,  1484,  1488,
    1489,  1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,  1498,
    1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,  1507,  1508,
    1509,  1510,  1511,  1512,  1513,  1514,  1515,  1519,  1520,  1521,
    1522,  1523,  1524,  1525,  1526,  1527,  1528,  1529,  1530,  1534,
    1535,  1539,  1543,  1544,  1548,  1549,  1553,  1555,  1557,  1559,
    1561,  1563,  1568,  1569,  1573,  1574,  1575,  1576,  1577,  1578,
    1579,  1580,  1581,  1585,  1586,  1587,  1588,  1589,  1590,  1594,
    1595,  1596,  1600,  1601,  1602,  1603,  1604,  1605,  1609,  1610,
    1613,  1614,  1618,  1619,  1623,  1627,  1628,  1629,  1637,  1638,
    1640,  1642,  1644,  1649,  1651,  1656,  1657,  1658,  1659,  1660,
    1661,  1662,  1666,  1668,  1673,  1675,  1677,  1682,  1695,  1712,
    1713,  1715,  1720,  1721,  1722,  1723,  1724,  1728,  1734,  1742,
    1743,  1751,  1753,  1758,  1760,  1762,  1767,  1769,  1771,  1778,
    1779,  1780,  1785,  1787,  1789,  1793,  1797,  1799,  1803,  1811,
    1812,  1813,  1814,  1815,  1820,  1821,  1822,  1823,  1824,  1844,
    1848,  1852,  1860,  1867,  1868,  1869,  1873,  1875,  1881,  1883,
    1885,  1890,  1891,  1892,  1893,  1894,  1900,  1901,  1902,  1903,
    1907,  1908,  1912,  1913,  1914,  1918,  1919,  1923,  1924,  1928,
    1929,  1933,  1934,  1935,  1936,  1940,  1941,  1952,  1954,  1956,
    1962,  1963,  1964,  1965,  1966,  1967,  1969,  1971,  1973,  1975,
    1977,  1979,  1982,  1984,  1986,  1988,  1990,  1992,  1994,  1996,
    1999,  2001,  2006,  2008,  2010,  2012,  2014,  2016,  2018,  2020,
    2022,  2024,  2026,  2028,  2030,  2037,  2043,  2049,  2055,  2064,
    2074,  2082,  2083,  2084,  2085,  2086,  2087,  2088,  2089,  2094,
    2095,  2099,  2103,  2104,  2108,  2112,  2113,  2117,  2121,  2125,
    2132,  2133,  2134,  2135,  2136,  2137,  2141,  2142,  2147,  2149,
    2153,  2157,  2161,  2169,  2174,  2180,  2186,  2193,  2203,  2211,
    2212,  2213,  2214,  2215,  2216,  2217,  2218,  2219,  2220,  2222,
    2224,  2226,  2241,  2243,  2245,  2247,  2252,  2253,  2257,  2258,
    2259,  2263,  2264,  2265,  2266,  2275,  2276,  2277,  2278,  2279,
    2283,  2284,  2285,  2289,  2290,  2291,  2292,  2293,  2301,  2302,
    2303,  2304,  2308,  2309,  2313,  2314,  2318,  2319,  2320,  2321,
    2322,  2323,  2324,  2325,  2327,  2329,  2330,  2331,  2335,  2343,
    2344,  2348,  2349,  2350,  2351,  2352,  2353,  2354,  2355,  2356,
    2357,  2358,  2359,  2360,  2361,  2362,  2363,  2364,  2365,  2366,
    2367,  2368,  2369,  2370,  2375,  2376,  2377,  2378,  2379,  2380,
    2381,  2385,  2386,  2387,  2388,  2392,  2393,  2394,  2395,  2400,
    2401,  2402,  2403,  2404,  2405,  2406
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
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
  "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE", "TFOR", "TFORALL", "TFOREACH",
  "TFORWARDING", "TIF", "TIMAG", "TIMPORT", "TIN", "TINCLUDE", "TINDEX",
  "TINLINE", "TINOUT", "TINT", "TITER", "TINITEQUALS", "TIMPLEMENTS",
  "TINTERFACE", "TLABEL", "TLAMBDA", "TLET", "TLIFETIME", "TLOCAL",
  "TLOCALE", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TNONE",
  "TNOTHING", "TON", "TONLY", "TOPERATOR", "TOTHERWISE", "TOUT",
  "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE",
  "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC", "TREAL", "TRECORD",
  "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL",
  "TSHARED", "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN", "TSYNC",
  "TTHEN", "TTHIS", "TTHROW", "TTHROWS", "TTRUE", "TTRY", "TTRYBANG",
  "TTYPE", "TUINT", "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUSE", "TVAR",
  "TVOID", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP",
  "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR",
  "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR",
  "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY", "TASSIGNPLUS",
  "TASSIGNREDUCE", "TASSIGNSL", "TASSIGNSR", "TBANG", "TBAND", "TBNOT",
  "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT",
  "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL", "THASH",
  "TIO", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOTEQUAL", "TOR",
  "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR",
  "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR", "TNOELSE",
  "TDOTDOTOPENHIGH", "TUMINUS", "TUPLUS", "TLNOT", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_start", "module_decl_stmt", "access_control",
  "opt_prototype", "include_access_control", "include_module_stmt", "$@1",
  "block_stmt", "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls",
  "except_ls", "use_access_control", "use_stmt", "import_stmt",
  "import_expr", "import_ls", "require_stmt", "assignment_stmt",
  "opt_label_ident", "ident_fn_def", "ident_def", "ident_use",
  "internal_type_ident_def", "scalar_type", "reserved_type_ident_use",
  "do_stmt", "return_stmt", "class_level_stmt", "@2", "private_decl",
  "forwarding_stmt", "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6",
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
  "req_formal_ls", "formal_ls_inner", "formal_ls", "formal",
  "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_iter_or_op", "opt_ret_tag", "opt_throws_error",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "var_arg_expr", "opt_lifetime_where", "lifetime_components_expr",
  "lifetime_expr", "lifetime_ident", "type_alias_decl_stmt",
  "type_alias_decl_stmt_inner", "opt_init_type", "var_decl_type",
  "var_decl_stmt", "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "ret_array_type", "opt_ret_type", "opt_type",
  "array_type", "opt_formal_array_elt_type", "formal_array_type",
  "opt_formal_type", "expr_ls", "simple_expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "sub_type_level_expr", "for_expr",
  "cond_expr", "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "shadow_var_prefix", "io_expr",
  "new_maybe_decorated", "new_expr", "let_expr", "expr", "opt_expr",
  "opt_try_expr", "lhs_expr", "call_base_expr", "call_expr", "dot_expr",
  "parenthesized_expr", "bool_literal", "str_bytes_literal",
  "literal_expr", "assoc_expr_ls", "binary_op_expr", "unary_op_expr",
  "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULL
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
     425,   426,   427,   428,   429,   430,   431,   432,   433
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   179,   180,   181,   181,   182,   182,   183,   183,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   185,   186,   186,   186,   187,   187,   187,
     188,   188,   189,   189,   189,   191,   190,   192,   192,   192,
     193,   193,   194,   194,   194,   194,   195,   195,   195,   195,
     195,   195,   196,   196,   197,   197,   198,   198,   198,   199,
     199,   199,   199,   199,   199,   199,   200,   201,   201,   201,
     202,   202,   203,   204,   204,   204,   204,   204,   204,   205,
     205,   206,   206,   206,   206,   206,   206,   207,   207,   207,
     207,   207,   207,   208,   208,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   211,   211,   211,   211,
     212,   212,   213,   213,   214,   214,   214,   215,   214,   216,
     216,   216,   216,   216,   216,   216,   217,   217,   217,   217,
     219,   218,   220,   218,   221,   218,   222,   218,   218,   218,
     218,   218,   223,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   225,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   227,   227,   228,   228,   229,   229,   230,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   232,   232,   232,   232,   232,   232,   233,   233,   233,
     234,   234,   234,   235,   236,   236,   236,   236,   236,   236,
     237,   237,   238,   238,   238,   239,   239,   240,   241,   241,
     242,   242,   243,   243,   243,   244,   244,   245,   245,   245,
     246,   246,   247,   247,   247,   248,   248,   249,   250,   250,
     250,   251,   251,   253,   254,   252,   255,   255,   255,   257,
     258,   256,   259,   259,   259,   259,   259,   260,   260,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   263,
     263,   264,   265,   265,   266,   266,   267,   267,   267,   267,
     267,   267,   268,   268,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   270,   270,   270,   270,   270,   270,   271,
     271,   271,   272,   272,   272,   272,   272,   272,   273,   273,
     274,   274,   275,   275,   276,   277,   277,   277,   278,   278,
     278,   278,   278,   279,   279,   280,   280,   280,   280,   280,
     280,   280,   281,   281,   282,   282,   282,   283,   283,   284,
     284,   284,   285,   285,   285,   285,   285,   286,   286,   287,
     287,   288,   288,   289,   289,   289,   290,   290,   290,   291,
     291,   291,   292,   292,   292,   292,   292,   292,   292,   293,
     293,   293,   293,   293,   294,   294,   294,   294,   294,   295,
     295,   295,   295,   296,   296,   296,   297,   297,   297,   297,
     297,   298,   298,   298,   298,   298,   299,   299,   299,   299,
     300,   300,   301,   301,   301,   302,   302,   303,   303,   304,
     304,   305,   305,   305,   305,   306,   306,   307,   307,   307,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   310,
     311,   312,   312,   312,   312,   312,   312,   312,   312,   313,
     313,   314,   315,   315,   316,   317,   317,   318,   318,   318,
     319,   319,   319,   319,   319,   319,   320,   320,   321,   321,
     321,   321,   321,   322,   322,   322,   322,   322,   323,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   325,   325,   326,   326,
     326,   327,   327,   327,   327,   328,   328,   328,   328,   328,
     329,   329,   329,   330,   330,   330,   330,   330,   331,   331,
     331,   331,   332,   332,   333,   333,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   335,
     335,   336,   336,   336,   336,   336,   336,   336,   336,   336,
     336,   336,   336,   336,   336,   336,   336,   336,   336,   336,
     336,   336,   336,   336,   337,   337,   337,   337,   337,   337,
     337,   338,   338,   338,   338,   339,   339,   339,   339,   340,
     340,   340,   340,   340,   340,   340
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
       5,     5,     2,     5,     3,     3,     6,     6,     4,     5,
       5,     3,     3,     6,     5,     6,     5,     6,     3,     4,
       3,     4,     5,     6,     5,     6,     3,     4,     3,     4,
       6,     7,     6,     7,     4,     5,     4,     5,     4,     4,
       3,     6,     5,     4,     3,     6,     5,     6,     5,     8,
       7,     4,     4,     6,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     4,     7,
       5,     3,     6,     2,     3,     3,     2,     2,     3,     3,
       0,     2,     2,     3,     5,     1,     3,     3,     5,     5,
       0,     2,     3,     2,     3,     6,     6,     1,     1,     1,
       0,     2,     0,     2,     3,     5,     5,     1,     1,     2,
       3,     1,     3,     0,     0,     8,     0,     1,     1,     0,
       0,    10,     3,     3,     5,     5,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     3,     1,     3,     0,     1,     4,     5,     4,     5,
       6,     6,     0,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     0,     1,     2,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     0,     2,
       2,     4,     4,     1,     3,     3,     3,     3,     3,     3,
       3,     2,     1,     1,     3,     4,     4,     2,     4,     0,
       2,     2,     1,     1,     1,     2,     1,     4,     3,     1,
       3,     3,     5,     1,     1,     3,     1,     2,     3,     0,
       2,     2,     3,     2,     4,     3,     3,     4,     3,     0,
       2,     2,     2,     1,     0,     2,     2,     2,     1,     4,
       4,     6,     3,     0,     1,     1,     3,     4,     3,     4,
       6,     0,     2,     2,     2,     2,     1,     1,     3,     3,
       1,     3,     1,     1,     1,     3,     3,     0,     1,     1,
       3,     3,     3,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     2,     4,     4,     4,     5,
       2,     2,     1,     2,     1,     2,     1,     2,     1,     2,
       1,     1,     6,     6,     4,     9,     9,     7,     6,     6,
       4,     9,     9,     7,     4,     6,     6,     9,     9,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     4,     1,     3,     4,     1,     3,     4,     3,     3,
       1,     1,     2,     1,     2,     1,     1,     3,     1,     2,
       2,     2,     2,     2,     8,     8,     9,     9,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     3,
       3,     3,     2,     2,     2,     1,     0,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       4,     4,     4,     3,     3,     3,     3,     5,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     3,     4,     3,     4,     3,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,   113,   638,   639,   640,   634,
     635,   641,     0,   559,    99,   134,   528,   141,   530,   559,
       0,   140,     0,   433,    99,     0,     0,     0,   255,   135,
     606,   606,   632,     0,     0,     0,     0,     0,   139,    55,
     256,   307,   136,     0,     0,     0,   303,     0,     0,   143,
       0,   578,   550,   642,   144,     0,   308,   522,   432,     0,
       0,     0,   157,   306,   138,   531,   434,     0,     0,     0,
       0,   526,     0,     0,   142,     0,     0,   114,     0,   633,
       0,     0,     0,   137,   289,   524,   436,   145,     0,     0,
     691,     0,   693,     0,   694,   695,   605,     0,   692,   689,
     509,   154,   690,     0,     0,     0,     0,     4,     0,     5,
       0,     9,    50,    10,    11,     0,    12,    13,    14,    16,
     505,   506,    26,    15,   155,   164,   165,    17,    21,    18,
      20,     0,   250,    19,   597,    23,    24,    25,    22,   163,
       0,   161,     0,   594,     0,   159,   162,     0,   160,   611,
     590,   507,   591,   512,   510,     0,     0,     0,   595,   596,
       0,   511,     0,   612,   613,   614,   636,   637,   589,   514,
     513,   592,   593,     0,    42,    28,   520,     0,     0,   560,
     100,     0,     0,   530,   135,     0,     0,     0,     0,   531,
       0,     0,     0,     0,   594,   611,   510,   595,   596,   529,
     511,   612,   613,     0,   559,     0,     0,   435,     0,   263,
       0,   490,     0,   497,   634,   531,   607,   306,   634,   182,
     531,     0,   306,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   169,     0,     0,     0,     0,     0,    52,   497,
     107,   115,   127,   121,   120,   129,   110,   119,   130,   116,
     131,   108,   132,   125,   118,   126,   124,   122,   123,   109,
     111,   117,   128,   133,     0,   112,     0,     0,     0,     0,
       0,     0,   439,     0,   151,    36,     0,   676,   582,   579,
     580,   581,     0,   523,   677,     7,   497,   306,   287,   297,
     606,   288,   156,   404,   487,     0,   486,     0,     0,   152,
     610,     0,     0,    39,     0,   527,   515,     0,   497,    40,
     521,     0,   270,   266,     0,   511,   270,   267,     0,   429,
       0,   525,     0,     0,     0,   678,   680,   603,   675,   674,
       0,    57,    60,     0,     0,   492,     0,   494,     0,     0,
     493,     0,     0,   486,     0,   604,     0,     6,     0,    51,
       0,     0,     0,     0,   290,     0,   390,   391,   389,   309,
       0,   508,    27,     0,   583,     0,     0,     0,     0,     0,
       0,   679,     0,     0,     0,     0,     0,     0,   602,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,   354,   355,   356,   351,
     353,     0,     0,   349,   352,   350,   348,     0,   358,   357,
       0,     0,   497,   497,     0,     0,     0,    29,    30,     0,
       0,     0,     0,     0,     0,     0,    31,     0,     0,     0,
       0,    32,     0,    33,     0,   505,   503,     0,   498,   499,
     504,   176,     0,   179,     0,   172,     0,     0,   178,     0,
       0,     0,   192,     0,     0,   191,     0,   200,     0,     0,
       0,   198,     0,   208,     0,     0,   206,     0,     0,    72,
     166,     0,     0,     0,   224,     0,   347,   220,     0,    54,
      53,    50,     0,     0,     0,   234,    34,   372,   304,   443,
       0,   444,   446,     0,   468,     0,   449,     0,     0,   150,
      35,     0,     0,    37,     0,   158,     0,    92,   608,   609,
     153,     0,    38,     0,     0,   277,   268,   264,   269,   265,
       0,   427,   424,   185,   184,    41,    59,    58,    61,     0,
     643,     0,     0,   628,     0,   630,     0,     0,     0,     0,
       0,     0,     0,     0,   647,     8,     0,    44,     0,     0,
      90,     0,    87,     0,    66,   261,     0,     0,     0,   383,
     438,   558,   671,   670,   673,   682,   681,   686,   685,   667,
     664,   665,   666,   599,   654,   113,     0,   625,   626,   114,
     624,   623,   600,   658,   669,   663,   661,   672,   662,   660,
     652,   657,   659,   668,   651,   655,   656,   653,   601,     0,
       0,     0,     0,     0,     0,     0,     0,   684,   683,   688,
     687,   570,   571,   573,   575,     0,   562,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   645,   261,   559,
     559,   188,   425,   437,   491,     0,     0,   517,     0,     0,
     290,     0,     0,   290,   426,     0,     0,     0,     0,   534,
       0,     0,     0,   201,     0,   540,     0,     0,   199,   209,
       0,     0,   207,   690,    75,     0,    62,    73,     0,     0,
       0,   223,     0,   219,     0,     0,     0,     0,   516,     0,
     237,     0,   235,   377,   374,   375,   376,   380,   381,   382,
     372,   365,     0,   362,     0,   373,   392,     0,   447,     0,
     148,   149,   147,   146,     0,   467,   466,   590,     0,   441,
     588,   440,     0,     0,   622,   489,   488,     0,     0,     0,
     518,     0,   271,   431,   590,     0,   644,   598,   629,   495,
     631,   496,   216,     0,     0,     0,   646,   214,   544,     0,
     649,   648,     0,    46,    45,    43,     0,    86,     0,     0,
       0,    79,     0,     0,    72,   258,     0,   291,     0,     0,
     301,     0,   298,   387,   384,   385,   388,   310,     0,     0,
      98,    96,    97,    95,    94,    93,   620,   621,   572,   574,
       0,   561,   134,   141,   140,   139,   136,   143,   144,   138,
     142,   137,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   183,   501,
     502,   500,   290,     0,   181,   290,     0,   180,     0,   218,
       0,     0,   190,     0,   189,     0,   565,     0,     0,   196,
       0,     0,   194,     0,   204,     0,   202,     0,     0,   167,
       0,   168,   232,   231,     0,   226,     0,     0,   222,     0,
     228,     0,   260,     0,     0,   378,   379,     0,   372,   361,
       0,   481,   393,   396,   395,   397,     0,   445,   448,   449,
       0,     0,   450,   451,     0,     0,   279,     0,     0,   278,
     281,   519,     0,   272,   275,     0,   428,   217,     0,     0,
       0,     0,   215,     0,    91,    88,     0,    69,    68,    67,
       0,     0,     0,     0,   306,   296,     0,   299,   295,   386,
     392,   359,   101,   341,   115,   339,   121,   120,   104,   119,
     116,   344,   131,   102,   132,   118,   122,   103,   105,   117,
     133,   338,   320,   323,   321,   322,   345,   333,   324,   337,
     329,   327,   340,   343,   328,   326,   331,   336,   325,   330,
     334,   335,   332,   342,     0,   319,     0,   106,     0,   359,
     359,   317,   627,   563,   449,   611,   611,     0,     0,     0,
       0,     0,     0,   260,     0,     0,     0,   187,   186,     0,
     292,     0,   292,   193,     0,     0,   533,     0,   532,     0,
     564,     0,     0,   539,   197,     0,   538,   195,   205,   203,
      64,    63,   225,   221,   549,   227,     0,     0,   257,   236,
     233,   481,   363,     0,     0,   449,   394,   408,   442,   472,
       0,   645,   497,   497,     0,   283,     0,     0,     0,   273,
       0,   212,   546,     0,     0,   210,   545,     0,   650,     0,
       0,     0,    72,     0,    72,    80,    83,   262,   286,   157,
     306,   285,   306,   293,   302,   300,     0,   372,   316,     0,
     346,     0,   312,   313,   567,     0,     0,     0,     0,     0,
       0,     0,     0,   262,   292,   306,   292,   306,   537,     0,
       0,   566,   543,     0,     0,     0,     0,   230,    56,   449,
     481,     0,   484,   483,   485,   590,   405,   368,   366,     0,
       0,     0,     0,   470,   590,     0,     0,   284,   282,     0,
     276,     0,   213,     0,   211,    89,    71,    70,     0,     0,
       0,     0,   259,   294,   463,     0,   398,     0,   318,   101,
     103,   359,   359,     0,     0,     0,     0,     0,     0,   306,
     175,   306,   171,     0,     0,     0,     0,    65,   229,   369,
     367,   449,   473,     0,   407,   406,   422,     0,   423,   410,
     413,     0,   409,   402,   403,   305,     0,   584,   585,   274,
       0,     0,    82,    85,    81,    84,     0,   462,   461,   590,
     399,   408,   360,   314,   315,     0,     0,     0,     0,     0,
       0,   177,   173,   536,   535,   542,   541,   371,   370,   475,
     476,   478,   590,     0,   645,   421,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   590,   586,   587,   548,   547,
       0,   453,     0,     0,     0,   477,   479,   412,   414,   415,
     418,   419,   420,   416,   417,   411,   458,   456,   590,   645,
     400,   311,   401,   473,   457,   590,   480
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   332,   108,   673,   110,   111,   112,   350,
     481,   113,   238,   114,   333,   664,   553,   668,   665,   115,
     116,   117,   550,   551,   118,   119,   181,   955,   270,   120,
     265,   121,   705,   275,   122,   123,   287,   124,   125,   126,
     446,   642,   442,   639,   127,   128,   803,   129,   236,   130,
     681,   682,   193,   132,   133,   134,   135,   136,   516,   722,
     885,   137,   138,   718,   880,   139,   140,   557,   904,   141,
     142,   761,   762,   194,   268,   696,   144,   145,   559,   910,
     767,   958,   959,   478,  1058,   488,   691,   692,   693,   694,
     695,   768,   359,   866,  1181,  1241,  1165,   436,  1097,  1101,
    1159,  1160,  1161,   146,   320,   521,   147,   148,   271,   272,
     492,   493,   709,  1178,  1126,   496,   706,  1200,  1094,  1015,
     334,   210,   338,   339,   437,   438,   439,   195,   150,   151,
     152,   153,   196,   155,   178,   179,   615,   458,   825,   616,
     617,   156,   157,   197,   198,   160,   222,   440,   200,   162,
     201,   202,   165,   166,   167,   168,   344,   169,   170,   171,
     172,   173
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1102
static const yytype_int16 yypact[] =
{
   -1102,   120,  3655, -1102,   -40,    98, -1102, -1102, -1102, -1102,
   -1102, -1102,  5039,    71,   196,   297, 14924,   421, 19859,    71,
   11583,   456,    21,   441,   196,  5039, 11583,  5039,   370, 19944,
   11754,  8489,   486,  8660,  9865,  9865,  7113,  8831,   506, -1102,
     403, -1102,   528, 19985, 19985, 19985, -1102,  1735, 10036,   533,
   11583,   367, -1102,   536,   544, 11583, -1102, 14924, -1102, 11583,
     595,   436,   385,  2407,   552, 20070, -1102, 10209,  7974, 11583,
   10036, 14924, 11583,   520,   570,   460,  5039,   580, 11583,   582,
   11925, 11925, 19985,   583, -1102, 14924, -1102,   586,  8831, 11583,
   -1102, 11583, -1102, 11583, -1102, -1102, 14448, 11583, -1102, 11583,
   -1102, -1102, -1102,  4001,  7286,  9004, 11583, -1102,  4866, -1102,
     488, -1102,   569, -1102, -1102,    32, -1102, -1102, -1102, -1102,
   -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102,
   -1102,   598, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102,
   19985, -1102, 19985,   256,    47, -1102, -1102,  1735, -1102,   496,
   -1102,   498, -1102, -1102,   499,   502,   513, 11583,   510,   512,
   19445, 14441,   112,   514,   521, -1102, -1102,   134, -1102, -1102,
   -1102, -1102, -1102,   463, -1102, -1102, 19445,   509,  5039, -1102,
   -1102,   522, 11583, -1102, -1102, 11583, 11583, 11583, 19985, -1102,
   11583, 10209, 10209,   613,   290, -1102, -1102, -1102, -1102,   -25,
     390, -1102, -1102,   508, 16567, 19985,  1735, -1102,   523, -1102,
     -33, 19445,   572,  8147,   607, 20111, 19445,   425,   609, -1102,
   20196, 19985,   425, 19985,   527,    42, 16167,    22,  3460,    22,
   16091,   382, -1102, 16250, 19985, 19985,    -1, 15377,   329,  8147,
   -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102,
   -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102,
   -1102, -1102, -1102, -1102,   529, -1102,   344,  5039,   530,  1373,
      96,    14, -1102,  5039, -1102, -1102, 16648,   184, -1102,   532,
     534, -1102, 16648,   -25,   184, -1102,  8147,  3295, -1102, -1102,
   10380, -1102, -1102, -1102, -1102,   -30, 19445, 11583, 11583, -1102,
   19445,   535, 17198, -1102, 16648,   -25, 19445,   537,  8147, -1102,
   19445, 17239, -1102, -1102, 17284,  2964, -1102, -1102, 17389,   585,
     540,   -25,    42, 16648, 17434,   434,   434,  1884,   184,   184,
      99, -1102, -1102,  4174,   -37, -1102, 11583, -1102,   117,   122,
   -1102,   125,   104, 17483,   121,  1884,   697, -1102,  4347, -1102,
     652, 11583, 11583, 19985,   575,   555, -1102, -1102, -1102, -1102,
     158,   398, -1102, 11583,   574, 11583, 11583, 11583,  9865,  9865,
   11583,   447, 11583, 11583, 11583, 11583, 11583,   287, 14448, 11583,
   11583, 11583, 11583, 11583, 11583, 11583, 11583, 11583, 11583, 11583,
   11583, 11583, 11583, 11583, 11583,   665, -1102, -1102, -1102, -1102,
   -1102,  9175,  9175, -1102, -1102, -1102, -1102,  9175, -1102, -1102,
    9175,  9175,  8147,  8147,  9865,  9865,  7803, -1102, -1102, 16724,
   16805, 17584,   563,   -13, 19985,  4520, -1102,  9865,    42,   573,
     249, -1102, 11583, -1102, 11583,   612, -1102,   566,   601, -1102,
   -1102, -1102, 19985, -1102,  1735, -1102, 19985,   584, -1102,  1735,
     699, 10209, -1102,  5212,  9865, -1102,   578, -1102,    42,  5385,
    9865, -1102,    42, -1102,    42,  9865, -1102,    42, 12096, 11583,
   -1102,   630,   631,  5039,   721,  5039, -1102,   725, 11583, -1102,
   -1102,   569,   589,  8147, 19985, -1102, -1102,   516, -1102, -1102,
    1373, -1102,   619,   593, -1102, 12267,   642, 11583,  1735, -1102,
   -1102, 11583, 11583, -1102,   599, -1102, 10209, -1102, 19445, 19445,
   -1102,    31, -1102,  8147,   602, -1102,   753, -1102,   753, -1102,
   12438,   632, -1102, -1102, -1102, -1102, -1102, -1102, -1102,  9348,
   -1102, 17633,  7459, -1102,  7632, -1102,  5039,   604,  9865,  9521,
    3828,   606, 11583, 10551, -1102, -1102,   267, -1102,  4693, 19985,
   -1102,   323, 17673,   330, 16333,   376, 10209,   617, 19818,   237,
   -1102, 17790, 19619, 19619,   451, -1102,   451, -1102,   451,  1518,
    2113,   960,  2032,   -25,   434, -1102,   605, -1102, -1102, -1102,
   -1102, -1102,  1884,   765,   451,  2177,  2177, 19619,  2177,  2177,
     505,   434,   765, 19659,   505,   184,   184,   434,  1884,   618,
     623,   625,   627,   628,   635,   624,   621, -1102,   451, -1102,
     451,   169, -1102, -1102, -1102,   129, -1102,  1411, 19562,   484,
   12609,  9865, 12780,  9865, 11583,  8147,  9865, 15196,   636,    71,
   17835, -1102, -1102, -1102, 19445, 17875,  8147, -1102,  8147, 19985,
     575,   371, 19985,   575, -1102,   374, 11583,   137,  8831, 19445,
      63, 16881,  7803, -1102,  8831, 19445,    39, 16409, -1102, -1102,
      22, 16491, -1102,   637,   657,   649, 17992,   657,   651, 11583,
   11583,   781,  5039,   783, 18032,  5039, 16963,   756, -1102,   166,
   -1102,   247, -1102,   207, -1102, -1102, -1102, -1102, -1102, -1102,
     692,   677,   653, -1102,  2835, -1102,   381,   658,  1373,    96,
     -14,    60, 11583, 11583,  6942, -1102, -1102,   538,  8318, -1102,
   19445, -1102, 18072, 18189, -1102, -1102, 19445,   661,    77,   660,
   -1102,  1521, -1102, -1102,   388, 19985, -1102, -1102, -1102, -1102,
   -1102, -1102, -1102,  5039,   138, 17039, -1102, -1102, 19445,  5039,
   19445, -1102, 18230, -1102, -1102, -1102, 11583, -1102,    87,   119,
   11583, -1102, 10722, 12096, 11583, -1102,  8147,   688,  1271,   664,
     711,   123, -1102,   748, -1102, -1102, -1102, -1102, 14282,   666,
   -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102,
    7803, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102,
   -1102, -1102, -1102,    43,  9865,  9865, 11583,   808, 18270, 11583,
     810, 18420,   261,   670, 18460,  8147,    42,    42, -1102, -1102,
   -1102, -1102,   575,   676, -1102,   575,   679, -1102, 16648, -1102,
   15453,  5558, -1102,  5731, -1102,   263, -1102, 15535,  5904, -1102,
      42,  6077, -1102,    42, -1102,    42, -1102,    42, 11583, -1102,
   11583, -1102, 19445, 19445,  5039, -1102,  5039, 11583, -1102,  5039,
     814, 19985,   687, 19985,   508, -1102, -1102, 19985,   811, -1102,
    1373,   708,   766, -1102, -1102, -1102,    95, -1102, -1102,   642,
     680,    18, -1102, -1102,   685,   689, -1102,  6250, 10209, -1102,
   -1102, -1102, 19985, -1102,   714,   508, -1102, -1102,  6423,   690,
    6596,   691, -1102, 11583, -1102, -1102, 11583, 18500,    66, 17122,
     698,   704,   279,   700,  1160, -1102, 11583, 19985, -1102, -1102,
     381,   701,   278, -1102,   722, -1102,   726,   728,   737,   731,
     733, -1102,   734,   744,   740,   741,   743,   378,   745,   746,
     747, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102,
   -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102,
   -1102, -1102, -1102, -1102, 11583, -1102,   757,   758,   751,   701,
     701, -1102, -1102, -1102,   642,   328,   337, 18617, 12951, 13122,
   18657, 13293, 13464, -1102, 13635, 13806,   352, -1102, -1102,   730,
   -1102,   732, -1102, -1102,  5039,  8831, 19445,  8831, 19445,  7803,
   -1102,  5039,  8831, 19445, -1102,  8831, 19445, -1102, -1102, -1102,
   18697, 19445, -1102, -1102, 19445,   826,  5039,   738, -1102, -1102,
   -1102,   708, -1102,   754, 10895,   350, -1102,   163, -1102, -1102,
    9865, 15060,  8147,  8147,  5039, -1102,    73,   761, 11583, -1102,
    8831, -1102, 19445,  5039,  8831, -1102, 19445,  5039, 19445,   338,
   11066, 12096, 11583, 12096, 11583, -1102, -1102,   739, -1102, -1102,
    3295, -1102,  3106, -1102, 19445, -1102,    81,   516, -1102, 18814,
   -1102, 15296, -1102, -1102, -1102, 11583, 11583, 11583, 11583, 11583,
   11583, 11583, 11583, -1102, -1102,  3032, -1102,  3171, 18032, 15611,
   15693, -1102, 18032, 15769, 15851, 11583,  5039, -1102, -1102,   350,
     708,  9694, -1102, -1102, -1102,   157, 10209, -1102, -1102,   146,
   11583,    34, 18854, -1102,   634,   762,   763,   572, -1102,   508,
   19445, 15927, -1102, 16009, -1102, -1102, -1102, 19445,   742,   764,
     772,   773, -1102, -1102, -1102, 13977,   816,   767, -1102,   790,
     803,   701,   701, 18971, 19011, 19051, 19168, 19208, 19248,  3418,
   -1102, 19586, -1102,  5039,  5039,  5039,  5039, 19445, -1102, -1102,
   -1102,   350, 11239,    25, -1102, 19445, -1102,   144, -1102,   153,
   -1102,   354, 19365, -1102, -1102, -1102, 13806,   782,   784, -1102,
    5039,  5039, -1102, -1102, -1102, -1102,  6769, -1102, -1102,   229,
   -1102,   163, -1102, -1102, -1102, 11583, 11583, 11583, 11583, 11583,
   11583, -1102, -1102, 18032, 18032, 18032, 18032, -1102, -1102, -1102,
   -1102, -1102,   273,  9865, 14618, -1102, 11583,   146,   144,   144,
     144,   144,   144,   144,   146,   702, -1102, -1102, 18032, 18032,
     775, 14148,    59,    92, 19405, -1102, -1102, 19445, -1102, -1102,
   -1102, -1102, -1102, -1102, -1102,   801, -1102, -1102,   243, 14788,
   -1102, -1102, -1102, 11412, -1102,   324, -1102
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1102, -1102, -1102,     5,  -475,  2590, -1102, -1102, -1102,   469,
   -1102, -1102, -1102,   485,   603,  -463, -1102,  -727,  -725, -1102,
   -1102, -1102,   208, -1102, -1102,   255,   929, -1102,  1782,  -200,
    -752, -1102,  -964,  2738, -1064,  -869, -1102,   -58, -1102, -1102,
   -1102, -1102, -1102, -1102, -1102, -1102,   218, -1102,   867, -1102,
   -1102,   105,  1269, -1102, -1102, -1102, -1102, -1102,   645, -1102,
      82, -1102, -1102, -1102, -1102, -1102, -1102,  -594,  -639, -1102,
   -1102, -1102,    58,  1399, -1102, -1102, -1102,   342, -1102, -1102,
   -1102, -1102,   -95,  -152,  -897, -1102, -1102,   -90,   110,   280,
   -1102, -1102, -1102,    61, -1102, -1102,  -254,    13,  -979,  -207,
    -238,  -229,  -483, -1102,  -190, -1102,    11,   945,  -118,   487,
   -1102,  -473,  -839,  -963, -1102,  -657,  -517, -1101, -1069,  -936,
     -65, -1102,    97, -1102,  -219,  -457,  -443,   912,  -394, -1102,
   -1102, -1102,  1628, -1102,    -8, -1102, -1102,  -177, -1102,  -642,
   -1102, -1102, -1102,  1855,  1907,   -12,   952,    44,  1040, -1102,
    2131,  2497, -1102, -1102, -1102, -1102, -1102, -1102, -1102, -1102,
   -1102,  -415
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -646
static const yytype_int16 yytable[] =
{
     176,   619,   295,   723,   199,   292,   667,   107,   204,   411,
     826,   203,   690,   435,   211,   429,   957,   697,   216,   216,
     482,   226,   228,   230,   233,   237,   679,   901,   900,   360,
    1018,   447,   717,   206,   626,  1053,   276,  1164,   277,   435,
     342,  -255,   869,   282,   494,   283,   813,   284,    23,   816,
    1092,   462,   464,   467,   273,   296,   300,   302,   304,   305,
     306,   497,  1062,  1063,   310,  1020,   311,   504,   314,   318,
     719,   828,  1203,   321,   273,  1089,   323,   324,   351,   325,
     294,   326,  1124,  1201,   327,   328,   435,   329,   430,   514,
     575,   296,   300,   343,   345,   821,   494,   494,    58,   473,
     356,   707,  1041,  1225,  -280,   273,   626,   529,   435,    66,
    1149,   432,   301,   371,   506,  -256,   294,   337,   294,   357,
       3,   377,   575,   174,    68,  1064,   724,   423,   205,   358,
     433,   539,   530,   507,    86,  1226,   964,  1042,   963,   576,
     456,  -459,  1246,  -464,   352,   364,  -280,  1156,   340,  1156,
     877,   538,   577,  -238,  1151,  -464,   213,   456,   498,  1164,
     627,  1177,   539,   411,   537,   541,  -464,   425,   802,   539,
     176,  -459,  1197,   419,   420,   421,  1098,   581,   310,   296,
     343,   578,    68,  1123,  -459,  -464,   495,  -464,   579,   177,
     425,  1021,   878,   605,   606,   811,   428,  -459,  1204,   180,
    -280,   300,   425,   539,   294,   294,  1053,   425,  1053,  1017,
     425,  -464,   435,   435,  -464,   367,   778,   506,   979,  -464,
     579,   981,   456,  1099,  1125,   868,   580,   300,   444,   505,
     239,   425,  1239,   449,  1183,  1184,  1157,   619,   495,   495,
    -464,   425,   206,   456,  -459,  1158,   879,  1158,   539,  -459,
    1150,   225,   227,   229,   855,  1240,   456,   779,  1237,  -464,
     425,   532,   174,  -464,   763,   543,   534,   907,   526,  1206,
    1053,   368,  1053,   780,   300,   369,  1244,   540,   216,  1100,
    -482,   506,   412,   435,   413,   508,   509,   896,   533,  -460,
     575,   667,   908,   535,   544,   856,   300,  1207,   536,   902,
     781,  -482,   498,  -452,  -619,  -482,  -619,   576,   819,   957,
     638,   888,  1198,   435,   764,  1119,  1118,  1121,  1120,  -460,
     577,   560,   371,   341,   531,   765,   641,   375,  -482,   376,
     377,   645,  -460,  -452,   380,   313,   317,   852,   528,   552,
     554,  1075,   387,  1077,   766,  -460,  -452,  1081,   976,   578,
     393,   561,  -239,   562,   563,   564,   566,   568,   569,  -452,
     570,   571,   572,   573,   574,   619,   582,   583,   584,   585,
     586,   587,   588,   589,   590,   591,   592,   593,   594,   595,
     596,   597,   598,   690,  -454,   278,   647,  1013,   579,   300,
     300,   853,  -460,   498,   580,   300,  -474,  -460,   300,   300,
     300,   300,   608,   610,   618,   638,  -452,   989,   862,    23,
     479,  -452,   633,   480,  -454,   630,  -107,  -474,   854,  -555,
     634,  -474,   635,   638,  -113,   435,  -618,  -454,  -618,  1052,
     174,   -78,   973,  1039,   990,  1139,   743,  1141,   435,   296,
    -454,   649,   651,   279,  -474,   600,   601,   655,   657,   -49,
    1047,   602,    23,   661,   603,   604,   666,   666,   863,    58,
    -618,   280,  -618,   674,   294,   367,   676,   746,   -49,   864,
      66,   300,  -569,   708,   750,    41,  -245,  1208,   281,   830,
     833,  -568,   838,   835,   837,   710,   747,  -454,   865,   712,
     713,   757,  -454,   751,   716,    86,   638,   -78,  1096,  -569,
      56,   300,    58,  1209,  1103,  1210,  1211,  1115,  -568,  1212,
    1213,  -244,   425,    66,   484,   498,  -109,   716,   498,   715,
     300,   368,   300,  1073,  -114,   369,   735,   716,   738,   207,
     740,   742,  -430,   274,   814,   886,   367,   817,    86,   755,
     213,  -253,   715,   683,   296,   337,   756,   337,   895,   581,
     414,  -430,   715,   528,   415,   274,   435,   889,   891,   443,
    -615,  -243,  -615,   684,   448,   312,   316,   685,  -617,   294,
    -617,   795,   371,   239,   619,   415,   340,   375,   340,   667,
     377,   667,   690,  -240,   380,  -465,   565,   567,  -247,   371,
     686,  -251,   368,   687,   375,    60,   369,   377,  -465,  -248,
    1052,   380,  1052,   285,   688,   435,   286,  -242,   649,   798,
     655,   801,   674,   300,   804,   738,   960,  -616,   307,  -616,
    1095,   806,   807,   689,   300,  -246,   300,  1104,  -465,   729,
     308,   731,   607,   609,   818,  -252,   820,  -254,  -241,   871,
     618,  -249,   827,   371,   372,   629,   373,   374,   375,   809,
     376,   377,   349,   353,  -465,   380,   348,   842,   843,  -552,
     361,  -465,  -551,   387,  1052,   362,  1052,   363,   424,   391,
     392,   393,   650,  -556,  1205,  -557,   425,  -554,   656,   416,
     810,  -469,  -465,   660,  -553,   418,   431,  -364,   426,   434,
     306,   310,   343,   441,  -469,   445,   300,   451,   510,   483,
     487,  -465,   501,   522,   502,   545,  -465,   513,   520,  -465,
     274,   274,   274,   274,   274,   274,   549,   294,   556,   683,
     377,   474,   477,   558,  -469,  1229,  1230,  1231,  1232,  1233,
    1234,  1179,   599,   625,   552,   636,   632,   637,   897,   684,
     899,   666,   666,   685,   300,   638,   646,   644,   652,  -471,
    -469,   485,   873,   669,   670,   672,   734,  -469,  1202,   675,
     678,   274,  -471,   698,   699,   708,   686,   274,   618,   687,
     714,   346,  1215,   720,   721,   769,   725,   733,  -469,   739,
     688,   770,   566,   608,   967,   758,   771,   970,   772,   274,
     773,   774,  -471,   300,   777,   776,   367,  -469,   775,   689,
     -74,   838,  -469,  1105,  1106,  -469,   805,   274,   274,   986,
    1202,   988,   839,  1026,   841,   844,   993,   846,  -471,   996,
     851,   858,   435,   435,   859,  -471,  1000,  1238,  1001,   867,
     876,   881,   506,   905,   906,  1004,   909,   962,   683,   797,
     968,   800,   971,   974,   980,  1245,  -471,   982,  1006,  1202,
    1008,  1014,   368,  1019,  1016,  1022,   369,  1028,   684,  1023,
    1086,  1045,   685,  1033,  1037,  -471,   296,  1046,  -134,  1048,
    -471,  1057,  -141,  -471,  -140,  -110,  1032,  -139,  1036,  -136,
    -143,  1038,  -108,  -111,   666,   686,  -144,  -138,   687,  -142,
      60,   294,  -137,  -145,  1054,  1060,  -112,  1061,  1074,   688,
    1076,  1088,  1122,   371,   372,  1172,   373,   374,   375,  1132,
     376,   377,   378,   274,   149,   380,   381,   382,   689,  1180,
     384,   385,   386,   387,   149,  1090,   390,  1173,  -107,   391,
     392,   393,  1109,  1167,  1168,  1174,  1175,   149,  1182,   149,
     394,  -109,  1059,   274,  1216,  1207,  1217,   274,  1236,   274,
     677,   548,   274,   208,   894,   322,   986,   988,  1009,   993,
     996,   518,  1032,  1036,  1027,  1055,  1131,  1127,  1012,  1242,
     857,  1056,  1078,  1079,  1223,  1080,  1235,   618,  1228,  1082,
    1083,   232,   217,  1084,     0,   711,     0,     0,   149,     0,
       0,   367,   292,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1102,   738,
     300,   300,   565,   607,     0,   149,  1110,     0,  1111,     0,
     149,     0,  1113,     0,     0,     0,  1153,  1093,  1117,   666,
     666,   666,   666,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   161,     0,     0,     0,     0,   368,     0,     0,
       0,   369,   161,  1078,  1133,  1134,  1082,  1135,  1136,  1137,
    1138,     0,     0,     0,     0,   161,     0,   161,     0,     0,
       0,     0,     0,  1147,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,  1155,     0,     0,     0,  1162,     0,
     149,     0,     0,     0,     0,     0,     0,     0,   371,   372,
       0,     0,   374,   375,   294,   376,   377,     0,     0,  1154,
     380,  1222,     0,     0,     0,     0,   161,     0,   387,     0,
     315,   315,     0,     0,   391,   392,   393,     0,     0,     0,
       0,  1193,  1194,  1195,  1196,   274,   274,     0,     0,     0,
       0,   274,   274,   161,     0,   274,   274,     0,   161,     0,
       0,     0,     0,     0,  1036,     0,     0,     0,  1218,  1219,
       0,   850,     0,     0,   343,  1199,     0,     0,     0,     0,
       0,     0,     0,  1193,  1194,  1195,  1196,  1218,  1219,   149,
       0,     0,   288,     0,     0,   149,    22,    23,     0,   294,
       0,  1224,   738,     0,  1227,   289,     0,    30,   290,     0,
       0,     0,     0,    36,     0,     0,   883,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,  1199,   161,     0,
       0,     0,     0,     0,     0,     0,     0,   738,     0,     0,
       0,  1036,     0,     0,     0,    56,     0,    58,     0,    60,
       0,  1049,     0,     0,  1050,   149,   291,     0,    66,     0,
       0,     0,     0,     0,     0,     0,  1199,     0,     0,     0,
     149,     0,     0,     0,     0,     0,     0,    82,     0,     0,
      84,   131,   903,    86,     0,     0,     0,     0,     0,     0,
       0,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,   274,  -292,   131,     0,   131,  -292,  -292,     0,
       0,     0,     0,   274,     0,   477,  -292,   161,  -292,  -292,
       0,     0,   477,   161,  -292,   274,     0,     0,   274,     0,
     274,  -292,   274,   101,  -292,     0,     0,     0,     0,  1051,
       0,     0,     0,     0,     0,     0,     0,   149,     0,  1010,
       0,     0,     0,  -292,     0,   131,  -292,     0,  -292,     0,
    -292,     0,  -292,  -292,     0,  -292,     0,  -292,     0,  -292,
       0,     0,     0,     0,     0,   149,     0,     0,     0,     0,
    1029,   149,   131,   161,     0,     0,   240,   131,  -292,     0,
       0,  -292,     0,     0,  -292,   149,     0,   149,   161,     0,
     241,   242,     0,   243,     0,     0,     0,     0,   244,     0,
       0,   143,     0,     0,     0,     0,   245,     0,     0,     0,
       0,   143,   246,     0,   575,     0,     0,     0,   247,     0,
       0,     0,   248,     0,   143,   249,   143,     0,   782,     0,
       0,   783,     0,     0,  -292,   250,   784,     0,     0,     0,
    -292,   251,   252,     0,     0,     0,   184,   131,   149,   253,
       0,     0,   149,     0,     0,     0,   785,     0,   254,     0,
     149,     0,     0,   786,     0,   161,     0,   255,   256,     0,
     257,     0,   258,   787,   259,   143,     0,   260,     0,     0,
     788,   261,   489,     0,   262,     0,     0,   263,     0,     0,
       0,     0,     0,   161,     0,     0,   789,     0,     0,   161,
       0,     0,   143,     0,     0,     0,     0,   143,   790,     0,
       0,   274,   579,   161,     0,   161,     0,     0,     0,   791,
       0,     0,     0,     0,   240,   792,     0,     0,     0,   793,
       0,     0,     0,     0,     0,     0,   131,     0,   241,   242,
       0,   243,   131,   490,     0,     0,   244,     0,     0,   367,
       0,     0,     0,     0,   245,     0,     0,     0,     0,     0,
     246,     0,     0,     0,   477,   477,   247,     0,   477,   477,
     248,     0,     0,   249,     0,     0,   161,   143,     0,     0,
     161,     0,     0,   250,   149,     0,  1163,   149,   161,   251,
     252,     0,     0,     0,  1169,     0,   477,   253,   477,     0,
       0,     0,   131,     0,     0,   368,   254,     0,     0,   369,
       0,     0,     0,     0,     0,   255,   256,   131,   257,     0,
     258,     0,   259,     0,     0,   260,     0,     0,     0,   261,
     154,     0,   262,     0,     0,   263,     0,     0,     0,     0,
     154,     0,     0,     0,     0,   149,     0,     0,     0,     0,
       0,   149,     0,   154,     0,   154,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,   143,   379,   380,   381,
     382,     0,   143,   384,   385,   386,   387,   388,     0,   390,
     961,     0,   391,   392,   393,     0,     0,     0,     0,   425,
       0,   882,     0,   394,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   154,     0,   965,   966,  1163,     0,
       0,     0,   161,     0,     0,   161,     0,     0,     0,     0,
       0,     0,   131,     0,     0,     0,     0,     0,   131,     0,
       0,   154,   143,   149,     0,   149,   154,     0,   240,     0,
     149,     0,   131,   149,   131,     0,     0,   143,     0,     0,
       0,     0,   241,   242,     0,   243,   149,     0,   149,     0,
     244,   149,     0,     0,     0,     0,     0,     0,   245,     0,
       0,     0,     0,   161,   246,     0,     0,     0,     0,   161,
     247,     0,     0,     0,   248,     0,     0,   249,     0,   149,
       0,     0,     0,     0,     0,     0,     0,   250,     0,     0,
     149,     0,   149,   251,   252,   131,   154,     0,     0,   131,
       0,   253,     0,     0,     0,     0,     0,   131,     0,     0,
     254,     0,     0,     0,   143,   264,   266,   267,     0,   255,
     256,     0,   257,     0,   258,     0,   259,     0,     0,   260,
       0,     0,     0,   261,     0,     0,   262,     0,     0,   263,
       0,     0,   143,     0,     0,     0,     0,   158,   143,     0,
       0,   161,     0,   161,   319,     0,     0,   158,   161,     0,
       0,   161,   143,     0,   143,     0,     0,     0,     0,     0,
     158,     0,   158,     0,   161,     0,   161,     0,     0,   161,
       0,     0,     0,     0,     0,   154,   149,     0,     0,     0,
       0,   154,     0,   149,     0,   269,     0,     0,     0,   159,
       0,     0,     0,     0,     0,   367,     0,   161,   149,   159,
       0,     0,   354,     0,   355,     0,     0,     0,   161,     0,
     161,   158,   159,     0,   159,   143,   149,     0,     0,   143,
       0,   131,     0,     0,   131,   149,     0,   143,     0,   149,
       0,     0,     0,     0,     0,     0,     0,     0,   158,     0,
       0,   154,     0,   158,     0,     0,     0,     0,     0,     0,
     422,   368,     0,     0,     0,   369,   154,     0,     0,     0,
       0,     0,     0,   159,     0,     0,     0,   319,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   149,     0,
       0,     0,   131,   319,     0,   450,     0,     0,   131,     0,
     159,     0,     0,     0,     0,   159,   471,   472,     0,     0,
       0,     0,   371,   372,   161,   373,   374,   375,     0,   376,
     377,   161,     0,   158,   380,     0,     0,     0,     0,     0,
       0,   386,   387,     0,     0,   390,   161,     0,   391,   392,
     393,   491,     0,   154,     0,   149,   149,   149,   149,     0,
       0,     0,     0,   367,   161,     0,     0,     0,     0,     0,
       0,   143,     0,   161,   143,     0,     0,   161,     0,     0,
       0,   154,   149,   149,     0,   159,     0,   154,     0,     0,
     131,     0,   131,     0,     0,     0,     0,   131,     0,     0,
     131,   154,     0,   154,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,     0,   131,     0,     0,   131,   368,
       0,     0,   158,   369,     0,     0,   161,     0,   158,     0,
       0,     0,   143,   163,     0,   555,     0,     0,   143,     0,
       0,     0,     0,   163,   367,     0,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   163,   131,   163,   131,
       0,     0,     0,     0,   154,     0,     0,     0,   154,     0,
     371,   372,     0,     0,   159,   375,   154,   376,   377,     0,
     159,     0,   380,   161,   161,   161,   161,     0,   158,     0,
     387,     0,     0,     0,     0,     0,   391,   392,   393,     0,
     368,     0,     0,   158,   369,     0,   628,   163,   367,     0,
     161,   161,     0,     0,     0,     0,     0,     0,     0,     0,
     143,     0,   143,     0,   640,     0,     0,   143,   643,     0,
     143,     0,     0,     0,   163,     0,     0,     0,     0,   163,
     159,     0,     0,   143,     0,   143,     0,     0,   143,     0,
       0,   371,     0,   131,     0,   159,   375,     0,   376,   377,
     131,     0,     0,   380,   368,     0,   680,     0,   369,     0,
       0,   387,   491,     0,     0,   131,   143,   391,   392,   393,
     158,     0,     0,     0,     0,     0,     0,   143,     0,   143,
       0,     0,     0,   131,     0,     0,     0,     0,     0,     0,
     154,     0,   131,   154,     0,     0,   131,     0,   158,   163,
       0,     0,     0,     0,   158,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,     0,   380,   158,     0,
     158,   745,   159,     0,   386,   387,     0,     0,   390,     0,
     760,   391,   392,   393,     0,     0,     0,     0,     0,     0,
       0,     0,   394,     0,     0,   131,     0,     0,     0,     0,
     159,   154,     0,     0,     0,     0,   159,   154,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     159,     0,   159,   143,     0,     0,     0,     0,     0,     0,
     143,   158,     0,     0,     0,   158,     0,     0,   163,     0,
       0,     0,     0,   158,   163,   143,     0,     0,     0,     0,
       0,     0,   131,   131,   131,   131,     0,     0,     0,     0,
       0,   812,     0,   143,   815,     0,     0,     0,     0,   288,
       0,     0,   143,    22,    23,     0,   143,     0,     0,   131,
     131,     0,   289,   159,    30,   290,     0,   159,     0,   154,
      36,   154,     0,   -77,     0,   159,   154,    41,     0,   154,
       0,     0,     0,     0,   163,     0,     0,     0,     0,     0,
       0,   -48,   154,     0,   154,     0,   861,   154,     0,   163,
     491,     0,    56,     0,    58,   143,     0,     0,     0,     0,
     -48,     0,     0,   291,     0,    66,     0,     0,     0,   164,
       0,     0,     0,   884,     0,   154,     0,   319,     0,   164,
       0,     0,     0,     0,    82,     0,   154,    84,   154,   -77,
      86,     0,   164,     0,   164,     0,     0,   158,     0,     0,
     158,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   143,   143,   143,   143,     0,     0,     0,     0,
     956,     0,     0,     0,     0,     0,   163,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   143,
     143,     0,     0,   164,     0,     0,     0,     0,     0,   159,
       0,     0,   159,     0,   163,     0,     0,     0,   158,     0,
     163,     0,   109,     0,   158,     0,     0,     0,     0,     0,
     164,     0,   175,     0,   163,   164,   163,     0,     0,     0,
       0,     0,   154,     0,     0,   209,     0,   212,     0,   154,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1007,   154,   680,     0,     0,     0,  1011,
     159,     0,   491,     0,     0,     0,   159,     0,     0,     0,
       0,     0,   154,     0,     0,     0,     0,     0,     0,     0,
       0,   154,     0,     0,   884,   154,   309,   163,     0,     0,
       0,   163,     0,     0,     0,   164,   158,     0,   158,   163,
       0,     0,     0,   158,     0,     0,   158,     0,     0,   760,
       0,     0,     0,   109,     0,     0,     0,     0,   347,   158,
       0,   158,     0,     0,   158,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   154,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   159,     0,
     159,     0,   158,     0,     0,   159,     0,     0,   159,     0,
       0,     0,     0,   158,     0,   158,     0,     0,     0,     0,
       0,   159,     0,   159,     0,     0,   159,     0,     0,     0,
       0,     0,     0,     0,   164,     0,     0,     0,   417,     0,
     164,   154,   154,   154,   154,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   159,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   159,     0,   159,   154,   154,
       0,     0,     0,   163,     0,     0,   163,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     164,     0,     0,     0,     0,     0,     0,     0,   240,   158,
       0,     0,     0,   956,     0,   164,   158,     0,     0,     0,
       0,     0,   241,   242,     0,   243,     0,   486,     0,     0,
     244,   158,     0,   499,   163,     0,     0,     0,   245,     0,
     163,     0,     0,     0,   246,     0,     0,     0,     0,   158,
     247,     0,     0,     0,   248,     0,     0,   249,   158,     0,
       0,   159,   158,     0,     0,     0,     0,   250,   159,     0,
       0,     0,     0,   251,   252,     0,     0,     0,     0,     0,
       0,   253,     0,   159,     0,     0,     0,     0,     0,     0,
     254,     0,   164,   109,     0,     0,     0,     0,     0,   255,
     256,   159,   257,     0,   258,     0,   259,     0,   109,   260,
     159,   158,     0,   261,   159,     0,   262,     0,     0,   263,
     164,     0,   163,     0,   163,     0,   164,     0,     0,   163,
       0,     0,   163,   452,   455,   457,   461,   463,   466,     0,
     164,     0,   164,     0,     0,   163,     0,   163,     0,     0,
     163,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   159,     0,     0,     0,     0,   158,   158,
     158,   158,     0,     0,     0,   860,     0,     0,   163,     0,
       0,     0,     0,     0,   500,   109,     0,     0,     0,   163,
     503,   163,     0,     0,     0,   158,   158,     0,     0,     0,
       0,     0,     0,   164,     0,     0,     0,   164,     0,     0,
       0,     0,   512,   499,     0,   164,     0,     0,     0,   499,
     159,   159,   159,   159,   288,     0,     0,     0,    22,    23,
     523,   524,     0,   671,     0,     0,     0,   289,     0,    30,
     290,     0,     0,     0,     0,    36,     0,   159,   159,     0,
       0,     0,    41,     0,     0,     0,     0,   395,   396,   397,
     398,   399,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   409,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,  1049,     0,   163,  1050,     0,   291,     0,
      66,     0,   163,     0,     0,     0,   732,     0,   288,     0,
     737,   410,    22,    23,  -615,     0,  -615,   163,   109,    82,
       0,   289,    84,    30,   290,    86,     0,     0,     0,    36,
       0,     0,     0,     0,     0,   163,    41,     0,     0,     0,
       0,     0,     0,     0,   163,     0,   631,     0,   163,   164,
       0,     0,   164,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,   346,     0,  1049,     0,     0,
    1050,     0,   291,   288,    66,   101,   653,    22,    23,     0,
     658,  1140,   659,     0,     0,   662,   289,     0,    30,   290,
       0,     0,     0,    82,    36,     0,    84,   163,     0,    86,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
     164,     0,     0,     0,     0,     0,   164,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,  1049,     0,     0,  1050,     0,   291,     0,    66,
       0,     0,   845,     0,     0,   848,     0,     0,     0,   101,
       0,     0,     0,     0,   163,   163,   163,   163,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   163,   163,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   288,   164,     0,
     164,    22,    23,   887,     0,   164,     0,     0,   164,   892,
     289,     0,    30,   290,   101,     0,     0,     0,    36,     0,
    1142,   164,     0,   164,     0,    41,   164,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,   164,     0,     0,     0,     0,     0,
       0,   291,     0,    66,     0,   164,     0,   164,   822,   824,
       0,     0,     0,     0,   829,   832,     0,     0,   834,   836,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,   499,     0,   499,     0,     0,     0,     0,   499,     0,
       0,   499,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1002,     0,  1003,     0,     0,  1005,
     288,     0,     0,     0,    22,    23,     0,     0,     0,     0,
       0,     0,     0,   289,     0,    30,   290,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,  1025,    41,     0,
       0,     0,   365,     0,     0,     0,     0,     0,  1031,   366,
    1035,   164,     0,     0,     0,     0,     0,     0,   164,     0,
       0,   367,   459,    56,     0,    58,     0,    60,     0,  1049,
       0,     0,  1050,   164,   291,     0,    66,   460,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   164,     0,     0,     0,    82,     0,     0,    84,     0,
     164,    86,     0,     0,   164,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   977,   978,     0,   368,     0,     0,
       0,   369,     0,     0,     0,     0,   983,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   994,     0,
       0,   997,     0,   998,     0,   999,     0,     0,   456,     0,
       0,   101,   370,   164,     0,     0,     0,  1191,     0,     0,
       0,     0,     0,     0,     0,     0,  1087,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,  1107,   384,   385,   386,   387,   388,
     389,   390,     0,  1112,   391,   392,   393,  1114,   425,     0,
       0,     0,     0,     0,     0,   394,     0,     0,     0,     0,
     164,   164,   164,   164,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    -2,     4,     0,     5,     0,
       6,     7,     8,     9,    10,    11,     0,   164,   164,    12,
      13,    14,    15,    16,     0,    17,  1148,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,   -76,     0,    39,    40,    41,     0,    42,  -306,     0,
      43,    44,    45,    46,    47,     0,    48,    49,    50,   -47,
      51,    52,     0,    53,    54,    55,     0,  -306,     0,     0,
      56,    57,    58,    59,    60,    61,    62,  -306,   -47,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,    76,     0,    77,    78,     0,    79,
      80,    81,    82,    83,  1108,    84,    85,   -76,    86,    87,
       0,     0,    88,     0,    89,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,   101,     0,
       0,   102,     0,   103,     0,   104,     0,   105,     0,     4,
     106,     5,     0,     6,     7,     8,     9,    10,    11,     0,
    -645,     0,    12,    13,    14,    15,    16,  -645,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,  -645,
      27,    28,  -645,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,   -76,     0,    39,    40,    41,     0,
      42,  -306,     0,    43,    44,    45,    46,    47,     0,    48,
      49,    50,   -47,    51,    52,     0,    53,    54,    55,     0,
    -306,     0,     0,    56,    57,    58,    59,     0,    61,    62,
    -306,   -47,    63,    64,    65,  -645,    66,    67,    68,  -645,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,    80,    81,    82,    83,     0,    84,    85,
     -76,    86,    87,     0,     0,    88,     0,    89,     0,     0,
    -645,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -645,  -645,    93,  -645,
    -645,  -645,  -645,  -645,  -645,  -645,     0,  -645,  -645,  -645,
    -645,  -645,     0,  -645,  -645,  -645,  -645,  -645,  -645,  -645,
    -645,   101,  -645,  -645,  -645,     0,   103,  -645,   104,     0,
     105,     0,   330,  -645,     5,   293,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,    38,   -76,     0,    39,
      40,    41,     0,    42,  -306,     0,    43,    44,    45,    46,
      47,     0,    48,    49,    50,   -47,    51,    52,     0,    53,
      54,    55,     0,  -306,     0,     0,    56,    57,    58,    59,
      60,    61,    62,  -306,   -47,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
      76,     0,    77,    78,     0,    79,    80,    81,    82,    83,
       0,    84,    85,   -76,    86,    87,     0,     0,    88,     0,
      89,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,   101,     0,     0,   102,     0,   103,
     331,   104,     0,   105,     0,     4,   106,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
     -76,     0,    39,    40,    41,     0,    42,  -306,     0,    43,
      44,    45,    46,    47,     0,    48,    49,    50,   -47,    51,
      52,     0,    53,    54,    55,     0,  -306,     0,     0,    56,
      57,    58,    59,    60,    61,    62,  -306,   -47,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,    76,     0,    77,    78,     0,    79,    80,
      81,    82,    83,     0,    84,    85,   -76,    86,    87,     0,
       0,    88,     0,    89,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,   101,     0,     0,
     102,     0,   103,   527,   104,     0,   105,     0,   546,   106,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,    38,   -76,     0,    39,    40,    41,     0,    42,
    -306,     0,    43,    44,    45,    46,    47,     0,    48,    49,
      50,   -47,    51,    52,     0,    53,    54,    55,     0,  -306,
       0,     0,    56,    57,    58,    59,    60,    61,    62,  -306,
     -47,    63,    64,    65,     0,    66,    67,    68,     0,    69,
      70,    71,    72,    73,    74,    75,    76,     0,    77,    78,
       0,    79,    80,    81,    82,    83,     0,    84,    85,   -76,
      86,    87,     0,     0,    88,     0,    89,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
     101,     0,     0,   102,     0,   103,   547,   104,     0,   105,
       0,   330,   106,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,    38,   -76,     0,    39,    40,
      41,     0,    42,  -306,     0,    43,    44,    45,    46,    47,
       0,    48,    49,    50,   -47,    51,    52,     0,    53,    54,
      55,     0,  -306,     0,     0,    56,    57,    58,    59,    60,
      61,    62,  -306,   -47,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,    77,    78,     0,    79,    80,    81,    82,    83,     0,
      84,    85,   -76,    86,    87,     0,     0,    88,     0,    89,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,   101,     0,     0,   102,     0,   103,   331,
     104,     0,   105,     0,     4,   106,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,   -76,
       0,    39,    40,    41,     0,    42,  -306,     0,    43,    44,
      45,    46,    47,     0,    48,    49,    50,   -47,    51,    52,
       0,    53,    54,    55,     0,  -306,     0,     0,    56,    57,
      58,    59,    60,    61,    62,  -306,   -47,    63,    64,    65,
       0,    66,    67,    68,     0,    69,    70,    71,    72,    73,
      74,    75,    76,     0,    77,    78,     0,    79,    80,    81,
      82,    83,     0,    84,    85,   -76,    86,    87,     0,     0,
      88,     0,    89,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,   101,     0,     0,   102,
       0,   103,   744,   104,     0,   105,     0,     4,   106,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,    38,   -76,     0,    39,    40,    41,     0,    42,  -306,
       0,    43,    44,    45,    46,    47,     0,    48,    49,    50,
     -47,    51,    52,     0,    53,    54,    55,     0,  -306,     0,
       0,    56,    57,    58,    59,   346,    61,    62,  -306,   -47,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,     0,    84,    85,   -76,    86,
      87,     0,     0,    88,     0,    89,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,   101,
       0,     0,   102,     0,   103,     0,   104,     0,   105,     0,
       4,   106,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,   -76,     0,    39,    40,    41,
       0,    42,  -306,     0,    43,    44,    45,    46,    47,     0,
      48,    49,    50,   -47,    51,    52,     0,    53,    54,    55,
       0,  -306,     0,     0,    56,    57,    58,    59,     0,    61,
      62,  -306,   -47,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,    76,     0,
      77,    78,     0,    79,    80,    81,    82,    83,     0,    84,
      85,   -76,    86,    87,     0,     0,    88,     0,    89,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,   101,     0,     0,   102,     0,   103,     0,   104,
       0,   105,     0,     4,   106,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,   648,    38,   -76,     0,
      39,    40,    41,     0,    42,  -306,     0,    43,    44,    45,
      46,    47,     0,    48,    49,    50,   -47,    51,    52,     0,
      53,    54,    55,     0,  -306,     0,     0,    56,    57,    58,
      59,     0,    61,    62,  -306,   -47,    63,    64,    65,     0,
      66,    67,    68,     0,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,     0,    84,    85,   -76,    86,    87,     0,     0,    88,
       0,    89,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,   101,     0,     0,   102,     0,
     103,     0,   104,     0,   105,     0,     4,   106,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,   654,
      38,   -76,     0,    39,    40,    41,     0,    42,  -306,     0,
      43,    44,    45,    46,    47,     0,    48,    49,    50,   -47,
      51,    52,     0,    53,    54,    55,     0,  -306,     0,     0,
      56,    57,    58,    59,     0,    61,    62,  -306,   -47,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,    76,     0,    77,    78,     0,    79,
      80,    81,    82,    83,     0,    84,    85,   -76,    86,    87,
       0,     0,    88,     0,    89,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,   101,     0,
       0,   102,     0,   103,     0,   104,     0,   105,     0,     4,
     106,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,   985,    38,   -76,     0,    39,    40,    41,     0,
      42,  -306,     0,    43,    44,    45,    46,    47,     0,    48,
      49,    50,   -47,    51,    52,     0,    53,    54,    55,     0,
    -306,     0,     0,    56,    57,    58,    59,     0,    61,    62,
    -306,   -47,    63,    64,    65,     0,    66,    67,    68,     0,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,    80,    81,    82,    83,     0,    84,    85,
     -76,    86,    87,     0,     0,    88,     0,    89,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,   101,     0,     0,   102,     0,   103,     0,   104,     0,
     105,     0,     4,   106,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,   987,    38,   -76,     0,    39,
      40,    41,     0,    42,  -306,     0,    43,    44,    45,    46,
      47,     0,    48,    49,    50,   -47,    51,    52,     0,    53,
      54,    55,     0,  -306,     0,     0,    56,    57,    58,    59,
       0,    61,    62,  -306,   -47,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
      76,     0,    77,    78,     0,    79,    80,    81,    82,    83,
       0,    84,    85,   -76,    86,    87,     0,     0,    88,     0,
      89,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,   101,     0,     0,   102,     0,   103,
       0,   104,     0,   105,     0,     4,   106,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,   992,    38,
     -76,     0,    39,    40,    41,     0,    42,  -306,     0,    43,
      44,    45,    46,    47,     0,    48,    49,    50,   -47,    51,
      52,     0,    53,    54,    55,     0,  -306,     0,     0,    56,
      57,    58,    59,     0,    61,    62,  -306,   -47,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,    76,     0,    77,    78,     0,    79,    80,
      81,    82,    83,     0,    84,    85,   -76,    86,    87,     0,
       0,    88,     0,    89,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,   101,     0,     0,
     102,     0,   103,     0,   104,     0,   105,     0,     4,   106,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,   995,    38,   -76,     0,    39,    40,    41,     0,    42,
    -306,     0,    43,    44,    45,    46,    47,     0,    48,    49,
      50,   -47,    51,    52,     0,    53,    54,    55,     0,  -306,
       0,     0,    56,    57,    58,    59,     0,    61,    62,  -306,
     -47,    63,    64,    65,     0,    66,    67,    68,     0,    69,
      70,    71,    72,    73,    74,    75,    76,     0,    77,    78,
       0,    79,    80,    81,    82,    83,     0,    84,    85,   -76,
      86,    87,     0,     0,    88,     0,    89,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
     101,     0,     0,   102,     0,   103,     0,   104,     0,   105,
       0,     4,   106,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,  1024,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,    38,   -76,     0,    39,    40,
      41,     0,    42,  -306,     0,    43,    44,    45,    46,    47,
       0,    48,    49,    50,   -47,    51,    52,     0,    53,    54,
      55,     0,  -306,     0,     0,    56,    57,    58,    59,     0,
      61,    62,  -306,   -47,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,    77,    78,     0,    79,    80,    81,    82,    83,     0,
      84,    85,   -76,    86,    87,     0,     0,    88,     0,    89,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,   101,     0,     0,   102,     0,   103,     0,
     104,     0,   105,     0,     4,   106,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,  1030,    38,   -76,
       0,    39,    40,    41,     0,    42,  -306,     0,    43,    44,
      45,    46,    47,     0,    48,    49,    50,   -47,    51,    52,
       0,    53,    54,    55,     0,  -306,     0,     0,    56,    57,
      58,    59,     0,    61,    62,  -306,   -47,    63,    64,    65,
       0,    66,    67,    68,     0,    69,    70,    71,    72,    73,
      74,    75,    76,     0,    77,    78,     0,    79,    80,    81,
      82,    83,     0,    84,    85,   -76,    86,    87,     0,     0,
      88,     0,    89,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,   101,     0,     0,   102,
       0,   103,     0,   104,     0,   105,     0,     4,   106,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
    1034,    38,   -76,     0,    39,    40,    41,     0,    42,  -306,
       0,    43,    44,    45,    46,    47,     0,    48,    49,    50,
     -47,    51,    52,     0,    53,    54,    55,     0,  -306,     0,
       0,    56,    57,    58,    59,     0,    61,    62,  -306,   -47,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,     0,    84,    85,   -76,    86,
      87,     0,     0,    88,     0,    89,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,   101,
       0,     0,   102,     0,   103,     0,   104,     0,   105,     0,
    1220,   106,     5,   293,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,  1221,   870,   106,     5,   293,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,    40,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,     0,    57,     0,
      59,     0,    61,     0,     0,     0,     0,    64,   189,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   190,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,     0,     0,     0,   102,     0,
     191,     0,   104,     0,   192,     0,     5,   106,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,   231,
      23,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
      58,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,    66,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,    86,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     0,     0,   106,     5,
     293,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,   297,   298,     0,    83,   335,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,   336,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   102,     0,   191,     0,   104,     0,   192,     0,
       0,   106,     5,   293,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,   297,   298,     0,    83,   335,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
     728,   192,     0,     0,   106,     5,   293,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,    40,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,     0,    57,     0,
      59,     0,    61,     0,     0,     0,     0,    64,   189,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   190,     0,    77,     0,     0,    79,   297,   298,     0,
      83,   335,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,     0,     0,     0,   102,     0,
     191,     0,   104,   730,   192,     0,     5,   106,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
     611,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
     612,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,   613,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,   614,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     0,     5,   106,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     189,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,   297,
     298,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,   299,     0,     0,
     102,     0,   191,     0,   104,     0,   192,     0,     0,   106,
       5,   293,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,   187,    38,     0,     0,     0,    40,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,   297,   298,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,   192,
       0,     5,   106,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,   872,    53,    54,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,   297,   298,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,     0,     0,     0,   102,     0,   191,     0,   104,     0,
     192,     0,     5,   106,     6,     7,     8,   218,    10,    11,
     219,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   220,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,   221,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,    57,   223,    59,     0,
      61,     0,     0,     0,     0,    64,   189,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   190,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
     224,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,     0,     0,     0,   102,     0,   191,     0,
     104,     0,   192,     0,     5,   106,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,     0,   234,     0,
       0,     0,     0,     0,    28,     0,   184,     0,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,   235,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,     0,   192,     0,     0,   106,     5,   293,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     189,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,   224,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,     0,   104,     0,   192,     0,     5,   106,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     184,     0,     0,     0,    32,   185,   186,     0,     0,   187,
      38,     0,     0,     0,    40,     0,     0,    42,     0,     0,
     188,     0,     0,    46,    47,     0,     0,    49,    50,     0,
      51,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
     297,   298,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,   192,     0,     0,
     106,     5,   293,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,     0,     0,     0,   102,     0,   191,   726,   104,     0,
     192,     0,     0,   106,     5,   293,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   184,     0,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,     0,   192,   736,     0,   106,     5,   293,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     189,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,     0,   104,     0,   192,  1152,     5,   106,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     184,     0,     0,     0,    32,   185,   186,     0,     0,   187,
      38,     0,     0,     0,    40,     0,     0,    42,     0,     0,
     188,     0,     0,    46,    47,     0,     0,    49,    50,     0,
      51,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,   224,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,   192,     0,     5,
     106,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,   273,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   102,     0,   103,     0,   104,     0,   192,     0,
       0,   106,     5,   293,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,   218,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   220,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   190,
       0,    77,     0,     0,    79,     0,     0,   221,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,     0,     0,     0,   102,     0,   191,     0,
     104,     0,   192,     0,     5,   106,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   184,     0,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,     0,   192,   741,     5,   106,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,    40,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,     0,    57,     0,
      59,     0,    61,     0,     0,     0,     0,    64,   189,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   190,     0,    77,     0,     0,    79,     0,     0,     0,
      83,   898,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,     0,     0,     0,   102,     0,
     191,     0,   104,     0,   192,     0,     0,   106,     5,   293,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,   700,     0,
     184,     0,     0,     0,    32,   185,   186,     0,     0,   187,
      38,     0,     0,     0,   701,     0,     0,    42,     0,     0,
     188,     0,     0,    46,    47,     0,     0,    49,    50,     0,
      51,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
     702,    73,    74,    75,   703,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,  1091,     0,     5,
     106,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,     0,    83,  1116,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   102,     0,   191,     0,   104,     0,   192,     0,
       0,   106,     5,   293,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,  1091,     0,     0,   106,     5,   293,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,  1072,    38,     0,     0,
       0,    40,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,     0,    57,     0,
      59,     0,    61,     0,     0,     0,     0,    64,   189,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   190,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,     0,     0,     0,   102,     0,
     191,     0,   104,     0,   192,     0,     5,   106,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     0,     5,   106,     6,
       7,     8,   214,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     215,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,     0,   104,     0,   192,     0,     5,   106,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     184,     0,     0,     0,    32,   185,   186,     0,     0,   187,
      38,     0,     0,     0,    40,     0,     0,    42,     0,     0,
     188,     0,     0,    46,    47,     0,     0,    49,    50,     0,
      51,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   103,     0,   104,     0,   192,     0,     5,
     106,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   663,     0,   191,     0,   104,     0,   192,     0,
       5,   106,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     700,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,   187,    38,     0,     0,     0,   701,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,   702,    73,    74,    75,   703,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,   704,
       0,     5,   106,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,     0,     0,     0,   102,     0,   191,     0,   104,     0,
     704,     0,     5,   106,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   796,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   799,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   189,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   190,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,     0,     0,     0,   102,     0,   191,     0,
     104,     0,   192,     0,     5,   106,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   184,     0,     0,     0,
      32,   185,   186,     0,     0,  1066,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,     0,   192,     0,     5,   106,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,  1067,    38,     0,     0,
       0,    40,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,     0,    57,     0,
      59,     0,    61,     0,     0,     0,     0,    64,   189,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   190,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,     0,     0,     0,   102,     0,
     191,     0,   104,     0,   192,     0,     5,   106,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,  1069,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     0,     5,   106,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,  1070,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     189,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,     0,   104,     0,   192,     0,     5,   106,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     184,     0,     0,     0,    32,   185,   186,     0,     0,  1071,
      38,     0,     0,     0,    40,     0,     0,    42,     0,     0,
     188,     0,     0,    46,    47,     0,     0,    49,    50,     0,
      51,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,   192,     0,     5,
     106,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
    1072,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   102,     0,   191,     0,   104,     0,   192,     0,
       5,   106,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     700,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,   187,    38,     0,     0,     0,   701,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,   702,    73,    74,    75,   703,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,  1176,
       0,     5,   106,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   911,     0,   912,    91,    92,    93,    94,
      95,     0,     0,     0,   913,    96,     0,     0,     0,   914,
     242,   915,   916,     0,     0,    97,     0,   917,    98,    99,
     100,     0,     0,     0,   102,   245,     0,   184,   104,     0,
    1176,   918,     0,   106,     0,     0,     0,   919,     0,     0,
       0,   248,     0,     0,   920,     0,   921,     0,     0,     0,
       0,     0,     0,     0,   922,     0,     0,     0,     0,     0,
     923,   924,     0,     0,     0,     0,     0,     0,   253,     0,
       0,     0,     0,     0,     0,     0,     0,   925,     0,     0,
       0,     0,     0,     0,     0,     0,   255,   256,     0,   926,
       0,   258,     0,   927,     0,     0,   928,     0,     0,     0,
     929,     0,     0,   262,     0,     0,   930,     0,     0,     0,
       0,     0,     0,     0,     0,   476,   396,   397,   398,   399,
     400,     0,     0,   403,   404,   405,   406,     0,   408,   409,
     931,   932,   933,   934,   935,   936,     0,   937,     0,     0,
       0,   938,   939,   940,   941,   942,   943,   944,   945,   946,
     947,   948,     0,   949,     0,     0,   950,   951,   952,   953,
       0,     5,   954,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,  -576,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,   410,    99,
     100,  -615,     0,  -615,   102,     0,   191,     0,   104,     0,
     192,     5,   293,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,  -473,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,  -473,     0,     0,     0,  -473,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,   191,     0,   104,  -473,
    1091,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,  -455,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,     0,     0,  -455,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,  -455,    79,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,  -455,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,    93,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
     100,  -455,     0,     0,     0,     0,  -455,    28,   104,   184,
    1176,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     189,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     5,    93,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,   100,     0,     0,     0,
       0,     0,   191,    28,   104,   184,   192,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   189,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   190,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
      93,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,   100,     0,     0,     0,     0,     0,   191,    28,
     104,   184,   704,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,  1129,
      79,     0,     0,     0,    83,     0,     0,    85,   913,     0,
      87,     0,     0,   241,   242,   915,   243,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,   245,
       0,     0,     0,     0,     0,   918,    93,     0,     0,     0,
       0,   247,     0,     0,     0,   248,     0,     0,   249,     0,
     921,     0,     0,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,   923,   252,   104,     0,   192,     0,
       0,     0,   253,     0,     0,     0,     0,     0,     0,     0,
       0,   254,     0,     0,     0,     0,     0,     0,     0,   365,
     255,   256,     0,   257,     0,   258,   366,  1130,     0,     0,
     928,     0,     0,     0,   261,     0,     0,   262,   367,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,   476,
     396,   397,   398,   399,   400,     0,     0,   403,   404,   405,
     406,     0,   408,   409,   931,   932,   933,   934,   935,   936,
       0,   937,     0,     0,     0,   938,   939,   940,   941,   942,
     943,   944,   945,   946,   947,   948,     0,   949,     0,     0,
     950,   951,   952,   953,   368,   365,     0,     0,   369,     0,
       0,     0,   366,     0,     0,     0,     0,   475,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   370,
     476,   396,   397,   398,   399,   400,     0,     0,   403,   404,
     405,   406,     0,   408,   409,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,     0,
     368,   391,   392,   393,   369,   425,     0,   365,     0,     0,
       0,     0,   394,   984,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,     0,     0,     0,     0,   370,   476,   396,   397,   398,
     399,   400,     0,     0,   403,   404,   405,   406,     0,   408,
     409,   371,   372,     0,   373,   374,   375,     0,   376,   377,
     378,     0,   379,   380,   381,   382,   383,     0,   384,   385,
     386,   387,   388,   389,   390,     0,     0,   391,   392,   393,
       0,   425,   368,   365,     0,     0,   369,     0,   394,     0,
     366,     0,     0,     0,     0,   991,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   370,   476,   396,
     397,   398,   399,   400,     0,     0,   403,   404,   405,   406,
       0,   408,   409,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   368,   391,
     392,   393,   369,   425,     0,   365,     0,     0,     0,     0,
     394,  1143,   366,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,     0,
       0,     0,     0,   370,   476,   396,   397,   398,   399,   400,
       0,     0,   403,   404,   405,   406,     0,   408,   409,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,     0,   391,   392,   393,     0,   425,
     368,   365,     0,     0,   369,     0,   394,     0,   366,     0,
       0,     0,     0,  1144,     0,     0,     0,     0,     0,     0,
     367,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   370,   476,   396,   397,   398,
     399,   400,     0,     0,   403,   404,   405,   406,     0,   408,
     409,   371,   372,     0,   373,   374,   375,     0,   376,   377,
     378,     0,   379,   380,   381,   382,   383,     0,   384,   385,
     386,   387,   388,   389,   390,     0,   368,   391,   392,   393,
     369,   425,     0,   365,     0,     0,     0,     0,   394,  1145,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,     0,     0,     0,
       0,   370,   476,   396,   397,   398,   399,   400,     0,     0,
     403,   404,   405,   406,     0,   408,   409,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,     0,     0,   391,   392,   393,     0,   425,   368,   365,
       0,     0,   369,     0,   394,     0,   366,     0,     0,     0,
       0,  1146,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   370,   476,   396,   397,   398,   399,   400,
       0,     0,   403,   404,   405,   406,     0,   408,   409,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,   368,   391,   392,   393,   369,   425,
       0,   365,     0,     0,     0,     0,   394,  1170,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     367,     0,     0,     0,     0,     0,     0,     0,     0,   370,
     476,   396,   397,   398,   399,   400,     0,     0,   403,   404,
     405,   406,     0,   408,   409,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,     0,
       0,   391,   392,   393,     0,   425,   368,     0,     0,     0,
     369,     0,   394,   365,     0,     0,     0,     0,     0,  1171,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,   273,     0,     0,     0,     0,     0,     0,
       0,   370,   476,   396,   397,   398,   399,   400,   465,     0,
     403,   404,   405,   406,     0,   408,   409,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,     0,     0,   391,   392,   393,     0,   425,   368,   365,
       0,     0,   369,     0,   394,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   456,
       0,     0,     0,   370,   454,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,   368,   391,   392,   393,   369,   425,
       0,     0,   365,     0,     0,     0,   394,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,   468,     0,     0,   370,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   469,   384,   385,   386,   387,   388,   389,   390,     0,
       0,   391,   392,   393,     0,   425,     0,   368,     0,     0,
       0,   369,   394,     0,     0,   365,   752,     0,     0,     0,
       0,     0,   366,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,   753,
       0,     0,   370,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,   754,   384,   385,   386,   387,   388,
     389,   390,     0,   470,   391,   392,   393,     0,     0,     0,
     368,   365,     0,     0,   369,   394,     0,     0,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     367,   831,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   370,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   371,   372,     0,   373,   374,   375,     0,   376,   377,
     378,     0,   379,   380,   381,   382,   383,     0,   384,   385,
     386,   387,   388,   389,   390,     0,   368,   391,   392,   393,
     369,     0,     0,   365,     0,     0,     0,     0,   394,     0,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,   273,     0,     0,     0,   456,     0,     0,
       0,   370,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,     0,     0,   391,   392,   393,     0,   425,   368,   365,
       0,     0,   369,     0,   394,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   456,
       0,     0,     0,   370,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,   368,   391,   392,   393,   369,   425,
     365,     0,     0,     0,     0,     0,   394,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
     273,     0,     0,     0,     0,   177,     0,     0,     0,   370,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,     0,
       0,   391,   392,   393,     0,   368,   365,     0,     0,   369,
       0,     0,   394,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,   620,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     370,   621,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,   368,   391,   392,   393,   369,   425,   365,     0,     0,
       0,     0,     0,   394,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,   622,     0,     0,
       0,     0,     0,     0,     0,     0,   370,     0,     0,     0,
       0,     0,   623,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,   368,   365,     0,     0,   369,     0,     0,   394,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,   823,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   370,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   368,   391,
     392,   393,   369,     0,     0,   365,     0,     0,     0,     0,
     394,     0,   366,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,     0,
       0,     0,     0,   370,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,     0,   391,   392,   393,     0,   425,
     368,   365,     0,     0,   369,     0,   394,     0,   366,     0,
       0,     0,     0,   849,     0,     0,     0,     0,     0,     0,
     367,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   370,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   371,   372,     0,   373,   374,   375,     0,   376,   377,
     378,     0,   379,   380,   381,   382,   383,     0,   384,   385,
     386,   387,   388,   389,   390,     0,   368,   391,   392,   393,
     369,   425,     0,     0,   365,     0,     0,     0,   394,     0,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,   456,  1043,     0,
       0,   370,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,  1044,   384,   385,   386,   387,   388,   389,
     390,     0,     0,   391,   392,   393,     0,     0,     0,   368,
     365,     0,   890,   369,   394,     0,     0,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,     0,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,   366,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
     367,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,   368,   391,   392,   393,   369,
       0,     0,     0,     0,     0,     0,   365,   394,     0,     0,
       0,     0,     0,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
     370,     0,     0,     0,     0,     0,   368,     0,     0,     0,
     369,     0,     0,     0,     0,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,   370,   391,   392,   393,     0,   511,     0,     0,     0,
       0,   368,     0,   394,     0,   369,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,   365,   515,   391,   392,   393,   370,     0,   366,     0,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
     367,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,   365,   517,   391,   392,
     393,     0,     0,   366,     0,     0,     0,     0,     0,   394,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   368,     0,     0,     0,
     369,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,   366,     0,     0,     0,     0,     0,     0,     0,
       0,   370,     0,     0,   367,     0,     0,     0,     0,     0,
       0,   368,     0,     0,     0,   369,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,     0,   519,   391,   392,   393,   370,     0,     0,     0,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
     368,     0,   371,   372,   369,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,   365,   525,   391,   392,
     393,     0,     0,   366,   542,   370,     0,     0,     0,   394,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,   371,   372,     0,   373,   374,   375,     0,   376,   377,
     378,     0,   379,   380,   381,   382,   383,     0,   384,   385,
     386,   387,   388,   389,   390,   365,     0,   391,   392,   393,
       0,     0,   366,     0,     0,     0,     0,     0,   394,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,     0,
       0,   368,     0,     0,     0,   369,     0,     0,     0,     0,
       0,     0,     0,     0,   624,   365,   748,     0,     0,     0,
       0,     0,   366,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,   370,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     368,     0,   371,   372,   369,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,     0,     0,     0,   370,     0,     0,     0,   394,
     368,     0,     0,     0,   369,     0,     0,     0,     0,     0,
       0,   371,   372,     0,   373,   374,   375,     0,   376,   377,
     378,     0,   379,   380,   381,   382,   383,     0,   384,   385,
     386,   387,   388,   389,   390,   370,     0,   391,   392,   393,
       0,     0,   365,     0,   727,     0,     0,     0,   394,   366,
       0,   371,   372,     0,   373,   374,   375,     0,   376,   749,
     378,   367,   379,   380,   381,   382,   383,     0,   384,   385,
     386,   387,   388,   389,   390,     0,     0,   391,   392,   393,
       0,     0,     0,     0,     0,     0,     0,   365,   394,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   368,     0,     0,
       0,   369,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,     0,   370,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   368,     0,     0,     0,   369,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,  -577,   384,   385,   386,   387,   388,
     389,   390,     0,   177,   391,   392,   393,   370,     0,     0,
       0,     0,   368,     0,     0,   394,   369,     0,     0,     0,
       0,     0,     0,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,   370,     0,   391,
     392,   393,     0,     0,   365,   840,     0,     0,     0,     0,
     394,   366,     0,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,   367,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   808,   391,
     392,   393,     0,     0,   365,     0,     0,     0,     0,     0,
     394,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,   847,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   368,
       0,     0,     0,   369,   365,     0,     0,     0,     0,     0,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,     0,     0,     0,     0,   368,
       0,     0,     0,   369,     0,     0,     0,     0,     0,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,   370,     0,   391,   392,   393,   368,
       0,     0,     0,   369,     0,     0,     0,   394,     0,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,   370,     0,   391,   392,   393,     0,
       0,   365,     0,     0,     0,     0,     0,   394,   366,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
     367,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,     0,   391,   392,   393,     0,
       0,     0,   365,   874,     0,     0,     0,   394,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   368,     0,     0,     0,
     369,     0,   365,     0,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,   969,     0,     0,     0,     0,     0,     0,     0,
       0,   370,     0,     0,     0,     0,     0,   368,     0,     0,
       0,   369,     0,     0,     0,     0,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,   893,   370,   391,   392,   393,     0,   368,     0,     0,
     875,   369,     0,     0,   394,     0,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,   370,     0,   391,   392,   393,     0,     0,     0,
       0,     0,     0,     0,     0,   394,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,   365,     0,   391,   392,   393,     0,     0,   366,
       0,     0,     0,     0,     0,   394,     0,     0,     0,     0,
       0,   367,   972,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,     0,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   368,     0,     0,
       0,   369,   365,  1040,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,     0,     0,     0,     0,   368,     0,     0,
       0,   369,     0,     0,     0,     0,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,   370,     0,   391,   392,   393,   368,     0,     0,
       0,   369,     0,     0,     0,   394,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,   370,     0,   391,   392,   393,     0,     0,   365,
       0,     0,     0,   975,     0,   394,   366,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,   367,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,     0,     0,   391,   392,   393,     0,     0,   365,
       0,     0,     0,     0,     0,   394,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   368,     0,     0,     0,   369,   365,
    1085,     0,     0,     0,     0,     0,   366,  1065,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   370,
       0,     0,     0,     0,   368,     0,     0,     0,   369,     0,
       0,     0,     0,     0,     0,   371,   372,  1068,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,   370,
       0,   391,   392,   393,   368,     0,     0,     0,   369,     0,
       0,     0,   394,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,   370,
       0,   391,   392,   393,     0,     0,   365,     0,     0,     0,
       0,     0,   394,   366,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,   367,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,     0,
       0,   391,   392,   393,     0,     0,   365,     0,     0,     0,
       0,     0,   394,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   368,     0,     0,     0,   369,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   370,     0,     0,     0,
       0,   368,     0,     0,     0,   369,     0,     0,     0,     0,
       0,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,   370,     0,   391,   392,
     393,     0,     0,   365,     0,  1128,     0,     0,     0,   394,
     366,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,   367,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,     0,   365,     0,     0,     0,  1166,     0,   394,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   368,     0,
       0,     0,   369,   365,     0,     0,     0,     0,     0,     0,
     366,  1185,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   370,     0,     0,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,   371,
     372,  1186,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,   370,     0,   391,   392,   393,   368,     0,
       0,     0,   369,     0,     0,     0,   394,     0,     0,   371,
     372,  1187,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,   370,     0,   391,   392,   393,     0,     0,
     365,     0,     0,     0,     0,     0,   394,   366,     0,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,   367,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,     0,   391,   392,   393,     0,     0,
     365,     0,     0,     0,     0,     0,   394,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   368,     0,     0,     0,   369,
     365,     0,     0,     0,     0,     0,     0,   366,  1188,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     370,     0,     0,     0,     0,   368,     0,     0,     0,   369,
       0,     0,     0,     0,     0,     0,   371,   372,  1189,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
     370,     0,   391,   392,   393,   368,     0,     0,     0,   369,
       0,     0,     0,   394,     0,     0,   371,   372,  1190,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
     370,     0,   391,   392,   393,     0,     0,   365,     0,     0,
       0,     0,     0,   394,   366,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,   367,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,     0,   391,   392,   393,     0,     0,   365,     0,     0,
       0,     0,     0,   394,   366,  1214,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   368,     0,     0,     0,   369,   365,     0,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   370,     0,     0,
       0,     0,   368,     0,     0,     0,   369,     0,     0,     0,
       0,     0,     0,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,   370,     0,   391,
     392,   393,   368,     0,     0,     0,   369,     0,     0,     0,
     394,     0,     0,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,   370,     0,   391,
     392,   393,     0,     0,   365,     0,     0,     0,  1243,     0,
     394,   366,     0,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,   367,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   288,   391,
     392,   393,    22,    23,     0,     0,     0,     0,     0,     0,
     394,   289,     0,    30,   290,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   794,
     367,     0,     0,   369,     0,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,  1049,     0,     0,
    1050,     0,   291,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,     0,     0,     0,     0,     0,
     367,     0,     0,    82,     0,     0,    84,     0,     0,    86,
     371,   372,     0,   373,   374,   375,   368,   376,   377,   378,
     369,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,     0,   391,   392,   393,     0,
       0,     0,     0,     0,     0,     0,     0,   394,     0,     0,
       0,   370,     0,     0,     0,     0,   368,     0,     0,   101,
     369,     0,     0,     0,     0,  1192,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,     0,     0,   384,   385,   386,   387,   388,   389,
     390,   370,     0,   391,   392,   393,     0,     0,     0,     0,
       0,     0,     0,     0,   394,     0,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,     0,     0,   384,   385,   386,   387,   388,   759,
     390,   240,     0,   391,   392,   393,     0,     0,     0,     0,
       0,     0,     0,     0,   394,   241,   242,     0,   243,     0,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,   245,     0,     0,     0,     0,     0,   246,     0,     0,
       0,     0,  -287,   247,     0,     0,     0,   248,     0,     0,
     249,     0,     0,     0,     0,     0,  -287,  -287,     0,  -287,
     250,     0,     0,     0,  -287,     0,   251,   252,     0,     0,
       0,     0,  -287,     0,   253,     0,     0,     0,  -287,     0,
       0,     0,     0,   254,  -287,     0,     0,     0,  -287,     0,
       0,  -287,   255,   256,     0,   257,     0,   258,     0,   259,
       0,  -287,   260,     0,     0,     0,   261,  -287,  -287,   262,
       0,     0,   263,     0,     0,  -287,     0,     0,     0,     0,
       0,     0,     0,     0,  -287,     0,     0,  -297,     0,     0,
       0,     0,     0,  -287,  -287,     0,  -287,     0,  -287,     0,
    -287,  -297,  -297,  -287,  -297,     0,     0,  -287,     0,  -297,
    -287,     0,     0,  -287,     0,     0,     0,  -297,     0,     0,
       0,     0,     0,  -297,     0,     0,     0,     0,   240,  -297,
       0,     0,     0,  -297,     0,     0,  -297,     0,     0,     0,
       0,     0,   241,   242,     0,   243,  -297,     0,     0,     0,
     244,     0,  -297,  -297,     0,     0,     0,     0,   245,     0,
    -297,     0,     0,     0,   246,     0,     0,     0,     0,  -297,
     247,     0,     0,     0,   248,     0,     0,   249,  -297,  -297,
       0,  -297,     0,  -297,     0,  -297,     0,   250,  -297,     0,
       0,     0,  -297,   251,   252,  -297,     0,     0,  -297,     0,
       0,   253,     0,     0,     0,     0,     0,     0,     0,     0,
     254,     0,     0,  -288,     0,     0,     0,     0,     0,   255,
     256,     0,   257,     0,   258,     0,   259,  -288,  -288,   260,
    -288,     0,     0,   261,     0,  -288,   262,     0,     0,   263,
       0,     0,     0,  -288,     0,     0,     0,     0,     0,  -288,
       0,     0,     0,     0,  -174,  -288,     0,     0,     0,  -288,
       0,     0,  -288,     0,     0,     0,     0,     0,  -174,  -174,
       0,  -174,  -288,     0,     0,     0,  -174,     0,  -288,  -288,
       0,     0,     0,     0,  -174,     0,  -288,     0,     0,     0,
    -174,     0,     0,     0,     0,  -288,  -174,     0,     0,     0,
    -174,     0,     0,  -174,  -288,  -288,     0,  -288,     0,  -288,
       0,  -288,     0,  -174,  -288,     0,     0,     0,  -288,  -174,
    -174,  -288,     0,     0,  -288,     0,     0,  -174,     0,     0,
       0,     0,     0,     0,     0,     0,  -174,     0,     0,  -170,
       0,     0,     0,     0,     0,  -174,  -174,     0,  -174,     0,
    -174,     0,  -174,  -170,  -170,  -174,  -170,     0,     0,  -174,
       0,  -170,  -174,     0,     0,  -174,     0,     0,     0,  -170,
       0,     0,     0,     0,     0,  -170,     0,     0,     0,     0,
       0,  -170,     0,     0,     0,  -170,     0,     0,  -170,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -170,     0,
       0,     0,     0,     0,  -170,  -170,     0,     0,     0,     0,
       0,     0,  -170,     0,     0,     0,     0,     0,     0,     0,
       0,  -170,     0,     0,     0,     0,     0,     0,     0,     0,
    -170,  -170,     0,  -170,     0,  -170,     0,  -170,     0,     0,
    -170,     0,     0,     0,  -170,     0,     0,  -170,     0,     0,
    -170
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1102)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,   416,    67,   520,    16,    63,   469,     2,    20,   161,
     652,    19,   487,   213,    26,   205,   768,   490,    30,    31,
     239,    33,    34,    35,    36,    37,   483,   754,   753,   147,
     869,   221,     1,    22,    47,   904,    48,  1101,    50,   239,
     105,    55,   699,    55,     1,    57,   640,    59,    27,   643,
    1014,   228,   229,   230,    32,    67,    68,    69,    70,    71,
      72,    47,   959,   960,    76,    47,    78,   286,    80,    81,
     513,    32,    47,    85,    32,  1011,    88,    89,    46,    91,
      67,    93,     1,  1152,    96,    97,   286,    99,   206,   308,
       3,   103,   104,   105,   106,    32,     1,     1,    77,   100,
      53,   495,    36,  1204,    73,    32,    47,   144,   308,    88,
    1089,   144,    68,   138,   144,    55,   103,   104,   105,    72,
       0,   146,     3,   163,    90,   964,   520,   192,   107,    82,
     163,   144,   169,   163,   113,  1204,   793,    71,   780,    20,
     118,    60,  1243,    47,   112,   157,   115,     3,   104,     3,
      73,    47,    33,    55,  1090,    60,   170,   118,   144,  1223,
     173,  1125,   144,   315,   341,   342,   123,   168,   625,   144,
     182,    90,  1151,   185,   186,   187,  1015,   377,   190,   191,
     192,    62,    90,  1052,   103,    90,   143,   144,   101,   118,
     168,   173,   115,   412,   413,   638,   204,   116,   173,     3,
     169,   213,   168,   144,   191,   192,  1075,   168,  1077,   866,
     168,   116,   412,   413,   171,    31,    47,   144,   812,   123,
     101,   815,   118,    60,   143,   698,   107,   239,   217,   287,
     170,   168,   173,   222,  1131,  1132,    90,   652,   143,   143,
     144,   168,   231,   118,   163,   101,   169,   101,   144,   168,
    1089,    33,    34,    35,    47,   163,   118,    88,  1221,   163,
     168,   144,   163,   168,    27,   144,   144,   144,   169,   116,
    1139,    87,  1141,   144,   286,    91,  1239,   173,   290,   116,
     123,   144,   170,   483,   172,   297,   298,   168,   171,    60,
       3,   754,   169,   171,   173,    88,   308,   144,   173,   756,
     171,   144,   144,    60,   170,   148,   172,    20,   171,  1061,
     144,   173,  1151,   513,    77,  1042,  1041,  1044,  1043,    90,
      33,   163,   138,   105,   336,    88,   444,   143,   171,   145,
     146,   449,   103,    90,   150,    80,    81,   171,   333,   351,
     352,   980,   158,   982,   107,   116,   103,   989,   805,    62,
     166,   363,    55,   365,   366,   367,   368,   369,   370,   116,
     372,   373,   374,   375,   376,   780,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   858,    60,    18,   451,   860,   101,   401,
     402,   144,   163,   144,   107,   407,   123,   168,   410,   411,
     412,   413,   414,   415,   416,   144,   163,   144,    27,    27,
      81,   168,   163,    84,    90,   427,   138,   144,   171,   163,
     432,   148,   434,   144,   146,   625,   170,   103,   172,   904,
     163,    46,   171,   896,   171,  1074,   169,  1076,   638,   451,
     116,   453,   454,    76,   171,   401,   402,   459,   460,    64,
     171,   407,    27,   465,   410,   411,   468,   469,    77,    77,
     170,    94,   172,   475,   451,    31,   478,   144,    83,    88,
      88,   483,   144,   123,   144,    50,    55,   123,   111,   656,
     657,   144,   144,   660,   661,   497,   163,   163,   107,   501,
     502,   556,   168,   163,   506,   113,   144,   112,   148,   171,
      75,   513,    77,   149,  1021,   151,   152,   169,   171,   155,
     156,    55,   168,    88,   170,   144,   138,   529,   144,   506,
     532,    87,   534,   171,   146,    91,   538,   539,   540,    88,
     542,   543,   144,    48,   163,   725,    31,   163,   113,   163,
     170,    55,   529,    27,   556,   532,   170,   534,   748,   749,
      87,   163,   539,   548,    91,    70,   756,   734,   735,   217,
     170,    55,   172,    47,   222,    80,    81,    51,   170,   556,
     172,    87,   138,   170,   989,    91,   532,   143,   534,  1042,
     146,  1044,  1057,    55,   150,    47,   368,   369,    55,   138,
      74,    55,    87,    77,   143,    79,    91,   146,    60,    55,
    1075,   150,  1077,     8,    88,   805,   170,    55,   620,   621,
     622,   623,   624,   625,   626,   627,   768,   170,    98,   172,
    1014,   629,   630,   107,   636,    55,   638,  1021,    90,   532,
     170,   534,   414,   415,   646,    55,   648,    55,    55,   704,
     652,    55,   654,   138,   139,   427,   141,   142,   143,   636,
     145,   146,    83,    55,   116,   150,   168,   669,   670,   163,
     162,   123,   163,   158,  1139,   163,  1141,   154,    55,   164,
     165,   166,   454,   163,  1157,   163,   168,   163,   460,   170,
     636,    47,   144,   465,   163,   163,   163,   171,   203,   117,
     702,   703,   704,    86,    60,    86,   708,   170,   163,   170,
     170,   163,   170,   163,   170,     8,   168,   170,   123,   171,
     225,   226,   227,   228,   229,   230,    64,   704,   143,    27,
     146,   236,   237,   168,    90,  1208,  1209,  1210,  1211,  1212,
    1213,  1125,    67,   170,   746,   123,   163,   171,   750,    47,
     752,   753,   754,    51,   756,   144,    47,   163,   170,    47,
     116,   266,   708,   123,   123,    34,   538,   123,  1152,    34,
     171,   276,    60,   144,   171,   123,    74,   282,   780,    77,
     171,    79,  1166,   171,    21,   170,   144,   173,   144,   173,
      88,   163,   794,   795,   796,   168,   163,   799,   163,   304,
     163,   163,    90,   805,   173,   171,    31,   163,   163,   107,
     163,   144,   168,  1022,  1023,   171,   170,   322,   323,   821,
    1204,   823,   163,   878,   163,    34,   828,    34,   116,   831,
      64,   144,  1022,  1023,   171,   123,   838,  1221,   840,   171,
     169,   171,   144,   169,   123,   847,    88,   171,    27,   621,
      32,   623,    32,   173,   168,  1239,   144,   168,    34,  1243,
     163,   143,    87,   173,    88,   170,    91,   143,    47,   170,
      34,   163,    51,   173,   173,   163,   878,   163,   146,   169,
     168,   170,   146,   171,   146,   138,   888,   146,   890,   146,
     146,   893,   138,   138,   896,    74,   146,   146,    77,   146,
      79,   878,   146,   146,   906,   138,   138,   146,   168,    88,
     168,   163,   163,   138,   139,   163,   141,   142,   143,  1061,
     145,   146,   147,   428,     2,   150,   151,   152,   107,   103,
     155,   156,   157,   158,    12,   171,   161,   163,   138,   164,
     165,   166,   171,   171,   171,   163,   163,    25,   171,    27,
     175,   138,   954,   458,   162,   144,   162,   462,   173,   464,
     481,   348,   467,    24,   746,    88,   968,   969,   853,   971,
     972,   316,   974,   975,   882,   907,  1061,  1057,   858,  1223,
     690,   910,   984,   985,  1181,   987,  1214,   989,  1207,   991,
     992,    36,    30,   995,    -1,   498,    -1,    -1,    76,    -1,
      -1,    31,  1050,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1020,  1021,
    1022,  1023,   794,   795,    -1,   103,  1028,    -1,  1030,    -1,
     108,    -1,  1034,    -1,    -1,    -1,  1091,  1014,  1040,  1041,
    1042,  1043,  1044,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     2,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    91,    12,  1065,  1066,  1067,  1068,  1069,  1070,  1071,
    1072,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,  1085,    -1,    -1,    -1,    -1,    -1,  1091,
      -1,    -1,    -1,    -1,  1096,    -1,    -1,    -1,  1100,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,    -1,   142,   143,  1091,   145,   146,    -1,    -1,  1096,
     150,  1176,    -1,    -1,    -1,    -1,    76,    -1,   158,    -1,
      80,    81,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,
      -1,  1143,  1144,  1145,  1146,   650,   651,    -1,    -1,    -1,
      -1,   656,   657,   103,    -1,   660,   661,    -1,   108,    -1,
      -1,    -1,    -1,    -1,  1166,    -1,    -1,    -1,  1170,  1171,
      -1,   676,    -1,    -1,  1176,  1152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1185,  1186,  1187,  1188,  1189,  1190,   267,
      -1,    -1,    22,    -1,    -1,   273,    26,    27,    -1,  1176,
      -1,  1203,  1204,    -1,  1206,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,   721,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,  1204,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1239,    -1,    -1,
      -1,  1243,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    84,   333,    86,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1243,    -1,    -1,    -1,
     348,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,
     110,     2,     1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   806,   807,    22,    25,    -1,    27,    26,    27,    -1,
      -1,    -1,    -1,   818,    -1,   820,    35,   267,    37,    38,
      -1,    -1,   827,   273,    43,   830,    -1,    -1,   833,    -1,
     835,    50,   837,   163,    53,    -1,    -1,    -1,    -1,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   425,    -1,   854,
      -1,    -1,    -1,    72,    -1,    76,    75,    -1,    77,    -1,
      79,    -1,    81,    82,    -1,    84,    -1,    86,    -1,    88,
      -1,    -1,    -1,    -1,    -1,   453,    -1,    -1,    -1,    -1,
     885,   459,   103,   333,    -1,    -1,     3,   108,   107,    -1,
      -1,   110,    -1,    -1,   113,   473,    -1,   475,   348,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,     2,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    12,    39,    -1,     3,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    25,    52,    27,    -1,    17,    -1,
      -1,    20,    -1,    -1,   163,    62,    25,    -1,    -1,    -1,
     169,    68,    69,    -1,    -1,    -1,    35,   178,   536,    76,
      -1,    -1,   540,    -1,    -1,    -1,    45,    -1,    85,    -1,
     548,    -1,    -1,    52,    -1,   425,    -1,    94,    95,    -1,
      97,    -1,    99,    62,   101,    76,    -1,   104,    -1,    -1,
      69,   108,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,   453,    -1,    -1,    85,    -1,    -1,   459,
      -1,    -1,   103,    -1,    -1,    -1,    -1,   108,    97,    -1,
      -1,  1026,   101,   473,    -1,   475,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,     3,   114,    -1,    -1,    -1,   617,
      -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,    17,    18,
      -1,    20,   273,   170,    -1,    -1,    25,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,  1079,  1080,    45,    -1,  1083,  1084,
      49,    -1,    -1,    52,    -1,    -1,   536,   178,    -1,    -1,
     540,    -1,    -1,    62,   672,    -1,  1101,   675,   548,    68,
      69,    -1,    -1,    -1,  1109,    -1,  1111,    76,  1113,    -1,
      -1,    -1,   333,    -1,    -1,    87,    85,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    94,    95,   348,    97,    -1,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
       2,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,   733,    -1,    -1,    -1,    -1,
      -1,   739,    -1,    25,    -1,    27,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   267,   149,   150,   151,
     152,    -1,   273,   155,   156,   157,   158,   159,    -1,   161,
     768,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,   168,
      -1,   170,    -1,   175,   425,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,   794,   795,  1223,    -1,
      -1,    -1,   672,    -1,    -1,   675,    -1,    -1,    -1,    -1,
      -1,    -1,   453,    -1,    -1,    -1,    -1,    -1,   459,    -1,
      -1,   103,   333,   821,    -1,   823,   108,    -1,     3,    -1,
     828,    -1,   473,   831,   475,    -1,    -1,   348,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,   844,    -1,   846,    -1,
      25,   849,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,   733,    39,    -1,    -1,    -1,    -1,   739,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,   877,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
     888,    -1,   890,    68,    69,   536,   178,    -1,    -1,   540,
      -1,    76,    -1,    -1,    -1,    -1,    -1,   548,    -1,    -1,
      85,    -1,    -1,    -1,   425,    43,    44,    45,    -1,    94,
      95,    -1,    97,    -1,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,   453,    -1,    -1,    -1,    -1,     2,   459,    -1,
      -1,   821,    -1,   823,    82,    -1,    -1,    12,   828,    -1,
      -1,   831,   473,    -1,   475,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,   844,    -1,   846,    -1,    -1,   849,
      -1,    -1,    -1,    -1,    -1,   267,   984,    -1,    -1,    -1,
      -1,   273,    -1,   991,    -1,   170,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    31,    -1,   877,  1006,    12,
      -1,    -1,   140,    -1,   142,    -1,    -1,    -1,   888,    -1,
     890,    76,    25,    -1,    27,   536,  1024,    -1,    -1,   540,
      -1,   672,    -1,    -1,   675,  1033,    -1,   548,    -1,  1037,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
      -1,   333,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    87,    -1,    -1,    -1,    91,   348,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,   205,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1086,    -1,
      -1,    -1,   733,   221,    -1,   223,    -1,    -1,   739,    -1,
     103,    -1,    -1,    -1,    -1,   108,   234,   235,    -1,    -1,
      -1,    -1,   138,   139,   984,   141,   142,   143,    -1,   145,
     146,   991,    -1,   178,   150,    -1,    -1,    -1,    -1,    -1,
      -1,   157,   158,    -1,    -1,   161,  1006,    -1,   164,   165,
     166,   269,    -1,   425,    -1,  1143,  1144,  1145,  1146,    -1,
      -1,    -1,    -1,    31,  1024,    -1,    -1,    -1,    -1,    -1,
      -1,   672,    -1,  1033,   675,    -1,    -1,  1037,    -1,    -1,
      -1,   453,  1170,  1171,    -1,   178,    -1,   459,    -1,    -1,
     821,    -1,   823,    -1,    -1,    -1,    -1,   828,    -1,    -1,
     831,   473,    -1,   475,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   844,    -1,   846,    -1,    -1,   849,    87,
      -1,    -1,   267,    91,    -1,    -1,  1086,    -1,   273,    -1,
      -1,    -1,   733,     2,    -1,   353,    -1,    -1,   739,    -1,
      -1,    -1,    -1,    12,    31,    -1,   877,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,   888,    27,   890,
      -1,    -1,    -1,    -1,   536,    -1,    -1,    -1,   540,    -1,
     138,   139,    -1,    -1,   267,   143,   548,   145,   146,    -1,
     273,    -1,   150,  1143,  1144,  1145,  1146,    -1,   333,    -1,
     158,    -1,    -1,    -1,    -1,    -1,   164,   165,   166,    -1,
      87,    -1,    -1,   348,    91,    -1,   424,    76,    31,    -1,
    1170,  1171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     821,    -1,   823,    -1,   442,    -1,    -1,   828,   446,    -1,
     831,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,   108,
     333,    -1,    -1,   844,    -1,   846,    -1,    -1,   849,    -1,
      -1,   138,    -1,   984,    -1,   348,   143,    -1,   145,   146,
     991,    -1,    -1,   150,    87,    -1,   484,    -1,    91,    -1,
      -1,   158,   490,    -1,    -1,  1006,   877,   164,   165,   166,
     425,    -1,    -1,    -1,    -1,    -1,    -1,   888,    -1,   890,
      -1,    -1,    -1,  1024,    -1,    -1,    -1,    -1,    -1,    -1,
     672,    -1,  1033,   675,    -1,    -1,  1037,    -1,   453,   178,
      -1,    -1,    -1,    -1,   459,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,    -1,   150,   473,    -1,
     475,   549,   425,    -1,   157,   158,    -1,    -1,   161,    -1,
     558,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,  1086,    -1,    -1,    -1,    -1,
     453,   733,    -1,    -1,    -1,    -1,   459,   739,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     473,    -1,   475,   984,    -1,    -1,    -1,    -1,    -1,    -1,
     991,   536,    -1,    -1,    -1,   540,    -1,    -1,   267,    -1,
      -1,    -1,    -1,   548,   273,  1006,    -1,    -1,    -1,    -1,
      -1,    -1,  1143,  1144,  1145,  1146,    -1,    -1,    -1,    -1,
      -1,   639,    -1,  1024,   642,    -1,    -1,    -1,    -1,    22,
      -1,    -1,  1033,    26,    27,    -1,  1037,    -1,    -1,  1170,
    1171,    -1,    35,   536,    37,    38,    -1,   540,    -1,   821,
      43,   823,    -1,    46,    -1,   548,   828,    50,    -1,   831,
      -1,    -1,    -1,    -1,   333,    -1,    -1,    -1,    -1,    -1,
      -1,    64,   844,    -1,   846,    -1,   694,   849,    -1,   348,
     698,    -1,    75,    -1,    77,  1086,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    86,    -1,    88,    -1,    -1,    -1,     2,
      -1,    -1,    -1,   721,    -1,   877,    -1,   725,    -1,    12,
      -1,    -1,    -1,    -1,   107,    -1,   888,   110,   890,   112,
     113,    -1,    25,    -1,    27,    -1,    -1,   672,    -1,    -1,
     675,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1143,  1144,  1145,  1146,    -1,    -1,    -1,    -1,
     768,    -1,    -1,    -1,    -1,    -1,   425,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1170,
    1171,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,   672,
      -1,    -1,   675,    -1,   453,    -1,    -1,    -1,   733,    -1,
     459,    -1,     2,    -1,   739,    -1,    -1,    -1,    -1,    -1,
     103,    -1,    12,    -1,   473,   108,   475,    -1,    -1,    -1,
      -1,    -1,   984,    -1,    -1,    25,    -1,    27,    -1,   991,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   851,  1006,   853,    -1,    -1,    -1,   857,
     733,    -1,   860,    -1,    -1,    -1,   739,    -1,    -1,    -1,
      -1,    -1,  1024,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1033,    -1,    -1,   882,  1037,    76,   536,    -1,    -1,
      -1,   540,    -1,    -1,    -1,   178,   821,    -1,   823,   548,
      -1,    -1,    -1,   828,    -1,    -1,   831,    -1,    -1,   907,
      -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,   108,   844,
      -1,   846,    -1,    -1,   849,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1086,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   821,    -1,
     823,    -1,   877,    -1,    -1,   828,    -1,    -1,   831,    -1,
      -1,    -1,    -1,   888,    -1,   890,    -1,    -1,    -1,    -1,
      -1,   844,    -1,   846,    -1,    -1,   849,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,   178,    -1,
     273,  1143,  1144,  1145,  1146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   877,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   888,    -1,   890,  1170,  1171,
      -1,    -1,    -1,   672,    -1,    -1,   675,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,   984,
      -1,    -1,    -1,  1061,    -1,   348,   991,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,   267,    -1,    -1,
      25,  1006,    -1,   273,   733,    -1,    -1,    -1,    33,    -1,
     739,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,  1024,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,  1033,    -1,
      -1,   984,  1037,    -1,    -1,    -1,    -1,    62,   991,    -1,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,  1006,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,   425,   333,    -1,    -1,    -1,    -1,    -1,    94,
      95,  1024,    97,    -1,    99,    -1,   101,    -1,   348,   104,
    1033,  1086,    -1,   108,  1037,    -1,   111,    -1,    -1,   114,
     453,    -1,   821,    -1,   823,    -1,   459,    -1,    -1,   828,
      -1,    -1,   831,   225,   226,   227,   228,   229,   230,    -1,
     473,    -1,   475,    -1,    -1,   844,    -1,   846,    -1,    -1,
     849,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1086,    -1,    -1,    -1,    -1,  1143,  1144,
    1145,  1146,    -1,    -1,    -1,   170,    -1,    -1,   877,    -1,
      -1,    -1,    -1,    -1,   276,   425,    -1,    -1,    -1,   888,
     282,   890,    -1,    -1,    -1,  1170,  1171,    -1,    -1,    -1,
      -1,    -1,    -1,   536,    -1,    -1,    -1,   540,    -1,    -1,
      -1,    -1,   304,   453,    -1,   548,    -1,    -1,    -1,   459,
    1143,  1144,  1145,  1146,    22,    -1,    -1,    -1,    26,    27,
     322,   323,    -1,   473,    -1,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,  1170,  1171,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    81,    -1,   984,    84,    -1,    86,    -1,
      88,    -1,   991,    -1,    -1,    -1,   536,    -1,    22,    -1,
     540,   167,    26,    27,   170,    -1,   172,  1006,   548,   107,
      -1,    35,   110,    37,    38,   113,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,  1024,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1033,    -1,   428,    -1,  1037,   672,
      -1,    -1,   675,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      84,    -1,    86,    22,    88,   163,   458,    26,    27,    -1,
     462,   169,   464,    -1,    -1,   467,    35,    -1,    37,    38,
      -1,    -1,    -1,   107,    43,    -1,   110,  1086,    -1,   113,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     733,    -1,    -1,    -1,    -1,    -1,   739,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    84,    -1,    86,    -1,    88,
      -1,    -1,   672,    -1,    -1,   675,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,  1143,  1144,  1145,  1146,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1170,  1171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,   821,    -1,
     823,    26,    27,   733,    -1,   828,    -1,    -1,   831,   739,
      35,    -1,    37,    38,   163,    -1,    -1,    -1,    43,    -1,
     169,   844,    -1,   846,    -1,    50,   849,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    -1,   877,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    88,    -1,   888,    -1,   890,   650,   651,
      -1,    -1,    -1,    -1,   656,   657,    -1,    -1,   660,   661,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   821,    -1,   823,    -1,    -1,    -1,    -1,   828,    -1,
      -1,   831,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   844,    -1,   846,    -1,    -1,   849,
      22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,   877,    50,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   888,    19,
     890,   984,    -1,    -1,    -1,    -1,    -1,    -1,   991,    -1,
      -1,    31,    32,    75,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    84,  1006,    86,    -1,    88,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1024,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,
    1033,   113,    -1,    -1,  1037,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   806,   807,    -1,    87,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,   818,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   830,    -1,
      -1,   833,    -1,   835,    -1,   837,    -1,    -1,   118,    -1,
      -1,   163,   122,  1086,    -1,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1006,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,  1024,   155,   156,   157,   158,   159,
     160,   161,    -1,  1033,   164,   165,   166,  1037,   168,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
    1143,  1144,  1145,  1146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     0,     1,    -1,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,  1170,  1171,    14,
      15,    16,    17,    18,    -1,    20,  1086,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    -1,    68,    69,    70,    -1,    72,    -1,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,   101,   102,    -1,   104,
     105,   106,   107,   108,  1026,   110,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,     1,
     175,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    -1,    -1,    75,    76,    77,    78,    -1,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,   101,
     102,    -1,   104,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,   168,   169,   170,    -1,
     172,    -1,     1,   175,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    -1,    68,
      69,    70,    -1,    72,    -1,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    90,    -1,    92,    93,    94,    95,    96,    97,    98,
      99,    -1,   101,   102,    -1,   104,   105,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,   168,
     169,   170,    -1,   172,    -1,     1,   175,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    -1,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    92,    93,    94,    95,
      96,    97,    98,    99,    -1,   101,   102,    -1,   104,   105,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,
     166,    -1,   168,   169,   170,    -1,   172,    -1,     1,   175,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    -1,    68,    69,    70,    -1,    72,
      -1,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    92,
      93,    94,    95,    96,    97,    98,    99,    -1,   101,   102,
      -1,   104,   105,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
     163,    -1,    -1,   166,    -1,   168,   169,   170,    -1,   172,
      -1,     1,   175,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    -1,    72,    -1,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,   101,   102,    -1,   104,   105,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,   163,    -1,    -1,   166,    -1,   168,   169,
     170,    -1,   172,    -1,     1,   175,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    -1,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,   101,   102,    -1,   104,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,
      -1,   168,   169,   170,    -1,   172,    -1,     1,   175,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    -1,    68,    69,    70,    -1,    72,    -1,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    92,    93,
      94,    95,    96,    97,    98,    99,    -1,   101,   102,    -1,
     104,   105,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,
       1,   175,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    -1,    -1,    75,    76,    77,    78,    -1,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    90,
      -1,    92,    93,    94,    95,    96,    97,    98,    99,    -1,
     101,   102,    -1,   104,   105,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,    -1,     1,   175,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      68,    69,    70,    -1,    72,    -1,    -1,    75,    76,    77,
      78,    -1,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    92,    93,    94,    95,    96,    97,
      98,    99,    -1,   101,   102,    -1,   104,   105,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,    -1,    -1,   117,
      -1,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,   172,    -1,     1,   175,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    -1,    68,    69,    70,    -1,    72,    -1,    -1,
      75,    76,    77,    78,    -1,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,   101,   102,    -1,   104,
     105,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,     1,
     175,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    -1,    -1,    75,    76,    77,    78,    -1,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    90,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,   101,
     102,    -1,   104,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,   163,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,    -1,     1,   175,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    -1,    68,
      69,    70,    -1,    72,    -1,    -1,    75,    76,    77,    78,
      -1,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    90,    -1,    92,    93,    94,    95,    96,    97,    98,
      99,    -1,   101,   102,    -1,   104,   105,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,     1,   175,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    -1,    -1,    75,
      76,    77,    78,    -1,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    92,    93,    94,    95,
      96,    97,    98,    99,    -1,   101,   102,    -1,   104,   105,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,    -1,     1,   175,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    -1,    68,    69,    70,    -1,    72,
      -1,    -1,    75,    76,    77,    78,    -1,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    92,
      93,    94,    95,    96,    97,    98,    99,    -1,   101,   102,
      -1,   104,   105,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
     163,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,     1,   175,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    -1,    72,    -1,    -1,    75,    76,    77,    78,    -1,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,   101,   102,    -1,   104,   105,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,     1,   175,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    -1,    -1,    75,    76,
      77,    78,    -1,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,   101,   102,    -1,   104,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,     1,   175,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    -1,    68,    69,    70,    -1,    72,    -1,
      -1,    75,    76,    77,    78,    -1,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    92,    93,
      94,    95,    96,    97,    98,    99,    -1,   101,   102,    -1,
     104,   105,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,
       1,   175,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,   173,     1,   175,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,   172,    -1,     3,   175,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    26,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      77,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,    -1,   175,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,    -1,
     104,   105,   106,    -1,   108,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,
      -1,   175,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,   105,   106,    -1,   108,   109,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,
     171,   172,    -1,    -1,   175,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,    -1,   101,    -1,    -1,   104,   105,   106,    -1,
     108,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,   171,   172,    -1,     3,   175,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      47,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,     3,   175,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,   105,
     106,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,   175,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,    -1,
      -1,   104,   105,   106,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,     3,   175,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,   105,   106,    -1,   108,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,    -1,     3,   175,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,    -1,     3,   175,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    78,    -1,
      80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,     3,   175,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,   175,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,   175,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,    -1,    -1,   104,
     105,   106,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
     175,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,   169,   170,    -1,
     172,    -1,    -1,   175,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,   173,    -1,   175,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,   173,     3,   175,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,
     175,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,
      -1,   175,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,    -1,     3,   175,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,     3,   175,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,   173,     3,   175,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
     108,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,   172,    -1,    -1,   175,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,
     175,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,
      -1,   175,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,   175,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,   172,    -1,     3,   175,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,     3,   175,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,   175,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,
     175,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,
       3,   175,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,     3,   175,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,    -1,     3,   175,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,    -1,     3,   175,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,     3,   175,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,     3,   175,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,   172,    -1,     3,   175,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,     3,   175,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,   175,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,
     175,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,
       3,   175,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,     3,   175,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    12,   147,    -1,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,   157,    -1,    25,   160,   161,
     162,    -1,    -1,    -1,   166,    33,    -1,    35,   170,    -1,
     172,    39,    -1,   175,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    97,
      -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,    -1,    -1,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,    -1,   145,    -1,    -1,
      -1,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,    -1,   161,    -1,    -1,   164,   165,   166,   167,
      -1,     3,   170,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,    -1,   114,    -1,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   167,   161,
     162,   170,    -1,   172,   166,    -1,   168,    -1,   170,    -1,
     172,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
      -1,    -1,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,    -1,    -1,    -1,    -1,   168,    -1,   170,   171,
     172,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,   103,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,    -1,   114,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,   140,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
     162,   163,    -1,    -1,    -1,    -1,   168,    33,   170,    35,
     172,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,     3,   140,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,   162,    -1,    -1,    -1,
      -1,    -1,   168,    33,   170,    35,   172,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
     140,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,   162,    -1,    -1,    -1,    -1,    -1,   168,    33,
     170,    35,   172,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,     3,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    12,    -1,
     114,    -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,   140,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    68,    69,   170,    -1,   172,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      94,    95,    -1,    97,    -1,    99,    19,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    31,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,   125,   126,   127,   128,    -1,    -1,   131,   132,   133,
     134,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
      -1,   145,    -1,    -1,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,    -1,   161,    -1,    -1,
     164,   165,   166,   167,    87,    12,    -1,    -1,    91,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
     123,   124,   125,   126,   127,   128,    -1,    -1,   131,   132,
     133,   134,    -1,   136,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,    -1,
      87,   164,   165,   166,    91,   168,    -1,    12,    -1,    -1,
      -1,    -1,   175,   100,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,   128,    -1,    -1,   131,   132,   133,   134,    -1,   136,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      -1,   168,    87,    12,    -1,    -1,    91,    -1,   175,    -1,
      19,    -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,    87,   164,
     165,   166,    91,   168,    -1,    12,    -1,    -1,    -1,    -1,
     175,   100,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,   126,   127,   128,
      -1,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,   168,
      87,    12,    -1,    -1,    91,    -1,   175,    -1,    19,    -1,
      -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,   128,    -1,    -1,   131,   132,   133,   134,    -1,   136,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    -1,    87,   164,   165,   166,
      91,   168,    -1,    12,    -1,    -1,    -1,    -1,   175,   100,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,    -1,    -1,
     131,   132,   133,   134,    -1,   136,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    -1,   168,    87,    12,
      -1,    -1,    91,    -1,   175,    -1,    19,    -1,    -1,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,   126,   127,   128,
      -1,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    87,   164,   165,   166,    91,   168,
      -1,    12,    -1,    -1,    -1,    -1,   175,   100,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
     123,   124,   125,   126,   127,   128,    -1,    -1,   131,   132,
     133,   134,    -1,   136,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,   168,    87,    -1,    -1,    -1,
      91,    -1,   175,    12,    -1,    -1,    -1,    -1,    -1,   100,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,    47,    -1,
     131,   132,   133,   134,    -1,   136,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    -1,   168,    87,    12,
      -1,    -1,    91,    -1,   175,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,   122,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    87,   164,   165,   166,    91,   168,
      -1,    -1,    12,    -1,    -1,    -1,   175,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    71,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,   168,    -1,    87,    -1,    -1,
      -1,    91,   175,    -1,    -1,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    71,   155,   156,   157,   158,   159,
     160,   161,    -1,   163,   164,   165,   166,    -1,    -1,    -1,
      87,    12,    -1,    -1,    91,   175,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    -1,    87,   164,   165,   166,
      91,    -1,    -1,    12,    -1,    -1,    -1,    -1,   175,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,   118,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    -1,   168,    87,    12,
      -1,    -1,    91,    -1,   175,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,   122,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    87,   164,   165,   166,    91,   168,
      12,    -1,    -1,    -1,    -1,    -1,   175,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,    87,    12,    -1,    -1,    91,
      -1,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,    87,   164,   165,   166,    91,   168,    12,    -1,    -1,
      -1,    -1,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,    87,    12,    -1,    -1,    91,    -1,    -1,   175,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,    87,   164,
     165,   166,    91,    -1,    -1,    12,    -1,    -1,    -1,    -1,
     175,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,   168,
      87,    12,    -1,    -1,    91,    -1,   175,    -1,    19,    -1,
      -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    -1,    87,   164,   165,   166,
      91,   168,    -1,    -1,    12,    -1,    -1,    -1,   175,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,   118,    36,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    71,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,    87,
      12,    -1,   173,    91,   175,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      31,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    87,   164,   165,   166,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    12,   175,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,   122,   164,   165,   166,    -1,   168,    -1,    -1,    -1,
      -1,    87,    -1,   175,    -1,    91,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    12,   163,   164,   165,   166,   122,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      31,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    12,   163,   164,   165,
     166,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,   163,   164,   165,   166,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      87,    -1,   138,   139,    91,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    12,   163,   164,   165,
     166,    -1,    -1,    19,   121,   122,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    12,    -1,   164,   165,   166,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,   138,   139,    91,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,   175,
      87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   122,    -1,   164,   165,   166,
      -1,    -1,    12,    -1,   171,    -1,    -1,    -1,   175,    19,
      -1,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,    31,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,   175,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    91,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,   118,   164,   165,   166,   122,    -1,    -1,
      -1,    -1,    87,    -1,    -1,   175,    91,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   122,    -1,   164,
     165,   166,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,
     175,    19,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    31,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,   163,   164,
     165,   166,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     175,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    91,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   122,    -1,   164,   165,   166,    87,
      -1,    -1,    -1,    91,    -1,    -1,    -1,   175,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   122,    -1,   164,   165,   166,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   175,    19,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      31,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
      -1,    -1,    12,   171,    -1,    -1,    -1,   175,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   121,   122,   164,   165,   166,    -1,    87,    -1,    -1,
     171,    91,    -1,    -1,   175,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    12,    -1,   164,   165,   166,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    91,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    87,    -1,    -1,
      -1,    91,    -1,    -1,    -1,   175,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    -1,    -1,    12,
      -1,    -1,    -1,   173,    -1,   175,    19,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    31,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   175,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   100,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,   175,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   175,    19,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    31,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    -1,    -1,    12,    -1,   171,    -1,    -1,    -1,   175,
      19,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    31,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    12,    -1,    -1,    -1,   173,    -1,   175,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   100,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    87,    -1,
      -1,    -1,    91,    -1,    -1,    -1,   175,    -1,    -1,   138,
     139,   100,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   175,    19,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    31,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   175,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   100,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     122,    -1,   164,   165,   166,    87,    -1,    -1,    -1,    91,
      -1,    -1,    -1,   175,    -1,    -1,   138,   139,   100,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     122,    -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   175,    19,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    31,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   175,    19,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    91,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   122,    -1,   164,
     165,   166,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,
     175,    -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   122,    -1,   164,
     165,   166,    -1,    -1,    12,    -1,    -1,    -1,   173,    -1,
     175,    19,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    31,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,    22,   164,
     165,   166,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      31,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      84,    -1,    86,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
     138,   139,    -1,   141,   142,   143,    87,   145,   146,   147,
      91,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    87,    -1,    -1,   163,
      91,    -1,    -1,    -1,    -1,   169,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,    -1,    -1,   155,   156,   157,   158,   159,   160,
     161,   122,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,    -1,    -1,   155,   156,   157,   158,   159,     1,
     161,     3,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    17,    18,    -1,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,     3,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      62,    -1,    -1,    -1,    25,    -1,    68,    69,    -1,    -1,
      -1,    -1,    33,    -1,    76,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    85,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    94,    95,    -1,    97,    -1,    99,    -1,   101,
      -1,    62,   104,    -1,    -1,    -1,   108,    68,    69,   111,
      -1,    -1,   114,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    97,    -1,    99,    -1,
     101,    17,    18,   104,    20,    -1,    -1,   108,    -1,    25,
     111,    -1,    -1,   114,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,     3,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    62,    -1,    -1,    -1,
      25,    -1,    68,    69,    -1,    -1,    -1,    -1,    33,    -1,
      76,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    85,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    94,    95,
      -1,    97,    -1,    99,    -1,   101,    -1,    62,   104,    -1,
      -1,    -1,   108,    68,    69,   111,    -1,    -1,   114,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    94,
      95,    -1,    97,    -1,    99,    -1,   101,    17,    18,   104,
      20,    -1,    -1,   108,    -1,    25,   111,    -1,    -1,   114,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,     3,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    62,    -1,    -1,    -1,    25,    -1,    68,    69,
      -1,    -1,    -1,    -1,    33,    -1,    76,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    85,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    94,    95,    -1,    97,    -1,    99,
      -1,   101,    -1,    62,   104,    -1,    -1,    -1,   108,    68,
      69,   111,    -1,    -1,   114,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    97,    -1,
      99,    -1,   101,    17,    18,   104,    20,    -1,    -1,   108,
      -1,    25,   111,    -1,    -1,   114,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    -1,    97,    -1,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   180,   181,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    48,
      49,    50,    52,    55,    56,    57,    58,    59,    61,    62,
      63,    65,    66,    68,    69,    70,    75,    76,    77,    78,
      79,    80,    81,    84,    85,    86,    88,    89,    90,    92,
      93,    94,    95,    96,    97,    98,    99,   101,   102,   104,
     105,   106,   107,   108,   110,   111,   113,   114,   117,   119,
     122,   138,   139,   140,   141,   142,   147,   157,   160,   161,
     162,   163,   166,   168,   170,   172,   175,   182,   183,   184,
     185,   186,   187,   190,   192,   198,   199,   200,   203,   204,
     208,   210,   213,   214,   216,   217,   218,   223,   224,   226,
     228,   231,   232,   233,   234,   235,   236,   240,   241,   244,
     245,   248,   249,   252,   255,   256,   282,   285,   286,   306,
     307,   308,   309,   310,   311,   312,   320,   321,   322,   323,
     324,   327,   328,   329,   330,   331,   332,   333,   334,   336,
     337,   338,   339,   340,   163,   184,   324,   118,   313,   314,
       3,   205,    14,    22,    35,    40,    41,    44,    55,    86,
      99,   168,   172,   231,   252,   306,   311,   322,   323,   324,
     327,   329,   330,   313,   324,   107,   285,    88,   205,   184,
     300,   324,   184,   170,     8,    86,   324,   325,     8,    11,
      86,   107,   325,    77,   120,   225,   324,   225,   324,   225,
     324,    26,   286,   324,    27,   113,   227,   324,   191,   170,
       3,    17,    18,    20,    25,    33,    39,    45,    49,    52,
      62,    68,    69,    76,    85,    94,    95,    97,    99,   101,
     104,   108,   111,   114,   207,   209,   207,   207,   253,   170,
     207,   287,   288,    32,   192,   212,   324,   324,    18,    76,
      94,   111,   324,   324,   324,     8,   170,   215,    22,    35,
      38,    86,   216,     4,   276,   299,   324,   105,   106,   163,
     324,   326,   324,   212,   324,   324,   324,    98,   170,   184,
     324,   324,   192,   204,   324,   327,   192,   204,   324,   207,
     283,   324,   227,   324,   324,   324,   324,   324,   324,   324,
       1,   169,   182,   193,   299,   109,   148,   276,   301,   302,
     326,   225,   299,   324,   335,   324,    79,   184,   168,    83,
     188,    46,   112,    55,   207,   207,    53,    72,    82,   271,
     287,   162,   163,   154,   324,    12,    19,    31,    87,    91,
     122,   138,   139,   141,   142,   143,   145,   146,   147,   149,
     150,   151,   152,   153,   155,   156,   157,   158,   159,   160,
     161,   164,   165,   166,   175,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     167,   262,   170,   172,    87,    91,   170,   184,   163,   324,
     324,   324,   207,   299,    55,   168,   192,    47,   313,   283,
     287,   163,   144,   163,   117,   208,   276,   303,   304,   305,
     326,    86,   221,   256,   285,    86,   219,   283,   256,   285,
     207,   170,   212,    32,    47,   212,   118,   212,   316,    32,
      47,   212,   316,   212,   316,    47,   212,   316,    36,    71,
     163,   207,   207,   100,   192,   100,   123,   192,   262,    81,
      84,   189,   303,   170,   170,   192,   184,   170,   264,   109,
     170,   207,   289,   290,     1,   143,   294,    47,   144,   184,
     212,   170,   170,   212,   303,   216,   144,   163,   324,   324,
     163,   168,   212,   170,   303,   163,   237,   163,   237,   163,
     123,   284,   163,   212,   212,   163,   169,   169,   182,   144,
     169,   324,   144,   171,   144,   171,   173,   316,    47,   144,
     173,   316,   121,   144,   173,     8,     1,   169,   193,    64,
     201,   202,   324,   195,   324,   207,   143,   246,   168,   257,
     163,   324,   324,   324,   324,   225,   324,   225,   324,   324,
     324,   324,   324,   324,   324,     3,    20,    33,    62,   101,
     107,   208,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,    67,
     326,   326,   326,   326,   326,   303,   303,   225,   324,   225,
     324,    27,    47,    88,   113,   315,   318,   319,   324,   340,
      32,    47,    32,    47,   100,   170,    47,   173,   207,   225,
     324,   212,   163,   163,   324,   324,   123,   171,   144,   222,
     207,   287,   220,   207,   163,   287,    47,   299,    44,   324,
     225,   324,   170,   212,    44,   324,   225,   324,   212,   212,
     225,   324,   212,   166,   194,   197,   324,   194,   196,   123,
     123,   184,    34,   184,   324,    34,   324,   188,   171,   304,
     207,   229,   230,    27,    47,    51,    74,    77,    88,   107,
     183,   265,   266,   267,   268,   269,   254,   290,   144,   171,
      33,    49,    95,    99,   172,   211,   295,   307,   123,   291,
     324,   288,   324,   324,   171,   276,   324,     1,   242,   305,
     171,    21,   238,   295,   307,   144,   169,   171,   171,   301,
     171,   301,   184,   173,   225,   324,   173,   184,   324,   173,
     324,   173,   324,   169,   169,   207,   144,   163,    13,   146,
     144,   163,    13,    36,    71,   163,   170,   299,   168,     1,
     207,   250,   251,    27,    77,    88,   107,   259,   270,   170,
     163,   163,   163,   163,   163,   163,   171,   173,    47,    88,
     144,   171,    17,    20,    25,    45,    52,    62,    69,    85,
      97,   108,   114,   306,    87,    87,    44,   225,   324,    44,
     225,   324,   304,   225,   324,   170,   313,   313,   163,   276,
     326,   305,   207,   246,   163,   207,   246,   163,   324,   171,
     324,    32,   212,    32,   212,   317,   318,   324,    32,   212,
     316,    32,   212,   316,   212,   316,   212,   316,   144,   163,
      13,   163,   324,   324,    34,   184,    34,    34,   184,   100,
     192,    64,   171,   144,   171,    47,    88,   268,   144,   171,
     170,   207,    27,    77,    88,   107,   272,   171,   290,   294,
       1,   299,    67,   326,   171,   171,   169,    73,   115,   169,
     243,   171,   170,   192,   207,   239,   283,   184,   173,   316,
     173,   316,   184,   121,   201,   208,   168,   324,   109,   324,
     197,   196,   304,     1,   247,   169,   123,   144,   169,    88,
     258,     1,     3,    12,    17,    19,    20,    25,    39,    45,
      52,    54,    62,    68,    69,    85,    97,   101,   104,   108,
     114,   138,   139,   140,   141,   142,   143,   145,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   161,
     164,   165,   166,   167,   170,   206,   207,   209,   260,   261,
     262,   306,   171,   318,   294,   306,   306,   324,    32,    32,
     324,    32,    32,   171,   173,   173,   304,   212,   212,   246,
     168,   246,   168,   212,   100,    44,   324,    44,   324,   144,
     171,   100,    44,   324,   212,    44,   324,   212,   212,   212,
     324,   324,   184,   184,   324,   184,    34,   207,   163,   230,
     192,   207,   267,   290,   143,   298,    88,   294,   291,   173,
      47,   173,   170,   170,    32,   184,   299,   239,   143,   192,
      44,   184,   324,   173,    44,   184,   324,   173,   324,   194,
      13,    36,    71,    36,    71,   163,   163,   171,   169,    81,
      84,   169,   183,   214,   324,   251,   272,   170,   263,   324,
     138,   146,   263,   263,   291,   100,    44,    44,   100,    44,
      44,    44,    44,   171,   168,   247,   168,   247,   324,   324,
     324,   318,   324,   324,   324,    13,    34,   184,   163,   298,
     171,   172,   211,   276,   297,   307,   148,   277,   291,    60,
     116,   278,   324,   295,   307,   303,   303,   184,   212,   171,
     324,   324,   184,   324,   184,   169,   109,   324,   197,   196,
     197,   196,   163,   214,     1,   143,   293,   266,   171,     3,
     101,   261,   262,   324,   324,   324,   324,   324,   324,   247,
     169,   247,   169,   100,   100,   100,   100,   324,   184,   277,
     291,   298,   173,   299,   276,   324,     3,    90,   101,   279,
     280,   281,   324,   192,   213,   275,   173,   171,   171,   192,
     100,   100,   163,   163,   163,   163,   172,   211,   292,   307,
     103,   273,   171,   263,   263,   100,   100,   100,   100,   100,
     100,   169,   169,   324,   324,   324,   324,   277,   291,   276,
     296,   297,   307,    47,   173,   281,   116,   144,   123,   149,
     151,   152,   155,   156,    60,   307,   162,   162,   324,   324,
       1,   173,   299,   278,   324,   296,   297,   324,   280,   281,
     281,   281,   281,   281,   281,   279,   173,   292,   307,   173,
     163,   274,   275,   173,   292,   307,   296
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

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, context, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (YYID (N))                                                     \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (YYID (0))
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

__attribute__((__unused__))
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
#else
static unsigned
yy_location_print_ (yyo, yylocp)
    FILE *yyo;
    YYLTYPE const * const yylocp;
#endif
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += fprintf (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += fprintf (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += fprintf (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += fprintf (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += fprintf (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

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
  FILE *yyo = yyoutput;
  YYUSE (yyo);
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
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
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
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

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

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

       Refer to the stacks through separate pointers, to allow yyoverflow
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
    return YY_NULL;
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
yypush_parse (yypstate *yyps, int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE *yypushed_loc, ParserContext* context)
#else
int
yypush_parse (yyps, yypushed_char, yypushed_val, yypushed_loc, context)
    yypstate *yyps;
    int yypushed_char;
    YYSTYPE const *yypushed_val;
    YYLTYPE *yypushed_loc;
    ParserContext* context;
#endif
{
/* The lookahead symbol.  */
int yychar;


#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
static YYSTYPE yyval_default;
# define YY_INITIAL_VALUE(Value) = Value
#endif
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc = yyloc_default;


  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = *yypushed_loc;
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
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
/* Line 1792 of yacc.c  */
#line 512 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 518 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 525 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 530 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 554 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 555 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 556 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 557 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 558 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 559 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 560 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 562 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 563 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 564 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 565 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 566 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 567 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 568 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 569 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 574 "chapel.ypp"
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
/* Line 1792 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 592 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 597 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 605 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 611 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 615 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 617 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 622 "chapel.ypp"
    {
    (yylsp[(1) - (1)]).comment = context->latestComment;
    context->latestComment = NULL;
  }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 627 "chapel.ypp"
    {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[(6) - (7)].pch), (yyvsp[(3) - (7)].b), (yyvsp[(4) - (7)].b), (yylsp[(1) - (7)]).comment);
 }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 643 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 644 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 649 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 650 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 655 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 660 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 665 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 669 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 678 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 683 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 709 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 718 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 728 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].ponlylist), (yyvsp[(1) - (4)].b)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), true, (yyvsp[(1) - (8)].b)); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), new UnresolvedSymExpr("_"), (yyvsp[(7) - (8)].ponlylist), true, (yyvsp[(1) - (8)].b)); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), false, (yyvsp[(1) - (8)].b)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), new UnresolvedSymExpr("_"), (yyvsp[(7) - (8)].ponlylist), false, (yyvsp[(1) - (8)].b)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(3) - (4)].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[(1) - (4)].b));}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 749 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist)); }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pimportstmt)); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (3)].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[(3) - (3)].pimportstmt)); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 764 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), new SymExpr(gNoInit), "="); }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 780 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 781 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 785 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 786 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 789 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 790 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 793 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 795 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 796 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 797 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 798 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 810 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 811 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 822 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 823 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 824 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 825 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 826 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 827 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 828 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 829 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 830 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 831 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 832 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 833 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 834 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 835 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 836 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 837 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 838 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 839 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 840 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 844 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 845 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 846 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 847 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 848 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 849 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 850 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 851 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 852 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 853 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 854 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 855 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 862 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 863 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 864 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 869 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 870 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 875 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 892 "chapel.ypp"
    { parsingPrivate=(yyvsp[(2) - (3)].b); applyPrivateToBlock((yyvsp[(3) - (3)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(3) - (3)].pblockstmt); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 907 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 908 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 909 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 914 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 919 "chapel.ypp"
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
/* Line 1792 of yacc.c  */
#line 929 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 934 "chapel.ypp"
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
/* Line 1792 of yacc.c  */
#line 945 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 950 "chapel.ypp"
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
/* Line 1792 of yacc.c  */
#line 960 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 965 "chapel.ypp"
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
/* Line 1792 of yacc.c  */
#line 976 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 980 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 985 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 993 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 1004 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 1010 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 1011 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 1012 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), true, false); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), true, false); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1035 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1036 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1038 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1044 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1050 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1056 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1062 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1069 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1076 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1080 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1090 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1091 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1100 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1102 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1104 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1106 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pexpr) = buildIfVar((yyvsp[(2) - (4)].pch), (yyvsp[(4) - (4)].pexpr), false); }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1112 "chapel.ypp"
    { (yyval.pexpr) = buildIfVar((yyvsp[(2) - (4)].pch), (yyvsp[(4) - (4)].pexpr), true);  }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt))); }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1119 "chapel.ypp"
    { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (3)].pch), ls, (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1126 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[(1) - (1)].pch), INTENT_TYPE); }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1141 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1142 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1143 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1147 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1148 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1150 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[(1) - (1)].pch)); }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1156 "chapel.ypp"
    { (yyval.pch) = "none"; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1157 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pch) = "false"; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1159 "chapel.ypp"
    { (yyval.pch) = "true"; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1169 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1174 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), NULL)); }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (4)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (4)].pch), act, NULL)); }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1179 "chapel.ypp"
    { (yyvsp[(5) - (7)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (7)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (7)].pch), (yyvsp[(5) - (7)].pcallexpr), NULL)); }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1185 "chapel.ypp"
    { (yyval.pexpr) = IfcConstraint::build((yyvsp[(2) - (5)].pch), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1187 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (3)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (3)].pch), act); }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1190 "chapel.ypp"
    { (yyvsp[(5) - (6)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (6)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pcallexpr)); }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1195 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1199 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1201 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1207 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1208 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1214 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1219 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1227 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1241 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1243 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1250 "chapel.ypp"
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

  case 286:
/* Line 1792 of yacc.c  */
#line 1260 "chapel.ypp"
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

  case 287:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1283 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
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

  case 296:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1329 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1338 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1346 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1350 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1367 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1372 "chapel.ypp"
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

  case 306:
/* Line 1792 of yacc.c  */
#line 1394 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1397 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1401 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1409 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1419 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1426 "chapel.ypp"
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

      if ((yyvsp[(2) - (10)].procIterOp) == ProcIterOp_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }
      if ((yyvsp[(2) - (10)].procIterOp) == ProcIterOp_OP) {
        fn->addFlag(FLAG_OPERATOR);
        if (fn->_this != NULL) {
          updateOpThisTagOrErr(fn);
          setupTypeIntentArg(toArgSymbol(fn->_this));
        }
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[(4) - (10)].pfnsymbol), (yyvsp[(6) - (10)].retTag), (yyvsp[(7) - (10)].pexpr), (yyvsp[(8) - (10)].b), (yyvsp[(9) - (10)].lifetimeAndWhere).where, (yyvsp[(9) - (10)].lifetimeAndWhere).lifetime, (yyvsp[(10) - (10)].pblockstmt), (yylsp[(2) - (10)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1459 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1463 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1472 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1492 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pch) = ":"; }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1548 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(1) - (1)].pfnsymbol); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pexpr))); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1564 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1568 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1576 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1577 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1578 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1586 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1587 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1588 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_PROC; }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_ITER; }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_OP; }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1600 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1602 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1613 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1619 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1627 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1637 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1639 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1641 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1643 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1652 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1656 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1657 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1658 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1659 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1660 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1661 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1667 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1676 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1683 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1696 "chapel.ypp"
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

  case 429:
/* Line 1792 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1720 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1722 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1729 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1735 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1744 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1754 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1759 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1761 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1763 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1768 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1770 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1772 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1779 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1798 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1804 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1811 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1812 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1813 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1815 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1820 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1822 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1824 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1845 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1849 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1853 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1861 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1867 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1868 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1869 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1874 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1876 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1882 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1884 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1886 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1890 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1891 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1892 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1893 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1894 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1900 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1901 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1902 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1903 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1907 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1908 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1912 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1913 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1918 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1919 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1923 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1928 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1929 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1933 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1935 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1936 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1940 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1941 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1953 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1955 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1957 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1968 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1970 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1972 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1974 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1976 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1978 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1980 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1989 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 2000 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 2002 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 2007 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 2009 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 2011 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 2013 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 2015 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 2017 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 2019 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 2021 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2023 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2025 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 2027 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2029 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2031 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 2038 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 2044 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2050 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2056 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2065 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2090 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2094 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2099 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2103 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2104 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2108 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2112 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2113 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2118 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2122 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2126 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2132 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2133 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2134 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2135 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2136 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2137 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2143 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2148 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2150 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2154 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2158 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2162 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2170 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2175 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2181 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2187 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2194 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2204 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2221 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2223 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2225 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2227 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), false, true); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2242 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2244 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2246 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr), true); }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2248 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2252 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 607:
/* Line 1792 of yacc.c  */
#line 2253 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 608:
/* Line 1792 of yacc.c  */
#line 2257 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 609:
/* Line 1792 of yacc.c  */
#line 2258 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 610:
/* Line 1792 of yacc.c  */
#line 2259 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 615:
/* Line 1792 of yacc.c  */
#line 2275 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 616:
/* Line 1792 of yacc.c  */
#line 2276 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 617:
/* Line 1792 of yacc.c  */
#line 2277 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 618:
/* Line 1792 of yacc.c  */
#line 2278 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 620:
/* Line 1792 of yacc.c  */
#line 2283 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 621:
/* Line 1792 of yacc.c  */
#line 2284 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 622:
/* Line 1792 of yacc.c  */
#line 2285 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 623:
/* Line 1792 of yacc.c  */
#line 2289 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 624:
/* Line 1792 of yacc.c  */
#line 2290 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 625:
/* Line 1792 of yacc.c  */
#line 2291 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 626:
/* Line 1792 of yacc.c  */
#line 2292 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 627:
/* Line 1792 of yacc.c  */
#line 2293 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 628:
/* Line 1792 of yacc.c  */
#line 2301 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 629:
/* Line 1792 of yacc.c  */
#line 2302 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 630:
/* Line 1792 of yacc.c  */
#line 2303 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 631:
/* Line 1792 of yacc.c  */
#line 2304 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 632:
/* Line 1792 of yacc.c  */
#line 2308 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 633:
/* Line 1792 of yacc.c  */
#line 2309 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 634:
/* Line 1792 of yacc.c  */
#line 2313 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 635:
/* Line 1792 of yacc.c  */
#line 2314 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 638:
/* Line 1792 of yacc.c  */
#line 2320 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 639:
/* Line 1792 of yacc.c  */
#line 2321 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 640:
/* Line 1792 of yacc.c  */
#line 2322 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 641:
/* Line 1792 of yacc.c  */
#line 2323 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 642:
/* Line 1792 of yacc.c  */
#line 2324 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 643:
/* Line 1792 of yacc.c  */
#line 2325 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr),
                                            new SymExpr(gTrue)); }
    break;

  case 644:
/* Line 1792 of yacc.c  */
#line 2327 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr),
                                                   new SymExpr(gTrue)); }
    break;

  case 645:
/* Line 1792 of yacc.c  */
#line 2329 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 646:
/* Line 1792 of yacc.c  */
#line 2330 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 647:
/* Line 1792 of yacc.c  */
#line 2332 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 648:
/* Line 1792 of yacc.c  */
#line 2336 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 649:
/* Line 1792 of yacc.c  */
#line 2343 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 650:
/* Line 1792 of yacc.c  */
#line 2344 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 651:
/* Line 1792 of yacc.c  */
#line 2348 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 652:
/* Line 1792 of yacc.c  */
#line 2349 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 653:
/* Line 1792 of yacc.c  */
#line 2350 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 654:
/* Line 1792 of yacc.c  */
#line 2351 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 655:
/* Line 1792 of yacc.c  */
#line 2352 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 656:
/* Line 1792 of yacc.c  */
#line 2353 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 657:
/* Line 1792 of yacc.c  */
#line 2354 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 658:
/* Line 1792 of yacc.c  */
#line 2355 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 659:
/* Line 1792 of yacc.c  */
#line 2356 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 660:
/* Line 1792 of yacc.c  */
#line 2357 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 661:
/* Line 1792 of yacc.c  */
#line 2358 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 662:
/* Line 1792 of yacc.c  */
#line 2359 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 663:
/* Line 1792 of yacc.c  */
#line 2360 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 664:
/* Line 1792 of yacc.c  */
#line 2361 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 665:
/* Line 1792 of yacc.c  */
#line 2362 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 666:
/* Line 1792 of yacc.c  */
#line 2363 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 667:
/* Line 1792 of yacc.c  */
#line 2364 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 668:
/* Line 1792 of yacc.c  */
#line 2365 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 669:
/* Line 1792 of yacc.c  */
#line 2366 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 670:
/* Line 1792 of yacc.c  */
#line 2367 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 671:
/* Line 1792 of yacc.c  */
#line 2368 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 672:
/* Line 1792 of yacc.c  */
#line 2369 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 673:
/* Line 1792 of yacc.c  */
#line 2370 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr),
                                               new SymExpr(gTrue)); }
    break;

  case 674:
/* Line 1792 of yacc.c  */
#line 2375 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 675:
/* Line 1792 of yacc.c  */
#line 2376 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 676:
/* Line 1792 of yacc.c  */
#line 2377 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 677:
/* Line 1792 of yacc.c  */
#line 2378 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 678:
/* Line 1792 of yacc.c  */
#line 2379 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 679:
/* Line 1792 of yacc.c  */
#line 2380 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 680:
/* Line 1792 of yacc.c  */
#line 2381 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 681:
/* Line 1792 of yacc.c  */
#line 2385 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 682:
/* Line 1792 of yacc.c  */
#line 2386 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 683:
/* Line 1792 of yacc.c  */
#line 2387 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 684:
/* Line 1792 of yacc.c  */
#line 2388 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 685:
/* Line 1792 of yacc.c  */
#line 2392 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 686:
/* Line 1792 of yacc.c  */
#line 2393 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 687:
/* Line 1792 of yacc.c  */
#line 2394 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 688:
/* Line 1792 of yacc.c  */
#line 2395 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 689:
/* Line 1792 of yacc.c  */
#line 2400 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 690:
/* Line 1792 of yacc.c  */
#line 2401 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 691:
/* Line 1792 of yacc.c  */
#line 2402 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 692:
/* Line 1792 of yacc.c  */
#line 2403 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 693:
/* Line 1792 of yacc.c  */
#line 2404 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 694:
/* Line 1792 of yacc.c  */
#line 2405 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 695:
/* Line 1792 of yacc.c  */
#line 2406 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 11223 "bison-chapel.cpp"
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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

#if !defined yyoverflow || YYERROR_VERBOSE
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


