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
#define YYLAST   20371

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  179
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  161
/* YYNRULES -- Number of rules.  */
#define YYNRULES  693
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1244

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
     546,   552,   558,   561,   567,   571,   578,   585,   590,   596,
     602,   606,   610,   617,   623,   630,   636,   643,   647,   652,
     656,   661,   667,   674,   680,   687,   691,   696,   700,   705,
     712,   720,   727,   735,   740,   746,   751,   757,   762,   767,
     771,   778,   784,   789,   793,   800,   806,   813,   819,   828,
     836,   841,   846,   853,   857,   859,   863,   865,   867,   869,
     871,   873,   875,   877,   879,   881,   883,   885,   887,   889,
     891,   893,   895,   897,   899,   901,   903,   910,   915,   923,
     929,   933,   940,   943,   947,   951,   954,   957,   961,   965,
     966,   969,   972,   976,   982,   984,   988,   992,   998,  1004,
    1005,  1008,  1012,  1015,  1019,  1026,  1033,  1035,  1037,  1039,
    1040,  1043,  1044,  1047,  1051,  1057,  1063,  1065,  1067,  1070,
    1074,  1076,  1080,  1081,  1082,  1091,  1092,  1094,  1096,  1097,
    1098,  1109,  1113,  1117,  1123,  1129,  1133,  1135,  1139,  1141,
    1143,  1145,  1147,  1149,  1151,  1153,  1155,  1157,  1159,  1161,
    1163,  1165,  1167,  1169,  1171,  1173,  1175,  1177,  1179,  1181,
    1183,  1185,  1187,  1189,  1191,  1193,  1196,  1198,  1200,  1202,
    1204,  1206,  1208,  1210,  1212,  1214,  1216,  1218,  1220,  1221,
    1225,  1229,  1230,  1232,  1236,  1241,  1247,  1252,  1258,  1265,
    1272,  1273,  1275,  1277,  1279,  1281,  1283,  1286,  1289,  1291,
    1293,  1295,  1296,  1298,  1300,  1303,  1305,  1307,  1309,  1311,
    1313,  1314,  1316,  1319,  1321,  1323,  1325,  1326,  1328,  1330,
    1332,  1334,  1336,  1338,  1340,  1343,  1346,  1347,  1350,  1353,
    1358,  1363,  1365,  1369,  1373,  1377,  1381,  1385,  1389,  1393,
    1396,  1398,  1400,  1404,  1409,  1414,  1417,  1422,  1423,  1426,
    1429,  1431,  1433,  1435,  1438,  1440,  1445,  1449,  1451,  1455,
    1459,  1465,  1467,  1469,  1473,  1475,  1478,  1482,  1483,  1486,
    1489,  1493,  1496,  1501,  1505,  1509,  1514,  1518,  1519,  1522,
    1525,  1528,  1530,  1531,  1534,  1537,  1540,  1542,  1547,  1552,
    1559,  1563,  1564,  1566,  1568,  1572,  1577,  1581,  1586,  1593,
    1594,  1597,  1600,  1603,  1606,  1608,  1610,  1614,  1618,  1620,
    1624,  1626,  1628,  1630,  1634,  1638,  1639,  1641,  1643,  1647,
    1651,  1655,  1657,  1659,  1661,  1663,  1665,  1668,  1670,  1672,
    1674,  1676,  1678,  1680,  1683,  1688,  1693,  1698,  1704,  1707,
    1710,  1712,  1715,  1717,  1720,  1722,  1725,  1727,  1730,  1732,
    1734,  1741,  1748,  1753,  1763,  1773,  1781,  1788,  1795,  1800,
    1810,  1820,  1828,  1833,  1840,  1847,  1857,  1867,  1874,  1876,
    1878,  1880,  1882,  1884,  1886,  1888,  1890,  1894,  1895,  1897,
    1902,  1904,  1908,  1913,  1915,  1919,  1924,  1928,  1932,  1934,
    1936,  1939,  1941,  1944,  1946,  1948,  1952,  1954,  1957,  1960,
    1963,  1966,  1969,  1978,  1987,  1997,  2007,  2012,  2014,  2016,
    2018,  2020,  2022,  2024,  2026,  2028,  2030,  2035,  2039,  2043,
    2047,  2050,  2053,  2056,  2058,  2059,  2061,  2064,  2067,  2069,
    2071,  2073,  2075,  2077,  2079,  2082,  2085,  2087,  2089,  2094,
    2099,  2104,  2108,  2112,  2116,  2120,  2126,  2130,  2135,  2139,
    2144,  2146,  2148,  2150,  2152,  2154,  2156,  2158,  2160,  2162,
    2164,  2166,  2170,  2175,  2179,  2184,  2188,  2193,  2197,  2203,
    2207,  2211,  2215,  2219,  2223,  2227,  2231,  2235,  2239,  2243,
    2247,  2251,  2255,  2259,  2263,  2267,  2271,  2275,  2279,  2283,
    2287,  2291,  2295,  2298,  2301,  2304,  2307,  2310,  2313,  2316,
    2320,  2324,  2328,  2332,  2336,  2340,  2344,  2348,  2350,  2352,
    2354,  2356,  2358,  2360
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     180,     0,    -1,   181,    -1,    -1,   181,   182,    -1,   184,
      -1,   183,   184,    -1,    79,     8,    -1,   183,    79,     8,
      -1,   186,    -1,   190,    -1,   192,    -1,   199,    -1,   200,
      -1,   203,    -1,   214,    -1,   204,    -1,   223,    -1,   226,
      -1,   233,    -1,   228,    -1,   224,    -1,   241,    -1,   235,
      -1,   236,    -1,   240,    -1,   213,    -1,   311,   163,    -1,
      14,   184,    -1,    15,   312,   184,    -1,    16,   205,   163,
      -1,    23,   312,   192,    -1,    28,   205,   163,    -1,    30,
     299,   163,    -1,    57,   207,   184,    -1,    61,   323,   212,
      -1,    61,   212,    -1,    70,   323,   212,    -1,    93,   323,
     212,    -1,    93,   212,    -1,    99,   184,    -1,   119,   323,
     163,    -1,     1,   163,    -1,   187,   188,    64,   207,    -1,
     185,   168,   169,    -1,   185,   168,   193,   169,    -1,   185,
     168,     1,   169,    -1,    -1,    84,    -1,    81,    -1,    -1,
      83,    -1,    -1,    84,    -1,    81,    -1,    -1,    48,   191,
     189,   188,    64,   207,   163,    -1,   168,   169,    -1,   168,
     193,   169,    -1,   168,     1,   169,    -1,   182,    -1,   193,
     182,    -1,   323,    -1,   323,    13,   323,    -1,   194,   144,
     323,    -1,   194,   144,   323,    13,   323,    -1,   323,    -1,
     323,    13,   323,    -1,   323,    13,   109,    -1,   195,   144,
     323,    -1,   195,   144,   323,    13,   323,    -1,   195,   144,
     323,    13,   109,    -1,    -1,   194,    -1,   166,    -1,   194,
      -1,    -1,    84,    -1,    81,    -1,   198,   112,   195,   163,
      -1,   198,   112,   323,    36,   197,   163,    -1,   198,   112,
     323,    13,   323,    36,   197,   163,    -1,   198,   112,   323,
      13,   109,    36,   197,   163,    -1,   198,   112,   323,    71,
     196,   163,    -1,   198,   112,   323,    13,   323,    71,   196,
     163,    -1,   198,   112,   323,    13,   109,    71,   196,   163,
      -1,   198,    46,   202,   163,    -1,   323,    -1,   323,    13,
     208,    -1,   323,   146,   168,   194,   169,    -1,   201,    -1,
     202,   144,   201,    -1,    89,   298,   163,    -1,   326,   262,
     325,   163,    -1,   326,   167,   325,   163,    -1,   326,   135,
     325,   163,    -1,   326,   129,   325,   163,    -1,   326,   130,
     325,   163,    -1,   326,   123,    67,   163,    -1,    -1,     3,
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
     184,    -1,   192,    -1,    90,   163,    -1,    90,   325,   163,
      -1,   163,    -1,   216,    -1,    84,   216,    -1,    -1,    81,
     215,   216,    -1,   256,    -1,   285,    -1,   248,    -1,   281,
      -1,   244,    -1,   217,    -1,   218,    -1,    43,   323,   163,
      -1,    43,   323,    36,   197,   163,    -1,    43,   323,    71,
     196,   163,    -1,    43,   285,    -1,    -1,    38,    86,   219,
     207,   246,   168,   247,   169,    -1,    -1,    38,     8,    86,
     220,   207,   246,   168,   247,   169,    -1,    -1,    37,    86,
     221,   207,   246,   168,   247,   169,    -1,    -1,    37,     8,
      86,   222,   207,   246,   168,   247,   169,    -1,    38,   324,
     256,    -1,    37,   324,   256,    -1,    38,   324,   284,   286,
     163,    -1,    37,   324,   284,   286,   163,    -1,    38,    11,
      -1,    32,   184,   117,   323,   163,    -1,   117,   323,   212,
      -1,    24,   323,    47,   323,   312,   212,    -1,    24,   323,
      47,   225,   312,   212,    -1,    24,   323,   312,   212,    -1,
      40,   323,    47,   323,   212,    -1,    40,   323,    47,   225,
     212,    -1,    40,   323,   212,    -1,    40,   225,   212,    -1,
      40,    77,   207,    47,   323,   212,    -1,    41,   323,    47,
     323,   212,    -1,    41,   323,    47,   323,   315,   212,    -1,
      41,   323,    47,   225,   212,    -1,    41,   323,    47,   225,
     315,   212,    -1,    41,   323,   212,    -1,    41,   323,   315,
     212,    -1,    41,   225,   212,    -1,    41,   225,   315,   212,
      -1,    42,   323,    47,   323,   212,    -1,    42,   323,    47,
     323,   315,   212,    -1,    42,   323,    47,   225,   212,    -1,
      42,   323,    47,   225,   315,   212,    -1,    42,   323,   212,
      -1,    42,   323,   315,   212,    -1,    42,   225,   212,    -1,
      42,   225,   315,   212,    -1,   172,   298,    47,   323,   173,
     184,    -1,   172,   298,    47,   323,   315,   173,   184,    -1,
     172,   298,    47,   225,   173,   184,    -1,   172,   298,    47,
     225,   315,   173,   184,    -1,   172,   298,   173,   184,    -1,
     172,   298,   315,   173,   184,    -1,   172,   225,   173,   184,
      -1,   172,   225,   315,   173,   184,    -1,   120,   170,   298,
     171,    -1,    44,   323,   100,   184,    -1,    44,   323,   192,
      -1,    44,   323,   100,   184,    34,   184,    -1,    44,   323,
     192,    34,   184,    -1,    44,   227,   100,   184,    -1,    44,
     227,   192,    -1,    44,   227,   100,   184,    34,   184,    -1,
      44,   227,   192,    34,   184,    -1,    44,   323,   262,   323,
     100,   184,    -1,    44,   323,   262,   323,   192,    -1,    44,
     323,   262,   323,   100,   184,    34,   184,    -1,    44,   323,
     262,   323,   192,    34,   184,    -1,   113,   207,   123,   323,
      -1,    27,   207,   123,   323,    -1,    56,   207,   170,   229,
     171,   192,    -1,    56,   207,   192,    -1,   230,    -1,   229,
     144,   230,    -1,   207,    -1,     3,    -1,    17,    -1,    52,
      -1,   108,    -1,    85,    -1,    45,    -1,    25,    -1,    20,
      -1,    97,    -1,    62,    -1,    69,    -1,   114,    -1,   232,
      -1,    68,    -1,   101,    -1,    39,    -1,   104,    -1,    33,
      -1,    49,    -1,    55,   207,   170,   303,   171,   163,    -1,
     231,    55,   207,   163,    -1,   231,    55,   207,   170,   303,
     171,   163,    -1,    55,   207,   170,   303,   171,    -1,   231,
      55,   207,    -1,   231,    55,   207,   170,   303,   171,    -1,
      29,   184,    -1,   105,   323,   163,    -1,   106,   323,   163,
      -1,   105,   204,    -1,   106,   204,    -1,   105,   192,   237,
      -1,   106,   192,   237,    -1,    -1,   237,   238,    -1,    21,
     192,    -1,    21,   239,   192,    -1,    21,   170,   239,   171,
     192,    -1,   207,    -1,   207,   143,   323,    -1,   102,   323,
     163,    -1,    92,   323,   168,   242,   169,    -1,    92,   323,
     168,     1,   169,    -1,    -1,   242,   243,    -1,   115,   298,
     212,    -1,    73,   184,    -1,    73,    32,   184,    -1,   245,
     207,   246,   168,   247,   169,    -1,   245,   207,   246,   168,
       1,   169,    -1,    22,    -1,    86,    -1,   110,    -1,    -1,
     143,   298,    -1,    -1,   247,   214,    -1,   247,   183,   214,
      -1,   249,   207,   168,   250,   169,    -1,   249,   207,   168,
       1,   169,    -1,    35,    -1,   251,    -1,   250,   144,    -1,
     250,   144,   251,    -1,   207,    -1,   207,   123,   323,    -1,
      -1,    -1,    58,   253,   264,   254,   271,   293,   277,   274,
      -1,    -1,    50,    -1,    75,    -1,    -1,    -1,   255,   270,
     257,   259,   258,   271,   292,   272,   277,   273,    -1,   269,
     261,   263,    -1,   269,   262,   263,    -1,   269,   260,   146,
     261,   263,    -1,   269,   260,   146,   262,   263,    -1,   269,
       1,   263,    -1,   305,    -1,   170,   323,   171,    -1,   206,
      -1,   139,    -1,   141,    -1,   142,    -1,   140,    -1,   149,
      -1,   159,    -1,   156,    -1,   152,    -1,   155,    -1,   151,
      -1,   161,    -1,   157,    -1,   166,    -1,   145,    -1,   164,
      -1,   165,    -1,   158,    -1,   150,    -1,   138,    -1,    19,
      -1,   153,    -1,    12,    -1,   167,    -1,   154,    -1,    54,
      -1,   143,    -1,   207,   138,    -1,   123,    -1,   134,    -1,
     131,    -1,   133,    -1,   127,    -1,   132,    -1,   128,    -1,
     124,    -1,   125,    -1,   126,    -1,   137,    -1,   136,    -1,
      -1,   170,   265,   171,    -1,   170,   265,   171,    -1,    -1,
     266,    -1,   265,   144,   266,    -1,   267,   207,   297,   290,
      -1,   183,   267,   207,   297,   290,    -1,   267,   207,   297,
     276,    -1,   183,   267,   207,   297,   276,    -1,   267,   170,
     289,   171,   297,   290,    -1,   267,   170,   289,   171,   297,
     276,    -1,    -1,   268,    -1,    47,    -1,    51,    -1,    74,
      -1,    27,    -1,    27,    47,    -1,    27,    88,    -1,    77,
      -1,    88,    -1,   107,    -1,    -1,    77,    -1,    88,    -1,
      27,    88,    -1,    27,    -1,   107,    -1,    82,    -1,    53,
      -1,    72,    -1,    -1,    27,    -1,    27,    88,    -1,    88,
      -1,    77,    -1,   107,    -1,    -1,   103,    -1,   163,    -1,
     274,    -1,   192,    -1,   213,    -1,     4,    -1,   148,    -1,
     148,   323,    -1,   148,   275,    -1,    -1,   116,   323,    -1,
      60,   278,    -1,   116,   323,    60,   278,    -1,    60,   278,
     116,   323,    -1,   279,    -1,   278,   144,   279,    -1,   280,
     123,   280,    -1,   280,   155,   280,    -1,   280,   156,   280,
      -1,   280,   149,   280,    -1,   280,   151,   280,    -1,   280,
     152,   280,    -1,    90,   280,    -1,     3,    -1,   101,    -1,
     107,   282,   163,    -1,    26,   107,   282,   163,    -1,    38,
     107,   282,   163,    -1,   207,   283,    -1,   207,   283,   144,
     282,    -1,    -1,   123,   306,    -1,   123,   294,    -1,    77,
      -1,    27,    -1,    88,    -1,    27,    88,    -1,   113,    -1,
      26,   284,   286,   163,    -1,   284,   286,   163,    -1,   287,
      -1,   286,   144,   287,    -1,   207,   293,   290,    -1,   170,
     289,   171,   293,   290,    -1,   109,    -1,   207,    -1,   170,
     289,   171,    -1,   288,    -1,   288,   144,    -1,   288,   144,
     289,    -1,    -1,   123,    67,    -1,   123,   325,    -1,   172,
     173,   306,    -1,   172,   173,    -1,   172,   298,   173,   306,
      -1,   172,   298,   173,    -1,   172,   173,   291,    -1,   172,
     298,   173,   291,    -1,   172,     1,   173,    -1,    -1,   143,
     306,    -1,   143,   291,    -1,   143,   211,    -1,     1,    -1,
      -1,   143,   306,    -1,   143,   294,    -1,   143,   211,    -1,
       1,    -1,   172,   298,   173,   306,    -1,   172,   298,   173,
     294,    -1,   172,   298,    47,   323,   173,   306,    -1,   172,
       1,   173,    -1,    -1,   306,    -1,   275,    -1,   172,   173,
     295,    -1,   172,   298,   173,   295,    -1,   172,   173,   296,
      -1,   172,   298,   173,   296,    -1,   172,   298,    47,   323,
     173,   295,    -1,    -1,   143,   306,    -1,   143,   275,    -1,
     143,   211,    -1,   143,   296,    -1,   323,    -1,   275,    -1,
     298,   144,   323,    -1,   298,   144,   275,    -1,   323,    -1,
     299,   144,   323,    -1,   109,    -1,   325,    -1,   275,    -1,
     300,   144,   300,    -1,   301,   144,   300,    -1,    -1,   303,
      -1,   304,    -1,   303,   144,   304,    -1,   208,   123,   275,
      -1,   208,   123,   325,    -1,   275,    -1,   325,    -1,   208,
      -1,   210,    -1,   307,    -1,   307,   162,    -1,   162,    -1,
     310,    -1,   326,    -1,   309,    -1,   336,    -1,   335,    -1,
      95,   323,    -1,    49,   170,   302,   171,    -1,    33,   170,
     302,   171,    -1,    98,   170,   302,   171,    -1,    96,    98,
     170,   302,   171,    -1,    14,   323,    -1,    99,   323,    -1,
      76,    -1,    76,   323,    -1,   111,    -1,   111,   323,    -1,
      94,    -1,    94,   323,    -1,    18,    -1,    18,   323,    -1,
      22,    -1,    86,    -1,    40,   323,    47,   323,    32,   323,
      -1,    40,   323,    47,   225,    32,   323,    -1,    40,   323,
      32,   323,    -1,    40,   323,    47,   323,    32,    44,   323,
     100,   323,    -1,    40,   323,    47,   225,    32,    44,   323,
     100,   323,    -1,    40,   323,    32,    44,   323,   100,   323,
      -1,    41,   323,    47,   323,    32,   323,    -1,    41,   323,
      47,   225,    32,   323,    -1,    41,   323,    32,   323,    -1,
      41,   323,    47,   323,    32,    44,   323,   100,   323,    -1,
      41,   323,    47,   225,    32,    44,   323,   100,   323,    -1,
      41,   323,    32,    44,   323,   100,   323,    -1,   172,   298,
     173,   323,    -1,   172,   298,    47,   323,   173,   323,    -1,
     172,   298,    47,   225,   173,   323,    -1,   172,   298,    47,
     323,   173,    44,   323,   100,   323,    -1,   172,   298,    47,
     225,   173,    44,   323,   100,   323,    -1,    44,   323,   100,
     323,    34,   323,    -1,    66,    -1,   310,    -1,   305,    -1,
     329,    -1,   328,    -1,   252,    -1,   321,    -1,   322,    -1,
     319,   154,   323,    -1,    -1,   313,    -1,   118,   170,   314,
     171,    -1,   317,    -1,   314,   144,   317,    -1,   118,   170,
     316,   171,    -1,   317,    -1,   316,   144,   317,    -1,   318,
     305,   293,   290,    -1,   339,    87,   305,    -1,   323,    87,
     305,    -1,    27,    -1,    47,    -1,    27,    47,    -1,    88,
      -1,    27,    88,    -1,   113,    -1,   326,    -1,   319,   154,
     323,    -1,    65,    -1,    65,    76,    -1,    65,    94,    -1,
      65,   111,    -1,    65,    18,    -1,   320,   323,    -1,    65,
      76,   170,   323,   171,   170,   302,   171,    -1,    65,    94,
     170,   323,   171,   170,   302,   171,    -1,    65,    76,   170,
     323,   171,   170,   302,   171,   162,    -1,    65,    94,   170,
     323,   171,   170,   302,   171,   162,    -1,    59,   286,    47,
     323,    -1,   333,    -1,   306,    -1,   308,    -1,   337,    -1,
     338,    -1,   252,    -1,   321,    -1,   322,    -1,   234,    -1,
     170,   148,   323,   171,    -1,   323,   143,   323,    -1,   323,
     147,   323,    -1,   323,   175,   323,    -1,   323,   147,    -1,
     147,   323,    -1,   175,   323,    -1,   147,    -1,    -1,   323,
      -1,   105,   323,    -1,   106,   323,    -1,   323,    -1,   305,
      -1,   328,    -1,   329,    -1,   330,    -1,   326,    -1,   323,
     138,    -1,   307,   162,    -1,   252,    -1,   332,    -1,   327,
     170,   302,   171,    -1,   327,   172,   302,   173,    -1,    80,
     170,   302,   171,    -1,   323,   146,   208,    -1,   323,   146,
     107,    -1,   323,   146,    33,    -1,   323,   146,    62,    -1,
     323,   146,    20,   170,   171,    -1,   170,   300,   171,    -1,
     170,   300,   144,   171,    -1,   170,   301,   171,    -1,   170,
     301,   144,   171,    -1,    39,    -1,   104,    -1,     8,    -1,
       9,    -1,   331,    -1,   332,    -1,     5,    -1,     6,    -1,
       7,    -1,    10,    -1,    68,    -1,   168,   298,   169,    -1,
     168,   298,   144,   169,    -1,   172,   298,   173,    -1,   172,
     298,   144,   173,    -1,   172,   334,   173,    -1,   172,   334,
     144,   173,    -1,   323,   121,   323,    -1,   334,   144,   323,
     121,   323,    -1,   323,   161,   323,    -1,   323,   157,   323,
      -1,   323,   166,   323,    -1,   323,   145,   323,    -1,   323,
     164,   323,    -1,   323,   165,   323,    -1,   323,   158,   323,
      -1,   323,   149,   323,    -1,   323,   159,   323,    -1,   323,
     156,   323,    -1,   323,   152,   323,    -1,   323,   155,   323,
      -1,   323,   151,   323,    -1,   323,   139,   323,    -1,   323,
     141,   323,    -1,   323,   142,   323,    -1,   323,   122,   323,
      -1,   323,   160,   323,    -1,   323,   150,   323,    -1,   323,
      19,   323,    -1,   323,    12,   323,    -1,   323,   153,   323,
      -1,   323,    31,   323,    -1,   161,   323,    -1,   157,   323,
      -1,    63,   323,    -1,    78,   323,    -1,   138,   323,    -1,
     323,   138,    -1,   140,   323,    -1,   323,    87,   323,    -1,
     323,    87,   225,    -1,   339,    87,   323,    -1,   339,    87,
     225,    -1,   323,    91,   323,    -1,   323,    91,   225,    -1,
     339,    91,   323,    -1,   339,    91,   225,    -1,   161,    -1,
     166,    -1,   122,    -1,   160,    -1,   139,    -1,   141,    -1,
     142,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   511,   511,   516,   517,   523,   524,   529,   530,   535,
     536,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   572,   585,   590,   595,   603,   604,   605,
     609,   610,   614,   615,   616,   621,   620,   641,   642,   643,
     648,   649,   654,   659,   664,   668,   677,   682,   687,   692,
     696,   700,   708,   713,   717,   722,   726,   727,   728,   732,
     733,   734,   735,   736,   737,   738,   742,   747,   748,   749,
     753,   754,   758,   762,   764,   766,   768,   770,   772,   779,
     780,   784,   785,   786,   787,   788,   789,   792,   793,   794,
     795,   796,   797,   809,   810,   821,   822,   823,   824,   825,
     826,   827,   828,   829,   830,   831,   832,   833,   834,   835,
     836,   837,   838,   839,   843,   844,   845,   846,   847,   848,
     849,   850,   851,   852,   853,   854,   861,   862,   863,   864,
     868,   869,   873,   874,   878,   879,   880,   890,   890,   895,
     896,   897,   898,   899,   900,   901,   905,   906,   907,   908,
     913,   912,   928,   927,   944,   943,   959,   958,   974,   978,
     983,   991,  1002,  1009,  1010,  1011,  1012,  1013,  1014,  1015,
    1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,
    1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,  1035,
    1041,  1047,  1053,  1059,  1066,  1073,  1077,  1084,  1088,  1089,
    1090,  1091,  1093,  1094,  1095,  1096,  1098,  1100,  1102,  1104,
    1109,  1110,  1114,  1116,  1124,  1125,  1130,  1135,  1136,  1137,
    1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,  1147,
    1154,  1155,  1156,  1157,  1166,  1167,  1171,  1173,  1176,  1182,
    1184,  1187,  1193,  1196,  1197,  1198,  1199,  1200,  1201,  1205,
    1206,  1210,  1211,  1212,  1216,  1217,  1221,  1224,  1226,  1231,
    1232,  1236,  1238,  1240,  1247,  1257,  1271,  1276,  1281,  1289,
    1290,  1295,  1296,  1298,  1303,  1319,  1326,  1335,  1343,  1347,
    1354,  1355,  1360,  1365,  1359,  1392,  1395,  1399,  1407,  1417,
    1406,  1456,  1460,  1465,  1469,  1474,  1481,  1482,  1486,  1487,
    1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,
    1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,  1507,
    1508,  1509,  1510,  1511,  1512,  1513,  1517,  1518,  1519,  1520,
    1521,  1522,  1523,  1524,  1525,  1526,  1527,  1528,  1532,  1533,
    1537,  1541,  1542,  1543,  1547,  1549,  1551,  1553,  1555,  1557,
    1562,  1563,  1567,  1568,  1569,  1570,  1571,  1572,  1573,  1574,
    1575,  1579,  1580,  1581,  1582,  1583,  1584,  1588,  1589,  1590,
    1594,  1595,  1596,  1597,  1598,  1599,  1603,  1604,  1607,  1608,
    1612,  1613,  1617,  1621,  1622,  1623,  1631,  1632,  1634,  1636,
    1638,  1643,  1645,  1650,  1651,  1652,  1653,  1654,  1655,  1656,
    1660,  1662,  1667,  1669,  1671,  1676,  1689,  1706,  1707,  1709,
    1714,  1715,  1716,  1717,  1718,  1722,  1728,  1736,  1737,  1745,
    1747,  1752,  1754,  1756,  1761,  1763,  1765,  1772,  1773,  1774,
    1779,  1781,  1783,  1787,  1791,  1793,  1797,  1805,  1806,  1807,
    1808,  1809,  1814,  1815,  1816,  1817,  1818,  1838,  1842,  1846,
    1854,  1861,  1862,  1863,  1867,  1869,  1875,  1877,  1879,  1884,
    1885,  1886,  1887,  1888,  1894,  1895,  1896,  1897,  1901,  1902,
    1906,  1907,  1908,  1912,  1913,  1917,  1918,  1922,  1923,  1927,
    1928,  1929,  1930,  1934,  1935,  1946,  1948,  1950,  1956,  1957,
    1958,  1959,  1960,  1961,  1963,  1965,  1967,  1969,  1971,  1973,
    1976,  1978,  1980,  1982,  1984,  1986,  1988,  1990,  1993,  1995,
    2000,  2002,  2004,  2006,  2008,  2010,  2012,  2014,  2016,  2018,
    2020,  2022,  2024,  2031,  2037,  2043,  2049,  2058,  2068,  2076,
    2077,  2078,  2079,  2080,  2081,  2082,  2083,  2088,  2089,  2093,
    2097,  2098,  2102,  2106,  2107,  2111,  2115,  2119,  2126,  2127,
    2128,  2129,  2130,  2131,  2135,  2136,  2141,  2143,  2147,  2151,
    2155,  2163,  2168,  2174,  2180,  2187,  2197,  2205,  2206,  2207,
    2208,  2209,  2210,  2211,  2212,  2213,  2214,  2216,  2218,  2220,
    2235,  2237,  2239,  2241,  2246,  2247,  2251,  2252,  2253,  2257,
    2258,  2259,  2260,  2269,  2270,  2271,  2272,  2273,  2277,  2278,
    2279,  2283,  2284,  2285,  2286,  2287,  2295,  2296,  2297,  2298,
    2302,  2303,  2307,  2308,  2312,  2313,  2314,  2315,  2316,  2317,
    2318,  2319,  2321,  2323,  2324,  2325,  2329,  2337,  2338,  2342,
    2343,  2344,  2345,  2346,  2347,  2348,  2349,  2350,  2351,  2352,
    2353,  2354,  2355,  2356,  2357,  2358,  2359,  2360,  2361,  2362,
    2363,  2364,  2369,  2370,  2371,  2372,  2373,  2374,  2375,  2379,
    2380,  2381,  2382,  2386,  2387,  2388,  2389,  2394,  2395,  2396,
    2397,  2398,  2399,  2400
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
  "req_formal_ls", "formal_ls", "formal", "opt_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_iter_or_op",
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
  "reduce_scan_op_expr", YY_NULL
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
     224,   224,   224,   224,   224,   224,   224,   225,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     227,   227,   228,   228,   229,   229,   230,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     232,   232,   232,   232,   232,   232,   233,   233,   233,   234,
     234,   234,   235,   236,   236,   236,   236,   236,   236,   237,
     237,   238,   238,   238,   239,   239,   240,   241,   241,   242,
     242,   243,   243,   243,   244,   244,   245,   245,   245,   246,
     246,   247,   247,   247,   248,   248,   249,   250,   250,   250,
     251,   251,   253,   254,   252,   255,   255,   255,   257,   258,
     256,   259,   259,   259,   259,   259,   260,   260,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   263,   263,
     264,   265,   265,   265,   266,   266,   266,   266,   266,   266,
     267,   267,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   269,   269,   269,   269,   269,   269,   270,   270,   270,
     271,   271,   271,   271,   271,   271,   272,   272,   273,   273,
     274,   274,   275,   276,   276,   276,   277,   277,   277,   277,
     277,   278,   278,   279,   279,   279,   279,   279,   279,   279,
     280,   280,   281,   281,   281,   282,   282,   283,   283,   283,
     284,   284,   284,   284,   284,   285,   285,   286,   286,   287,
     287,   288,   288,   288,   289,   289,   289,   290,   290,   290,
     291,   291,   291,   291,   291,   291,   291,   292,   292,   292,
     292,   292,   293,   293,   293,   293,   293,   294,   294,   294,
     294,   295,   295,   295,   296,   296,   296,   296,   296,   297,
     297,   297,   297,   297,   298,   298,   298,   298,   299,   299,
     300,   300,   300,   301,   301,   302,   302,   303,   303,   304,
     304,   304,   304,   305,   305,   306,   306,   306,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   309,   310,   311,
     311,   311,   311,   311,   311,   311,   311,   312,   312,   313,
     314,   314,   315,   316,   316,   317,   317,   317,   318,   318,
     318,   318,   318,   318,   319,   319,   320,   320,   320,   320,
     320,   321,   321,   321,   321,   321,   322,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   324,   324,   325,   325,   325,   326,
     326,   326,   326,   327,   327,   327,   327,   327,   328,   328,
     328,   329,   329,   329,   329,   329,   330,   330,   330,   330,
     331,   331,   332,   332,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   334,   334,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   336,   336,   336,   336,   336,   336,   336,   337,
     337,   337,   337,   338,   338,   338,   338,   339,   339,   339,
     339,   339,   339,   339
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
       4,     5,     6,     5,     6,     3,     4,     3,     4,     6,
       7,     6,     7,     4,     5,     4,     5,     4,     4,     3,
       6,     5,     4,     3,     6,     5,     6,     5,     8,     7,
       4,     4,     6,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     6,     4,     7,     5,
       3,     6,     2,     3,     3,     2,     2,     3,     3,     0,
       2,     2,     3,     5,     1,     3,     3,     5,     5,     0,
       2,     3,     2,     3,     6,     6,     1,     1,     1,     0,
       2,     0,     2,     3,     5,     5,     1,     1,     2,     3,
       1,     3,     0,     0,     8,     0,     1,     1,     0,     0,
      10,     3,     3,     5,     5,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     3,
       3,     0,     1,     3,     4,     5,     4,     5,     6,     6,
       0,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     0,     1,     1,     2,     1,     1,     1,     1,     1,
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
       3,     0,     0,     1,     0,   113,   636,   637,   638,   632,
     633,   639,     0,   557,    99,   134,   526,   141,   528,   557,
       0,   140,     0,   431,    99,     0,     0,     0,   254,   135,
     604,   604,   630,     0,     0,     0,     0,     0,   139,    55,
     255,   306,   136,     0,     0,     0,   302,     0,     0,   143,
       0,   576,   548,   640,   144,     0,   307,   520,   430,     0,
       0,     0,   157,   305,   138,   529,   432,     0,     0,     0,
       0,   524,     0,     0,   142,     0,     0,   114,     0,   631,
       0,     0,     0,   137,   288,   522,   434,   145,     0,     0,
     689,     0,   691,     0,   692,   693,   603,     0,   690,   687,
     507,   154,   688,     0,     0,     0,     0,     4,     0,     5,
       0,     9,    50,    10,    11,     0,    12,    13,    14,    16,
     503,   504,    26,    15,   155,   164,   165,    17,    21,    18,
      20,     0,   249,    19,   595,    23,    24,    25,    22,   163,
       0,   161,     0,   592,     0,   159,   162,     0,   160,   609,
     588,   505,   589,   510,   508,     0,     0,     0,   593,   594,
       0,   509,     0,   610,   611,   612,   634,   635,   587,   512,
     511,   590,   591,     0,    42,    28,   518,     0,     0,   558,
     100,     0,     0,   528,   135,     0,     0,     0,     0,   529,
       0,     0,     0,     0,   592,   609,   508,   593,   594,   527,
     509,   610,   611,     0,   557,     0,     0,   433,     0,   262,
       0,   488,     0,   495,   632,   529,   605,   305,   632,   182,
     529,     0,   305,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   169,     0,     0,     0,     0,     0,    52,   495,
     107,   115,   127,   121,   120,   129,   110,   119,   130,   116,
     131,   108,   132,   125,   118,   126,   124,   122,   123,   109,
     111,   117,   128,   133,     0,   112,     0,     0,     0,     0,
       0,     0,   437,     0,   151,    36,     0,   674,   580,   577,
     578,   579,     0,   521,   675,     7,   495,   305,   286,   296,
     604,   287,   156,   402,   485,     0,   484,     0,     0,   152,
     608,     0,     0,    39,     0,   525,   513,     0,   495,    40,
     519,     0,   269,   265,     0,   509,   269,   266,     0,   427,
       0,   523,     0,     0,   676,   678,   601,   673,   672,     0,
      57,    60,     0,     0,   490,     0,   492,     0,     0,   491,
       0,     0,   484,     0,   602,     0,     6,     0,    51,     0,
       0,     0,     0,   289,     0,   388,   389,   387,   308,     0,
     506,    27,     0,   581,     0,     0,     0,     0,     0,     0,
     677,     0,     0,     0,     0,     0,     0,   600,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   353,   354,   355,   350,   352,
       0,     0,   348,   351,   349,   347,     0,   357,   356,     0,
       0,   495,   495,     0,     0,     0,    29,    30,     0,     0,
       0,     0,     0,     0,     0,    31,     0,     0,     0,     0,
      32,     0,    33,     0,   503,   501,     0,   496,   497,   502,
     176,     0,   179,     0,   172,     0,     0,   178,     0,     0,
       0,   191,     0,     0,   190,     0,   199,     0,     0,     0,
     197,     0,   207,     0,     0,   205,     0,     0,    72,   166,
       0,     0,     0,   223,     0,   346,   219,     0,    54,    53,
      50,     0,     0,     0,   233,    34,   370,   303,   441,     0,
     442,   444,     0,   466,     0,   447,     0,     0,   150,    35,
       0,     0,    37,     0,   158,     0,    92,   606,   607,   153,
       0,    38,   495,     0,   276,   267,   263,   268,   264,     0,
     425,   422,   184,    41,    59,    58,    61,     0,   641,     0,
       0,   626,     0,   628,     0,     0,     0,     0,     0,     0,
       0,     0,   645,     8,     0,    44,     0,     0,    90,     0,
      87,     0,    66,   260,     0,     0,     0,   381,   436,   556,
     669,   668,   671,   680,   679,   684,   683,   665,   662,   663,
     664,   597,   652,   113,     0,   623,   624,   114,   622,   621,
     598,   656,   667,   661,   659,   670,   660,   658,   650,   655,
     657,   666,   649,   653,   654,   651,   599,     0,     0,     0,
       0,     0,     0,     0,     0,   682,   681,   686,   685,   568,
     569,   571,   573,     0,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   643,   260,   557,   557,   187,
     423,   435,   489,     0,     0,   515,     0,     0,   289,     0,
       0,   289,   424,     0,     0,     0,     0,   532,     0,     0,
       0,   200,     0,   538,     0,     0,   198,   208,     0,     0,
     206,   688,    75,     0,    62,    73,     0,     0,     0,   222,
       0,   218,     0,     0,     0,     0,   514,     0,   236,     0,
     234,   375,   372,   373,   374,   378,   379,   380,   370,     0,
     362,     0,   371,   390,     0,   445,     0,   148,   149,   147,
     146,     0,   465,   464,   588,     0,   439,   586,   438,     0,
       0,   620,   487,   486,     0,     0,     0,   516,     0,   270,
     429,   588,     0,   642,   596,   627,   493,   629,   494,   215,
       0,     0,     0,   644,   213,   542,     0,   647,   646,     0,
      46,    45,    43,     0,    86,     0,     0,     0,    79,     0,
       0,    72,   257,     0,   290,     0,     0,   300,     0,   297,
     385,   382,   383,   386,   309,     0,     0,    98,    96,    97,
      95,    94,    93,   618,   619,   570,   572,     0,   559,   134,
     141,   140,   139,   136,   143,   144,   138,   142,   137,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   183,   499,   500,   498,   289,
       0,   181,   289,     0,   180,     0,   217,     0,     0,   189,
       0,   188,     0,   563,     0,     0,   195,     0,     0,   193,
       0,   203,     0,   201,     0,     0,   167,     0,   168,   231,
     230,     0,   225,     0,     0,   221,     0,   227,     0,   259,
       0,     0,   376,   377,     0,   370,   360,     0,   479,   391,
     394,   393,   395,     0,   443,   446,   447,     0,     0,   448,
     449,     0,     0,   278,     0,     0,   277,   280,   517,     0,
     271,   274,     0,   426,   216,     0,     0,     0,     0,   214,
       0,    91,    88,     0,    69,    68,    67,     0,     0,     0,
       0,   305,   295,     0,   298,   294,   384,   390,   358,   101,
     340,   115,   338,   121,   120,   104,   119,   116,   343,   131,
     102,   132,   118,   122,   103,   105,   117,   133,   337,   319,
     322,   320,   321,   344,   332,   323,   336,   328,   326,   339,
     342,   327,   325,   330,   335,   324,   329,   333,   334,   331,
     341,     0,   318,     0,   106,     0,   358,   358,   316,   625,
     561,   447,   609,   609,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,   186,   185,     0,   291,     0,   291,
     192,     0,     0,   531,     0,   530,     0,   562,     0,     0,
     537,   196,     0,   536,   194,   204,   202,    64,    63,   224,
     220,   547,   226,     0,     0,   256,   235,   232,   479,   363,
       0,     0,   447,   392,   406,   440,   470,     0,   643,   495,
     495,     0,   282,     0,     0,     0,   272,     0,   211,   544,
       0,     0,   209,   543,     0,   648,     0,     0,     0,    72,
       0,    72,    80,    83,   261,   285,   157,   305,   284,   305,
     292,   301,   299,     0,   370,   315,     0,   345,     0,   311,
     312,   565,     0,     0,     0,     0,     0,     0,     0,     0,
     261,   291,   305,   291,   305,   535,     0,     0,   564,   541,
       0,     0,     0,     0,   229,    56,   447,   479,     0,   482,
     481,   483,   588,   403,   366,   364,     0,     0,     0,     0,
     468,   588,     0,     0,   283,   281,     0,   275,     0,   212,
       0,   210,    89,    71,    70,     0,     0,     0,     0,   258,
     293,   461,     0,   396,     0,   317,   101,   103,   358,   358,
       0,     0,     0,     0,     0,     0,   305,   175,   305,   171,
       0,     0,     0,     0,    65,   228,   367,   365,   447,   471,
       0,   405,   404,   420,     0,   421,   408,   411,     0,   407,
     400,   401,   304,     0,   582,   583,   273,     0,     0,    82,
      85,    81,    84,     0,   460,   459,   588,   397,   406,   359,
     313,   314,     0,     0,     0,     0,     0,     0,   177,   173,
     534,   533,   540,   539,   369,   368,   473,   474,   476,   588,
       0,   643,   419,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   588,   584,   585,   546,   545,     0,   451,     0,
       0,     0,   475,   477,   410,   412,   413,   416,   417,   418,
     414,   415,   409,   456,   454,   588,   643,   398,   310,   399,
     471,   455,   588,   478
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   331,   108,   671,   110,   111,   112,   349,
     480,   113,   238,   114,   332,   662,   551,   666,   663,   115,
     116,   117,   548,   549,   118,   119,   181,   952,   270,   120,
     265,   121,   702,   275,   122,   123,   287,   124,   125,   126,
     445,   640,   441,   637,   127,   128,   800,   129,   236,   130,
     679,   680,   193,   132,   133,   134,   135,   136,   515,   719,
     882,   137,   138,   715,   877,   139,   140,   555,   901,   141,
     142,   758,   759,   194,   268,   693,   144,   145,   557,   907,
     764,   955,   956,   477,  1055,   487,   689,   690,   691,   692,
     765,   358,   863,  1178,  1238,  1162,   435,  1094,  1098,  1156,
    1157,  1158,   146,   320,   520,   147,   148,   271,   272,   491,
     492,   706,  1175,  1123,   495,   703,  1197,  1091,  1012,   333,
     210,   337,   338,   436,   437,   438,   195,   150,   151,   152,
     153,   196,   155,   178,   179,   613,   457,   822,   614,   615,
     156,   157,   197,   198,   160,   222,   439,   200,   162,   201,
     202,   165,   166,   167,   168,   343,   169,   170,   171,   172,
     173
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1130
static const yytype_int16 yypact[] =
{
   -1130,    97,  3741, -1130,   -56,    65, -1130, -1130, -1130, -1130,
   -1130, -1130,  5125,     5,   151,   201, 14881,   210, 19920,     5,
   11669,   287,   406,   257,   151,  5125, 11669,  5125,   176, 20005,
   11840,  8575,   371,  8746,  9951,  9951,  7199,  8917,   402, -1130,
     280, -1130,   416, 20046, 20046, 20046, -1130,  3306, 10122,   426,
   11669,   197, -1130,   447,   462, 11669, -1130, 14881, -1130, 11669,
     491,   344,   216, 18390,   480, 20131, -1130, 10295,  8060, 11669,
   10122, 14881, 11669,   442,   499,   385,  5125,   509, 11669,   516,
   12011, 12011, 20046,   529, -1130, 14881, -1130,   533, 11669, 11669,
   -1130, 11669, -1130, 11669, -1130, -1130, 14405, 11669, -1130, 11669,
   -1130, -1130, -1130,  4087,  7372,  9090, 11669, -1130,  4952, -1130,
     428, -1130,   508, -1130, -1130,    84, -1130, -1130, -1130, -1130,
   -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130,
   -1130,   544, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130,
   20046, -1130, 20046,   241,    47, -1130, -1130,  3306, -1130,   457,
   -1130,   443, -1130, -1130,   479,   488,   489, 11669,   495,   497,
   19424,  3308,   425,   500,   501, -1130, -1130,   432, -1130, -1130,
   -1130, -1130, -1130,   207, -1130, -1130, 19424,   482,  5125, -1130,
   -1130,   503, 11669, -1130, -1130, 11669, 11669, 11669, 20046, -1130,
   11669, 10295, 10295,   607,   453, -1130, -1130, -1130, -1130,   297,
     456, -1130, -1130,   507,  2464, 20046,  3306, -1130,   505, -1130,
     185, 19424,   560,  8233,   594, 20172, 19424,   227,   599, -1130,
   20257, 20046,   227, 20046,   520,    18, 16305,     0, 16147,     0,
   16223,   468, -1130, 16388, 20046, 20046,   -65, 15433,    25,  8233,
   -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130,
   -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130,
   -1130, -1130, -1130, -1130,   524, -1130,   459,  5125,   525,  2849,
     103,    70, -1130,  5125, -1130, -1130, 16627,   616, -1130,   526,
     530, -1130, 16627,   297,   616, -1130,  8233,  1844, -1130, -1130,
   10466, -1130, -1130, -1130, -1130,   186, 19424, 11669, 11669, -1130,
   19424,   523, 17177, -1130, 16627,   297, 19424,   531,  8233, -1130,
   19424, 17218, -1130, -1130, 17263,  3524, -1130, -1130, 17368,   574,
     536,   297, 16627, 17413,   121,   121,  2934,   616,   616,    92,
   -1130, -1130,  4260,    91, -1130, 11669, -1130,   135,   163, -1130,
     -38,   114, 17462,   -48,  2934,   696, -1130,  4433, -1130,   641,
   11669, 11669, 20046,   563,   540, -1130, -1130, -1130, -1130,   265,
     461, -1130, 11669,   565, 11669, 11669, 11669,  9951,  9951, 11669,
     469, 11669, 11669, 11669, 11669, 11669,   404, 14405, 11669, 11669,
   11669, 11669, 11669, 11669, 11669, 11669, 11669, 11669, 11669, 11669,
   11669, 11669, 11669, 11669,   642, -1130, -1130, -1130, -1130, -1130,
    9261,  9261, -1130, -1130, -1130, -1130,  9261, -1130, -1130,  9261,
    9261,  8233,  8233,  9951,  9951,  7889, -1130, -1130, 16703, 16784,
   17563,   543,    15, 20046,  4606, -1130,  9951,    18,   551,   271,
   -1130, 11669, -1130, 11669,   592, -1130,   545,   573, -1130, -1130,
   -1130, 20046, -1130,  3306, -1130, 20046,   555, -1130,  3306,   672,
   10295, -1130,  5298,  9951, -1130,   550, -1130,    18,  5471,  9951,
   -1130,    18, -1130,    18,  9951, -1130,    18, 12182, 11669, -1130,
     598,   600,  5125,   688,  5125, -1130,   690, 11669, -1130, -1130,
     508,   554,  8233, 20046, -1130, -1130,   365, -1130, -1130,  2849,
   -1130,   582,   557, -1130, 12353,   609, 11669,  3306, -1130, -1130,
   11669, 11669, -1130,   559, -1130, 10295, -1130, 19424, 19424, -1130,
      29, -1130,  8233,   569, -1130,   713, -1130,   713, -1130, 12524,
     603, -1130, -1130, -1130, -1130, -1130, -1130,  9434, -1130, 17612,
    7545, -1130,  7718, -1130,  5125,   570,  9951,  9607,  3914,   571,
   11669, 10637, -1130, -1130,   423, -1130,  4779, 20046, -1130,   285,
   17652,   319,  1221,   188, 10295,   580, 19879,   204, -1130, 17769,
   19598, 19598,   444, -1130,   444, -1130,   444, 19678,  1159,  2549,
     779,   297,   121, -1130,   572, -1130, -1130, -1130, -1130, -1130,
    2934, 19720,   444,   950,   950, 19598,   950,   950,  1357,   121,
   19720, 19638,  1357,   616,   616,   121,  2934,   587,   588,   589,
     593,   595,   597,   586,   590, -1130,   444, -1130,   444,   181,
   -1130, -1130, -1130,   166, -1130,  1637, 19541,   218, 12695,  9951,
   12866,  9951, 11669,  8233,  9951, 15153,   602,     5, 17814, -1130,
   -1130, -1130, 19424, 17854,  8233, -1130,  8233, 20046,   563,   329,
   20046,   563, -1130,   341, 11669,   172,  8917, 19424,    31, 16860,
    7889, -1130,  8917, 19424,    19, 16469, -1130, -1130,     0, 16545,
   -1130,   601,   623,   606, 17971,   623,   614, 11669, 11669,   744,
    5125,   749, 18011,  5125, 16942,   721, -1130,   239, -1130,   274,
   -1130,   194, -1130, -1130, -1130, -1130, -1130, -1130,   542,   315,
   -1130, 15252, -1130,   226,   615,  2849,   103,     6,    35, 11669,
   11669,  7028, -1130, -1130,   335,  8404, -1130, 19424, -1130, 18051,
   18168, -1130, -1130, 19424,   618,    49,   617, -1130,  3130, -1130,
   -1130,   388, 20046, -1130, -1130, -1130, -1130, -1130, -1130, -1130,
    5125,   122, 17018, -1130, -1130, 19424,  5125, 19424, -1130, 18209,
   -1130, -1130, -1130, 11669, -1130,    96,    62, 11669, -1130, 10808,
   12182, 11669, -1130,  8233,   645,  2052,   622,   669,   128, -1130,
     705, -1130, -1130, -1130, -1130,  3476,   624, -1130, -1130, -1130,
   -1130, -1130, -1130, -1130, -1130, -1130, -1130,  7889, -1130, -1130,
   -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130,
      39,  9951,  9951, 11669,   762, 18249, 11669,   764, 18399,   316,
     625, 18439,  8233,    18,    18, -1130, -1130, -1130, -1130,   563,
     629, -1130,   563,   631, -1130, 16627, -1130, 15509,  5644, -1130,
    5817, -1130,   320, -1130, 15591,  5990, -1130,    18,  6163, -1130,
      18, -1130,    18, -1130,    18, 11669, -1130, 11669, -1130, 19424,
   19424,  5125, -1130,  5125, 11669, -1130,  5125,   769, 20046,   644,
   20046,   507, -1130, -1130, 20046,   935, -1130,  2849,   661,   717,
   -1130, -1130, -1130,    41, -1130, -1130,   609,   639,    27, -1130,
   -1130,   647,   648, -1130,  6336, 10295, -1130, -1130, -1130, 20046,
   -1130,   677,   507, -1130, -1130,  6509,   649,  6682,   651, -1130,
   11669, -1130, -1130, 11669, 18479,    69, 17101,   652,   658,   336,
     657,  1516, -1130, 11669, 20046, -1130, -1130,   226,   659,   430,
   -1130,   681, -1130,   682,   685,   695,   689,   697, -1130,   698,
     704,   699,   706,   707,   439,   712,   709,   710, -1130, -1130,
   -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130,
   -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130,
   -1130, 11669, -1130,   720,   722,   715,   659,   659, -1130, -1130,
   -1130,   609,   386,   395, 18596, 13037, 13208, 18636, 13379, 13550,
   -1130, 13721, 13892,   403, -1130, -1130,   666, -1130,   691, -1130,
   -1130,  5125,  8917, 19424,  8917, 19424,  7889, -1130,  5125,  8917,
   19424, -1130,  8917, 19424, -1130, -1130, -1130, 18676, 19424, -1130,
   -1130, 19424,   828,  5125,   701, -1130, -1130, -1130,   661, -1130,
     694, 10981,   152, -1130,    23, -1130, -1130,  9951, 15017,  8233,
    8233,  5125, -1130,    21,   703, 11669, -1130,  8917, -1130, 19424,
    5125,  8917, -1130, 19424,  5125, 19424,   157, 11152, 12182, 11669,
   12182, 11669, -1130, -1130,   716, -1130, -1130,  1844, -1130,  1963,
   -1130, 19424, -1130,   407,   365, -1130, 18793, -1130, 15352, -1130,
   -1130, -1130, 11669, 11669, 11669, 11669, 11669, 11669, 11669, 11669,
   -1130, -1130,  1732, -1130,  2400, 18011, 15667, 15749, -1130, 18011,
   15825, 15907, 11669,  5125, -1130, -1130,   152,   661,  9780, -1130,
   -1130, -1130,   283, 10295, -1130, -1130,   148, 11669,    -2, 18833,
   -1130,   686,   711,   714,   560, -1130,   507, 19424, 15983, -1130,
   16065, -1130, -1130, -1130, 19424,   725,   726,   727,   729, -1130,
   -1130, -1130, 14063,   765,   408, -1130,   731,   734,   659,   659,
   18950, 18990, 19030, 19147, 19187, 19227,  2728, -1130, 19565, -1130,
    5125,  5125,  5125,  5125, 19424, -1130, -1130, -1130,   152, 11325,
      51, -1130, 19424, -1130,   110, -1130,   113, -1130,   527, 19344,
   -1130, -1130, -1130, 13892,   718,   732, -1130,  5125,  5125, -1130,
   -1130, -1130, -1130,  6855, -1130, -1130,   173, -1130,    23, -1130,
   -1130, -1130, 11669, 11669, 11669, 11669, 11669, 11669, -1130, -1130,
   18011, 18011, 18011, 18011, -1130, -1130, -1130, -1130, -1130,   389,
    9951, 14575, -1130, 11669,   148,   110,   110,   110,   110,   110,
     110,   148,   824, -1130, -1130, 18011, 18011,   723, 14234,    63,
      22, 19384, -1130, -1130, 19424, -1130, -1130, -1130, -1130, -1130,
   -1130, -1130,   739, -1130, -1130,   228, 14745, -1130, -1130, -1130,
   11498, -1130,   333, -1130
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1130, -1130, -1130,     3,  -471,  2694, -1130, -1130, -1130,   387,
   -1130, -1130, -1130,   611,   548,  -461, -1130,  -717,  -713, -1130,
   -1130, -1130,   133, -1130, -1130,   141,   873, -1130,  2832,   -92,
    -737, -1130,  -967,   679, -1071,  -868, -1130,   -62, -1130, -1130,
   -1130, -1130, -1130, -1130, -1130, -1130,   108, -1130, -1130, -1130,
   -1130,    48,  1289, -1130, -1130, -1130, -1130, -1130,   585, -1130,
      24, -1130, -1130, -1130, -1130, -1130, -1130,  -592,  -821, -1130,
   -1130, -1130,     7,  1382, -1130, -1130, -1130,   200, -1130, -1130,
   -1130, -1130,  -156,  -148,  -886, -1130,  -142,    58,   231, -1130,
   -1130, -1130,     9, -1130, -1130,  -300,    11, -1000,  -255,  -285,
    -274,  -536, -1130,  -195, -1130,    17,   895,  -131,   435, -1130,
    -472,  -835,  -831, -1130,  -644,  -510, -1129, -1041,  -960,   -64,
   -1130,   116, -1130,  -219,  -470,   298,   743,  -465, -1130, -1130,
   -1130,  1512, -1130,   -13, -1130, -1130,   -96, -1130,  -639, -1130,
   -1130, -1130,  1902,  1921,   -12,   905,    68,  1122, -1130,  2286,
    2316, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130, -1130,
    -413
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -644
static const yytype_int16 yytable[] =
{
     176,   292,   617,   295,   199,   107,   203,   665,   204,   720,
     428,   823,   677,   410,   211,   688,   359,   694,   216,   216,
     481,   226,   228,   230,   233,   237,   446,  1161,   954,   704,
     714,  1015,   273,  1050,   898,   472,   276,   897,   277,   206,
     493,   341,   493,   282,  1089,   283,   810,   284,  1086,   813,
     273,   825,   866,   273,   721,   296,   300,   302,   304,   305,
     306,  -254,   624,   818,   310,   573,   311,   503,   314,   318,
    1059,  1060,  1222,   321,  1017,   429,   322,   323,   294,   324,
     455,   325,   574,  1096,   326,   327,  1146,   328,    68,   513,
    -255,   296,   300,   342,   344,   575,   541,     3,  1200,   573,
     355,  -462,  -279,   424,   493,  1038,   478,   174,  1198,   479,
     624,  1243,    68,  1153,   294,   336,   294,   496,   455,   356,
    -237,   434,   874,   177,   576,   542,  1061,  1148,   422,   357,
     350,  -462,   461,   463,   466,   534,   301,   455,   960,  1097,
    1039,   225,   227,   229,  -279,   363,   961,   434,  1194,  1161,
    -462,  1153,   366,   799,   180,  1174,  1072,  -462,  1074,   537,
    1223,   536,  -462,   577,   875,   505,   424,   410,   424,   578,
     176,   537,   339,   418,   419,   420,   213,  1095,   310,   296,
     342,  1120,   494,  -462,   494,  1237,   424,   424,   625,   424,
     424,   427,   603,   604,   434,   537,   351,   577,  -279,   424,
    1018,   300,   294,   294,  1050,   239,  1050,   537,   367,  -462,
    -462,  1155,   368,   340,   497,   278,   434,   976,   876,  1014,
     978,   313,   317,   865,  1201,   504,  -462,   300,   775,  1203,
     893,   760,   455,  -458,   443,   527,  1236,   617,  1154,   448,
     455,   852,  1180,  1181,   535,   539,   494,  -462,   206,  1155,
    1136,  1147,  1138,   859,    23,   174,  -238,  1204,   537,   370,
     528,   524,   -78,  -458,   374,  -244,  -462,   376,  1050,   776,
    1050,   379,   904,   279,   300,   705,  -458,    41,   216,   530,
     -49,   761,   853,   899,   579,   507,   508,   538,  -450,  -458,
     665,   280,   762,   716,   413,   885,   300,   905,   414,   -49,
    1093,   835,    56,   860,    58,   792,   531,   532,   281,   414,
     777,   763,   639,  1195,   861,    66,   505,   643,  -450,   434,
     434,   954,  1116,   529,  1118,  1115,  1112,  1117,   -78,   431,
     505,  -450,   973,   862,   533,   526,  -458,   778,   550,   552,
      86,  -458,  -243,   816,  -450,   207,   213,  1078,   432,   506,
     559,   752,   560,   561,   562,   564,   566,   567,   753,   568,
     569,   570,   571,   572,   617,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,  -463,   636,   688,  1010,   645,  1234,   300,   300,
     434,  -450,   681,  -452,   300,  -463,  -450,   300,   300,   300,
     300,   606,   608,   616,  -553,  1241,  -480,   573,  1121,   497,
     849,  -616,   682,  -616,   628,   497,   683,   442,   850,   632,
     434,   633,   447,  -452,   574,  -463,  -252,  -480,   558,   743,
    1049,  -480,  1036,    23,   631,   370,  -452,   575,   296,   684,
     647,   649,   685,   376,    60,   851,   653,   655,   744,  -452,
     239,  -463,   659,   686,  -480,   664,   664,  -242,  -463,   855,
     636,   294,   672,   747,   986,   674,   576,  -457,   598,   599,
     300,  -239,   687,   497,   600,   563,   565,   601,   602,  -463,
     636,  -246,   748,    58,   707,   497,   856,   970,   709,   710,
     754,   987,   811,   713,    66,    23,  -452,  -457,  -463,   285,
     300,  -452,  -250,  -463,   814,   577,  -463,  1044,  1100,  -361,
    -457,   578,  -472,   205,   286,   713,   712,  -247,   300,    86,
     300,   605,   607,  -457,   732,   713,   735,   883,   737,   739,
    -567,   434,  -428,  -472,   627,  -241,  -361,  -472,   712,  -566,
     307,   336,   296,   336,   434,    58,  1092,   636,   712,   526,
    1122,  -428,   855,  1101,  -245,   308,    66,  -567,   827,   830,
    -472,   648,   832,   834,  -251,   294,  -566,   654,  -107,   681,
    -457,  -253,   658,   617,  1070,  -457,  -113,  -109,   665,  1179,
     665,    86,   370,   688,  -240,  -114,   174,   374,  -248,   682,
     376,   348,   740,   683,   379,   411,   347,   412,   339,   352,
     339,  1049,  -617,  1049,  -617,   360,   647,   795,   653,   798,
     672,   300,   801,   735,   803,   804,   684,   957,  1202,   685,
    -550,   345,   300,  -616,   300,  -616,  -613,   424,  -613,   483,
     686,  -615,   815,  -615,   817,   886,   888,   868,   616,  -614,
     824,  -614,  -549,   362,   731,   806,   726,   366,   728,   687,
    1205,   361,   415,   892,   579,   839,   840,  1176,  -554,   274,
    -555,   434,   423,  -552,  -551,  1049,   417,  1049,   430,  1226,
    1227,  1228,  1229,  1230,  1231,   424,  1206,   433,  1207,  1208,
     440,   274,  1209,  1210,  1199,   444,   509,   306,   310,   342,
     450,   312,   316,   300,   482,   486,   500,   519,  1212,   521,
     501,   512,   807,   367,   543,   547,   554,   368,   556,   597,
     434,   376,   294,   623,   630,   634,   635,   636,   642,   644,
     650,   667,   670,   668,   673,   676,   695,   794,   696,   797,
     711,   550,   705,  -467,   718,   894,  1199,   896,   664,   664,
     717,   300,   766,   730,   736,   149,  -467,   722,   755,   303,
     767,   768,   769,  1235,   370,   149,   770,   773,   771,   374,
     772,   375,   376,   774,   -74,   616,   379,   835,   149,   836,
     149,  1242,   802,   870,   386,  1199,  -467,   838,   841,   564,
     606,   964,   392,   843,   967,   848,   864,   873,   878,   505,
     300,   902,   903,   906,   965,   959,   968,   977,   971,   979,
    1102,  1103,  -467,  1003,  1011,  1013,   983,  1005,   985,  -467,
     366,  1023,  1016,   990,   425,  1042,   993,  1019,  1020,   149,
    1025,  1043,  1030,   997,  1034,   998,  1045,  -134,  -141,  1054,
    -467,  -140,  1001,  -110,  1071,  -139,   274,   274,   274,   274,
     274,   274,  -108,  -136,  -143,  -144,   149,   473,   476,  -467,
    -111,   149,  -138,  -142,  -467,  -137,  -145,  -467,  1057,  1073,
    -112,  1058,  1083,   296,  1085,  1087,   367,   675,  1177,  -107,
     368,  -469,  -109,  1029,  1106,  1033,   891,   484,  1035,  1119,
    1213,   664,  1164,  1204,  -469,  1165,   294,   274,  1169,  1170,
    1171,  1051,  1172,   274,  1214,   546,  1233,   208,  1006,   563,
     605,   517,  1128,  1024,   451,   454,   456,   460,   462,   465,
    1129,  1052,  1124,  1009,  -469,   274,  1053,   370,   371,   854,
    1239,   149,   374,  1220,   375,   376,  1232,   434,   434,   379,
    1225,   232,   708,   274,   808,   217,     0,   386,     0,  1056,
    -469,     0,     0,   390,   391,   392,     0,  -469,     0,     0,
       0,     0,     0,   983,   985,   499,   990,   993,     0,  1029,
    1033,   502,   681,     0,     0,     0,     0,     0,  -469,  1075,
    1076,     0,  1077,     0,   616,     0,  1079,  1080,     0,     0,
    1081,   366,   682,   511,     0,   292,   683,  -469,     0,     0,
       0,     0,  -469,     0,     0,  -469,     0,     0,     0,     0,
       0,   522,     0,     0,     0,  1099,   735,   300,   300,   684,
     149,     0,   685,  1107,    60,  1108,   149,     0,     0,  1110,
       0,     0,  1090,   686,  1150,  1114,   664,   664,   664,   664,
       0,     0,     0,     0,     0,     0,     0,   367,   274,     0,
       0,   368,   687,     0,     0,     0,     0,     0,     0,     0,
    1075,  1130,  1131,  1079,  1132,  1133,  1134,  1135,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   274,     0,
    1144,     0,   274,     0,   274,   149,   342,   274,     0,     0,
       0,  1152,     0,     0,     0,  1159,     0,     0,   370,   371,
     149,   372,   373,   374,     0,   375,   376,   377,     0,   294,
     379,     0,     0,     0,  1151,     0,   629,   385,   386,  1219,
       0,   389,     0,     0,   390,   391,   392,     0,     0,     0,
       0,     0,     0,     0,   161,   393,     0,     0,  1190,  1191,
    1192,  1193,     0,     0,   161,     0,   651,     0,     0,     0,
     656,     0,   657,     0,     0,   660,     0,   161,     0,   161,
       0,  1033,     0,     0,     0,  1215,  1216,     0,     0,     0,
    1196,   342,     0,     0,     0,     0,     0,   149,     0,     0,
    1190,  1191,  1192,  1193,  1215,  1216,     0,     0,     0,     0,
       0,     0,     0,     0,   294,     0,     0,     0,  1221,   735,
     366,  1224,     0,     0,     0,   149,     0,     0,   161,     0,
       0,   149,   315,   315,     0,     0,     0,     0,     0,     0,
       0,     0,  1196,     0,     0,   149,     0,   149,     0,     0,
       0,     0,     0,     0,   735,   161,     0,     0,  1033,     0,
     161,     0,     0,   364,   749,     0,     0,     0,     0,     0,
     365,     0,     0,     0,     0,     0,   367,     0,     0,     0,
     368,  1196,   366,     0,     0,     0,     0,   750,     0,   274,
     274,     0,     0,     0,     0,   274,   274,     0,     0,   274,
     274,     0,     0,     0,     0,     0,     0,   149,     0,     0,
       0,   149,     0,     0,     0,   847,     0,     0,     0,   149,
       0,   131,   751,     0,     0,     0,     0,   370,     0,     0,
     161,   131,   374,     0,   375,   376,     0,     0,   367,   379,
       0,     0,   368,     0,   131,     0,   131,   386,     0,     0,
       0,     0,     0,   390,   391,   392,     0,   819,   821,   880,
       0,     0,     0,   826,   829,     0,     0,   831,   833,     0,
       0,     0,     0,   369,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   370,
     371,     0,   372,   373,   374,   131,   375,   376,   377,     0,
     378,   379,   380,   381,   382,     0,   383,   384,   385,   386,
     387,   388,   389,     0,   143,   390,   391,   392,   366,   161,
       0,     0,   131,     0,   143,   161,   393,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   143,     0,   143,
       0,     0,     0,   149,   274,   274,   149,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,     0,   476,     0,
       0,     0,     0,     0,     0,   476,     0,     0,   274,     0,
       0,   274,     0,   274,   367,   274,     0,     0,   368,     0,
       0,     0,     0,     0,   161,     0,     0,     0,   143,     0,
       0,     0,  1007,     0,     0,     0,     0,   131,     0,   161,
       0,     0,     0,   149,     0,     0,     0,     0,     0,   149,
       0,     0,   974,   975,     0,   143,     0,     0,     0,     0,
     143,     0,     0,  1026,   980,   370,   371,     0,   372,   373,
     374,     0,   375,   376,     0,     0,   991,   379,   958,   994,
       0,   995,     0,   996,   154,   386,     0,     0,     0,     0,
       0,   390,   391,   392,   154,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   962,   963,     0,   154,   288,   154,
       0,     0,    22,    23,     0,     0,   161,     0,     0,     0,
       0,   289,     0,    30,   290,     0,   131,     0,     0,    36,
     143,   149,   131,   149,     0,     0,    41,     0,   149,     0,
       0,   149,     0,     0,   161,     0,     0,     0,     0,     0,
     161,     0,     0,     0,   149,     0,   149,     0,   154,   149,
       0,    56,     0,    58,   161,    60,   161,  1046,     0,     0,
    1047,     0,   291,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   154,     0,   149,     0,     0,
     154,   131,     0,    82,     0,     0,    84,     0,   149,    86,
     149,     0,     0,     0,   274,     0,   131,     0,     0,     0,
     573,     0,     0,     0,     0,     0,     0,     0,     0,   143,
       0,     0,     0,     0,   779,   143,   161,   780,     0,     0,
     161,     0,   781,     0,     0,     0,     0,     0,   161,     0,
       0,     0,   184,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   782,     0,     0,  1048,     0,   476,   476,   783,
     154,   476,   476,     0,     0,     0,     0,     0,     0,   784,
       0,     0,  1105,     0,     0,     0,   785,     0,     0,  1160,
       0,     0,     0,   131,   143,     0,     0,  1166,     0,   476,
       0,   476,   786,     0,   149,     0,     0,     0,     0,   143,
       0,   149,     0,     0,   787,     0,     0,     0,   577,     0,
       0,   131,     0,     0,     0,   788,   149,   131,     0,     0,
       0,   789,     0,     0,   288,     0,     0,     0,    22,    23,
       0,   131,     0,   131,   149,     0,     0,   289,     0,    30,
     290,     0,     0,   149,     0,    36,     0,   149,     0,   154,
       0,     0,    41,     0,     0,   154,     0,     0,     0,     0,
       0,     0,   161,     0,     0,   161,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   143,    56,     0,    58,
       0,    60,     0,  1046,     0,     0,  1047,     0,   291,     0,
      66,     0,     0,   131,     0,     0,   149,   131,     0,     0,
       0,  1160,     0,     0,   143,   131,     0,     0,     0,    82,
     143,     0,    84,     0,   154,    86,     0,     0,     0,     0,
       0,     0,   161,     0,   143,     0,   143,     0,   161,   154,
       0,     0,     0,     0,     0,     0,   288,     0,     0,     0,
      22,    23,     0,     0,     0,     0,     0,     0,     0,   289,
       0,    30,   290,   149,   149,   149,   149,    36,     0,     0,
       0,     0,     0,     0,    41,   101,     0,     0,     0,     0,
       0,  1137,     0,     0,   158,     0,     0,     0,     0,     0,
     149,   149,     0,     0,   158,     0,   143,     0,     0,    56,
     143,    58,     0,   159,     0,     0,     0,   158,   143,   158,
     291,     0,    66,   159,     0,     0,   154,     0,     0,     0,
     161,     0,   161,     0,     0,     0,   159,   161,   159,     0,
     161,    82,     0,     0,    84,     0,     0,    86,     0,   131,
       0,     0,   131,   161,   154,   161,     0,     0,   161,     0,
     154,     0,     0,     0,     0,     0,     0,     0,   158,     0,
       0,     0,     0,     0,   154,   288,   154,     0,     0,    22,
      23,     0,     0,     0,     0,     0,   161,   159,   289,     0,
      30,   290,     0,     0,     0,   158,    36,   161,     0,   161,
     158,     0,     0,    41,     0,     0,     0,     0,     0,   131,
       0,     0,     0,     0,   159,   131,     0,     0,     0,   159,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,   345,     0,  1046,     0,   154,  1047,     0,   291,
     154,    66,   143,   900,     0,   143,     0,     0,   154,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,    84,  -291,     0,    86,     0,  -291,  -291,
     158,     0,     0,     0,     0,     0,     0,  -291,     0,  -291,
    -291,     0,     0,     0,     0,  -291,     0,     0,     0,   159,
       0,     0,  -291,   161,     0,  -291,     0,   131,     0,   131,
     161,     0,   143,     0,   131,     0,     0,   131,   143,     0,
       0,     0,     0,     0,  -291,   161,   101,  -291,     0,  -291,
     131,  -291,   131,  -291,  -291,   131,  -291,     0,  -291,     0,
    -291,     0,     0,   161,     0,     0,     0,     0,     0,     0,
       0,     0,   161,     0,     0,     0,   161,     0,     0,  -291,
       0,     0,  -291,   131,     0,  -291,     0,     0,     0,   158,
       0,     0,     0,     0,   131,   158,   131,     0,     0,     0,
       0,     0,   154,     0,     0,   154,     0,     0,   159,     0,
       0,     0,     0,     0,   159,     0,     0,     0,     0,     0,
     143,     0,   143,     0,     0,   161,     0,   143,     0,     0,
     143,     0,     0,     0,     0,  -291,     0,     0,     0,     0,
       0,  -291,     0,   143,     0,   143,     0,     0,   143,     0,
       0,     0,     0,     0,   158,     0,     0,     0,     0,     0,
       0,     0,   154,     0,     0,     0,     0,     0,   154,   158,
       0,     0,     0,   159,     0,     0,   143,     0,     0,     0,
       0,     0,   161,   161,   161,   161,     0,   143,   159,   143,
     131,     0,     0,     0,     0,     0,     0,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   163,   161,
     161,     0,   131,     0,     0,     0,     0,     0,   163,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     131,   163,     0,   163,     0,     0,     0,     0,   164,   131,
       0,     0,     0,   131,     0,     0,   158,     0,   164,     0,
     154,     0,   154,     0,     0,     0,     0,   154,     0,     0,
     154,   164,     0,   164,     0,   159,     0,     0,     0,     0,
       0,     0,     0,   154,   158,   154,     0,     0,   154,     0,
     158,     0,   163,   143,     0,     0,     0,     0,     0,     0,
     143,     0,   131,   159,   158,     0,   158,     0,     0,   159,
       0,     0,     0,     0,     0,   143,   154,     0,     0,   163,
       0,     0,   164,   159,   163,   159,     0,   154,     0,   154,
       0,     0,     0,   143,     0,     0,     0,     0,     0,     0,
       0,     0,   143,     0,     0,     0,   143,     0,     0,   164,
       0,     0,   288,     0,   164,     0,    22,    23,     0,   131,
     131,   131,   131,     0,     0,   289,   158,    30,   290,     0,
     158,     0,     0,    36,     0,     0,     0,     0,   158,     0,
      41,     0,     0,     0,     0,   159,   131,   131,     0,   159,
       0,     0,     0,     0,   163,   143,     0,   159,     0,     0,
       0,     0,     0,     0,     0,    56,   364,    58,     0,    60,
       0,  1046,     0,   365,  1047,     0,   291,     0,    66,     0,
       0,     0,     0,   154,   164,   366,     0,     0,     0,     0,
     154,     0,     0,     0,     0,     0,     0,    82,     0,     0,
      84,   426,     0,    86,     0,   154,     0,     0,     0,     0,
       0,     0,   143,   143,   143,   143,     0,     0,     0,     0,
       0,     0,     0,   154,     0,     0,     0,     0,     0,     0,
       0,     0,   154,     0,     0,     0,   154,     0,     0,   143,
     143,   367,     0,   163,     0,   368,     0,     0,     0,   163,
       0,     0,     0,   101,     0,     0,     0,     0,     0,  1139,
       0,     0,   158,     0,     0,   158,     0,     0,     0,     0,
     366,     0,   177,   164,     0,     0,   369,     0,     0,   164,
       0,   159,     0,     0,   159,   154,     0,     0,     0,     0,
       0,     0,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,     0,   378,   379,   380,   381,   382,   163,   383,
     384,   385,   386,   387,   388,   389,     0,     0,   390,   391,
     392,     0,   158,   163,     0,     0,   367,     0,   158,   393,
     368,     0,     0,     0,     0,     0,     0,     0,   164,     0,
       0,   159,   154,   154,   154,   154,     0,   159,     0,     0,
       0,     0,     0,   164,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   154,
     154,     0,     0,     0,     0,     0,     0,   370,   371,     0,
       0,   373,   374,     0,   375,   376,   109,     0,     0,   379,
       0,     0,     0,     0,     0,     0,   175,   386,     0,     0,
     163,     0,     0,   390,   391,   392,     0,     0,     0,   209,
     158,   212,   158,     0,     0,     0,     0,   158,     0,     0,
     158,     0,     0,     0,     0,     0,     0,     0,   163,   159,
     164,   159,     0,   158,   163,   158,   159,     0,   158,   159,
     288,     0,     0,     0,    22,    23,     0,     0,   163,     0,
     163,     0,   159,   289,   159,    30,   290,   159,   164,     0,
     309,    36,     0,     0,   164,     0,   158,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,   158,   164,   158,
     164,     0,     0,     0,     0,   159,     0,   109,     0,     0,
       0,     0,   346,    56,     0,    58,   159,    60,   159,  1046,
       0,     0,  1047,     0,   291,     0,    66,     0,     0,     0,
     163,     0,     0,     0,   163,     0,     0,     0,     0,     0,
       0,     0,   163,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
     164,     0,   240,     0,   164,     0,     0,     0,     0,     0,
       0,     0,   164,     0,     0,     0,   241,   242,     0,   243,
       0,     0,   416,     0,   244,   264,   266,   267,     0,     0,
       0,     0,   245,   158,     0,     0,     0,     0,   246,     0,
     158,   101,     0,     0,   247,     0,     0,  1188,   248,     0,
       0,   249,   159,     0,     0,   158,     0,     0,     0,   159,
       0,   250,     0,     0,   319,     0,     0,   251,   252,     0,
       0,     0,     0,   158,   159,   253,     0,     0,     0,     0,
       0,     0,   158,     0,   254,     0,   158,     0,     0,     0,
       0,     0,   159,   255,   256,     0,   257,     0,   258,     0,
     259,   159,     0,   260,     0,   159,   163,   261,   488,   163,
     262,   485,     0,   263,     0,   366,     0,   498,     0,     0,
       0,     0,   353,     0,   354,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   158,   164,     0,     0,   164,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   159,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   163,     0,     0,   489,
     421,   367,   163,     0,     0,   368,   109,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   319,     0,     0,
       0,   109,   158,   158,   158,   158,   164,     0,     0,     0,
       0,     0,   164,   319,     0,   449,     0,     0,     0,     0,
       0,   159,   159,   159,   159,     0,   470,   471,     0,   158,
     158,     0,   370,   371,     0,   372,   373,   374,     0,   375,
     376,     0,     0,     0,   379,     0,     0,     0,   159,   159,
       0,   385,   386,     0,     0,   389,     0,     0,   390,   391,
     392,   490,     0,     0,   163,     0,   163,     0,     0,     0,
       0,   163,     0,     0,   163,     0,     0,     0,   109,     0,
       0,     0,     0,     0,     0,     0,     0,   163,     0,   163,
       0,     0,   163,   240,   164,     0,   164,     0,     0,     0,
       0,   164,     0,     0,   164,     0,   498,   241,   242,     0,
     243,     0,   498,     0,     0,   244,     0,   164,     0,   164,
     163,     0,   164,   245,     0,     0,   669,     0,     0,   246,
       0,   163,     0,   163,     0,   247,     0,     0,     0,   248,
       0,     0,   249,     0,   553,     0,     0,     0,     0,     0,
     164,     0,   250,     0,     0,     0,     0,     0,   251,   252,
       0,   164,     0,   164,     0,     0,   253,     0,     0,     0,
       0,     0,     0,     0,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,   255,   256,     0,   257,   729,   258,
       0,   259,   734,     0,   260,     0,     0,     0,   261,     0,
     109,   262,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   626,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   163,     0,     0,
       0,     0,     0,   638,   163,     0,     0,   641,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   163,
       0,     0,     0,     0,     0,     0,     0,   164,   424,     0,
     879,     0,     0,     0,   164,     0,     0,   163,     0,   240,
       0,     0,     0,     0,     0,   678,   163,     0,     0,   164,
     163,   490,     0,   241,   242,     0,   243,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,   164,     0,   245,
       0,     0,     0,     0,     0,   246,   164,     0,     0,     0,
     164,   247,     0,     0,     0,   248,     0,     0,   249,     0,
       0,     0,     0,     0,   842,     0,     0,   845,   250,   163,
       0,     0,     0,     0,   251,   252,     0,     0,     0,   742,
       0,     0,   253,     0,     0,     0,     0,     0,   757,     0,
       0,   254,     0,     0,     0,     0,     0,     0,     0,   164,
     255,   256,     0,   257,     0,   258,     0,   259,     0,     0,
     260,     0,     0,     0,   261,     0,     0,   262,     0,     0,
     263,     0,     0,     0,   884,     0,   163,   163,   163,   163,
     889,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   404,   405,   406,   407,   408,     0,     0,     0,     0,
       0,     0,     0,   163,   163,     0,   164,   164,   164,   164,
       0,     0,  -574,     0,     0,     0,     0,     0,     0,   809,
       0,     0,   812,     0,     0,   409,   269,   908,  -613,   909,
    -613,     0,     0,   164,   164,     0,     0,     0,   910,     0,
       0,     0,     0,   911,   242,   912,   913,     0,     0,     0,
       0,   914,     0,     0,     0,     0,     0,     0,     0,   245,
       0,   184,   498,     0,   498,   915,     0,     0,     0,   498,
       0,   916,   498,   858,     0,   248,     0,   490,   917,     0,
     918,     0,     0,     0,     0,   999,     0,  1000,   919,     0,
    1002,     0,     0,     0,   920,   921,     0,     0,     0,     0,
     881,     0,   253,     0,   319,     0,     0,     0,     0,     0,
       0,   922,     0,     0,     0,     0,     0,     0,  1022,     0,
     255,   256,     0,   923,     0,   258,     0,   924,     0,  1028,
     925,  1032,     0,     0,   926,     0,     0,   262,     0,     0,
     927,     0,     0,     0,     0,     0,     0,   953,     0,   475,
     395,   396,   397,   398,   399,     0,     0,   402,   403,   404,
     405,     0,   407,   408,   928,   929,   930,   931,   932,   933,
       0,   934,     0,     0,     0,   935,   936,   937,   938,   939,
     940,   941,   942,   943,   944,   945,     0,   946,     0,     0,
     947,   948,   949,   950,     0,     0,   951,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1004,     0,   678,     0,     0,     0,  1008,     0,     0,   490,
       0,   409,     0,     0,  -613,     0,  -613,  1084,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   881,     0,     0,     0,  1104,     0,     0,     0,     0,
       0,     0,     0,     0,  1109,     0,     0,     0,  1111,     0,
       0,     0,     0,     0,     0,     0,   757,     0,     0,     0,
       0,    -2,     4,     0,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,  1145,    30,    31,
      32,    33,    34,    35,    36,    37,    38,   -76,     0,    39,
      40,    41,     0,    42,  -305,     0,    43,    44,    45,    46,
      47,     0,    48,    49,    50,   -47,    51,    52,     0,    53,
      54,    55,     0,  -305,     0,     0,    56,    57,    58,    59,
      60,    61,    62,  -305,   -47,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
      76,     0,    77,    78,     0,    79,    80,    81,    82,    83,
       0,    84,    85,   -76,    86,    87,     0,     0,    88,     0,
      89,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
     953,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,   101,     0,     0,   102,     0,   103,
       0,   104,     0,   105,     0,     4,   106,     5,     0,     6,
       7,     8,     9,    10,    11,     0,  -643,     0,    12,    13,
      14,    15,    16,  -643,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,  -643,    27,    28,  -643,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
     -76,     0,    39,    40,    41,     0,    42,  -305,     0,    43,
      44,    45,    46,    47,     0,    48,    49,    50,   -47,    51,
      52,     0,    53,    54,    55,     0,  -305,     0,     0,    56,
      57,    58,    59,     0,    61,    62,  -305,   -47,    63,    64,
      65,  -643,    66,    67,    68,  -643,    69,    70,    71,    72,
      73,    74,    75,    76,     0,    77,    78,     0,    79,    80,
      81,    82,    83,     0,    84,    85,   -76,    86,    87,     0,
       0,    88,     0,    89,     0,     0,  -643,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -643,  -643,    93,  -643,  -643,  -643,  -643,  -643,
    -643,  -643,     0,  -643,  -643,  -643,  -643,  -643,     0,  -643,
    -643,  -643,  -643,  -643,  -643,  -643,  -643,   101,  -643,  -643,
    -643,     0,   103,  -643,   104,     0,   105,     0,   329,  -643,
       5,   293,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,    38,   -76,     0,    39,    40,    41,     0,    42,
    -305,     0,    43,    44,    45,    46,    47,     0,    48,    49,
      50,   -47,    51,    52,     0,    53,    54,    55,     0,  -305,
       0,     0,    56,    57,    58,    59,    60,    61,    62,  -305,
     -47,    63,    64,    65,     0,    66,    67,    68,     0,    69,
      70,    71,    72,    73,    74,    75,    76,     0,    77,    78,
       0,    79,    80,    81,    82,    83,     0,    84,    85,   -76,
      86,    87,     0,     0,    88,     0,    89,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
     101,     0,     0,   102,     0,   103,   330,   104,     0,   105,
       0,     4,   106,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,    38,   -76,     0,    39,    40,
      41,     0,    42,  -305,     0,    43,    44,    45,    46,    47,
       0,    48,    49,    50,   -47,    51,    52,     0,    53,    54,
      55,     0,  -305,     0,     0,    56,    57,    58,    59,    60,
      61,    62,  -305,   -47,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,    77,    78,     0,    79,    80,    81,    82,    83,     0,
      84,    85,   -76,    86,    87,     0,     0,    88,     0,    89,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,   101,     0,     0,   102,     0,   103,   525,
     104,     0,   105,     0,   544,   106,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,   -76,
       0,    39,    40,    41,     0,    42,  -305,     0,    43,    44,
      45,    46,    47,     0,    48,    49,    50,   -47,    51,    52,
       0,    53,    54,    55,     0,  -305,     0,     0,    56,    57,
      58,    59,    60,    61,    62,  -305,   -47,    63,    64,    65,
       0,    66,    67,    68,     0,    69,    70,    71,    72,    73,
      74,    75,    76,     0,    77,    78,     0,    79,    80,    81,
      82,    83,     0,    84,    85,   -76,    86,    87,     0,     0,
      88,     0,    89,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,   101,     0,     0,   102,
       0,   103,   545,   104,     0,   105,     0,   329,   106,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,    38,   -76,     0,    39,    40,    41,     0,    42,  -305,
       0,    43,    44,    45,    46,    47,     0,    48,    49,    50,
     -47,    51,    52,     0,    53,    54,    55,     0,  -305,     0,
       0,    56,    57,    58,    59,    60,    61,    62,  -305,   -47,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,     0,    84,    85,   -76,    86,
      87,     0,     0,    88,     0,    89,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,   101,
       0,     0,   102,     0,   103,   330,   104,     0,   105,     0,
       4,   106,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,   -76,     0,    39,    40,    41,
       0,    42,  -305,     0,    43,    44,    45,    46,    47,     0,
      48,    49,    50,   -47,    51,    52,     0,    53,    54,    55,
       0,  -305,     0,     0,    56,    57,    58,    59,    60,    61,
      62,  -305,   -47,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,    76,     0,
      77,    78,     0,    79,    80,    81,    82,    83,     0,    84,
      85,   -76,    86,    87,     0,     0,    88,     0,    89,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,   101,     0,     0,   102,     0,   103,   741,   104,
       0,   105,     0,     4,   106,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,    38,   -76,     0,
      39,    40,    41,     0,    42,  -305,     0,    43,    44,    45,
      46,    47,     0,    48,    49,    50,   -47,    51,    52,     0,
      53,    54,    55,     0,  -305,     0,     0,    56,    57,    58,
      59,   345,    61,    62,  -305,   -47,    63,    64,    65,     0,
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
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,   -76,     0,    39,    40,    41,     0,    42,  -305,     0,
      43,    44,    45,    46,    47,     0,    48,    49,    50,   -47,
      51,    52,     0,    53,    54,    55,     0,  -305,     0,     0,
      56,    57,    58,    59,     0,    61,    62,  -305,   -47,    63,
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
      35,    36,   646,    38,   -76,     0,    39,    40,    41,     0,
      42,  -305,     0,    43,    44,    45,    46,    47,     0,    48,
      49,    50,   -47,    51,    52,     0,    53,    54,    55,     0,
    -305,     0,     0,    56,    57,    58,    59,     0,    61,    62,
    -305,   -47,    63,    64,    65,     0,    66,    67,    68,     0,
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
      32,    33,    34,    35,    36,   652,    38,   -76,     0,    39,
      40,    41,     0,    42,  -305,     0,    43,    44,    45,    46,
      47,     0,    48,    49,    50,   -47,    51,    52,     0,    53,
      54,    55,     0,  -305,     0,     0,    56,    57,    58,    59,
       0,    61,    62,  -305,   -47,    63,    64,    65,     0,    66,
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
       0,    30,    31,    32,    33,    34,    35,    36,   982,    38,
     -76,     0,    39,    40,    41,     0,    42,  -305,     0,    43,
      44,    45,    46,    47,     0,    48,    49,    50,   -47,    51,
      52,     0,    53,    54,    55,     0,  -305,     0,     0,    56,
      57,    58,    59,     0,    61,    62,  -305,   -47,    63,    64,
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
      36,   984,    38,   -76,     0,    39,    40,    41,     0,    42,
    -305,     0,    43,    44,    45,    46,    47,     0,    48,    49,
      50,   -47,    51,    52,     0,    53,    54,    55,     0,  -305,
       0,     0,    56,    57,    58,    59,     0,    61,    62,  -305,
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
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,   989,    38,   -76,     0,    39,    40,
      41,     0,    42,  -305,     0,    43,    44,    45,    46,    47,
       0,    48,    49,    50,   -47,    51,    52,     0,    53,    54,
      55,     0,  -305,     0,     0,    56,    57,    58,    59,     0,
      61,    62,  -305,   -47,    63,    64,    65,     0,    66,    67,
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
      30,    31,    32,    33,    34,    35,    36,   992,    38,   -76,
       0,    39,    40,    41,     0,    42,  -305,     0,    43,    44,
      45,    46,    47,     0,    48,    49,    50,   -47,    51,    52,
       0,    53,    54,    55,     0,  -305,     0,     0,    56,    57,
      58,    59,     0,    61,    62,  -305,   -47,    63,    64,    65,
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
      20,    21,    22,    23,    24,    25,    26,     0,  1021,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,    38,   -76,     0,    39,    40,    41,     0,    42,  -305,
       0,    43,    44,    45,    46,    47,     0,    48,    49,    50,
     -47,    51,    52,     0,    53,    54,    55,     0,  -305,     0,
       0,    56,    57,    58,    59,     0,    61,    62,  -305,   -47,
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
      34,    35,    36,  1027,    38,   -76,     0,    39,    40,    41,
       0,    42,  -305,     0,    43,    44,    45,    46,    47,     0,
      48,    49,    50,   -47,    51,    52,     0,    53,    54,    55,
       0,  -305,     0,     0,    56,    57,    58,    59,     0,    61,
      62,  -305,   -47,    63,    64,    65,     0,    66,    67,    68,
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
      31,    32,    33,    34,    35,    36,  1031,    38,   -76,     0,
      39,    40,    41,     0,    42,  -305,     0,    43,    44,    45,
      46,    47,     0,    48,    49,    50,   -47,    51,    52,     0,
      53,    54,    55,     0,  -305,     0,     0,    56,    57,    58,
      59,     0,    61,    62,  -305,   -47,    63,    64,    65,     0,
      66,    67,    68,     0,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,     0,    84,    85,   -76,    86,    87,     0,     0,    88,
       0,    89,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,   101,     0,     0,   102,     0,
     103,     0,   104,     0,   105,     0,  1217,   106,     5,   293,
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
       0,   102,     0,   191,     0,   104,     0,   192,  1218,   867,
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
     100,     0,     0,     0,   102,     0,   191,     0,   104,     0,
     192,     0,     5,   106,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,   231,    23,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,    58,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,    66,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,    86,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     0,   106,     5,   293,     6,     7,     8,
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
      83,   334,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
     335,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,     0,     0,     0,   102,     0,
     191,     0,   104,     0,   192,     0,     0,   106,     5,   293,
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
     297,   298,     0,    83,   334,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,   725,   192,     0,     0,
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
       0,     0,    79,   297,   298,     0,    83,   334,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,     0,     0,     0,   102,     0,   191,     0,   104,   727,
     192,     0,     5,   106,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,   609,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,   610,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,   611,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,   612,    87,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   189,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   190,
       0,    77,     0,     0,    79,   297,   298,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,   299,     0,     0,   102,     0,   191,     0,
     104,     0,   192,     0,     0,   106,     5,   293,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,   297,   298,
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
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,   869,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     189,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,   297,
     298,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,     0,   104,     0,   192,     0,     5,   106,
       6,     7,     8,   218,    10,    11,   219,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     184,     0,     0,     0,    32,   185,   186,     0,     0,   187,
      38,     0,     0,     0,    40,     0,     0,    42,     0,     0,
     188,     0,     0,    46,    47,     0,     0,    49,    50,     0,
      51,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   220,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,   221,    83,     0,     0,    85,     0,     0,    87,
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
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,   223,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,   224,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   102,     0,   191,     0,   104,     0,   192,     0,
       5,   106,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,     0,   234,     0,     0,     0,     0,     0,
      28,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,   187,    38,     0,     0,     0,    40,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
     235,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,   192,
       0,     0,   106,     5,   293,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
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
       0,    17,     0,   183,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   184,     0,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,   297,   298,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,   723,   104,     0,   192,     0,     0,   106,
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
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,   192,
     733,     0,   106,     5,   293,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
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
     104,     0,   192,  1149,     5,   106,     6,     7,     8,     9,
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
       0,   224,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,     0,   192,     0,     5,   106,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,   273,    28,     0,   184,     0,     0,
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
     103,     0,   104,     0,   192,     0,     0,   106,     5,   293,
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
       0,   102,     0,   191,     0,   104,     0,   192,     0,     5,
     106,     6,     7,     8,   218,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   220,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,   221,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   102,     0,   191,     0,   104,     0,   192,     0,
       5,   106,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,   187,    38,     0,     0,     0,    40,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,   192,
     738,     5,   106,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,     0,     0,     0,    83,   895,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,     0,     0,     0,   102,     0,   191,     0,   104,     0,
     192,     0,     0,   106,     5,   293,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   697,     0,   184,     0,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
     698,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,   699,    73,    74,    75,
     700,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,     0,  1088,     0,     5,   106,     6,     7,     8,
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
      83,  1113,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,     0,     0,     0,   102,     0,
     191,     0,   104,     0,   192,     0,     0,   106,     5,   293,
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
       0,   102,     0,   191,     0,   104,     0,  1088,     0,     0,
     106,     5,   293,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,  1069,    38,     0,     0,     0,    40,     0,     0,
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
     192,     0,     5,   106,     6,     7,     8,     9,    10,    11,
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
       0,   192,     0,     5,   106,     6,     7,     8,   214,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   215,     0,     0,     0,
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
       0,    98,    99,   100,     0,     0,     0,   102,     0,   103,
       0,   104,     0,   192,     0,     5,   106,     6,     7,     8,
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
       0,     0,    98,    99,   100,     0,     0,     0,   661,     0,
     191,     0,   104,     0,   192,     0,     5,   106,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,   697,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,   698,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,   699,    73,
      74,    75,   700,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   701,     0,     5,   106,     6,
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
     102,     0,   191,     0,   104,     0,   701,     0,     5,   106,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     184,     0,     0,     0,    32,   185,   186,     0,     0,   793,
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
     796,    38,     0,     0,     0,    40,     0,     0,    42,     0,
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
      28,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,  1063,    38,     0,     0,     0,    40,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
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
       0,     0,  1064,    38,     0,     0,     0,    40,     0,     0,
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
     192,     0,     5,   106,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,  1066,    38,     0,     0,     0,    40,     0,
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
     185,   186,     0,     0,  1067,    38,     0,     0,     0,    40,
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
      32,   185,   186,     0,     0,  1068,    38,     0,     0,     0,
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
       0,    32,   185,   186,     0,     0,  1069,    38,     0,     0,
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
       0,     0,     0,     0,     0,     0,   697,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,   698,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,   699,    73,
      74,    75,   700,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,  1173,     0,     5,   106,     6,
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
     102,     0,     0,     0,   104,     0,  1173,     0,     5,   106,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,     0,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,   192,     5,   293,
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
       0,     0,     0,     0,     0,     0,     0,     0,  -471,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,  -471,
       0,     0,     0,  -471,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,   191,     0,   104,  -471,  1088,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     184,     0,     0,     0,    32,   185,   186,     0,     0,   187,
      38,     0,     0,     0,    40,     0,     0,    42,     0,     0,
     188,     0,     0,    46,    47,  -453,     0,    49,    50,     0,
      51,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,  -453,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,  -453,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,  -453,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,    93,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,   100,  -453,     0,
       0,     0,     0,  -453,    28,   104,   184,  1173,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
       5,    93,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,   100,     0,     0,     0,     0,     0,   191,
      28,   104,   184,   192,     0,     0,    32,   185,   186,     0,
       0,   187,    38,     0,     0,     0,    40,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,    93,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,   100,
       0,     0,     0,     0,     0,   191,    28,   104,   184,   701,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,   240,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,   241,
     242,     0,   243,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,   245,     0,     0,     0,     0,
       0,   246,     0,    93,     0,     0,     0,   247,     0,     0,
       0,   248,     0,     0,   249,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   250,     0,     0,     0,     0,     0,
     251,   252,     0,   104,     0,   192,     0,     0,   253,     0,
       0,     0,     0,     0,     0,     0,     0,   254,     0,     0,
       0,     0,     0,     0,     0,     0,   255,   256,     0,   257,
       0,   258,     0,   259,     0,  1126,   260,     0,     0,     0,
     261,     0,     0,   262,   910,     0,   263,     0,     0,   241,
     242,   912,   243,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,   245,     0,     0,     0,     0,
       0,   915,     0,     0,     0,     0,     0,   247,     0,     0,
       0,   248,     0,     0,   249,     0,   918,     0,     0,     0,
       0,     0,     0,     0,   250,     0,     0,     0,     0,     0,
     920,   252,   857,     0,     0,     0,     0,     0,   253,     0,
       0,     0,     0,     0,     0,     0,     0,   254,     0,     0,
       0,     0,     0,     0,     0,   364,   255,   256,     0,   257,
       0,   258,   365,  1127,     0,     0,   925,     0,     0,     0,
     261,     0,     0,   262,   366,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,   475,   395,   396,   397,   398,
     399,     0,     0,   402,   403,   404,   405,     0,   407,   408,
     928,   929,   930,   931,   932,   933,     0,   934,     0,     0,
       0,   935,   936,   937,   938,   939,   940,   941,   942,   943,
     944,   945,     0,   946,     0,     0,   947,   948,   949,   950,
     367,   364,     0,     0,   368,     0,     0,     0,   365,     0,
       0,     0,     0,   474,     0,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   369,   475,   395,   396,   397,
     398,   399,     0,     0,   402,   403,   404,   405,     0,   407,
     408,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,     0,   378,   379,   380,   381,   382,     0,   383,   384,
     385,   386,   387,   388,   389,     0,   367,   390,   391,   392,
     368,   424,     0,   364,     0,     0,     0,     0,   393,   981,
     365,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,     0,     0,     0,     0,     0,     0,     0,
       0,   369,   475,   395,   396,   397,   398,   399,     0,     0,
     402,   403,   404,   405,     0,   407,   408,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,     0,   378,   379,
     380,   381,   382,     0,   383,   384,   385,   386,   387,   388,
     389,     0,     0,   390,   391,   392,     0,   424,   367,   364,
       0,     0,   368,     0,   393,     0,   365,     0,     0,     0,
       0,   988,     0,     0,     0,     0,     0,     0,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   369,   475,   395,   396,   397,   398,   399,
       0,     0,   402,   403,   404,   405,     0,   407,   408,   370,
     371,     0,   372,   373,   374,     0,   375,   376,   377,     0,
     378,   379,   380,   381,   382,     0,   383,   384,   385,   386,
     387,   388,   389,     0,   367,   390,   391,   392,   368,   424,
       0,   364,     0,     0,     0,     0,   393,  1140,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,     0,     0,     0,     0,   369,
     475,   395,   396,   397,   398,   399,     0,     0,   402,   403,
     404,   405,     0,   407,   408,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,     0,   378,   379,   380,   381,
     382,     0,   383,   384,   385,   386,   387,   388,   389,     0,
       0,   390,   391,   392,     0,   424,   367,   364,     0,     0,
     368,     0,   393,     0,   365,     0,     0,     0,     0,  1141,
       0,     0,     0,     0,     0,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   369,   475,   395,   396,   397,   398,   399,     0,     0,
     402,   403,   404,   405,     0,   407,   408,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,     0,   378,   379,
     380,   381,   382,     0,   383,   384,   385,   386,   387,   388,
     389,     0,   367,   390,   391,   392,   368,   424,     0,   364,
       0,     0,     0,     0,   393,  1142,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,     0,
       0,     0,     0,     0,     0,     0,     0,   369,   475,   395,
     396,   397,   398,   399,     0,     0,   402,   403,   404,   405,
       0,   407,   408,   370,   371,     0,   372,   373,   374,     0,
     375,   376,   377,     0,   378,   379,   380,   381,   382,     0,
     383,   384,   385,   386,   387,   388,   389,     0,     0,   390,
     391,   392,     0,   424,   367,   364,     0,     0,   368,     0,
     393,     0,   365,     0,     0,     0,     0,  1143,     0,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   369,
     475,   395,   396,   397,   398,   399,     0,     0,   402,   403,
     404,   405,     0,   407,   408,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,     0,   378,   379,   380,   381,
     382,     0,   383,   384,   385,   386,   387,   388,   389,     0,
     367,   390,   391,   392,   368,   424,     0,   364,     0,     0,
       0,     0,   393,  1167,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,   369,   475,   395,   396,   397,
     398,   399,     0,     0,   402,   403,   404,   405,     0,   407,
     408,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,     0,   378,   379,   380,   381,   382,     0,   383,   384,
     385,   386,   387,   388,   389,     0,     0,   390,   391,   392,
       0,   424,   367,     0,     0,     0,   368,     0,   393,   364,
       0,     0,     0,     0,     0,  1168,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,   458,
       0,     0,     0,     0,     0,     0,     0,   369,   475,   395,
     396,   397,   398,   399,   459,     0,   402,   403,   404,   405,
       0,   407,   408,   370,   371,     0,   372,   373,   374,     0,
     375,   376,   377,     0,   378,   379,   380,   381,   382,     0,
     383,   384,   385,   386,   387,   388,   389,     0,     0,   390,
     391,   392,     0,   424,   367,   364,     0,     0,   368,     0,
     393,     0,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,     0,     0,     0,   369,
     464,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,     0,   378,   379,   380,   381,
     382,     0,   383,   384,   385,   386,   387,   388,   389,     0,
     367,   390,   391,   392,   368,   424,     0,   364,     0,     0,
       0,     0,   393,     0,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,   452,     0,     0,
       0,   455,     0,     0,     0,   369,     0,     0,     0,     0,
       0,     0,   453,     0,     0,     0,     0,     0,     0,     0,
       0,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,     0,   378,   379,   380,   381,   382,     0,   383,   384,
     385,   386,   387,   388,   389,     0,     0,   390,   391,   392,
       0,   424,   367,     0,     0,     0,   368,     0,   393,     0,
     364,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,   467,     0,     0,   369,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   370,   371,     0,   372,   373,   374,     0,
     375,   376,   377,     0,   378,   379,   380,   381,   382,   468,
     383,   384,   385,   386,   387,   388,   389,     0,     0,   390,
     391,   392,     0,   424,     0,   367,     0,     0,     0,   368,
     393,   364,     0,     0,     0,     0,     0,     0,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,   828,     0,     0,     0,     0,     0,     0,     0,     0,
     369,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,     0,   378,   379,   380,
     381,   382,     0,   383,   384,   385,   386,   387,   388,   389,
       0,   469,   390,   391,   392,     0,   367,   364,     0,     0,
     368,     0,     0,   393,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,   273,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   455,     0,     0,
       0,   369,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,     0,   378,   379,
     380,   381,   382,     0,   383,   384,   385,   386,   387,   388,
     389,     0,   367,   390,   391,   392,   368,   424,     0,   364,
       0,     0,     0,     0,   393,     0,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,   273,
       0,     0,     0,   455,     0,     0,     0,   369,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   370,   371,     0,   372,   373,   374,     0,
     375,   376,   377,     0,   378,   379,   380,   381,   382,     0,
     383,   384,   385,   386,   387,   388,   389,     0,     0,   390,
     391,   392,     0,   424,   367,   364,     0,     0,   368,     0,
     393,     0,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,   618,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   369,
     619,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,     0,   378,   379,   380,   381,
     382,     0,   383,   384,   385,   386,   387,   388,   389,     0,
     367,   390,   391,   392,   368,   424,   364,     0,     0,     0,
       0,     0,   393,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   620,     0,     0,     0,
       0,     0,     0,     0,     0,   369,     0,     0,     0,     0,
       0,   621,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,     0,   378,   379,   380,   381,   382,     0,   383,   384,
     385,   386,   387,   388,   389,     0,     0,   390,   391,   392,
       0,   367,   364,     0,     0,   368,     0,     0,   393,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   820,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   369,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,     0,   378,   379,   380,   381,   382,     0,   383,
     384,   385,   386,   387,   388,   389,     0,   367,   390,   391,
     392,   368,     0,     0,   364,     0,     0,     0,     0,   393,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,     0,     0,     0,     0,     0,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   370,   371,
       0,   372,   373,   374,     0,   375,   376,   377,     0,   378,
     379,   380,   381,   382,     0,   383,   384,   385,   386,   387,
     388,   389,     0,     0,   390,   391,   392,     0,   424,   367,
     364,     0,     0,   368,     0,   393,     0,   365,     0,     0,
       0,     0,   846,     0,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   369,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     370,   371,     0,   372,   373,   374,     0,   375,   376,   377,
       0,   378,   379,   380,   381,   382,     0,   383,   384,   385,
     386,   387,   388,   389,     0,   367,   390,   391,   392,   368,
     424,     0,     0,   364,     0,     0,     0,   393,     0,     0,
     365,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,     0,     0,     0,   455,  1040,     0,     0,
     369,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,     0,   378,   379,   380,
     381,   382,  1041,   383,   384,   385,   386,   387,   388,   389,
       0,     0,   390,   391,   392,     0,     0,     0,   367,   364,
       0,   887,   368,   393,     0,     0,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   369,     0,     0,     0,     0,     0,     0,
     364,     0,     0,     0,     0,     0,     0,   365,     0,   370,
     371,     0,   372,   373,   374,     0,   375,   376,   377,   366,
     378,   379,   380,   381,   382,     0,   383,   384,   385,   386,
     387,   388,   389,     0,   367,   390,   391,   392,   368,     0,
       0,     0,     0,     0,     0,   364,   393,     0,     0,     0,
       0,     0,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,   369,
       0,     0,     0,     0,     0,   367,     0,     0,     0,   368,
       0,     0,     0,     0,     0,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,     0,   378,   379,   380,   381,
     382,     0,   383,   384,   385,   386,   387,   388,   389,     0,
     369,   390,   391,   392,     0,   510,     0,     0,     0,     0,
     367,     0,   393,     0,   368,     0,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,     0,   378,   379,   380,
     381,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     364,   514,   390,   391,   392,   369,     0,   365,     0,     0,
       0,     0,     0,   393,     0,     0,     0,     0,     0,   366,
       0,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,     0,   378,   379,   380,   381,   382,     0,   383,   384,
     385,   386,   387,   388,   389,   364,   516,   390,   391,   392,
       0,     0,   365,     0,     0,     0,     0,     0,   393,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,   368,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   364,     0,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
     369,     0,     0,   366,     0,     0,     0,     0,     0,     0,
     367,     0,     0,     0,   368,     0,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,     0,   378,   379,   380,
     381,   382,     0,   383,   384,   385,   386,   387,   388,   389,
       0,   518,   390,   391,   392,   369,     0,     0,     0,     0,
       0,     0,     0,   393,     0,     0,     0,     0,     0,   367,
       0,   370,   371,   368,   372,   373,   374,     0,   375,   376,
     377,     0,   378,   379,   380,   381,   382,     0,   383,   384,
     385,   386,   387,   388,   389,   364,   523,   390,   391,   392,
       0,     0,   365,   540,   369,     0,     0,     0,   393,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,     0,
     370,   371,     0,   372,   373,   374,     0,   375,   376,   377,
       0,   378,   379,   380,   381,   382,     0,   383,   384,   385,
     386,   387,   388,   389,   364,     0,   390,   391,   392,     0,
       0,   365,     0,     0,     0,     0,     0,   393,     0,     0,
       0,     0,     0,   366,     0,     0,     0,     0,     0,     0,
     367,     0,     0,     0,   368,     0,     0,     0,     0,     0,
       0,     0,     0,   622,   364,   745,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,     0,   369,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,   370,   371,   368,   372,   373,   374,     0,   375,   376,
     377,     0,   378,   379,   380,   381,   382,     0,   383,   384,
     385,   386,   387,   388,   389,     0,     0,   390,   391,   392,
       0,     0,     0,     0,   369,     0,     0,     0,   393,   367,
       0,     0,     0,   368,     0,     0,     0,     0,     0,     0,
     370,   371,     0,   372,   373,   374,     0,   375,   376,   377,
       0,   378,   379,   380,   381,   382,     0,   383,   384,   385,
     386,   387,   388,   389,   369,     0,   390,   391,   392,     0,
       0,   364,     0,   724,     0,     0,     0,   393,   365,     0,
     370,   371,     0,   372,   373,   374,     0,   375,   746,   377,
     366,   378,   379,   380,   381,   382,     0,   383,   384,   385,
     386,   387,   388,   389,     0,     0,   390,   391,   392,     0,
       0,     0,     0,     0,     0,     0,   364,   393,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
     368,     0,     0,     0,     0,     0,   364,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,     0,     0,     0,     0,
       0,   369,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,   368,     0,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,     0,   378,   379,
     380,   381,   382,  -575,   383,   384,   385,   386,   387,   388,
     389,     0,   177,   390,   391,   392,   369,     0,     0,     0,
       0,   367,     0,     0,   393,   368,     0,     0,     0,     0,
       0,     0,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,     0,   378,   379,   380,   381,   382,     0,   383,
     384,   385,   386,   387,   388,   389,   369,     0,   390,   391,
     392,     0,     0,   364,   837,     0,     0,     0,     0,   393,
     365,     0,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,   366,   378,   379,   380,   381,   382,     0,   383,
     384,   385,   386,   387,   388,   389,     0,   805,   390,   391,
     392,     0,     0,   364,     0,     0,     0,     0,     0,   393,
     365,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,     0,     0,   844,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,   368,   364,     0,     0,     0,     0,     0,     0,
     365,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   369,     0,     0,     0,     0,   367,     0,
       0,     0,   368,     0,     0,     0,     0,     0,     0,   370,
     371,     0,   372,   373,   374,     0,   375,   376,   377,     0,
     378,   379,   380,   381,   382,     0,   383,   384,   385,   386,
     387,   388,   389,   369,     0,   390,   391,   392,   367,     0,
       0,     0,   368,     0,     0,     0,   393,     0,     0,   370,
     371,     0,   372,   373,   374,     0,   375,   376,   377,     0,
     378,   379,   380,   381,   382,     0,   383,   384,   385,   386,
     387,   388,   389,   369,     0,   390,   391,   392,     0,     0,
     364,     0,     0,     0,     0,     0,   393,   365,     0,   370,
     371,     0,   372,   373,   374,     0,   375,   376,   377,   366,
     378,   379,   380,   381,   382,     0,   383,   384,   385,   386,
     387,   388,   389,     0,     0,   390,   391,   392,     0,     0,
       0,   364,   871,     0,     0,     0,   393,     0,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,   368,
       0,   364,     0,     0,     0,     0,     0,     0,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,   966,     0,     0,     0,     0,     0,     0,     0,     0,
     369,     0,     0,     0,     0,     0,   367,     0,     0,     0,
     368,     0,     0,     0,     0,     0,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,     0,   378,   379,   380,
     381,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     890,   369,   390,   391,   392,     0,   367,     0,     0,   872,
     368,     0,     0,   393,     0,     0,     0,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,     0,   378,   379,
     380,   381,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   369,     0,   390,   391,   392,     0,     0,     0,     0,
       0,     0,     0,     0,   393,     0,     0,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,     0,   378,   379,
     380,   381,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   364,   288,   390,   391,   392,    22,    23,   365,     0,
       0,     0,     0,     0,   393,   289,     0,    30,   290,     0,
     366,   969,     0,    36,     0,     0,   -77,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   364,     0,     0,   -48,     0,     0,     0,   365,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,     0,
     366,     0,     0,   -48,     0,     0,   291,     0,    66,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
     368,   364,  1037,     0,     0,     0,     0,    82,   365,     0,
      84,     0,   -77,    86,     0,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   369,     0,     0,     0,     0,   367,     0,     0,     0,
     368,     0,     0,     0,     0,     0,     0,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,     0,   378,   379,
     380,   381,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   369,     0,   390,   391,   392,   367,     0,     0,     0,
     368,     0,     0,     0,   393,     0,     0,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,     0,   378,   379,
     380,   381,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   369,     0,   390,   391,   392,     0,     0,   364,     0,
       0,     0,   972,     0,   393,   365,     0,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,   366,   378,   379,
     380,   381,   382,     0,   383,   384,   385,   386,   387,   388,
     389,     0,     0,   390,   391,   392,     0,     0,   364,     0,
       0,     0,     0,     0,   393,   365,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,   368,   364,  1082,
       0,     0,     0,     0,     0,   365,  1062,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   369,     0,
       0,     0,     0,   367,     0,     0,     0,   368,     0,     0,
       0,     0,     0,     0,   370,   371,  1065,   372,   373,   374,
       0,   375,   376,   377,     0,   378,   379,   380,   381,   382,
       0,   383,   384,   385,   386,   387,   388,   389,   369,     0,
     390,   391,   392,   367,     0,     0,     0,   368,     0,     0,
       0,   393,     0,     0,   370,   371,     0,   372,   373,   374,
       0,   375,   376,   377,     0,   378,   379,   380,   381,   382,
       0,   383,   384,   385,   386,   387,   388,   389,   369,     0,
     390,   391,   392,     0,     0,   364,     0,     0,     0,     0,
       0,   393,   365,     0,   370,   371,     0,   372,   373,   374,
       0,   375,   376,   377,   366,   378,   379,   380,   381,   382,
       0,   383,   384,   385,   386,   387,   388,   389,     0,     0,
     390,   391,   392,     0,     0,   364,     0,     0,     0,     0,
       0,   393,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     367,     0,     0,     0,   368,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   369,     0,     0,     0,     0,
     367,     0,     0,     0,   368,     0,     0,     0,     0,     0,
       0,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,     0,   378,   379,   380,   381,   382,     0,   383,   384,
     385,   386,   387,   388,   389,   369,     0,   390,   391,   392,
       0,     0,   364,     0,  1125,     0,     0,     0,   393,   365,
       0,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   366,   378,   379,   380,   381,   382,     0,   383,   384,
     385,   386,   387,   388,   389,     0,     0,   390,   391,   392,
       0,     0,   364,     0,     0,     0,  1163,     0,   393,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,     0,     0,
       0,   368,   364,     0,     0,     0,     0,     0,     0,   365,
    1182,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   369,     0,     0,     0,     0,   367,     0,     0,
       0,   368,     0,     0,     0,     0,     0,     0,   370,   371,
    1183,   372,   373,   374,     0,   375,   376,   377,     0,   378,
     379,   380,   381,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   369,     0,   390,   391,   392,   367,     0,     0,
       0,   368,     0,     0,     0,   393,     0,     0,   370,   371,
    1184,   372,   373,   374,     0,   375,   376,   377,     0,   378,
     379,   380,   381,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   369,     0,   390,   391,   392,     0,     0,   364,
       0,     0,     0,     0,     0,   393,   365,     0,   370,   371,
       0,   372,   373,   374,     0,   375,   376,   377,   366,   378,
     379,   380,   381,   382,     0,   383,   384,   385,   386,   387,
     388,   389,     0,     0,   390,   391,   392,     0,     0,   364,
       0,     0,     0,     0,     0,   393,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,   368,   364,
       0,     0,     0,     0,     0,     0,   365,  1185,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   369,
       0,     0,     0,     0,   367,     0,     0,     0,   368,     0,
       0,     0,     0,     0,     0,   370,   371,  1186,   372,   373,
     374,     0,   375,   376,   377,     0,   378,   379,   380,   381,
     382,     0,   383,   384,   385,   386,   387,   388,   389,   369,
       0,   390,   391,   392,   367,     0,     0,     0,   368,     0,
       0,     0,   393,     0,     0,   370,   371,  1187,   372,   373,
     374,     0,   375,   376,   377,     0,   378,   379,   380,   381,
     382,     0,   383,   384,   385,   386,   387,   388,   389,   369,
       0,   390,   391,   392,     0,     0,   364,     0,     0,     0,
       0,     0,   393,   365,     0,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,   366,   378,   379,   380,   381,
     382,     0,   383,   384,   385,   386,   387,   388,   389,     0,
       0,   390,   391,   392,     0,     0,   364,     0,     0,     0,
       0,     0,   393,   365,  1211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,   368,   364,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   369,     0,     0,     0,
       0,   367,     0,     0,     0,   368,     0,     0,     0,     0,
       0,     0,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,     0,   378,   379,   380,   381,   382,     0,   383,
     384,   385,   386,   387,   388,   389,   369,     0,   390,   391,
     392,   367,     0,     0,     0,   368,     0,     0,     0,   393,
       0,     0,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,     0,   378,   379,   380,   381,   382,     0,   383,
     384,   385,   386,   387,   388,   389,   369,     0,   390,   391,
     392,     0,     0,   364,     0,     0,     0,  1240,     0,   393,
     365,     0,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,   366,   378,   379,   380,   381,   382,     0,   383,
     384,   385,   386,   387,   388,   389,     0,   288,   390,   391,
     392,    22,    23,     0,     0,     0,     0,     0,     0,   393,
     289,     0,    30,   290,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,     0,    41,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   791,   366,
       0,     0,   368,     0,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,  1046,     0,     0,  1047,
       0,   291,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   369,     0,     0,     0,     0,     0,   366,
       0,     0,    82,     0,     0,    84,     0,     0,    86,   370,
     371,     0,   372,   373,   374,   367,   375,   376,   377,   368,
     378,   379,   380,   381,   382,     0,   383,   384,   385,   386,
     387,   388,   389,     0,     0,   390,   391,   392,     0,   366,
       0,     0,     0,     0,     0,     0,   393,     0,     0,     0,
     369,     0,     0,     0,     0,   367,     0,     0,   101,   368,
       0,     0,     0,     0,  1189,     0,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,     0,   378,   379,   380,
     381,   366,     0,   383,   384,   385,   386,   387,   388,   389,
     369,     0,   390,   391,   392,   367,     0,     0,     0,   368,
       0,     0,     0,   393,     0,     0,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,     0,   378,   379,   380,
     381,     0,     0,   383,   384,   385,   386,   387,     0,   389,
       0,     0,   390,   391,   392,     0,     0,   367,     0,     0,
       0,   368,     0,   393,     0,     0,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,     0,   378,   379,   380,
     381,     0,     0,   383,   384,   385,   386,   387,     0,   389,
       0,     0,   390,   391,   392,     0,     0,     0,     0,     0,
       0,     0,     0,   393,     0,     0,     0,     0,   370,   371,
       0,   372,   373,   374,     0,   375,   376,   377,     0,     0,
     379,   380,   381,     0,     0,   383,   384,   385,   386,     0,
     756,   389,   240,     0,   390,   391,   392,     0,     0,     0,
       0,     0,     0,     0,     0,   393,   241,   242,     0,   243,
       0,     0,     0,     0,   244,     0,     0,     0,     0,     0,
       0,     0,   245,     0,     0,     0,     0,     0,   246,     0,
       0,     0,     0,  -286,   247,     0,     0,     0,   248,     0,
       0,   249,     0,     0,     0,     0,     0,  -286,  -286,     0,
    -286,   250,     0,     0,     0,  -286,     0,   251,   252,     0,
       0,     0,     0,  -286,     0,   253,     0,     0,     0,  -286,
       0,     0,     0,     0,   254,  -286,     0,     0,     0,  -286,
       0,     0,  -286,   255,   256,     0,   257,     0,   258,     0,
     259,     0,  -286,   260,     0,     0,     0,   261,  -286,  -286,
     262,     0,     0,   263,     0,     0,  -286,     0,     0,     0,
       0,     0,     0,     0,     0,  -286,     0,     0,  -296,     0,
       0,     0,     0,     0,  -286,  -286,     0,  -286,     0,  -286,
       0,  -286,  -296,  -296,  -286,  -296,     0,     0,  -286,     0,
    -296,  -286,     0,     0,  -286,     0,     0,     0,  -296,     0,
       0,     0,     0,     0,  -296,     0,     0,     0,     0,   240,
    -296,     0,     0,     0,  -296,     0,     0,  -296,     0,     0,
       0,     0,     0,   241,   242,     0,   243,  -296,     0,     0,
       0,   244,     0,  -296,  -296,     0,     0,     0,     0,   245,
       0,  -296,     0,     0,     0,   246,     0,     0,     0,     0,
    -296,   247,     0,     0,     0,   248,     0,     0,   249,  -296,
    -296,     0,  -296,     0,  -296,     0,  -296,     0,   250,  -296,
       0,     0,     0,  -296,   251,   252,  -296,     0,     0,  -296,
       0,     0,   253,     0,     0,     0,     0,     0,     0,     0,
       0,   254,     0,     0,  -287,     0,     0,     0,     0,     0,
     255,   256,     0,   257,     0,   258,     0,   259,  -287,  -287,
     260,  -287,     0,     0,   261,     0,  -287,   262,     0,     0,
     263,     0,     0,     0,  -287,     0,     0,     0,     0,     0,
    -287,     0,     0,     0,     0,  -174,  -287,     0,     0,     0,
    -287,     0,     0,  -287,     0,     0,     0,     0,     0,  -174,
    -174,     0,  -174,  -287,     0,     0,     0,  -174,     0,  -287,
    -287,     0,     0,     0,     0,  -174,     0,  -287,     0,     0,
       0,  -174,     0,     0,     0,     0,  -287,  -174,     0,     0,
       0,  -174,     0,     0,  -174,  -287,  -287,     0,  -287,     0,
    -287,     0,  -287,     0,  -174,  -287,     0,     0,     0,  -287,
    -174,  -174,  -287,     0,     0,  -287,     0,     0,  -174,     0,
       0,     0,     0,     0,     0,     0,     0,  -174,     0,     0,
    -170,     0,     0,     0,     0,     0,  -174,  -174,     0,  -174,
       0,  -174,     0,  -174,  -170,  -170,  -174,  -170,     0,     0,
    -174,     0,  -170,  -174,     0,     0,  -174,     0,     0,     0,
    -170,     0,     0,     0,     0,     0,  -170,     0,     0,     0,
       0,     0,  -170,     0,     0,     0,  -170,     0,     0,  -170,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -170,
       0,     0,     0,     0,     0,  -170,  -170,     0,     0,     0,
       0,     0,     0,  -170,     0,     0,     0,     0,     0,     0,
       0,     0,  -170,     0,     0,     0,     0,     0,     0,     0,
       0,  -170,  -170,     0,  -170,     0,  -170,     0,  -170,     0,
       0,  -170,     0,     0,     0,  -170,     0,     0,  -170,     0,
       0,  -170
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1130)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,    63,   415,    67,    16,     2,    19,   468,    20,   519,
     205,   650,   482,   161,    26,   486,   147,   489,    30,    31,
     239,    33,    34,    35,    36,    37,   221,  1098,   765,   494,
       1,   866,    32,   901,   751,   100,    48,   750,    50,    22,
       1,   105,     1,    55,  1011,    57,   638,    59,  1008,   641,
      32,    32,   696,    32,   519,    67,    68,    69,    70,    71,
      72,    55,    47,    32,    76,     3,    78,   286,    80,    81,
     956,   957,  1201,    85,    47,   206,    88,    89,    67,    91,
     118,    93,    20,    60,    96,    97,  1086,    99,    90,   308,
      55,   103,   104,   105,   106,    33,   144,     0,    47,     3,
      53,    60,    73,   168,     1,    36,    81,   163,  1149,    84,
      47,  1240,    90,     3,   103,   104,   105,    47,   118,    72,
      55,   213,    73,   118,    62,   173,   961,  1087,   192,    82,
      46,    90,   228,   229,   230,   173,    68,   118,   777,   116,
      71,    33,    34,    35,   115,   157,   790,   239,  1148,  1220,
      47,     3,    31,   623,     3,  1122,   977,   116,   979,   144,
    1201,    47,   123,   101,   115,   144,   168,   315,   168,   107,
     182,   144,   104,   185,   186,   187,   170,  1012,   190,   191,
     192,  1049,   143,   144,   143,   163,   168,   168,   173,   168,
     168,   204,   411,   412,   286,   144,   112,   101,   169,   168,
     173,   213,   191,   192,  1072,   170,  1074,   144,    87,   168,
     171,   101,    91,   105,   144,    18,   308,   809,   169,   863,
     812,    80,    81,   695,   173,   287,   123,   239,    47,   116,
     168,    27,   118,    60,   217,   144,   173,   650,    90,   222,
     118,    47,  1128,  1129,   340,   341,   143,   144,   231,   101,
    1071,  1086,  1073,    27,    27,   163,    55,   144,   144,   138,
     169,   169,    46,    90,   143,    55,   163,   146,  1136,    88,
    1138,   150,   144,    76,   286,   123,   103,    50,   290,   144,
      64,    77,    88,   753,   376,   297,   298,   173,    60,   116,
     751,    94,    88,   512,    87,   173,   308,   169,    91,    83,
     148,   144,    75,    77,    77,    87,   171,   144,   111,    91,
     144,   107,   443,  1148,    88,    88,   144,   448,    90,   411,
     412,  1058,  1039,   335,  1041,  1038,   169,  1040,   112,   144,
     144,   103,   802,   107,   171,   332,   163,   171,   350,   351,
     113,   168,    55,   171,   116,    88,   170,   986,   163,   163,
     362,   163,   364,   365,   366,   367,   368,   369,   170,   371,
     372,   373,   374,   375,   777,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,    47,   144,   855,   857,   450,  1218,   400,   401,
     482,   163,    27,    60,   406,    60,   168,   409,   410,   411,
     412,   413,   414,   415,   163,  1236,   123,     3,     1,   144,
     171,   170,    47,   172,   426,   144,    51,   217,   144,   431,
     512,   433,   222,    90,    20,    90,    55,   144,   163,   144,
     901,   148,   893,    27,   163,   138,   103,    33,   450,    74,
     452,   453,    77,   146,    79,   171,   458,   459,   163,   116,
     170,   116,   464,    88,   171,   467,   468,    55,   123,   144,
     144,   450,   474,   144,   144,   477,    62,    60,   400,   401,
     482,    55,   107,   144,   406,   367,   368,   409,   410,   144,
     144,    55,   163,    77,   496,   144,   171,   171,   500,   501,
     554,   171,   163,   505,    88,    27,   163,    90,   163,     8,
     512,   168,    55,   168,   163,   101,   171,   171,  1018,   144,
     103,   107,   123,   107,   170,   527,   505,    55,   530,   113,
     532,   413,   414,   116,   536,   537,   538,   722,   540,   541,
     144,   623,   144,   144,   426,    55,   171,   148,   527,   144,
      98,   530,   554,   532,   636,    77,  1011,   144,   537,   546,
     143,   163,   144,  1018,    55,   170,    88,   171,   654,   655,
     171,   453,   658,   659,    55,   554,   171,   459,   138,    27,
     163,    55,   464,   986,   171,   168,   146,   138,  1039,   171,
    1041,   113,   138,  1054,    55,   146,   163,   143,    55,    47,
     146,    83,   169,    51,   150,   170,   168,   172,   530,    55,
     532,  1072,   170,  1074,   172,   162,   618,   619,   620,   621,
     622,   623,   624,   625,   627,   628,    74,   765,  1154,    77,
     163,    79,   634,   170,   636,   172,   170,   168,   172,   170,
      88,   170,   644,   172,   646,   731,   732,   701,   650,   170,
     652,   172,   163,   154,   536,   634,   530,    31,   532,   107,
     123,   163,   170,   745,   746,   667,   668,  1122,   163,    48,
     163,   753,    55,   163,   163,  1136,   163,  1138,   163,  1205,
    1206,  1207,  1208,  1209,  1210,   168,   149,   117,   151,   152,
      86,    70,   155,   156,  1149,    86,   163,   699,   700,   701,
     170,    80,    81,   705,   170,   170,   170,   123,  1163,   163,
     170,   170,   634,    87,     8,    64,   143,    91,   168,    67,
     802,   146,   701,   170,   163,   123,   171,   144,   163,    47,
     170,   123,    34,   123,    34,   171,   144,   619,   171,   621,
     171,   743,   123,    47,    21,   747,  1201,   749,   750,   751,
     171,   753,   170,   173,   173,     2,    60,   144,   168,    70,
     163,   163,   163,  1218,   138,    12,   163,   171,   163,   143,
     163,   145,   146,   173,   163,   777,   150,   144,    25,   163,
      27,  1236,   170,   705,   158,  1240,    90,   163,    34,   791,
     792,   793,   166,    34,   796,    64,   171,   169,   171,   144,
     802,   169,   123,    88,    32,   171,    32,   168,   173,   168,
    1019,  1020,   116,    34,   143,    88,   818,   163,   820,   123,
      31,   875,   173,   825,   203,   163,   828,   170,   170,    76,
     143,   163,   173,   835,   173,   837,   169,   146,   146,   170,
     144,   146,   844,   138,   168,   146,   225,   226,   227,   228,
     229,   230,   138,   146,   146,   146,   103,   236,   237,   163,
     138,   108,   146,   146,   168,   146,   146,   171,   138,   168,
     138,   146,    34,   875,   163,   171,    87,   480,   103,   138,
      91,    47,   138,   885,   171,   887,   743,   266,   890,   163,
     162,   893,   171,   144,    60,   171,   875,   276,   163,   163,
     163,   903,   163,   282,   162,   347,   173,    24,   850,   791,
     792,   316,  1058,   879,   225,   226,   227,   228,   229,   230,
    1058,   904,  1054,   855,    90,   304,   907,   138,   139,   688,
    1220,   178,   143,  1178,   145,   146,  1211,  1019,  1020,   150,
    1204,    36,   497,   322,   636,    30,    -1,   158,    -1,   951,
     116,    -1,    -1,   164,   165,   166,    -1,   123,    -1,    -1,
      -1,    -1,    -1,   965,   966,   276,   968,   969,    -1,   971,
     972,   282,    27,    -1,    -1,    -1,    -1,    -1,   144,   981,
     982,    -1,   984,    -1,   986,    -1,   988,   989,    -1,    -1,
     992,    31,    47,   304,    -1,  1047,    51,   163,    -1,    -1,
      -1,    -1,   168,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,   322,    -1,    -1,    -1,  1017,  1018,  1019,  1020,    74,
     267,    -1,    77,  1025,    79,  1027,   273,    -1,    -1,  1031,
      -1,    -1,  1011,    88,  1088,  1037,  1038,  1039,  1040,  1041,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,   427,    -1,
      -1,    91,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1062,  1063,  1064,  1065,  1066,  1067,  1068,  1069,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   457,    -1,
    1082,    -1,   461,    -1,   463,   332,  1088,   466,    -1,    -1,
      -1,  1093,    -1,    -1,    -1,  1097,    -1,    -1,   138,   139,
     347,   141,   142,   143,    -1,   145,   146,   147,    -1,  1088,
     150,    -1,    -1,    -1,  1093,    -1,   427,   157,   158,  1173,
      -1,   161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     2,   175,    -1,    -1,  1140,  1141,
    1142,  1143,    -1,    -1,    12,    -1,   457,    -1,    -1,    -1,
     461,    -1,   463,    -1,    -1,   466,    -1,    25,    -1,    27,
      -1,  1163,    -1,    -1,    -1,  1167,  1168,    -1,    -1,    -1,
    1149,  1173,    -1,    -1,    -1,    -1,    -1,   424,    -1,    -1,
    1182,  1183,  1184,  1185,  1186,  1187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1173,    -1,    -1,    -1,  1200,  1201,
      31,  1203,    -1,    -1,    -1,   452,    -1,    -1,    76,    -1,
      -1,   458,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1201,    -1,    -1,   472,    -1,   474,    -1,    -1,
      -1,    -1,    -1,    -1,  1236,   103,    -1,    -1,  1240,    -1,
     108,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,  1240,    31,    -1,    -1,    -1,    -1,    36,    -1,   648,
     649,    -1,    -1,    -1,    -1,   654,   655,    -1,    -1,   658,
     659,    -1,    -1,    -1,    -1,    -1,    -1,   534,    -1,    -1,
      -1,   538,    -1,    -1,    -1,   674,    -1,    -1,    -1,   546,
      -1,     2,    71,    -1,    -1,    -1,    -1,   138,    -1,    -1,
     178,    12,   143,    -1,   145,   146,    -1,    -1,    87,   150,
      -1,    -1,    91,    -1,    25,    -1,    27,   158,    -1,    -1,
      -1,    -1,    -1,   164,   165,   166,    -1,   648,   649,   718,
      -1,    -1,    -1,   654,   655,    -1,    -1,   658,   659,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   615,   138,
     139,    -1,   141,   142,   143,    76,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,     2,   164,   165,   166,    31,   267,
      -1,    -1,   103,    -1,    12,   273,   175,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,   670,   803,   804,   673,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   815,    -1,   817,    -1,
      -1,    -1,    -1,    -1,    -1,   824,    -1,    -1,   827,    -1,
      -1,   830,    -1,   832,    87,   834,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,   332,    -1,    -1,    -1,    76,    -1,
      -1,    -1,   851,    -1,    -1,    -1,    -1,   178,    -1,   347,
      -1,    -1,    -1,   730,    -1,    -1,    -1,    -1,    -1,   736,
      -1,    -1,   803,   804,    -1,   103,    -1,    -1,    -1,    -1,
     108,    -1,    -1,   882,   815,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,    -1,    -1,   827,   150,   765,   830,
      -1,   832,    -1,   834,     2,   158,    -1,    -1,    -1,    -1,
      -1,   164,   165,   166,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   791,   792,    -1,    25,    22,    27,
      -1,    -1,    26,    27,    -1,    -1,   424,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,    -1,   267,    -1,    -1,    43,
     178,   818,   273,   820,    -1,    -1,    50,    -1,   825,    -1,
      -1,   828,    -1,    -1,   452,    -1,    -1,    -1,    -1,    -1,
     458,    -1,    -1,    -1,   841,    -1,   843,    -1,    76,   846,
      -1,    75,    -1,    77,   472,    79,   474,    81,    -1,    -1,
      84,    -1,    86,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   874,    -1,    -1,
     108,   332,    -1,   107,    -1,    -1,   110,    -1,   885,   113,
     887,    -1,    -1,    -1,  1023,    -1,   347,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,
      -1,    -1,    -1,    -1,    17,   273,   534,    20,    -1,    -1,
     538,    -1,    25,    -1,    -1,    -1,    -1,    -1,   546,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    45,    -1,    -1,   169,    -1,  1076,  1077,    52,
     178,  1080,  1081,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    -1,  1023,    -1,    -1,    -1,    69,    -1,    -1,  1098,
      -1,    -1,    -1,   424,   332,    -1,    -1,  1106,    -1,  1108,
      -1,  1110,    85,    -1,   981,    -1,    -1,    -1,    -1,   347,
      -1,   988,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,   452,    -1,    -1,    -1,   108,  1003,   458,    -1,    -1,
      -1,   114,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,   472,    -1,   474,  1021,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,  1030,    -1,    43,    -1,  1034,    -1,   267,
      -1,    -1,    50,    -1,    -1,   273,    -1,    -1,    -1,    -1,
      -1,    -1,   670,    -1,    -1,   673,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   424,    75,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    84,    -1,    86,    -1,
      88,    -1,    -1,   534,    -1,    -1,  1083,   538,    -1,    -1,
      -1,  1220,    -1,    -1,   452,   546,    -1,    -1,    -1,   107,
     458,    -1,   110,    -1,   332,   113,    -1,    -1,    -1,    -1,
      -1,    -1,   730,    -1,   472,    -1,   474,    -1,   736,   347,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,  1140,  1141,  1142,  1143,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    50,   163,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,
    1167,  1168,    -1,    -1,    12,    -1,   534,    -1,    -1,    75,
     538,    77,    -1,     2,    -1,    -1,    -1,    25,   546,    27,
      86,    -1,    88,    12,    -1,    -1,   424,    -1,    -1,    -1,
     818,    -1,   820,    -1,    -1,    -1,    25,   825,    27,    -1,
     828,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,   670,
      -1,    -1,   673,   841,   452,   843,    -1,    -1,   846,    -1,
     458,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,   472,    22,   474,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,   874,    76,    35,    -1,
      37,    38,    -1,    -1,    -1,   103,    43,   885,    -1,   887,
     108,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   730,
      -1,    -1,    -1,    -1,   103,   736,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      77,    -1,    79,    -1,    81,    -1,   534,    84,    -1,    86,
     538,    88,   670,     1,    -1,   673,    -1,    -1,   546,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,    -1,   110,    22,    -1,   113,    -1,    26,    27,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,   178,
      -1,    -1,    50,   981,    -1,    53,    -1,   818,    -1,   820,
     988,    -1,   730,    -1,   825,    -1,    -1,   828,   736,    -1,
      -1,    -1,    -1,    -1,    72,  1003,   163,    75,    -1,    77,
     841,    79,   843,    81,    82,   846,    84,    -1,    86,    -1,
      88,    -1,    -1,  1021,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1030,    -1,    -1,    -1,  1034,    -1,    -1,   107,
      -1,    -1,   110,   874,    -1,   113,    -1,    -1,    -1,   267,
      -1,    -1,    -1,    -1,   885,   273,   887,    -1,    -1,    -1,
      -1,    -1,   670,    -1,    -1,   673,    -1,    -1,   267,    -1,
      -1,    -1,    -1,    -1,   273,    -1,    -1,    -1,    -1,    -1,
     818,    -1,   820,    -1,    -1,  1083,    -1,   825,    -1,    -1,
     828,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,
      -1,   169,    -1,   841,    -1,   843,    -1,    -1,   846,    -1,
      -1,    -1,    -1,    -1,   332,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   730,    -1,    -1,    -1,    -1,    -1,   736,   347,
      -1,    -1,    -1,   332,    -1,    -1,   874,    -1,    -1,    -1,
      -1,    -1,  1140,  1141,  1142,  1143,    -1,   885,   347,   887,
     981,    -1,    -1,    -1,    -1,    -1,    -1,   988,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,  1167,
    1168,    -1,  1003,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1021,    25,    -1,    27,    -1,    -1,    -1,    -1,     2,  1030,
      -1,    -1,    -1,  1034,    -1,    -1,   424,    -1,    12,    -1,
     818,    -1,   820,    -1,    -1,    -1,    -1,   825,    -1,    -1,
     828,    25,    -1,    27,    -1,   424,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   841,   452,   843,    -1,    -1,   846,    -1,
     458,    -1,    76,   981,    -1,    -1,    -1,    -1,    -1,    -1,
     988,    -1,  1083,   452,   472,    -1,   474,    -1,    -1,   458,
      -1,    -1,    -1,    -1,    -1,  1003,   874,    -1,    -1,   103,
      -1,    -1,    76,   472,   108,   474,    -1,   885,    -1,   887,
      -1,    -1,    -1,  1021,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1030,    -1,    -1,    -1,  1034,    -1,    -1,   103,
      -1,    -1,    22,    -1,   108,    -1,    26,    27,    -1,  1140,
    1141,  1142,  1143,    -1,    -1,    35,   534,    37,    38,    -1,
     538,    -1,    -1,    43,    -1,    -1,    -1,    -1,   546,    -1,
      50,    -1,    -1,    -1,    -1,   534,  1167,  1168,    -1,   538,
      -1,    -1,    -1,    -1,   178,  1083,    -1,   546,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    12,    77,    -1,    79,
      -1,    81,    -1,    19,    84,    -1,    86,    -1,    88,    -1,
      -1,    -1,    -1,   981,   178,    31,    -1,    -1,    -1,    -1,
     988,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,
     110,    47,    -1,   113,    -1,  1003,    -1,    -1,    -1,    -1,
      -1,    -1,  1140,  1141,  1142,  1143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1021,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1030,    -1,    -1,    -1,  1034,    -1,    -1,  1167,
    1168,    87,    -1,   267,    -1,    91,    -1,    -1,    -1,   273,
      -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,   169,
      -1,    -1,   670,    -1,    -1,   673,    -1,    -1,    -1,    -1,
      31,    -1,   118,   267,    -1,    -1,   122,    -1,    -1,   273,
      -1,   670,    -1,    -1,   673,  1083,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,   332,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,   730,   347,    -1,    -1,    87,    -1,   736,   175,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   332,    -1,
      -1,   730,  1140,  1141,  1142,  1143,    -1,   736,    -1,    -1,
      -1,    -1,    -1,   347,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1167,
    1168,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
      -1,   142,   143,    -1,   145,   146,     2,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    12,   158,    -1,    -1,
     424,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,    25,
     818,    27,   820,    -1,    -1,    -1,    -1,   825,    -1,    -1,
     828,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   452,   818,
     424,   820,    -1,   841,   458,   843,   825,    -1,   846,   828,
      22,    -1,    -1,    -1,    26,    27,    -1,    -1,   472,    -1,
     474,    -1,   841,    35,   843,    37,    38,   846,   452,    -1,
      76,    43,    -1,    -1,   458,    -1,   874,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   885,   472,   887,
     474,    -1,    -1,    -1,    -1,   874,    -1,   103,    -1,    -1,
      -1,    -1,   108,    75,    -1,    77,   885,    79,   887,    81,
      -1,    -1,    84,    -1,    86,    -1,    88,    -1,    -1,    -1,
     534,    -1,    -1,    -1,   538,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   546,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     534,    -1,     3,    -1,   538,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   546,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,   178,    -1,    25,    43,    44,    45,    -1,    -1,
      -1,    -1,    33,   981,    -1,    -1,    -1,    -1,    39,    -1,
     988,   163,    -1,    -1,    45,    -1,    -1,   169,    49,    -1,
      -1,    52,   981,    -1,    -1,  1003,    -1,    -1,    -1,   988,
      -1,    62,    -1,    -1,    82,    -1,    -1,    68,    69,    -1,
      -1,    -1,    -1,  1021,  1003,    76,    -1,    -1,    -1,    -1,
      -1,    -1,  1030,    -1,    85,    -1,  1034,    -1,    -1,    -1,
      -1,    -1,  1021,    94,    95,    -1,    97,    -1,    99,    -1,
     101,  1030,    -1,   104,    -1,  1034,   670,   108,   109,   673,
     111,   267,    -1,   114,    -1,    31,    -1,   273,    -1,    -1,
      -1,    -1,   140,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1083,   670,    -1,    -1,   673,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1083,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   730,    -1,    -1,   170,
     188,    87,   736,    -1,    -1,    91,   332,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,    -1,
      -1,   347,  1140,  1141,  1142,  1143,   730,    -1,    -1,    -1,
      -1,    -1,   736,   221,    -1,   223,    -1,    -1,    -1,    -1,
      -1,  1140,  1141,  1142,  1143,    -1,   234,   235,    -1,  1167,
    1168,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,  1167,  1168,
      -1,   157,   158,    -1,    -1,   161,    -1,    -1,   164,   165,
     166,   269,    -1,    -1,   818,    -1,   820,    -1,    -1,    -1,
      -1,   825,    -1,    -1,   828,    -1,    -1,    -1,   424,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   841,    -1,   843,
      -1,    -1,   846,     3,   818,    -1,   820,    -1,    -1,    -1,
      -1,   825,    -1,    -1,   828,    -1,   452,    17,    18,    -1,
      20,    -1,   458,    -1,    -1,    25,    -1,   841,    -1,   843,
     874,    -1,   846,    33,    -1,    -1,   472,    -1,    -1,    39,
      -1,   885,    -1,   887,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,   352,    -1,    -1,    -1,    -1,    -1,
     874,    -1,    62,    -1,    -1,    -1,    -1,    -1,    68,    69,
      -1,   885,    -1,   887,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    -1,    97,   534,    99,
      -1,   101,   538,    -1,   104,    -1,    -1,    -1,   108,    -1,
     546,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   423,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   981,    -1,    -1,
      -1,    -1,    -1,   441,   988,    -1,    -1,   445,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1003,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   981,   168,    -1,
     170,    -1,    -1,    -1,   988,    -1,    -1,  1021,    -1,     3,
      -1,    -1,    -1,    -1,    -1,   483,  1030,    -1,    -1,  1003,
    1034,   489,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,  1021,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,  1030,    -1,    -1,    -1,
    1034,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,   670,    -1,    -1,   673,    62,  1083,
      -1,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,   547,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,   556,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1083,
      94,    95,    -1,    97,    -1,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,   730,    -1,  1140,  1141,  1142,  1143,
     736,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1167,  1168,    -1,  1140,  1141,  1142,  1143,
      -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,   637,
      -1,    -1,   640,    -1,    -1,   167,   170,     1,   170,     3,
     172,    -1,    -1,  1167,  1168,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,   818,    -1,   820,    39,    -1,    -1,    -1,   825,
      -1,    45,   828,   691,    -1,    49,    -1,   695,    52,    -1,
      54,    -1,    -1,    -1,    -1,   841,    -1,   843,    62,    -1,
     846,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,
     718,    -1,    76,    -1,   722,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,   874,    -1,
      94,    95,    -1,    97,    -1,    99,    -1,   101,    -1,   885,
     104,   887,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,   765,    -1,   123,
     124,   125,   126,   127,   128,    -1,    -1,   131,   132,   133,
     134,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
      -1,   145,    -1,    -1,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,    -1,   161,    -1,    -1,
     164,   165,   166,   167,    -1,    -1,   170,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     848,    -1,   850,    -1,    -1,    -1,   854,    -1,    -1,   857,
      -1,   167,    -1,    -1,   170,    -1,   172,  1003,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   879,    -1,    -1,    -1,  1021,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1030,    -1,    -1,    -1,  1034,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   904,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,  1083,    37,    38,
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
    1058,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,     1,   175,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    -1,    -1,    75,
      76,    77,    78,    -1,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,    -1,   101,   102,    -1,   104,   105,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,   168,   169,   170,    -1,   172,    -1,     1,   175,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
      -1,    -1,   166,    -1,   168,   169,   170,    -1,   172,    -1,
       1,   175,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    -1,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    90,
      -1,    92,    93,    94,    95,    96,    97,    98,    99,    -1,
     101,   102,    -1,   104,   105,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,   163,    -1,    -1,   166,    -1,   168,   169,   170,
      -1,   172,    -1,     1,   175,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      68,    69,    70,    -1,    72,    -1,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
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
     168,    -1,   170,    -1,   172,    -1,     1,   175,     3,     4,
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
      -1,   166,    -1,   168,    -1,   170,    -1,   172,   173,     1,
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
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,    -1,     3,   175,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    77,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
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
      98,    99,    -1,   101,    -1,    -1,   104,   105,   106,    -1,
     108,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
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
     105,   106,    -1,   108,   109,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,   171,   172,    -1,    -1,
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
      -1,    -1,   104,   105,   106,    -1,   108,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,   171,
     172,    -1,     3,   175,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    47,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   101,    -1,    -1,   104,   105,   106,    -1,   108,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,   175,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,    -1,   104,   105,   106,
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
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,   105,
     106,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,   175,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,   107,   108,    -1,    -1,   111,    -1,    -1,   114,
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
      -1,    -1,    76,    77,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,
       3,   175,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,    -1,   175,     3,     4,     5,     6,     7,     8,     9,
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
     120,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      99,    -1,   101,    -1,    -1,   104,   105,   106,    -1,   108,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,   169,   170,    -1,   172,    -1,    -1,   175,
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
      -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
     173,    -1,   175,     3,     4,     5,     6,     7,     8,     9,
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
     170,    -1,   172,   173,     3,   175,     5,     6,     7,     8,
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
      -1,   120,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,     3,   175,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,
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
     104,    -1,    -1,   107,   108,    -1,    -1,   111,    -1,    -1,
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
     173,     3,   175,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
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
     166,    -1,    -1,    -1,   170,    -1,   172,    -1,     3,   175,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,    -1,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,     3,     4,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,   144,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,
      -1,    -1,    -1,   168,    -1,   170,   171,   172,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,    -1,   103,   104,
      -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,   140,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,   162,   163,    -1,
      -1,    -1,    -1,   168,    33,   170,    35,   172,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
       3,   140,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,   162,    -1,    -1,    -1,    -1,    -1,   168,
      33,   170,    35,   172,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,   140,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,   162,
      -1,    -1,    -1,    -1,    -1,   168,    33,   170,    35,   172,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,     3,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,   140,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      68,    69,    -1,   170,    -1,   172,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    97,
      -1,    99,    -1,   101,    -1,     3,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,    12,    -1,   114,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      68,    69,   170,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    94,    95,    -1,    97,
      -1,    99,    19,   101,    -1,    -1,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,    31,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,    -1,    -1,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,    -1,   145,    -1,    -1,
      -1,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,    -1,   161,    -1,    -1,   164,   165,   166,   167,
      87,    12,    -1,    -1,    91,    -1,    -1,    -1,    19,    -1,
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
      -1,   164,   165,   166,    -1,   168,    87,    12,    -1,    -1,
      91,    -1,   175,    -1,    19,    -1,    -1,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,    -1,    -1,
     131,   132,   133,   134,    -1,   136,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    87,   164,   165,   166,    91,   168,    -1,    12,
      -1,    -1,    -1,    -1,   175,   100,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    -1,   168,    87,    12,    -1,    -1,    91,    -1,
     175,    -1,    19,    -1,    -1,    -1,    -1,   100,    -1,    -1,
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
      -1,   168,    87,    -1,    -1,    -1,    91,    -1,   175,    12,
      -1,    -1,    -1,    -1,    -1,   100,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,   128,    47,    -1,   131,   132,   133,   134,
      -1,   136,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    -1,   168,    87,    12,    -1,    -1,    91,    -1,
     175,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   122,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,    -1,
      87,   164,   165,   166,    91,   168,    -1,    12,    -1,    -1,
      -1,    -1,   175,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,   118,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      -1,   168,    87,    -1,    -1,    -1,    91,    -1,   175,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    71,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    -1,   168,    -1,    87,    -1,    -1,    -1,    91,
     175,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,   163,   164,   165,   166,    -1,    87,    12,    -1,    -1,
      91,    -1,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    87,   164,   165,   166,    91,   168,    -1,    12,
      -1,    -1,    -1,    -1,   175,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,   118,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    -1,   168,    87,    12,    -1,    -1,    91,    -1,
     175,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,    -1,
      87,   164,   165,   166,    91,   168,    12,    -1,    -1,    -1,
      -1,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      -1,    87,    12,    -1,    -1,    91,    -1,    -1,   175,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    87,   164,   165,
     166,    91,    -1,    -1,    12,    -1,    -1,    -1,    -1,   175,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    -1,   168,    87,
      12,    -1,    -1,    91,    -1,   175,    -1,    19,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    87,   164,   165,   166,    91,
     168,    -1,    -1,    12,    -1,    -1,    -1,   175,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,   118,    36,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    71,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    -1,    87,    12,
      -1,   173,    91,   175,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    31,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    87,   164,   165,   166,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    12,   175,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,    -1,
     122,   164,   165,   166,    -1,   168,    -1,    -1,    -1,    -1,
      87,    -1,   175,    -1,    91,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      12,   163,   164,   165,   166,   122,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    31,
      -1,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    12,   163,   164,   165,   166,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    91,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,   163,   164,   165,   166,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    87,
      -1,   138,   139,    91,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    12,   163,   164,   165,   166,
      -1,    -1,    19,   121,   122,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    12,    -1,   164,   165,   166,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,   138,   139,    91,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,   175,    87,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   122,    -1,   164,   165,   166,    -1,
      -1,    12,    -1,   171,    -1,    -1,    -1,   175,    19,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      31,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,   175,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,   118,   164,   165,   166,   122,    -1,    -1,    -1,
      -1,    87,    -1,    -1,   175,    91,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,   175,
      19,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    31,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,   164,   165,
     166,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   175,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    87,    -1,
      -1,    -1,    91,    -1,    -1,    -1,   175,    -1,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   175,    19,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    31,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,
      -1,    12,   171,    -1,    -1,    -1,   175,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     121,   122,   164,   165,   166,    -1,    87,    -1,    -1,   171,
      91,    -1,    -1,   175,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   122,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    12,    22,   164,   165,   166,    26,    27,    19,    -1,
      -1,    -1,    -1,    -1,   175,    35,    -1,    37,    38,    -1,
      31,    32,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    64,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    -1,
      31,    -1,    -1,    83,    -1,    -1,    86,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    12,    13,    -1,    -1,    -1,    -1,   107,    19,    -1,
     110,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   122,    -1,   164,   165,   166,    87,    -1,    -1,    -1,
      91,    -1,    -1,    -1,   175,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   122,    -1,   164,   165,   166,    -1,    -1,    12,    -1,
      -1,    -1,   173,    -1,   175,    19,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    31,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   175,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   100,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   122,    -1,
     164,   165,   166,    87,    -1,    -1,    -1,    91,    -1,    -1,
      -1,   175,    -1,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   122,    -1,
     164,   165,   166,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   175,    19,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    31,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   175,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   122,    -1,   164,   165,   166,
      -1,    -1,    12,    -1,   171,    -1,    -1,    -1,   175,    19,
      -1,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,    31,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      -1,    -1,    12,    -1,    -1,    -1,   173,    -1,   175,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    91,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     100,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    87,    -1,    -1,
      -1,    91,    -1,    -1,    -1,   175,    -1,    -1,   138,   139,
     100,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   175,    19,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    31,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   175,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   100,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,   175,    -1,    -1,   138,   139,   100,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   175,    19,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    31,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   175,    19,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,   175,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    -1,    -1,    12,    -1,    -1,    -1,   173,    -1,   175,
      19,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    31,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    22,   164,   165,
     166,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    31,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    84,
      -1,    86,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,   138,
     139,    -1,   141,   142,   143,    87,   145,   146,   147,    91,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    87,    -1,    -1,   163,    91,
      -1,    -1,    -1,    -1,   169,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,    31,    -1,   155,   156,   157,   158,   159,   160,   161,
     122,    -1,   164,   165,   166,    87,    -1,    -1,    -1,    91,
      -1,    -1,    -1,   175,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,    -1,    -1,   155,   156,   157,   158,   159,    -1,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    87,    -1,    -1,
      -1,    91,    -1,   175,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,    -1,    -1,   155,   156,   157,   158,   159,    -1,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,    -1,
     150,   151,   152,    -1,    -1,   155,   156,   157,   158,    -1,
       1,   161,     3,    -1,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,     3,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    62,    -1,    -1,    -1,    25,    -1,    68,    69,    -1,
      -1,    -1,    -1,    33,    -1,    76,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    85,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    94,    95,    -1,    97,    -1,    99,    -1,
     101,    -1,    62,   104,    -1,    -1,    -1,   108,    68,    69,
     111,    -1,    -1,   114,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    94,    95,    -1,    97,    -1,    99,
      -1,   101,    17,    18,   104,    20,    -1,    -1,   108,    -1,
      25,   111,    -1,    -1,   114,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,     3,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    62,    -1,    -1,
      -1,    25,    -1,    68,    69,    -1,    -1,    -1,    -1,    33,
      -1,    76,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      85,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    94,
      95,    -1,    97,    -1,    99,    -1,   101,    -1,    62,   104,
      -1,    -1,    -1,   108,    68,    69,   111,    -1,    -1,   114,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      94,    95,    -1,    97,    -1,    99,    -1,   101,    17,    18,
     104,    20,    -1,    -1,   108,    -1,    25,   111,    -1,    -1,
     114,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,     3,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    62,    -1,    -1,    -1,    25,    -1,    68,
      69,    -1,    -1,    -1,    -1,    33,    -1,    76,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    85,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    94,    95,    -1,    97,    -1,
      99,    -1,   101,    -1,    62,   104,    -1,    -1,    -1,   108,
      68,    69,   111,    -1,    -1,   114,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    97,
      -1,    99,    -1,   101,    17,    18,   104,    20,    -1,    -1,
     108,    -1,    25,   111,    -1,    -1,   114,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    97,    -1,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114
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
     245,   248,   249,   252,   255,   256,   281,   284,   285,   305,
     306,   307,   308,   309,   310,   311,   319,   320,   321,   322,
     323,   326,   327,   328,   329,   330,   331,   332,   333,   335,
     336,   337,   338,   339,   163,   184,   323,   118,   312,   313,
       3,   205,    14,    22,    35,    40,    41,    44,    55,    86,
      99,   168,   172,   231,   252,   305,   310,   321,   322,   323,
     326,   328,   329,   312,   323,   107,   284,    88,   205,   184,
     299,   323,   184,   170,     8,    86,   323,   324,     8,    11,
      86,   107,   324,    77,   120,   225,   323,   225,   323,   225,
     323,    26,   285,   323,    27,   113,   227,   323,   191,   170,
       3,    17,    18,    20,    25,    33,    39,    45,    49,    52,
      62,    68,    69,    76,    85,    94,    95,    97,    99,   101,
     104,   108,   111,   114,   207,   209,   207,   207,   253,   170,
     207,   286,   287,    32,   192,   212,   323,   323,    18,    76,
      94,   111,   323,   323,   323,     8,   170,   215,    22,    35,
      38,    86,   216,     4,   275,   298,   323,   105,   106,   163,
     323,   325,   323,   212,   323,   323,   323,    98,   170,   184,
     323,   323,   192,   204,   323,   326,   192,   204,   323,   207,
     282,   323,   323,   323,   323,   323,   323,   323,   323,     1,
     169,   182,   193,   298,   109,   148,   275,   300,   301,   325,
     225,   298,   323,   334,   323,    79,   184,   168,    83,   188,
      46,   112,    55,   207,   207,    53,    72,    82,   270,   286,
     162,   163,   154,   323,    12,    19,    31,    87,    91,   122,
     138,   139,   141,   142,   143,   145,   146,   147,   149,   150,
     151,   152,   153,   155,   156,   157,   158,   159,   160,   161,
     164,   165,   166,   175,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   167,
     262,   170,   172,    87,    91,   170,   184,   163,   323,   323,
     323,   207,   298,    55,   168,   192,    47,   312,   282,   286,
     163,   144,   163,   117,   208,   275,   302,   303,   304,   325,
      86,   221,   256,   284,    86,   219,   282,   256,   284,   207,
     170,   212,    32,    47,   212,   118,   212,   315,    32,    47,
     212,   315,   212,   315,    47,   212,   315,    36,    71,   163,
     207,   207,   100,   192,   100,   123,   192,   262,    81,    84,
     189,   302,   170,   170,   192,   184,   170,   264,   109,   170,
     207,   288,   289,     1,   143,   293,    47,   144,   184,   212,
     170,   170,   212,   302,   216,   144,   163,   323,   323,   163,
     168,   212,   170,   302,   163,   237,   163,   237,   163,   123,
     283,   163,   212,   163,   169,   169,   182,   144,   169,   323,
     144,   171,   144,   171,   173,   315,    47,   144,   173,   315,
     121,   144,   173,     8,     1,   169,   193,    64,   201,   202,
     323,   195,   323,   207,   143,   246,   168,   257,   163,   323,
     323,   323,   323,   225,   323,   225,   323,   323,   323,   323,
     323,   323,   323,     3,    20,    33,    62,   101,   107,   208,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,    67,   325,   325,
     325,   325,   325,   302,   302,   225,   323,   225,   323,    27,
      47,    88,   113,   314,   317,   318,   323,   339,    32,    47,
      32,    47,   100,   170,    47,   173,   207,   225,   323,   212,
     163,   163,   323,   323,   123,   171,   144,   222,   207,   286,
     220,   207,   163,   286,    47,   298,    44,   323,   225,   323,
     170,   212,    44,   323,   225,   323,   212,   212,   225,   323,
     212,   166,   194,   197,   323,   194,   196,   123,   123,   184,
      34,   184,   323,    34,   323,   188,   171,   303,   207,   229,
     230,    27,    47,    51,    74,    77,    88,   107,   183,   265,
     266,   267,   268,   254,   289,   144,   171,    33,    49,    95,
      99,   172,   211,   294,   306,   123,   290,   323,   287,   323,
     323,   171,   275,   323,     1,   242,   302,   171,    21,   238,
     294,   306,   144,   169,   171,   171,   300,   171,   300,   184,
     173,   225,   323,   173,   184,   323,   173,   323,   173,   323,
     169,   169,   207,   144,   163,    13,   146,   144,   163,    13,
      36,    71,   163,   170,   298,   168,     1,   207,   250,   251,
      27,    77,    88,   107,   259,   269,   170,   163,   163,   163,
     163,   163,   163,   171,   173,    47,    88,   144,   171,    17,
      20,    25,    45,    52,    62,    69,    85,    97,   108,   114,
     305,    87,    87,    44,   225,   323,    44,   225,   323,   303,
     225,   323,   170,   312,   312,   163,   275,   325,   304,   207,
     246,   163,   207,   246,   163,   323,   171,   323,    32,   212,
      32,   212,   316,   317,   323,    32,   212,   315,    32,   212,
     315,   212,   315,   212,   315,   144,   163,    13,   163,   323,
     323,    34,   184,    34,    34,   184,   100,   192,    64,   171,
     144,   171,    47,    88,   267,   144,   171,   170,   207,    27,
      77,    88,   107,   271,   171,   289,   293,     1,   298,    67,
     325,   171,   171,   169,    73,   115,   169,   243,   171,   170,
     192,   207,   239,   282,   184,   173,   315,   173,   315,   184,
     121,   201,   208,   168,   323,   109,   323,   197,   196,   303,
       1,   247,   169,   123,   144,   169,    88,   258,     1,     3,
      12,    17,    19,    20,    25,    39,    45,    52,    54,    62,
      68,    69,    85,    97,   101,   104,   108,   114,   138,   139,
     140,   141,   142,   143,   145,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   161,   164,   165,   166,
     167,   170,   206,   207,   209,   260,   261,   262,   305,   171,
     317,   293,   305,   305,   323,    32,    32,   323,    32,    32,
     171,   173,   173,   303,   212,   212,   246,   168,   246,   168,
     212,   100,    44,   323,    44,   323,   144,   171,   100,    44,
     323,   212,    44,   323,   212,   212,   212,   323,   323,   184,
     184,   323,   184,    34,   207,   163,   230,   192,   207,   266,
     289,   143,   297,    88,   293,   290,   173,    47,   173,   170,
     170,    32,   184,   298,   239,   143,   192,    44,   184,   323,
     173,    44,   184,   323,   173,   323,   194,    13,    36,    71,
      36,    71,   163,   163,   171,   169,    81,    84,   169,   183,
     214,   323,   251,   271,   170,   263,   323,   138,   146,   263,
     263,   290,   100,    44,    44,   100,    44,    44,    44,    44,
     171,   168,   247,   168,   247,   323,   323,   323,   317,   323,
     323,   323,    13,    34,   184,   163,   297,   171,   172,   211,
     275,   296,   306,   148,   276,   290,    60,   116,   277,   323,
     294,   306,   302,   302,   184,   212,   171,   323,   323,   184,
     323,   184,   169,   109,   323,   197,   196,   197,   196,   163,
     214,     1,   143,   292,   265,   171,     3,   101,   261,   262,
     323,   323,   323,   323,   323,   323,   247,   169,   247,   169,
     100,   100,   100,   100,   323,   184,   276,   290,   297,   173,
     298,   275,   323,     3,    90,   101,   278,   279,   280,   323,
     192,   213,   274,   173,   171,   171,   192,   100,   100,   163,
     163,   163,   163,   172,   211,   291,   306,   103,   272,   171,
     263,   263,   100,   100,   100,   100,   100,   100,   169,   169,
     323,   323,   323,   323,   276,   290,   275,   295,   296,   306,
      47,   173,   280,   116,   144,   123,   149,   151,   152,   155,
     156,    60,   306,   162,   162,   323,   323,     1,   173,   298,
     277,   323,   295,   296,   323,   279,   280,   280,   280,   280,
     280,   280,   278,   173,   291,   306,   173,   163,   273,   274,
     173,   291,   306,   295
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
#line 511 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 516 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 524 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 529 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 530 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 553 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 554 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 555 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 556 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 557 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 558 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 559 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 560 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 562 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 563 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 564 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 565 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 566 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 567 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 568 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 573 "chapel.ypp"
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
#line 586 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 591 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 596 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 603 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 605 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 609 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 615 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 621 "chapel.ypp"
    {
    (yylsp[(1) - (1)]).comment = context->latestComment;
    context->latestComment = NULL;
  }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 626 "chapel.ypp"
    {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[(6) - (7)].pch), (yyvsp[(3) - (7)].b), (yyvsp[(4) - (7)].b), (yylsp[(1) - (7)]).comment);
 }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 643 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 649 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 654 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 664 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 668 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 677 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 682 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 692 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 708 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 717 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 726 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 728 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].ponlylist), (yyvsp[(1) - (4)].b)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 733 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), true, (yyvsp[(1) - (8)].b)); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), new UnresolvedSymExpr("_"), (yyvsp[(7) - (8)].ponlylist), true, (yyvsp[(1) - (8)].b)); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), false, (yyvsp[(1) - (8)].b)); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), new UnresolvedSymExpr("_"), (yyvsp[(7) - (8)].ponlylist), false, (yyvsp[(1) - (8)].b)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(3) - (4)].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[(1) - (4)].b));}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 748 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 749 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist)); }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pimportstmt)); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (3)].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[(3) - (3)].pimportstmt)); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 765 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), new SymExpr(gNoInit), "="); }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 779 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 780 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 784 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 785 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 786 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 789 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 792 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 793 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 795 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 796 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 797 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 809 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 810 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 821 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 822 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 823 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 824 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 825 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 826 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 827 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 828 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 829 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 830 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 831 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 832 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 833 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 834 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 835 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 836 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 837 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 838 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 839 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 843 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 844 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 845 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 846 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 847 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 848 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 849 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 850 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 851 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 852 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 853 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 854 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 861 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 862 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 863 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 864 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 869 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 873 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 880 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 891 "chapel.ypp"
    { parsingPrivate=(yyvsp[(2) - (3)].b); applyPrivateToBlock((yyvsp[(3) - (3)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(3) - (3)].pblockstmt); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 905 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 907 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 908 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 913 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 918 "chapel.ypp"
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
#line 928 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 933 "chapel.ypp"
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
#line 944 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 949 "chapel.ypp"
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
#line 959 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 964 "chapel.ypp"
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
#line 975 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 979 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 984 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 992 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 1003 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 1010 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 1011 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1012 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), true, false); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), true, false); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1036 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1042 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1048 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1054 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1060 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1067 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1074 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1078 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1089 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1090 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1091 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1098 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1100 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1102 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1104 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pexpr) = buildIfVar((yyvsp[(2) - (4)].pch), (yyvsp[(4) - (4)].pexpr), false); }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pexpr) = buildIfVar((yyvsp[(2) - (4)].pch), (yyvsp[(4) - (4)].pexpr), true);  }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt))); }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1117 "chapel.ypp"
    { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (3)].pch), ls, (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1125 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[(1) - (1)].pch), INTENT_TYPE); }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1135 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1136 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1141 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1142 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1143 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1148 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[(1) - (1)].pch)); }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyval.pch) = "none"; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1155 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1156 "chapel.ypp"
    { (yyval.pch) = "false"; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1157 "chapel.ypp"
    { (yyval.pch) = "true"; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1172 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), NULL)); }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1174 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (4)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (4)].pch), act, NULL)); }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1177 "chapel.ypp"
    { (yyvsp[(5) - (7)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (7)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (7)].pch), (yyvsp[(5) - (7)].pcallexpr), NULL)); }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1183 "chapel.ypp"
    { (yyval.pexpr) = IfcConstraint::build((yyvsp[(2) - (5)].pch), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1185 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (3)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (3)].pch), act); }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1188 "chapel.ypp"
    { (yyvsp[(5) - (6)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (6)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pcallexpr)); }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1193 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1196 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1199 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1201 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1205 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1221 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1227 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1231 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1232 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1237 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1241 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1248 "chapel.ypp"
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

  case 285:
/* Line 1792 of yacc.c  */
#line 1258 "chapel.ypp"
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

  case 286:
/* Line 1792 of yacc.c  */
#line 1271 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1276 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1304 "chapel.ypp"
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

  case 295:
/* Line 1792 of yacc.c  */
#line 1320 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1327 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1336 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1344 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1348 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1354 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1370 "chapel.ypp"
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

  case 305:
/* Line 1792 of yacc.c  */
#line 1392 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1395 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1399 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1407 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1417 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1424 "chapel.ypp"
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

  case 311:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1461 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1470 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1475 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1492 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pch) = ":"; }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1548 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1550 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pexpr))); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1558 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1568 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1570 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1572 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1582 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1588 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_PROC; }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_ITER; }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_OP; }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1613 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1617 "chapel.ypp"
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1621 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1622 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1635 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1637 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1639 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1644 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1646 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1650 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1652 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1653 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1654 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1661 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1668 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1670 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1672 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1677 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1690 "chapel.ypp"
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

  case 427:
/* Line 1792 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1729 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1738 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1748 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1753 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1755 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1757 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1772 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1773 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1798 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1805 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1806 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1807 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1809 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1815 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1816 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1818 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1839 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1843 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1847 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1855 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1861 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1862 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1863 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1868 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1870 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1876 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1878 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1880 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1884 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1885 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1886 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1887 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1888 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1894 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1895 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1896 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1897 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1901 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1902 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1906 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1907 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1908 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1912 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1913 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1917 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1922 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1923 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1927 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1928 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1929 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1930 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1935 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1947 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1949 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1951 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1962 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1964 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1966 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1968 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1970 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1972 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1974 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1977 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1979 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1981 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1989 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1994 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1996 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 2001 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 2003 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 2005 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 2007 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 2009 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 2011 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 2013 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 2015 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 2017 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 2019 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2021 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2023 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 2025 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2032 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2038 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 2044 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 2050 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2059 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2084 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2088 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2093 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2097 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2098 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2102 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2106 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2107 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2112 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2116 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2120 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2126 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2127 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2128 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2129 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2130 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2131 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2137 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2142 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2144 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2148 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2152 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2156 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2164 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2169 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2175 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2181 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2188 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2198 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2215 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2217 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2219 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2221 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), false, true); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2236 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2238 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2240 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr), true); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2242 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2246 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2247 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2251 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 607:
/* Line 1792 of yacc.c  */
#line 2252 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 608:
/* Line 1792 of yacc.c  */
#line 2253 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 613:
/* Line 1792 of yacc.c  */
#line 2269 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 614:
/* Line 1792 of yacc.c  */
#line 2270 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 615:
/* Line 1792 of yacc.c  */
#line 2271 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 616:
/* Line 1792 of yacc.c  */
#line 2272 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 618:
/* Line 1792 of yacc.c  */
#line 2277 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 619:
/* Line 1792 of yacc.c  */
#line 2278 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 620:
/* Line 1792 of yacc.c  */
#line 2279 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 621:
/* Line 1792 of yacc.c  */
#line 2283 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 622:
/* Line 1792 of yacc.c  */
#line 2284 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 623:
/* Line 1792 of yacc.c  */
#line 2285 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 624:
/* Line 1792 of yacc.c  */
#line 2286 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 625:
/* Line 1792 of yacc.c  */
#line 2287 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 626:
/* Line 1792 of yacc.c  */
#line 2295 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 627:
/* Line 1792 of yacc.c  */
#line 2296 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 628:
/* Line 1792 of yacc.c  */
#line 2297 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 629:
/* Line 1792 of yacc.c  */
#line 2298 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 630:
/* Line 1792 of yacc.c  */
#line 2302 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 631:
/* Line 1792 of yacc.c  */
#line 2303 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 632:
/* Line 1792 of yacc.c  */
#line 2307 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 633:
/* Line 1792 of yacc.c  */
#line 2308 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 636:
/* Line 1792 of yacc.c  */
#line 2314 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 637:
/* Line 1792 of yacc.c  */
#line 2315 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 638:
/* Line 1792 of yacc.c  */
#line 2316 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 639:
/* Line 1792 of yacc.c  */
#line 2317 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 640:
/* Line 1792 of yacc.c  */
#line 2318 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 641:
/* Line 1792 of yacc.c  */
#line 2319 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr),
                                            new SymExpr(gTrue)); }
    break;

  case 642:
/* Line 1792 of yacc.c  */
#line 2321 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr),
                                                   new SymExpr(gTrue)); }
    break;

  case 643:
/* Line 1792 of yacc.c  */
#line 2323 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 644:
/* Line 1792 of yacc.c  */
#line 2324 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 645:
/* Line 1792 of yacc.c  */
#line 2326 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 646:
/* Line 1792 of yacc.c  */
#line 2330 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 647:
/* Line 1792 of yacc.c  */
#line 2337 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 648:
/* Line 1792 of yacc.c  */
#line 2338 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 649:
/* Line 1792 of yacc.c  */
#line 2342 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 650:
/* Line 1792 of yacc.c  */
#line 2343 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 651:
/* Line 1792 of yacc.c  */
#line 2344 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 652:
/* Line 1792 of yacc.c  */
#line 2345 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 653:
/* Line 1792 of yacc.c  */
#line 2346 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 654:
/* Line 1792 of yacc.c  */
#line 2347 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 655:
/* Line 1792 of yacc.c  */
#line 2348 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 656:
/* Line 1792 of yacc.c  */
#line 2349 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 657:
/* Line 1792 of yacc.c  */
#line 2350 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 658:
/* Line 1792 of yacc.c  */
#line 2351 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 659:
/* Line 1792 of yacc.c  */
#line 2352 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 660:
/* Line 1792 of yacc.c  */
#line 2353 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 661:
/* Line 1792 of yacc.c  */
#line 2354 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 662:
/* Line 1792 of yacc.c  */
#line 2355 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 663:
/* Line 1792 of yacc.c  */
#line 2356 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 664:
/* Line 1792 of yacc.c  */
#line 2357 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 665:
/* Line 1792 of yacc.c  */
#line 2358 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 666:
/* Line 1792 of yacc.c  */
#line 2359 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 667:
/* Line 1792 of yacc.c  */
#line 2360 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 668:
/* Line 1792 of yacc.c  */
#line 2361 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 669:
/* Line 1792 of yacc.c  */
#line 2362 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 670:
/* Line 1792 of yacc.c  */
#line 2363 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 671:
/* Line 1792 of yacc.c  */
#line 2364 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr),
                                               new SymExpr(gTrue)); }
    break;

  case 672:
/* Line 1792 of yacc.c  */
#line 2369 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 673:
/* Line 1792 of yacc.c  */
#line 2370 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 674:
/* Line 1792 of yacc.c  */
#line 2371 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 675:
/* Line 1792 of yacc.c  */
#line 2372 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 676:
/* Line 1792 of yacc.c  */
#line 2373 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 677:
/* Line 1792 of yacc.c  */
#line 2374 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 678:
/* Line 1792 of yacc.c  */
#line 2375 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 679:
/* Line 1792 of yacc.c  */
#line 2379 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 680:
/* Line 1792 of yacc.c  */
#line 2380 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 681:
/* Line 1792 of yacc.c  */
#line 2381 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 682:
/* Line 1792 of yacc.c  */
#line 2382 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 683:
/* Line 1792 of yacc.c  */
#line 2386 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 684:
/* Line 1792 of yacc.c  */
#line 2387 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 685:
/* Line 1792 of yacc.c  */
#line 2388 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 686:
/* Line 1792 of yacc.c  */
#line 2389 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 687:
/* Line 1792 of yacc.c  */
#line 2394 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 688:
/* Line 1792 of yacc.c  */
#line 2395 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 689:
/* Line 1792 of yacc.c  */
#line 2396 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 690:
/* Line 1792 of yacc.c  */
#line 2397 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 691:
/* Line 1792 of yacc.c  */
#line 2398 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 692:
/* Line 1792 of yacc.c  */
#line 2399 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 693:
/* Line 1792 of yacc.c  */
#line 2400 "chapel.ypp"
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


