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
#define YYLAST   20323

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  179
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  161
/* YYNRULES -- Number of rules.  */
#define YYNRULES  694
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1246

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
    1225,  1229,  1233,  1234,  1236,  1240,  1245,  1251,  1256,  1262,
    1269,  1276,  1277,  1279,  1281,  1283,  1285,  1287,  1290,  1293,
    1295,  1297,  1299,  1300,  1302,  1304,  1307,  1309,  1311,  1313,
    1315,  1317,  1318,  1320,  1323,  1325,  1327,  1329,  1330,  1332,
    1334,  1336,  1338,  1340,  1342,  1344,  1347,  1350,  1351,  1354,
    1357,  1362,  1367,  1369,  1373,  1377,  1381,  1385,  1389,  1393,
    1397,  1400,  1402,  1404,  1408,  1413,  1418,  1421,  1426,  1427,
    1430,  1433,  1435,  1437,  1439,  1442,  1444,  1449,  1453,  1455,
    1459,  1463,  1469,  1471,  1473,  1477,  1479,  1482,  1486,  1487,
    1490,  1493,  1497,  1500,  1505,  1509,  1513,  1518,  1522,  1523,
    1526,  1529,  1532,  1534,  1535,  1538,  1541,  1544,  1546,  1551,
    1556,  1563,  1567,  1568,  1570,  1572,  1576,  1581,  1585,  1590,
    1597,  1598,  1601,  1604,  1607,  1610,  1612,  1614,  1618,  1622,
    1624,  1628,  1630,  1632,  1634,  1638,  1642,  1643,  1645,  1647,
    1651,  1655,  1659,  1661,  1663,  1665,  1667,  1669,  1672,  1674,
    1676,  1678,  1680,  1682,  1684,  1687,  1692,  1697,  1702,  1708,
    1711,  1714,  1716,  1719,  1721,  1724,  1726,  1729,  1731,  1734,
    1736,  1738,  1745,  1752,  1757,  1767,  1777,  1785,  1792,  1799,
    1804,  1814,  1824,  1832,  1837,  1844,  1851,  1861,  1871,  1878,
    1880,  1882,  1884,  1886,  1888,  1890,  1892,  1894,  1898,  1899,
    1901,  1906,  1908,  1912,  1917,  1919,  1923,  1928,  1932,  1936,
    1938,  1940,  1943,  1945,  1948,  1950,  1952,  1956,  1958,  1961,
    1964,  1967,  1970,  1973,  1982,  1991,  2001,  2011,  2016,  2018,
    2020,  2022,  2024,  2026,  2028,  2030,  2032,  2034,  2039,  2043,
    2047,  2051,  2054,  2057,  2060,  2062,  2063,  2065,  2068,  2071,
    2073,  2075,  2077,  2079,  2081,  2083,  2086,  2089,  2091,  2093,
    2098,  2103,  2108,  2112,  2116,  2120,  2124,  2130,  2134,  2139,
    2143,  2148,  2150,  2152,  2154,  2156,  2158,  2160,  2162,  2164,
    2166,  2168,  2170,  2174,  2179,  2183,  2188,  2192,  2197,  2201,
    2207,  2211,  2215,  2219,  2223,  2227,  2231,  2235,  2239,  2243,
    2247,  2251,  2255,  2259,  2263,  2267,  2271,  2275,  2279,  2283,
    2287,  2291,  2295,  2299,  2302,  2305,  2308,  2311,  2314,  2317,
    2320,  2324,  2328,  2332,  2336,  2340,  2344,  2348,  2352,  2354,
    2356,  2358,  2360,  2362,  2364
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
      -1,   117,   227,   212,    -1,    24,   323,    47,   323,   312,
     212,    -1,    24,   323,    47,   225,   312,   212,    -1,    24,
     323,   312,   212,    -1,    40,   323,    47,   323,   212,    -1,
      40,   323,    47,   225,   212,    -1,    40,   323,   212,    -1,
      40,   225,   212,    -1,    40,    77,   207,    47,   323,   212,
      -1,    41,   323,    47,   323,   212,    -1,    41,   323,    47,
     323,   315,   212,    -1,    41,   323,    47,   225,   212,    -1,
      41,   323,    47,   225,   315,   212,    -1,    41,   323,   212,
      -1,    41,   323,   315,   212,    -1,    41,   225,   212,    -1,
      41,   225,   315,   212,    -1,    42,   323,    47,   323,   212,
      -1,    42,   323,    47,   323,   315,   212,    -1,    42,   323,
      47,   225,   212,    -1,    42,   323,    47,   225,   315,   212,
      -1,    42,   323,   212,    -1,    42,   323,   315,   212,    -1,
      42,   225,   212,    -1,    42,   225,   315,   212,    -1,   172,
     298,    47,   323,   173,   184,    -1,   172,   298,    47,   323,
     315,   173,   184,    -1,   172,   298,    47,   225,   173,   184,
      -1,   172,   298,    47,   225,   315,   173,   184,    -1,   172,
     298,   173,   184,    -1,   172,   298,   315,   173,   184,    -1,
     172,   225,   173,   184,    -1,   172,   225,   315,   173,   184,
      -1,   120,   170,   298,   171,    -1,    44,   323,   100,   184,
      -1,    44,   323,   192,    -1,    44,   323,   100,   184,    34,
     184,    -1,    44,   323,   192,    34,   184,    -1,    44,   227,
     100,   184,    -1,    44,   227,   192,    -1,    44,   227,   100,
     184,    34,   184,    -1,    44,   227,   192,    34,   184,    -1,
      44,   323,   262,   323,   100,   184,    -1,    44,   323,   262,
     323,   192,    -1,    44,   323,   262,   323,   100,   184,    34,
     184,    -1,    44,   323,   262,   323,   192,    34,   184,    -1,
     113,   207,   123,   323,    -1,    27,   207,   123,   323,    -1,
      56,   207,   170,   229,   171,   192,    -1,    56,   207,   192,
      -1,   230,    -1,   229,   144,   230,    -1,   207,    -1,     3,
      -1,    17,    -1,    52,    -1,   108,    -1,    85,    -1,    45,
      -1,    25,    -1,    20,    -1,    97,    -1,    62,    -1,    69,
      -1,   114,    -1,   232,    -1,    68,    -1,   101,    -1,    39,
      -1,   104,    -1,    33,    -1,    49,    -1,    55,   207,   170,
     303,   171,   163,    -1,   231,    55,   207,   163,    -1,   231,
      55,   207,   170,   303,   171,   163,    -1,    55,   207,   170,
     303,   171,    -1,   231,    55,   207,    -1,   231,    55,   207,
     170,   303,   171,    -1,    29,   184,    -1,   105,   323,   163,
      -1,   106,   323,   163,    -1,   105,   204,    -1,   106,   204,
      -1,   105,   192,   237,    -1,   106,   192,   237,    -1,    -1,
     237,   238,    -1,    21,   192,    -1,    21,   239,   192,    -1,
      21,   170,   239,   171,   192,    -1,   207,    -1,   207,   143,
     323,    -1,   102,   323,   163,    -1,    92,   323,   168,   242,
     169,    -1,    92,   323,   168,     1,   169,    -1,    -1,   242,
     243,    -1,   115,   298,   212,    -1,    73,   184,    -1,    73,
      32,   184,    -1,   245,   207,   246,   168,   247,   169,    -1,
     245,   207,   246,   168,     1,   169,    -1,    22,    -1,    86,
      -1,   110,    -1,    -1,   143,   298,    -1,    -1,   247,   214,
      -1,   247,   183,   214,    -1,   249,   207,   168,   250,   169,
      -1,   249,   207,   168,     1,   169,    -1,    35,    -1,   251,
      -1,   250,   144,    -1,   250,   144,   251,    -1,   207,    -1,
     207,   123,   323,    -1,    -1,    -1,    58,   253,   264,   254,
     271,   293,   277,   274,    -1,    -1,    50,    -1,    75,    -1,
      -1,    -1,   255,   270,   257,   259,   258,   271,   292,   272,
     277,   273,    -1,   269,   261,   263,    -1,   269,   262,   263,
      -1,   269,   260,   146,   261,   263,    -1,   269,   260,   146,
     262,   263,    -1,   269,     1,   263,    -1,   305,    -1,   170,
     323,   171,    -1,   206,    -1,   139,    -1,   141,    -1,   142,
      -1,   140,    -1,   149,    -1,   159,    -1,   156,    -1,   152,
      -1,   155,    -1,   151,    -1,   161,    -1,   157,    -1,   166,
      -1,   145,    -1,   164,    -1,   165,    -1,   158,    -1,   150,
      -1,   138,    -1,    19,    -1,   153,    -1,    12,    -1,   167,
      -1,   154,    -1,    54,    -1,   143,    -1,   207,   138,    -1,
     123,    -1,   134,    -1,   131,    -1,   133,    -1,   127,    -1,
     132,    -1,   128,    -1,   124,    -1,   125,    -1,   126,    -1,
     137,    -1,   136,    -1,    -1,   170,   265,   171,    -1,   170,
     265,   171,    -1,    -1,   266,    -1,   265,   144,   266,    -1,
     267,   207,   297,   290,    -1,   183,   267,   207,   297,   290,
      -1,   267,   207,   297,   276,    -1,   183,   267,   207,   297,
     276,    -1,   267,   170,   289,   171,   297,   290,    -1,   267,
     170,   289,   171,   297,   276,    -1,    -1,   268,    -1,    47,
      -1,    51,    -1,    74,    -1,    27,    -1,    27,    47,    -1,
      27,    88,    -1,    77,    -1,    88,    -1,   107,    -1,    -1,
      77,    -1,    88,    -1,    27,    88,    -1,    27,    -1,   107,
      -1,    82,    -1,    53,    -1,    72,    -1,    -1,    27,    -1,
      27,    88,    -1,    88,    -1,    77,    -1,   107,    -1,    -1,
     103,    -1,   163,    -1,   274,    -1,   192,    -1,   213,    -1,
       4,    -1,   148,    -1,   148,   323,    -1,   148,   275,    -1,
      -1,   116,   323,    -1,    60,   278,    -1,   116,   323,    60,
     278,    -1,    60,   278,   116,   323,    -1,   279,    -1,   278,
     144,   279,    -1,   280,   123,   280,    -1,   280,   155,   280,
      -1,   280,   156,   280,    -1,   280,   149,   280,    -1,   280,
     151,   280,    -1,   280,   152,   280,    -1,    90,   280,    -1,
       3,    -1,   101,    -1,   107,   282,   163,    -1,    26,   107,
     282,   163,    -1,    38,   107,   282,   163,    -1,   207,   283,
      -1,   207,   283,   144,   282,    -1,    -1,   123,   306,    -1,
     123,   294,    -1,    77,    -1,    27,    -1,    88,    -1,    27,
      88,    -1,   113,    -1,    26,   284,   286,   163,    -1,   284,
     286,   163,    -1,   287,    -1,   286,   144,   287,    -1,   207,
     293,   290,    -1,   170,   289,   171,   293,   290,    -1,   109,
      -1,   207,    -1,   170,   289,   171,    -1,   288,    -1,   288,
     144,    -1,   288,   144,   289,    -1,    -1,   123,    67,    -1,
     123,   325,    -1,   172,   173,   306,    -1,   172,   173,    -1,
     172,   298,   173,   306,    -1,   172,   298,   173,    -1,   172,
     173,   291,    -1,   172,   298,   173,   291,    -1,   172,     1,
     173,    -1,    -1,   143,   306,    -1,   143,   291,    -1,   143,
     211,    -1,     1,    -1,    -1,   143,   306,    -1,   143,   294,
      -1,   143,   211,    -1,     1,    -1,   172,   298,   173,   306,
      -1,   172,   298,   173,   294,    -1,   172,   298,    47,   323,
     173,   306,    -1,   172,     1,   173,    -1,    -1,   306,    -1,
     275,    -1,   172,   173,   295,    -1,   172,   298,   173,   295,
      -1,   172,   173,   296,    -1,   172,   298,   173,   296,    -1,
     172,   298,    47,   323,   173,   295,    -1,    -1,   143,   306,
      -1,   143,   275,    -1,   143,   211,    -1,   143,   296,    -1,
     323,    -1,   275,    -1,   298,   144,   323,    -1,   298,   144,
     275,    -1,   323,    -1,   299,   144,   323,    -1,   109,    -1,
     325,    -1,   275,    -1,   300,   144,   300,    -1,   301,   144,
     300,    -1,    -1,   303,    -1,   304,    -1,   303,   144,   304,
      -1,   208,   123,   275,    -1,   208,   123,   325,    -1,   275,
      -1,   325,    -1,   208,    -1,   210,    -1,   307,    -1,   307,
     162,    -1,   162,    -1,   310,    -1,   326,    -1,   309,    -1,
     336,    -1,   335,    -1,    95,   323,    -1,    49,   170,   302,
     171,    -1,    33,   170,   302,   171,    -1,    98,   170,   302,
     171,    -1,    96,    98,   170,   302,   171,    -1,    14,   323,
      -1,    99,   323,    -1,    76,    -1,    76,   323,    -1,   111,
      -1,   111,   323,    -1,    94,    -1,    94,   323,    -1,    18,
      -1,    18,   323,    -1,    22,    -1,    86,    -1,    40,   323,
      47,   323,    32,   323,    -1,    40,   323,    47,   225,    32,
     323,    -1,    40,   323,    32,   323,    -1,    40,   323,    47,
     323,    32,    44,   323,   100,   323,    -1,    40,   323,    47,
     225,    32,    44,   323,   100,   323,    -1,    40,   323,    32,
      44,   323,   100,   323,    -1,    41,   323,    47,   323,    32,
     323,    -1,    41,   323,    47,   225,    32,   323,    -1,    41,
     323,    32,   323,    -1,    41,   323,    47,   323,    32,    44,
     323,   100,   323,    -1,    41,   323,    47,   225,    32,    44,
     323,   100,   323,    -1,    41,   323,    32,    44,   323,   100,
     323,    -1,   172,   298,   173,   323,    -1,   172,   298,    47,
     323,   173,   323,    -1,   172,   298,    47,   225,   173,   323,
      -1,   172,   298,    47,   323,   173,    44,   323,   100,   323,
      -1,   172,   298,    47,   225,   173,    44,   323,   100,   323,
      -1,    44,   323,   100,   323,    34,   323,    -1,    66,    -1,
     310,    -1,   305,    -1,   329,    -1,   328,    -1,   252,    -1,
     321,    -1,   322,    -1,   319,   154,   323,    -1,    -1,   313,
      -1,   118,   170,   314,   171,    -1,   317,    -1,   314,   144,
     317,    -1,   118,   170,   316,   171,    -1,   317,    -1,   316,
     144,   317,    -1,   318,   305,   293,   290,    -1,   339,    87,
     305,    -1,   323,    87,   305,    -1,    27,    -1,    47,    -1,
      27,    47,    -1,    88,    -1,    27,    88,    -1,   113,    -1,
     326,    -1,   319,   154,   323,    -1,    65,    -1,    65,    76,
      -1,    65,    94,    -1,    65,   111,    -1,    65,    18,    -1,
     320,   323,    -1,    65,    76,   170,   323,   171,   170,   302,
     171,    -1,    65,    94,   170,   323,   171,   170,   302,   171,
      -1,    65,    76,   170,   323,   171,   170,   302,   171,   162,
      -1,    65,    94,   170,   323,   171,   170,   302,   171,   162,
      -1,    59,   286,    47,   323,    -1,   333,    -1,   306,    -1,
     308,    -1,   337,    -1,   338,    -1,   252,    -1,   321,    -1,
     322,    -1,   234,    -1,   170,   148,   323,   171,    -1,   323,
     143,   323,    -1,   323,   147,   323,    -1,   323,   175,   323,
      -1,   323,   147,    -1,   147,   323,    -1,   175,   323,    -1,
     147,    -1,    -1,   323,    -1,   105,   323,    -1,   106,   323,
      -1,   323,    -1,   305,    -1,   328,    -1,   329,    -1,   330,
      -1,   326,    -1,   323,   138,    -1,   307,   162,    -1,   252,
      -1,   332,    -1,   327,   170,   302,   171,    -1,   327,   172,
     302,   173,    -1,    80,   170,   302,   171,    -1,   323,   146,
     208,    -1,   323,   146,   107,    -1,   323,   146,    33,    -1,
     323,   146,    62,    -1,   323,   146,    20,   170,   171,    -1,
     170,   300,   171,    -1,   170,   300,   144,   171,    -1,   170,
     301,   171,    -1,   170,   301,   144,   171,    -1,    39,    -1,
     104,    -1,     8,    -1,     9,    -1,   331,    -1,   332,    -1,
       5,    -1,     6,    -1,     7,    -1,    10,    -1,    68,    -1,
     168,   298,   169,    -1,   168,   298,   144,   169,    -1,   172,
     298,   173,    -1,   172,   298,   144,   173,    -1,   172,   334,
     173,    -1,   172,   334,   144,   173,    -1,   323,   121,   323,
      -1,   334,   144,   323,   121,   323,    -1,   323,   161,   323,
      -1,   323,   157,   323,    -1,   323,   166,   323,    -1,   323,
     145,   323,    -1,   323,   164,   323,    -1,   323,   165,   323,
      -1,   323,   158,   323,    -1,   323,   149,   323,    -1,   323,
     159,   323,    -1,   323,   156,   323,    -1,   323,   152,   323,
      -1,   323,   155,   323,    -1,   323,   151,   323,    -1,   323,
     139,   323,    -1,   323,   141,   323,    -1,   323,   142,   323,
      -1,   323,   122,   323,    -1,   323,   160,   323,    -1,   323,
     150,   323,    -1,   323,    19,   323,    -1,   323,    12,   323,
      -1,   323,   153,   323,    -1,   323,    31,   323,    -1,   161,
     323,    -1,   157,   323,    -1,    63,   323,    -1,    78,   323,
      -1,   138,   323,    -1,   323,   138,    -1,   140,   323,    -1,
     323,    87,   323,    -1,   323,    87,   225,    -1,   339,    87,
     323,    -1,   339,    87,   225,    -1,   323,    91,   323,    -1,
     323,    91,   225,    -1,   339,    91,   323,    -1,   339,    91,
     225,    -1,   161,    -1,   166,    -1,   122,    -1,   160,    -1,
     139,    -1,   141,    -1,   142,    -1
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
    1036,  1042,  1048,  1054,  1060,  1067,  1074,  1078,  1085,  1089,
    1090,  1091,  1092,  1094,  1095,  1096,  1097,  1099,  1101,  1103,
    1105,  1110,  1111,  1115,  1117,  1125,  1126,  1131,  1136,  1137,
    1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,  1147,
    1148,  1155,  1156,  1157,  1158,  1167,  1168,  1172,  1174,  1177,
    1183,  1185,  1188,  1194,  1197,  1198,  1199,  1200,  1201,  1202,
    1206,  1207,  1211,  1212,  1213,  1217,  1218,  1222,  1225,  1227,
    1232,  1233,  1237,  1239,  1241,  1248,  1258,  1272,  1277,  1282,
    1290,  1291,  1296,  1297,  1299,  1304,  1320,  1327,  1336,  1344,
    1348,  1355,  1356,  1361,  1366,  1360,  1393,  1396,  1400,  1408,
    1418,  1407,  1457,  1461,  1466,  1470,  1475,  1482,  1483,  1487,
    1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,
    1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,  1507,
    1508,  1509,  1510,  1511,  1512,  1513,  1514,  1518,  1519,  1520,
    1521,  1522,  1523,  1524,  1525,  1526,  1527,  1528,  1529,  1533,
    1534,  1538,  1542,  1543,  1544,  1548,  1550,  1552,  1554,  1556,
    1558,  1563,  1564,  1568,  1569,  1570,  1571,  1572,  1573,  1574,
    1575,  1576,  1580,  1581,  1582,  1583,  1584,  1585,  1589,  1590,
    1591,  1595,  1596,  1597,  1598,  1599,  1600,  1604,  1605,  1608,
    1609,  1613,  1614,  1618,  1622,  1623,  1624,  1632,  1633,  1635,
    1637,  1639,  1644,  1646,  1651,  1652,  1653,  1654,  1655,  1656,
    1657,  1661,  1663,  1668,  1670,  1672,  1677,  1690,  1707,  1708,
    1710,  1715,  1716,  1717,  1718,  1719,  1723,  1729,  1737,  1738,
    1746,  1748,  1753,  1755,  1757,  1762,  1764,  1766,  1773,  1774,
    1775,  1780,  1782,  1784,  1788,  1792,  1794,  1798,  1806,  1807,
    1808,  1809,  1810,  1815,  1816,  1817,  1818,  1819,  1839,  1843,
    1847,  1855,  1862,  1863,  1864,  1868,  1870,  1876,  1878,  1880,
    1885,  1886,  1887,  1888,  1889,  1895,  1896,  1897,  1898,  1902,
    1903,  1907,  1908,  1909,  1913,  1914,  1918,  1919,  1923,  1924,
    1928,  1929,  1930,  1931,  1935,  1936,  1947,  1949,  1951,  1957,
    1958,  1959,  1960,  1961,  1962,  1964,  1966,  1968,  1970,  1972,
    1974,  1977,  1979,  1981,  1983,  1985,  1987,  1989,  1991,  1994,
    1996,  2001,  2003,  2005,  2007,  2009,  2011,  2013,  2015,  2017,
    2019,  2021,  2023,  2025,  2032,  2038,  2044,  2050,  2059,  2069,
    2077,  2078,  2079,  2080,  2081,  2082,  2083,  2084,  2089,  2090,
    2094,  2098,  2099,  2103,  2107,  2108,  2112,  2116,  2120,  2127,
    2128,  2129,  2130,  2131,  2132,  2136,  2137,  2142,  2144,  2148,
    2152,  2156,  2164,  2169,  2175,  2181,  2188,  2198,  2206,  2207,
    2208,  2209,  2210,  2211,  2212,  2213,  2214,  2215,  2217,  2219,
    2221,  2236,  2238,  2240,  2242,  2247,  2248,  2252,  2253,  2254,
    2258,  2259,  2260,  2261,  2270,  2271,  2272,  2273,  2274,  2278,
    2279,  2280,  2284,  2285,  2286,  2287,  2288,  2296,  2297,  2298,
    2299,  2303,  2304,  2308,  2309,  2313,  2314,  2315,  2316,  2317,
    2318,  2319,  2320,  2322,  2324,  2325,  2326,  2330,  2338,  2339,
    2343,  2344,  2345,  2346,  2347,  2348,  2349,  2350,  2351,  2352,
    2353,  2354,  2355,  2356,  2357,  2358,  2359,  2360,  2361,  2362,
    2363,  2364,  2365,  2370,  2371,  2372,  2373,  2374,  2375,  2376,
    2380,  2381,  2382,  2383,  2387,  2388,  2389,  2390,  2395,  2396,
    2397,  2398,  2399,  2400,  2401
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
     263,   264,   265,   265,   265,   266,   266,   266,   266,   266,
     266,   267,   267,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   269,   269,   269,   269,   269,   269,   270,   270,
     270,   271,   271,   271,   271,   271,   271,   272,   272,   273,
     273,   274,   274,   275,   276,   276,   276,   277,   277,   277,
     277,   277,   278,   278,   279,   279,   279,   279,   279,   279,
     279,   280,   280,   281,   281,   281,   282,   282,   283,   283,
     283,   284,   284,   284,   284,   284,   285,   285,   286,   286,
     287,   287,   288,   288,   288,   289,   289,   289,   290,   290,
     290,   291,   291,   291,   291,   291,   291,   291,   292,   292,
     292,   292,   292,   293,   293,   293,   293,   293,   294,   294,
     294,   294,   295,   295,   295,   296,   296,   296,   296,   296,
     297,   297,   297,   297,   297,   298,   298,   298,   298,   299,
     299,   300,   300,   300,   301,   301,   302,   302,   303,   303,
     304,   304,   304,   304,   305,   305,   306,   306,   306,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   309,   310,
     311,   311,   311,   311,   311,   311,   311,   311,   312,   312,
     313,   314,   314,   315,   316,   316,   317,   317,   317,   318,
     318,   318,   318,   318,   318,   319,   319,   320,   320,   320,
     320,   320,   321,   321,   321,   321,   321,   322,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   324,   324,   325,   325,   325,
     326,   326,   326,   326,   327,   327,   327,   327,   327,   328,
     328,   328,   329,   329,   329,   329,   329,   330,   330,   330,
     330,   331,   331,   332,   332,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   334,   334,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   336,   336,   336,   336,   336,   336,   336,
     337,   337,   337,   337,   338,   338,   338,   338,   339,   339,
     339,   339,   339,   339,   339
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
       3,     3,     0,     1,     3,     4,     5,     4,     5,     6,
       6,     0,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     0,     1,     2,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     0,     2,     2,
       4,     4,     1,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     3,     4,     4,     2,     4,     0,     2,
       2,     1,     1,     1,     2,     1,     4,     3,     1,     3,
       3,     5,     1,     1,     3,     1,     2,     3,     0,     2,
       2,     3,     2,     4,     3,     3,     4,     3,     0,     2,
       2,     2,     1,     0,     2,     2,     2,     1,     4,     4,
       6,     3,     0,     1,     1,     3,     4,     3,     4,     6,
       0,     2,     2,     2,     2,     1,     1,     3,     3,     1,
       3,     1,     1,     1,     3,     3,     0,     1,     1,     3,
       3,     3,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     2,     4,     4,     4,     5,     2,
       2,     1,     2,     1,     2,     1,     2,     1,     2,     1,
       1,     6,     6,     4,     9,     9,     7,     6,     6,     4,
       9,     9,     7,     4,     6,     6,     9,     9,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       4,     1,     3,     4,     1,     3,     4,     3,     3,     1,
       1,     2,     1,     2,     1,     1,     3,     1,     2,     2,
       2,     2,     2,     8,     8,     9,     9,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     3,
       3,     2,     2,     2,     1,     0,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     4,
       4,     4,     3,     3,     3,     3,     5,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     4,     3,     4,     3,     4,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,   113,   637,   638,   639,   633,
     634,   640,     0,   558,    99,   134,   527,   141,   529,   558,
       0,   140,     0,   432,    99,     0,     0,     0,   255,   135,
     605,   605,   631,     0,     0,     0,     0,     0,   139,    55,
     256,   307,   136,     0,     0,     0,   303,     0,     0,   143,
       0,   577,   549,   641,   144,     0,   308,   521,   431,     0,
       0,     0,   157,   306,   138,   530,   433,     0,     0,     0,
       0,   525,     0,     0,   142,     0,     0,   114,     0,   632,
       0,     0,     0,   137,   289,   523,   435,   145,     0,     0,
     690,     0,   692,     0,   693,   694,   604,     0,   691,   688,
     508,   154,   689,     0,     0,     0,     0,     4,     0,     5,
       0,     9,    50,    10,    11,     0,    12,    13,    14,    16,
     504,   505,    26,    15,   155,   164,   165,    17,    21,    18,
      20,     0,   250,    19,   596,    23,    24,    25,    22,   163,
       0,   161,     0,   593,     0,   159,   162,     0,   160,   610,
     589,   506,   590,   511,   509,     0,     0,     0,   594,   595,
       0,   510,     0,   611,   612,   613,   635,   636,   588,   513,
     512,   591,   592,     0,    42,    28,   519,     0,     0,   559,
     100,     0,     0,   529,   135,     0,     0,     0,     0,   530,
       0,     0,     0,     0,   593,   610,   509,   594,   595,   528,
     510,   611,   612,     0,   558,     0,     0,   434,     0,   263,
       0,   489,     0,   496,   633,   530,   606,   306,   633,   182,
     530,     0,   306,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   169,     0,     0,     0,     0,     0,    52,   496,
     107,   115,   127,   121,   120,   129,   110,   119,   130,   116,
     131,   108,   132,   125,   118,   126,   124,   122,   123,   109,
     111,   117,   128,   133,     0,   112,     0,     0,     0,     0,
       0,     0,   438,     0,   151,    36,     0,   675,   581,   578,
     579,   580,     0,   522,   676,     7,   496,   306,   287,   297,
     605,   288,   156,   403,   486,     0,   485,     0,     0,   152,
     609,     0,     0,    39,     0,   526,   514,     0,   496,    40,
     520,     0,   270,   266,     0,   510,   270,   267,     0,   428,
       0,   524,     0,     0,     0,   677,   679,   602,   674,   673,
       0,    57,    60,     0,     0,   491,     0,   493,     0,     0,
     492,     0,     0,   485,     0,   603,     0,     6,     0,    51,
       0,     0,     0,     0,   290,     0,   389,   390,   388,   309,
       0,   507,    27,     0,   582,     0,     0,     0,     0,     0,
       0,   678,     0,     0,     0,     0,     0,     0,   601,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,   354,   355,   356,   351,
     353,     0,     0,   349,   352,   350,   348,     0,   358,   357,
       0,     0,   496,   496,     0,     0,     0,    29,    30,     0,
       0,     0,     0,     0,     0,     0,    31,     0,     0,     0,
       0,    32,     0,    33,     0,   504,   502,     0,   497,   498,
     503,   176,     0,   179,     0,   172,     0,     0,   178,     0,
       0,     0,   192,     0,     0,   191,     0,   200,     0,     0,
       0,   198,     0,   208,     0,     0,   206,     0,     0,    72,
     166,     0,     0,     0,   224,     0,   347,   220,     0,    54,
      53,    50,     0,     0,     0,   234,    34,   371,   304,   442,
       0,   443,   445,     0,   467,     0,   448,     0,     0,   150,
      35,     0,     0,    37,     0,   158,     0,    92,   607,   608,
     153,     0,    38,   496,     0,   277,   268,   264,   269,   265,
       0,   426,   423,   185,   184,    41,    59,    58,    61,     0,
     642,     0,     0,   627,     0,   629,     0,     0,     0,     0,
       0,     0,     0,     0,   646,     8,     0,    44,     0,     0,
      90,     0,    87,     0,    66,   261,     0,     0,     0,   382,
     437,   557,   670,   669,   672,   681,   680,   685,   684,   666,
     663,   664,   665,   598,   653,   113,     0,   624,   625,   114,
     623,   622,   599,   657,   668,   662,   660,   671,   661,   659,
     651,   656,   658,   667,   650,   654,   655,   652,   600,     0,
       0,     0,     0,     0,     0,     0,     0,   683,   682,   687,
     686,   569,   570,   572,   574,     0,   561,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   644,   261,   558,
     558,   188,   424,   436,   490,     0,     0,   516,     0,     0,
     290,     0,     0,   290,   425,     0,     0,     0,     0,   533,
       0,     0,     0,   201,     0,   539,     0,     0,   199,   209,
       0,     0,   207,   689,    75,     0,    62,    73,     0,     0,
       0,   223,     0,   219,     0,     0,     0,     0,   515,     0,
     237,     0,   235,   376,   373,   374,   375,   379,   380,   381,
     371,     0,   363,     0,   372,   391,     0,   446,     0,   148,
     149,   147,   146,     0,   466,   465,   589,     0,   440,   587,
     439,     0,     0,   621,   488,   487,     0,     0,     0,   517,
       0,   271,   430,   589,     0,   643,   597,   628,   494,   630,
     495,   216,     0,     0,     0,   645,   214,   543,     0,   648,
     647,     0,    46,    45,    43,     0,    86,     0,     0,     0,
      79,     0,     0,    72,   258,     0,   291,     0,     0,   301,
       0,   298,   386,   383,   384,   387,   310,     0,     0,    98,
      96,    97,    95,    94,    93,   619,   620,   571,   573,     0,
     560,   134,   141,   140,   139,   136,   143,   144,   138,   142,
     137,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,   500,   501,
     499,   290,     0,   181,   290,     0,   180,     0,   218,     0,
       0,   190,     0,   189,     0,   564,     0,     0,   196,     0,
       0,   194,     0,   204,     0,   202,     0,     0,   167,     0,
     168,   232,   231,     0,   226,     0,     0,   222,     0,   228,
       0,   260,     0,     0,   377,   378,     0,   371,   361,     0,
     480,   392,   395,   394,   396,     0,   444,   447,   448,     0,
       0,   449,   450,     0,     0,   279,     0,     0,   278,   281,
     518,     0,   272,   275,     0,   427,   217,     0,     0,     0,
       0,   215,     0,    91,    88,     0,    69,    68,    67,     0,
       0,     0,     0,   306,   296,     0,   299,   295,   385,   391,
     359,   101,   341,   115,   339,   121,   120,   104,   119,   116,
     344,   131,   102,   132,   118,   122,   103,   105,   117,   133,
     338,   320,   323,   321,   322,   345,   333,   324,   337,   329,
     327,   340,   343,   328,   326,   331,   336,   325,   330,   334,
     335,   332,   342,     0,   319,     0,   106,     0,   359,   359,
     317,   626,   562,   448,   610,   610,     0,     0,     0,     0,
       0,     0,   260,     0,     0,     0,   187,   186,     0,   292,
       0,   292,   193,     0,     0,   532,     0,   531,     0,   563,
       0,     0,   538,   197,     0,   537,   195,   205,   203,    64,
      63,   225,   221,   548,   227,     0,     0,   257,   236,   233,
     480,   364,     0,     0,   448,   393,   407,   441,   471,     0,
     644,   496,   496,     0,   283,     0,     0,     0,   273,     0,
     212,   545,     0,     0,   210,   544,     0,   649,     0,     0,
       0,    72,     0,    72,    80,    83,   262,   286,   157,   306,
     285,   306,   293,   302,   300,     0,   371,   316,     0,   346,
       0,   312,   313,   566,     0,     0,     0,     0,     0,     0,
       0,     0,   262,   292,   306,   292,   306,   536,     0,     0,
     565,   542,     0,     0,     0,     0,   230,    56,   448,   480,
       0,   483,   482,   484,   589,   404,   367,   365,     0,     0,
       0,     0,   469,   589,     0,     0,   284,   282,     0,   276,
       0,   213,     0,   211,    89,    71,    70,     0,     0,     0,
       0,   259,   294,   462,     0,   397,     0,   318,   101,   103,
     359,   359,     0,     0,     0,     0,     0,     0,   306,   175,
     306,   171,     0,     0,     0,     0,    65,   229,   368,   366,
     448,   472,     0,   406,   405,   421,     0,   422,   409,   412,
       0,   408,   401,   402,   305,     0,   583,   584,   274,     0,
       0,    82,    85,    81,    84,     0,   461,   460,   589,   398,
     407,   360,   314,   315,     0,     0,     0,     0,     0,     0,
     177,   173,   535,   534,   541,   540,   370,   369,   474,   475,
     477,   589,     0,   644,   420,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   589,   585,   586,   547,   546,     0,
     452,     0,     0,     0,   476,   478,   411,   413,   414,   417,
     418,   419,   415,   416,   410,   457,   455,   589,   644,   399,
     311,   400,   472,   456,   589,   479
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   332,   108,   673,   110,   111,   112,   350,
     481,   113,   238,   114,   333,   664,   553,   668,   665,   115,
     116,   117,   550,   551,   118,   119,   181,   954,   270,   120,
     265,   121,   704,   275,   122,   123,   287,   124,   125,   126,
     446,   642,   442,   639,   127,   128,   802,   129,   236,   130,
     681,   682,   193,   132,   133,   134,   135,   136,   516,   721,
     884,   137,   138,   717,   879,   139,   140,   557,   903,   141,
     142,   760,   761,   194,   268,   695,   144,   145,   559,   909,
     766,   957,   958,   478,  1057,   488,   691,   692,   693,   694,
     767,   359,   865,  1180,  1240,  1164,   436,  1096,  1100,  1158,
    1159,  1160,   146,   320,   521,   147,   148,   271,   272,   492,
     493,   708,  1177,  1125,   496,   705,  1199,  1093,  1014,   334,
     210,   338,   339,   437,   438,   439,   195,   150,   151,   152,
     153,   196,   155,   178,   179,   615,   458,   824,   616,   617,
     156,   157,   197,   198,   160,   222,   440,   200,   162,   201,
     202,   165,   166,   167,   168,   344,   169,   170,   171,   172,
     173
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1103
static const yytype_int16 yypact[] =
{
   -1103,   309,  3730, -1103,    90,   280, -1103, -1103, -1103, -1103,
   -1103, -1103,  5114,   147,   349,   304, 14999,   329,  3559,   147,
   11658,   372,   688,   325,   349,  5114, 11658,  5114,   282, 19957,
   11829,  8564,   452,  8735,  9940,  9940,  7188,  8906,   463, -1103,
     393, -1103,   482, 20042, 20042, 20042, -1103,  3284, 10111,   525,
   11658,   217, -1103,   532,   543, 11658, -1103, 14999, -1103, 11658,
     583,   458,   192, 18568,   563, 20083, -1103, 10284,  8049, 11658,
   10111, 14999, 11658,   533,   575,   465,  5114,   578, 11658,   582,
   12000, 12000, 20042,   590, -1103, 14999, -1103,   591,  8906, 11658,
   -1103, 11658, -1103, 11658, -1103, -1103, 14523, 11658, -1103, 11658,
   -1103, -1103, -1103,  4076,  7361,  9079, 11658, -1103,  4941, -1103,
     480, -1103,   568, -1103, -1103,   209, -1103, -1103, -1103, -1103,
   -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103,
   -1103,   598, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103,
   20042, -1103, 20042,   295,   210, -1103, -1103,  3284, -1103,   491,
   -1103,   494, -1103, -1103,   497,   500,   510, 11658,   502,   504,
   19602,  2210,   -42,   506,   529, -1103, -1103,   244, -1103, -1103,
   -1103, -1103, -1103,   459, -1103, -1103, 19602,   503,  5114, -1103,
   -1103,   535, 11658, -1103, -1103, 11658, 11658, 11658, 20042, -1103,
   11658, 10284, 10284,   617,   258, -1103, -1103, -1103, -1103,   -32,
     387, -1103, -1103,   517, 16724, 20042,  3284, -1103,   540, -1103,
     166, 19602,   571,  8222,   610, 20168, 19602,   231,   613, -1103,
   20209, 20042,   231, 20042,   538,    31, 16324,    20, 16166,    20,
   16242,   448, -1103, 16407, 20042, 20042,   132, 15452,   261,  8222,
   -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103,
   -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103,
   -1103, -1103, -1103, -1103,   548, -1103,   403,  5114,   549,  1354,
      97,    41, -1103,  5114, -1103, -1103, 16805,   686, -1103,   552,
     554, -1103, 16805,   -32,   686, -1103,  8222,  1840, -1103, -1103,
   10455, -1103, -1103, -1103, -1103,   178, 19602, 11658, 11658, -1103,
   19602,   542, 17355, -1103, 16805,   -32, 19602,   555,  8222, -1103,
   19602, 17396, -1103, -1103, 17441,  3288, -1103, -1103, 17546,   603,
     564,   -32,    31, 16805, 17591,   479,   479,  1538,   686,   686,
     352, -1103, -1103,  4249,   -47, -1103, 11658, -1103,   -40,   124,
   -1103,   -10,    23, 17640,   -44,  1538,   698, -1103,  4422, -1103,
     665, 11658, 11658, 20042,   587,   567, -1103, -1103, -1103, -1103,
     268,   402, -1103, 11658,   586, 11658, 11658, 11658,  9940,  9940,
   11658,   409, 11658, 11658, 11658, 11658, 11658,   287, 14523, 11658,
   11658, 11658, 11658, 11658, 11658, 11658, 11658, 11658, 11658, 11658,
   11658, 11658, 11658, 11658, 11658,   669, -1103, -1103, -1103, -1103,
   -1103,  9250,  9250, -1103, -1103, -1103, -1103,  9250, -1103, -1103,
    9250,  9250,  8222,  8222,  9940,  9940,  7878, -1103, -1103, 16881,
   16962, 17741,   572,    25, 20042,  4595, -1103,  9940,    31,   584,
     274, -1103, 11658, -1103, 11658,   615, -1103,   577,   602, -1103,
   -1103, -1103, 20042, -1103,  3284, -1103, 20042,   589, -1103,  3284,
     706, 10284, -1103,  5287,  9940, -1103,   585, -1103,    31,  5460,
    9940, -1103,    31, -1103,    31,  9940, -1103,    31, 12171, 11658,
   -1103,   631,   633,  5114,   724,  5114, -1103,   725, 11658, -1103,
   -1103,   568,   592,  8222, 20042, -1103, -1103,   461, -1103, -1103,
    1354, -1103,   620,   597, -1103, 12342,   638, 11658,  3284, -1103,
   -1103, 11658, 11658, -1103,   604, -1103, 10284, -1103, 19602, 19602,
   -1103,    50, -1103,  8222,   607, -1103,   748, -1103,   748, -1103,
   12513,   630, -1103, -1103, -1103, -1103, -1103, -1103, -1103,  9423,
   -1103, 17790,  7534, -1103,  7707, -1103,  5114,   606,  9940,  9596,
    3903,   612, 11658, 10626, -1103, -1103,   370, -1103,  4768, 20042,
   -1103,   306, 17830,   315, 16490,    89, 10284,   619, 19916,    19,
   -1103, 17947,  2939,  2939,   509, -1103,   509, -1103,   509,  2744,
     334,  1393,   703,   -32,   479, -1103,   621, -1103, -1103, -1103,
   -1103, -1103,  1538, 19757,   509,  1353,  1353,  2939,  1353,  1353,
     536,   479, 19757,  2468,   536,   686,   686,   479,  1538,   626,
     634,   635,   636,   641,   642,   622,   623, -1103,   509, -1103,
     509,    63, -1103, -1103, -1103,   159, -1103,  2157, 19719,   469,
   12684,  9940, 12855,  9940, 11658,  8222,  9940, 15271,   639,   147,
   17992, -1103, -1103, -1103, 19602, 18032,  8222, -1103,  8222, 20042,
     587,   318, 20042,   587, -1103,   330, 11658,   239,  8906, 19602,
      46, 17038,  7878, -1103,  8906, 19602,    21, 16566, -1103, -1103,
      20, 16648, -1103,   648,   670,   650, 18149,   670,   653, 11658,
   11658,   783,  5114,   785, 18189,  5114, 17120,   757, -1103,   252,
   -1103,   253, -1103,   195, -1103, -1103, -1103, -1103, -1103, -1103,
     808,   263, -1103,  3518, -1103,    59,   651,  1354,    97,    56,
      69, 11658, 11658,  7017, -1103, -1103,   560,  8393, -1103, 19602,
   -1103, 18229, 18346, -1103, -1103, 19602,   654,    80,   657, -1103,
    2099, -1103, -1103,   360, 20042, -1103, -1103, -1103, -1103, -1103,
   -1103, -1103,  5114,   153, 17196, -1103, -1103, 19602,  5114, 19602,
   -1103, 18387, -1103, -1103, -1103, 11658, -1103,    51,   129, 11658,
   -1103, 10797, 12171, 11658, -1103,  8222,   693,  1039,   671,   715,
     122, -1103,   751, -1103, -1103, -1103, -1103, 14357,   676, -1103,
   -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103,  7878,
   -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103,
   -1103, -1103,    38,  9940,  9940, 11658,   819, 18427, 11658,   826,
   18577,   264,   677, 18617,  8222,    31,    31, -1103, -1103, -1103,
   -1103,   587,   692, -1103,   587,   694, -1103, 16805, -1103, 15528,
    5633, -1103,  5806, -1103,   265, -1103, 15610,  5979, -1103,    31,
    6152, -1103,    31, -1103,    31, -1103,    31, 11658, -1103, 11658,
   -1103, 19602, 19602,  5114, -1103,  5114, 11658, -1103,  5114,   829,
   20042,   701, 20042,   517, -1103, -1103, 20042,   857, -1103,  1354,
     723,   782, -1103, -1103, -1103,    43, -1103, -1103,   638,   699,
      58, -1103, -1103,   704,   709, -1103,  6325, 10284, -1103, -1103,
   -1103, 20042, -1103,   728,   517, -1103, -1103,  6498,   713,  6671,
     716, -1103, 11658, -1103, -1103, 11658, 18657,    79, 17279,   729,
     732,   305,   707,  1223, -1103, 11658, 20042, -1103, -1103,    59,
     718,   365, -1103,   752, -1103,   753,   754,   759,   756,   760,
   -1103,   761,   765,   764,   767,   772,   367,   773,   774,   776,
   -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103,
   -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103,
   -1103, -1103, -1103, 11658, -1103,   781,   786,   777,   718,   718,
   -1103, -1103, -1103,   638,   324,   343, 18774, 13026, 13197, 18814,
   13368, 13539, -1103, 13710, 13881,   353, -1103, -1103,   737, -1103,
     769, -1103, -1103,  5114,  8906, 19602,  8906, 19602,  7878, -1103,
    5114,  8906, 19602, -1103,  8906, 19602, -1103, -1103, -1103, 18854,
   19602, -1103, -1103, 19602,   891,  5114,   766, -1103, -1103, -1103,
     723, -1103,   762, 10970,   150, -1103,   168, -1103, -1103,  9940,
   15135,  8222,  8222,  5114, -1103,    39,   768, 11658, -1103,  8906,
   -1103, 19602,  5114,  8906, -1103, 19602,  5114, 19602,   144, 11141,
   12171, 11658, 12171, 11658, -1103, -1103,   775, -1103, -1103,  1840,
   -1103,  1725, -1103, 19602, -1103,    94,   461, -1103, 18971, -1103,
   15371, -1103, -1103, -1103, 11658, 11658, 11658, 11658, 11658, 11658,
   11658, 11658, -1103, -1103,  1820, -1103,  1929, 18189, 15686, 15768,
   -1103, 18189, 15844, 15926, 11658,  5114, -1103, -1103,   150,   723,
    9769, -1103, -1103, -1103,   128, 10284, -1103, -1103,   143, 11658,
       9, 19011, -1103,   710,   778,   779,   571, -1103,   517, 19602,
   16002, -1103, 16084, -1103, -1103, -1103, 19602,   780,   784,   789,
     790, -1103, -1103, -1103, 14052,   824,   358, -1103,   794,   797,
     718,   718, 19128, 19168, 19208, 19325, 19365, 19405,  2203, -1103,
    2425, -1103,  5114,  5114,  5114,  5114, 19602, -1103, -1103, -1103,
     150, 11314,    72, -1103, 19602, -1103,    99, -1103,   145, -1103,
     182, 19522, -1103, -1103, -1103, 13881,   792,   795, -1103,  5114,
    5114, -1103, -1103, -1103, -1103,  6844, -1103, -1103,   164, -1103,
     168, -1103, -1103, -1103, 11658, 11658, 11658, 11658, 11658, 11658,
   -1103, -1103, 18189, 18189, 18189, 18189, -1103, -1103, -1103, -1103,
   -1103,   338,  9940, 14693, -1103, 11658,   143,    99,    99,    99,
      99,    99,    99,   143,   870, -1103, -1103, 18189, 18189,   755,
   14223,    73,    -8, 19562, -1103, -1103, 19602, -1103, -1103, -1103,
   -1103, -1103, -1103, -1103,   796, -1103, -1103,   380, 14863, -1103,
   -1103, -1103, 11487, -1103,   581, -1103
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1103, -1103, -1103,     3,  -470,  2696, -1103, -1103, -1103,   485,
   -1103, -1103, -1103,   484,   596,  -466, -1103,  -727,  -725, -1103,
   -1103, -1103,   218, -1103, -1103,   522,   943, -1103,  2793,  -165,
    -758, -1103,  -952,   953, -1035,  -861, -1103,   -62, -1103, -1103,
   -1103, -1103, -1103, -1103, -1103, -1103,   -22, -1103,   880, -1103,
   -1103,   121,  1267, -1103, -1103, -1103, -1103, -1103,   659, -1103,
      96, -1103, -1103, -1103, -1103, -1103, -1103,  -608,  -656, -1103,
   -1103, -1103,    74,  1543, -1103, -1103, -1103,   336, -1103, -1103,
   -1103, -1103,   -79,  -151,  -909, -1103,   -73,   127,   298, -1103,
   -1103, -1103,    76, -1103, -1103,  -233,    13, -1013,  -189,  -223,
    -212,  -615, -1103,  -190, -1103,    12,   959,   -38,   501, -1103,
    -474,  -838,  -794, -1103,  -657,  -514, -1102, -1061,  -897,   -65,
   -1103,    87, -1103,  -219,  -454,   362,   818,  -458, -1103, -1103,
   -1103,  1647, -1103,    14, -1103, -1103,   -72, -1103,  -645, -1103,
   -1103, -1103,  2028,  2128,   -12,   968,    44,   889, -1103,  2305,
    2404, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103, -1103,
    -388
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -645
static const yytype_int16 yytable[] =
{
     176,   292,   295,   667,   199,   107,   722,   825,   204,   956,
     411,   225,   227,   229,   211,   429,   696,   690,   216,   216,
     482,   226,   228,   230,   233,   237,   900,   899,   619,   679,
    1017,   447,   812,   203,   206,   815,   276,   706,   277,   494,
     342,   868,  1052,   282,   494,   283,   762,   284,   435,  1061,
    1062,   716,   273,   827,   575,   296,   300,   302,   304,   305,
     306,  1091,   723,   273,   310,  1163,   311,   504,   314,   318,
     538,   273,   626,   321,   435,  1148,   323,   324,   820,   325,
     294,   326,    68,   341,   327,   328,   861,   329,   497,   514,
    1200,   296,   300,   343,   345,  1123,   763,   529,   494,    68,
     543,  1224,  1155,  -463,   532,  1019,   371,   764,   456,   360,
     777,  -255,   301,  1088,   377,  1040,   294,   337,   294,  1202,
     626,   435,   530,  -280,  -256,  1063,   765,   423,   412,   544,
     413,   533,   575,  -463,   962,   963,   862,  1196,   456,   456,
    1245,   456,  1225,   435,  -463,   364,  1155,   863,   340,   576,
    1041,   778,   579,   876,  -458,  1239,   462,   464,   467,  -463,
     425,  -463,   577,   536,   411,  -280,   864,   539,   430,   539,
     176,   801,  1176,   419,   420,   421,  1097,   425,   310,   296,
     343,   495,  -463,   506,  -458,   498,   495,  1163,   425,   425,
    1122,   578,  1150,   605,   606,   877,   540,  -458,   627,   425,
    1157,   300,   539,   978,   294,   294,   980,   425,  1016,  -463,
    -458,  -463,   581,  1052,   425,  1052,   539,   539,   428,  -280,
    -463,  1182,  1183,   867,  -459,   505,   213,   300,  1098,   444,
     579,  1020,   473,  1156,   449,   278,   580,  1124,   -78,   239,
     495,  -463,   854,   206,  1157,  1203,  1238,   435,   435,   878,
    1149,  -481,   754,   174,  -459,   351,   -49,  -458,    23,   755,
    -463,  1205,  -458,   356,   619,   177,   906,  -459,   534,   537,
     541,   456,  -481,   707,   300,   -49,  -481,  1052,   216,  1052,
    -459,    41,   357,   855,  1099,   508,   509,   667,   837,  1206,
     575,   907,   358,   279,   718,   535,   300,   895,  1095,  -481,
     425,   901,   956,   779,   -78,  1207,    56,   576,    58,     3,
     432,   280,  1197,  1114,  1118,  1117,  1120,  1119,   435,    66,
     577,   352,   506,  1074,   531,  1076,   887,  -459,   281,   433,
     780,  1208,  -459,  1209,  1210,  -238,   528,  1211,  1212,   552,
     554,   507,   479,  1080,    86,   480,   565,   567,   435,   578,
     975,   561,   180,   562,   563,   564,   566,   568,   569,  -239,
     570,   571,   572,   573,   574,   367,   582,   583,   584,   585,
     586,   587,   588,   589,   590,   591,   592,   593,   594,   595,
     596,   597,   598,   506,  -245,  1012,   647,   690,   579,   300,
     300,   619,   607,   609,   580,   300,   638,   852,   300,   300,
     300,   300,   608,   610,   618,   629,   641,   857,   638,   988,
     818,   645,   498,   207,  -618,   630,  -618,  1138,   498,  1140,
     634,   368,   635,   851,   853,   369,  1236,  -244,  -617,  1038,
    -617,   560,   650,  1051,   858,   972,   989,   633,   656,   296,
    -451,   649,   651,   660,  1243,   600,   601,   655,   657,   638,
     745,   602,   213,   661,   603,   604,   666,   666,  -554,   749,
     435,  -473,   498,   674,   294,  -617,   676,  -617,  -568,   746,
    -451,   300,   371,   435,   498,    23,  1046,   375,   750,   376,
     377,   813,  -473,  -451,   380,   709,  -473,  -567,   683,   711,
     712,   756,   387,   816,   715,  -568,  -451,   638,   391,   392,
     393,   300,   857,  -107,  -429,  -109,  1102,  -253,   684,  -473,
     367,  -113,   685,  -114,  -567,   174,   733,   715,  -243,   714,
     300,   526,   300,  -429,  1072,    58,   734,   715,   737,  1181,
     739,   741,   274,   174,   885,   686,    66,  -240,   687,   742,
      60,  1204,   714,  -451,   296,   337,   414,   337,  -451,   688,
     415,   528,   714,   443,   274,  1094,   794,  -614,   448,  -614,
     415,    86,  1103,   239,   312,   316,   368,   367,   689,   294,
     369,   425,  -616,   484,  -616,   667,   340,   667,   340,  -615,
    -247,  -615,   894,   581,   829,   832,   690,  -251,   834,   836,
     435,   285,  1228,  1229,  1230,  1231,  1232,  1233,  -248,   796,
     619,   799,   313,   317,  1051,  -362,  1051,  -464,   649,   797,
     655,   800,   674,   300,   803,   737,   959,   371,  -242,   728,
    -464,   730,   375,   368,   300,   377,   300,   369,   286,   380,
    -246,   307,  -362,  -252,   817,   308,   819,  -254,   870,   435,
     618,  -453,   826,   805,   806,  -241,  -249,   371,   348,   808,
    -464,   349,   375,   353,  -551,   377,   361,   841,   842,   380,
    -550,   888,   890,   362,   363,  -555,  1178,  -556,  1051,  -553,
    1051,  -453,   424,   416,   371,   372,  -464,   373,   374,   375,
     809,   376,   377,  -464,  -453,   425,   380,   426,   434,   306,
     310,   343,  -552,  1201,   387,   300,   441,  -453,   418,   445,
     391,   392,   393,   431,  -464,   510,   545,  1214,   451,   274,
     274,   274,   274,   274,   274,    23,   294,   367,   483,   487,
     474,   477,   501,  -464,   502,   513,   520,   522,  -464,   549,
     556,  -464,   377,   552,   367,   558,   599,   896,   636,   898,
     666,   666,   625,   300,  -453,  1201,   638,   632,   637,  -453,
     485,   872,   644,   646,   669,   652,   670,  -468,   672,   675,
     274,   707,  1237,   678,   697,    58,   274,   618,   698,   720,
    -468,   565,   607,   368,   724,   713,    66,   369,   719,   732,
    1244,   566,   608,   966,  1201,   738,   969,   757,   274,   769,
     368,   768,   300,   775,   369,   205,   776,   770,   771,   772,
    -468,    86,  1104,  1105,   773,   774,   274,   274,   985,   804,
     987,   -74,  1025,   838,   837,   992,   840,   843,   995,   845,
     149,   850,   866,   875,   371,   999,  -468,  1000,   880,   375,
     149,   376,   377,  -468,  1003,   683,   380,   506,   905,   908,
     904,   371,   372,   149,   387,   149,   375,   961,   376,   377,
     973,   967,   393,   380,  -468,   684,   435,   435,   970,   685,
     979,   387,   981,  1005,  1007,   296,  1013,   391,   392,   393,
    1015,  1027,  1018,  -468,  1021,  1031,  1047,  1035,  -468,  1022,
    1037,  -468,   686,   666,   683,   687,  1032,   346,  1056,  1036,
     294,   161,  1044,  1053,   149,  1045,   688,  -110,  -134,  -141,
    -140,   161,  -139,  -108,   684,  1073,  -136,  -143,   685,  1131,
    -144,  -111,   274,  -138,   161,   689,   161,  -470,  -142,  1059,
    -137,   149,  -145,  1060,  -112,  1085,   149,  1179,  1235,  1087,
    -470,   686,  -107,  1089,   687,  -109,    60,  1075,  1121,  1108,
    1206,  1058,   274,  1171,   548,   688,   274,  1172,   274,  1166,
    1167,   274,  1173,  1174,  1215,   985,   987,  1216,   992,   995,
    -470,  1031,  1035,   893,   689,   161,   677,   208,   322,   315,
     315,  1077,  1078,  1008,  1079,   518,   618,  1026,  1081,  1082,
    1054,  1130,  1083,  1126,  1011,  1055,  -470,   292,   856,  1241,
    1234,  1222,   161,  -470,  1227,   232,   149,   161,   217,   710,
     810,     0,     0,     0,     0,     0,     0,  1101,   737,   300,
     300,     0,     0,     0,  -470,  1109,     0,  1110,     0,     0,
       0,  1112,     0,   303,     0,  1152,  1092,  1116,   666,   666,
     666,   666,     0,  -470,     0,     0,     0,     0,  -470,     0,
     902,  -470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1077,  1132,  1133,  1081,  1134,  1135,  1136,  1137,
       0,  -292,     0,     0,     0,  -292,  -292,   161,     0,     0,
       0,     0,  1146,     0,  -292,     0,  -292,  -292,   343,     0,
       0,     0,  -292,  1154,     0,   149,     0,  1161,     0,  -292,
       0,   149,  -292,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   294,     0,     0,     0,     0,  1153,     0,
    1221,  -292,     0,     0,  -292,     0,  -292,     0,  -292,     0,
    -292,  -292,     0,  -292,     0,  -292,     0,  -292,     0,     0,
    1192,  1193,  1194,  1195,   274,   274,     0,     0,     0,     0,
     274,   274,     0,     0,   274,   274,  -292,     0,     0,  -292,
       0,   149,  -292,  1035,     0,     0,   161,  1217,  1218,     0,
     849,     0,   161,   343,  1198,     0,   149,     0,     0,     0,
       0,     0,  1192,  1193,  1194,  1195,  1217,  1218,   452,   455,
     457,   461,   463,   466,     0,     0,     0,     0,   294,     0,
    1223,   737,     0,  1226,     0,     0,     0,     0,     0,     0,
       0,     0,  -292,     0,   882,     0,     0,     0,  -292,     0,
       0,     0,     0,     0,     0,     0,  1198,     0,     0,     0,
       0,     0,   161,     0,     0,     0,   737,     0,     0,   500,
    1035,     0,     0,     0,     0,   503,     0,   161,     0,     0,
       0,     0,     0,   149,     0,   288,     0,     0,     0,    22,
      23,     0,     0,     0,     0,  1198,     0,   512,   289,     0,
      30,   290,     0,     0,     0,     0,    36,     0,     0,   131,
       0,   149,     0,    41,     0,   523,   524,   149,     0,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   274,
     274,   149,   131,   149,   131,     0,     0,     0,    56,     0,
      58,   274,    60,   477,  1048,     0,     0,  1049,     0,   291,
     477,    66,     0,   274,   161,     0,   274,     0,   274,     0,
     274,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,  1009,     0,     0,
       0,     0,   161,   131,     0,     0,     0,     0,   161,     0,
       0,     0,     0,     0,   149,     0,     0,   240,   149,     0,
       0,     0,   161,     0,   161,     0,   149,     0,  1028,     0,
     131,   241,   242,     0,   243,   131,     0,     0,     0,   244,
       0,   631,     0,     0,   367,     0,   101,   245,     0,     0,
       0,     0,  1050,   246,     0,     0,     0,     0,     0,   247,
       0,     0,     0,   248,     0,     0,   249,     0,     0,     0,
       0,   653,     0,     0,     0,   658,   250,   659,     0,     0,
     662,     0,   251,   252,   367,   161,     0,     0,     0,   161,
     253,     0,     0,     0,     0,   792,     0,   161,     0,   254,
     368,     0,     0,     0,   369,   131,     0,     0,   255,   256,
       0,   257,     0,   258,     0,   259,     0,     0,   260,     0,
       0,     0,   261,   489,     0,   262,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     368,     0,     0,     0,   369,     0,     0,     0,     0,     0,
     149,   371,   372,   149,   373,   374,   375,     0,   376,   377,
     378,     0,     0,   380,     0,     0,     0,     0,     0,   274,
     386,   387,     0,     0,   390,     0,     0,   391,   392,   393,
       0,     0,     0,     0,   490,     0,     0,     0,   394,     0,
       0,   371,   372,     0,   131,   374,   375,     0,   376,   377,
     131,     0,     0,   380,     0,   143,     0,     0,     0,     0,
     149,   387,     0,     0,     0,   143,   149,   391,   392,   393,
       0,   161,   477,   477,   161,     0,   477,   477,   143,   367,
     143,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1162,   960,     0,     0,     0,     0,
       0,     0,  1168,     0,   477,     0,   477,     0,     0,     0,
     131,     0,     0,   821,   823,     0,     0,     0,     0,   828,
     831,   964,   965,   833,   835,   131,     0,     0,     0,   143,
       0,   161,     0,     0,     0,   368,     0,   161,     0,   369,
       0,     0,     0,     0,     0,     0,     0,     0,   149,     0,
     149,     0,     0,     0,     0,   149,   143,     0,   149,   154,
       0,   143,     0,     0,     0,     0,     0,     0,     0,   154,
       0,   149,     0,   149,     0,     0,   149,     0,     0,     0,
       0,     0,   154,     0,   154,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,     0,     0,     0,   380,     0,
       0,     0,   131,     0,   149,   386,   387,     0,     0,   390,
       0,     0,   391,   392,   393,   149,  1162,   149,     0,   161,
       0,   161,     0,     0,     0,     0,   161,     0,     0,   161,
     131,   143,     0,   154,     0,     0,   131,     0,     0,     0,
       0,     0,   161,     0,   161,     0,     0,   161,     0,     0,
     131,     0,   131,     0,     0,     0,     0,   288,     0,     0,
     154,    22,    23,     0,     0,   154,     0,     0,   976,   977,
     289,     0,    30,   290,     0,   161,     0,     0,    36,     0,
     982,     0,     0,     0,     0,    41,   161,     0,   161,     0,
       0,     0,   993,     0,     0,   996,     0,   997,     0,   998,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,   149,    58,   131,   346,     0,  1048,   131,   149,  1049,
     143,   291,     0,    66,     0,   131,   143,     0,     0,     0,
       0,     0,     0,   149,     0,   154,     0,     0,     0,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,   149,   288,     0,     0,     0,    22,    23,     0,     0,
     149,     0,     0,     0,   149,   289,     0,    30,   290,     0,
       0,     0,   288,    36,     0,     0,    22,    23,     0,     0,
      41,     0,   161,     0,     0,   289,   143,    30,   290,   161,
       0,     0,     0,    36,     0,     0,     0,     0,   101,     0,
      41,   143,     0,     0,   161,    56,     0,    58,     0,    60,
       0,  1048,     0,   149,  1049,     0,   291,     0,    66,     0,
       0,     0,   161,     0,   154,    56,     0,    58,     0,     0,
     154,   161,     0,     0,     0,   161,   291,    82,    66,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,   131,
       0,     0,   131,     0,     0,     0,     0,    82,     0,     0,
      84,   288,     0,    86,     0,    22,    23,     0,     0,     0,
     149,   149,   149,   149,   289,     0,    30,   290,   143,     0,
       0,     0,    36,     0,   161,     0,     0,     0,  1107,    41,
     154,     0,     0,   101,     0,     0,     0,   149,   149,  1139,
       0,     0,     0,     0,     0,   154,   143,     0,     0,   131,
       0,     0,   143,     0,    56,   131,    58,     0,    60,     0,
    1048,     0,     0,  1049,     0,   291,   143,    66,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     158,   161,   161,   161,   161,     0,    82,     0,     0,    84,
     158,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   158,     0,   158,     0,     0,   161,   161,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   154,     0,     0,     0,     0,     0,     0,   143,
       0,     0,     0,   143,     0,     0,     0,   131,     0,   131,
       0,   143,   101,     0,   131,     0,     0,   131,  1141,     0,
     154,     0,   240,     0,   158,     0,   154,     0,     0,     0,
     131,     0,   131,     0,     0,   131,   241,   242,     0,   243,
     154,     0,   154,     0,   244,     0,     0,     0,     0,     0,
     159,   158,   245,     0,     0,     0,   158,     0,   246,     0,
     159,     0,     0,   131,   247,     0,     0,     0,   248,     0,
       0,   249,     0,   159,   131,   159,   131,     0,     0,     0,
     575,   250,     0,     0,     0,     0,     0,   251,   252,     0,
       0,     0,     0,     0,   781,   253,     0,   782,     0,     0,
       0,     0,   783,   154,   254,     0,     0,   154,     0,     0,
       0,     0,   184,   255,   256,   154,   257,     0,   258,     0,
     259,     0,   784,   260,   159,     0,   158,   261,     0,   785,
     262,     0,     0,   263,     0,   143,     0,     0,   143,   786,
       0,     0,     0,     0,     0,   288,   787,     0,     0,    22,
      23,   159,     0,     0,     0,     0,   159,     0,   289,     0,
      30,   290,   788,     0,     0,     0,    36,     0,     0,     0,
     131,     0,     0,    41,   789,     0,     0,   131,   579,     0,
       0,     0,     0,     0,     0,   790,     0,   425,     0,   881,
       0,   791,   131,     0,     0,   143,     0,     0,    56,     0,
      58,   143,    60,     0,  1048,     0,     0,  1049,     0,   291,
     131,    66,     0,     0,     0,   158,     0,     0,     0,   131,
       0,   158,     0,   131,     0,     0,   159,   163,     0,     0,
      82,     0,     0,    84,     0,     0,    86,   163,     0,   154,
       0,     0,   154,     0,     0,     0,     0,     0,     0,     0,
     163,     0,   163,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,     0,     0,
       0,     0,   131,     0,     0,     0,     0,     0,     0,     0,
       0,   158,     0,   143,  -575,   143,   101,     0,     0,     0,
     143,     0,  1190,   143,     0,     0,   158,   410,     0,   154,
    -614,   163,  -614,     0,     0,   154,   143,     0,   143,     0,
       0,   143,     0,     0,     0,   159,     0,     0,     0,     0,
       0,   159,     0,     0,     0,     0,   164,     0,   163,   131,
     131,   131,   131,   163,     0,     0,   164,     0,     0,   143,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   164,
     143,   164,   143,     0,     0,     0,   131,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   288,     0,     0,
       0,    22,    23,   158,     0,     0,     0,     0,     0,     0,
     289,   159,    30,   290,     0,     0,     0,   154,    36,   154,
       0,     0,     0,     0,   154,    41,   159,   154,     0,     0,
     164,   158,     0,   163,     0,     0,     0,   158,     0,     0,
     154,     0,   154,     0,     0,   154,     0,     0,     0,   367,
      56,   158,    58,   158,    60,     0,  1048,   164,     0,  1049,
       0,   291,   164,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   154,     0,     0,   143,     0,     0,     0,
       0,     0,    82,   143,   154,    84,   154,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,   159,     0,   368,     0,     0,     0,   369,
       0,     0,     0,     0,   158,     0,   143,     0,   158,     0,
       0,     0,   163,     0,     0,   143,   158,     0,   163,   143,
       0,   159,   164,     0,     0,     0,     0,   159,   101,     0,
     370,     0,     0,     0,  1191,     0,     0,     0,     0,     0,
       0,   159,     0,   159,     0,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,     0,     0,   384,   385,   386,   387,   388,   143,   390,
     154,     0,   391,   392,   393,     0,     0,   154,   163,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,   154,   163,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   159,     0,     0,     0,   159,     0,
     154,   164,     0,     0,     0,     0,   159,   164,     0,   154,
       0,     0,     0,   154,     0,   143,   143,   143,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   109,     0,
     158,     0,     0,   158,     0,     0,     0,     0,   175,     0,
       0,     0,   143,   143,     0,     0,     0,     0,     0,     0,
       0,   209,     0,   212,     0,     0,     0,     0,     0,     0,
     163,     0,   154,     0,     0,     0,     0,   164,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   164,     0,     0,     0,     0,     0,   163,     0,
     158,     0,     0,     0,   163,     0,   158,     0,     0,     0,
       0,     0,   309,     0,     0,   367,     0,     0,   163,     0,
     163,     0,     0,     0,     0,     0,     0,     0,     0,   154,
     154,   154,   154,     0,     0,     0,     0,     0,     0,   109,
     159,     0,     0,   159,   347,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   154,   154,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   164,
       0,   368,     0,     0,     0,   369,   264,   266,   267,     0,
       0,   163,     0,     0,     0,   163,     0,     0,   158,     0,
     158,     0,     0,   163,     0,   158,     0,   164,   158,     0,
     159,     0,     0,   164,     0,     0,   159,     0,     0,     0,
       0,   158,     0,   158,   417,   319,   158,   164,     0,   164,
       0,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,     0,     0,   384,
     385,   386,   387,   388,   158,   390,     0,     0,   391,   392,
     393,     0,     0,     0,     0,   158,     0,   158,     0,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   354,     0,   355,     0,     0,     0,     0,
     164,     0,     0,     0,   164,     0,     0,     0,   159,     0,
     159,     0,   164,     0,     0,   159,     0,     0,   159,     0,
       0,     0,     0,   486,     0,     0,     0,     0,     0,   499,
     367,   159,     0,   159,     0,     0,   159,   163,     0,     0,
     163,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
       0,     0,     0,     0,   159,     0,     0,     0,     0,     0,
       0,   158,     0,     0,   319,   159,   450,   159,   158,     0,
       0,     0,     0,     0,     0,     0,   368,   471,   472,   109,
     369,     0,     0,   158,     0,     0,     0,   163,     0,     0,
       0,     0,     0,   163,   109,     0,     0,     0,     0,     0,
       0,   158,     0,     0,     0,     0,     0,     0,     0,     0,
     158,   370,   491,     0,   158,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   164,   371,   372,   164,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,     0,     0,   384,   385,   386,   387,   388,   389,
     390,     0,     0,   391,   392,   393,     0,     0,     0,     0,
       0,   159,     0,   158,   394,     0,     0,     0,   159,     0,
       0,   109,     0,     0,     0,   163,     0,   163,     0,     0,
       0,     0,   163,   159,     0,   163,   164,     0,     0,     0,
       0,     0,   164,     0,     0,     0,   555,     0,   163,   499,
     163,   159,     0,   163,     0,   499,     0,     0,     0,     0,
     159,     0,     0,     0,   159,     0,     0,     0,     0,   671,
     158,   158,   158,   158,     0,     0,     0,     0,     0,     0,
       0,   163,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   163,     0,   163,     0,     0,   158,   158,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   159,     0,     0,     0,   628,     0,     0,
       0,     0,     0,     0,   164,     0,   164,     0,     0,     0,
       0,   164,   731,     0,   164,   640,   736,     0,     0,   643,
       0,     0,     0,     0,   109,     0,     0,   164,     0,   164,
       0,     0,   164,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     159,   159,   159,   159,     0,     0,     0,   680,     0,     0,
     164,     0,     0,   491,     0,     0,     0,   240,   163,     0,
       0,   164,     0,   164,     0,   163,     0,   159,   159,     0,
       0,   241,   242,     0,   243,     0,     0,     0,     0,   244,
     163,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,   246,     0,     0,     0,     0,   163,   247,
       0,     0,     0,   248,     0,     0,   249,   163,     0,     0,
       0,   163,   744,     0,     0,     0,   250,     0,     0,     0,
       0,   759,   251,   252,     0,     0,     0,     0,     0,     0,
     253,     0,     0,     0,     0,     0,     0,     0,   844,   254,
       0,   847,     0,     0,     0,     0,     0,     0,   255,   256,
       0,   257,     0,   258,     0,   259,     0,   164,   260,     0,
     163,     0,   261,     0,   164,   262,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   164,
       0,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,     0,   164,   886,     0,
       0,     0,   811,     0,   891,   814,   164,     0,     0,     0,
     164,     0,     0,     0,     0,     0,     0,   163,   163,   163,
     163,     0,     0,     0,   269,   410,     0,     0,  -614,     0,
    -614,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   163,   163,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   860,     0,     0,   164,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   883,     0,     0,   499,   319,   499,     0,
       0,   240,     0,   499,     0,     0,   499,     0,     0,     0,
       0,     0,     0,     0,     0,   241,   242,     0,   243,  1001,
       0,  1002,     0,   244,  1004,     0,   164,   164,   164,   164,
       0,   245,     0,     0,     0,     0,     0,   246,     0,     0,
     955,     0,  -287,   247,     0,     0,     0,   248,     0,     0,
     249,     0,  1024,   164,   164,     0,  -287,  -287,     0,  -287,
     250,     0,     0,  1030,  -287,  1034,   251,   252,     0,     0,
       0,     0,  -287,     0,   253,     0,     0,     0,  -287,     0,
       0,     0,     0,   254,  -287,     0,     0,     0,  -287,     0,
       0,  -287,   255,   256,     0,   257,     0,   258,     0,   259,
       0,  -287,   260,     0,     0,     0,   261,  -287,  -287,   262,
       0,     0,   263,     0,     0,  -287,     0,     0,     0,     0,
       0,     0,     0,  1006,  -287,   680,     0,     0,     0,  1010,
       0,     0,   491,  -287,  -287,     0,  -287,     0,  -287,     0,
    -287,     0,     0,  -287,     0,     0,     0,  -287,     0,     0,
    -287,     0,     0,  -287,   883,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   859,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   759,
       0,  1086,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1106,
       0,     0,     0,     0,     0,     0,     0,     0,  1111,     0,
      -2,     4,  1113,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,    38,   -76,     0,    39,    40,
      41,  1147,    42,  -306,     0,    43,    44,    45,    46,    47,
       0,    48,    49,    50,   -47,    51,    52,     0,    53,    54,
      55,     0,  -306,     0,     0,    56,    57,    58,    59,    60,
      61,    62,  -306,   -47,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,    77,    78,     0,    79,    80,    81,    82,    83,     0,
      84,    85,   -76,    86,    87,     0,     0,    88,     0,    89,
       0,     0,    90,   955,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,   101,     0,     0,   102,     0,   103,     0,
     104,     0,   105,     0,     4,   106,     5,     0,     6,     7,
       8,     9,    10,    11,     0,  -644,     0,    12,    13,    14,
      15,    16,  -644,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,  -644,    27,    28,  -644,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,   -76,
       0,    39,    40,    41,     0,    42,  -306,     0,    43,    44,
      45,    46,    47,     0,    48,    49,    50,   -47,    51,    52,
       0,    53,    54,    55,     0,  -306,     0,     0,    56,    57,
      58,    59,     0,    61,    62,  -306,   -47,    63,    64,    65,
    -644,    66,    67,    68,  -644,    69,    70,    71,    72,    73,
      74,    75,    76,     0,    77,    78,     0,    79,    80,    81,
      82,    83,     0,    84,    85,   -76,    86,    87,     0,     0,
      88,     0,    89,     0,     0,  -644,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -644,  -644,    93,  -644,  -644,  -644,  -644,  -644,  -644,
    -644,     0,  -644,  -644,  -644,  -644,  -644,     0,  -644,  -644,
    -644,  -644,  -644,  -644,  -644,  -644,   101,  -644,  -644,  -644,
       0,   103,  -644,   104,     0,   105,     0,   330,  -644,     5,
     293,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,    38,   -76,     0,    39,    40,    41,     0,    42,  -306,
       0,    43,    44,    45,    46,    47,     0,    48,    49,    50,
     -47,    51,    52,     0,    53,    54,    55,     0,  -306,     0,
       0,    56,    57,    58,    59,    60,    61,    62,  -306,   -47,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,     0,    84,    85,   -76,    86,
      87,     0,     0,    88,     0,    89,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,   101,
       0,     0,   102,     0,   103,   331,   104,     0,   105,     0,
       4,   106,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,   -76,     0,    39,    40,    41,
       0,    42,  -306,     0,    43,    44,    45,    46,    47,     0,
      48,    49,    50,   -47,    51,    52,     0,    53,    54,    55,
       0,  -306,     0,     0,    56,    57,    58,    59,    60,    61,
      62,  -306,   -47,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,    76,     0,
      77,    78,     0,    79,    80,    81,    82,    83,     0,    84,
      85,   -76,    86,    87,     0,     0,    88,     0,    89,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,   101,     0,     0,   102,     0,   103,   527,   104,
       0,   105,     0,   546,   106,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,    38,   -76,     0,
      39,    40,    41,     0,    42,  -306,     0,    43,    44,    45,
      46,    47,     0,    48,    49,    50,   -47,    51,    52,     0,
      53,    54,    55,     0,  -306,     0,     0,    56,    57,    58,
      59,    60,    61,    62,  -306,   -47,    63,    64,    65,     0,
      66,    67,    68,     0,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,     0,    84,    85,   -76,    86,    87,     0,     0,    88,
       0,    89,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,   101,     0,     0,   102,     0,
     103,   547,   104,     0,   105,     0,   330,   106,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,   -76,     0,    39,    40,    41,     0,    42,  -306,     0,
      43,    44,    45,    46,    47,     0,    48,    49,    50,   -47,
      51,    52,     0,    53,    54,    55,     0,  -306,     0,     0,
      56,    57,    58,    59,    60,    61,    62,  -306,   -47,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,    76,     0,    77,    78,     0,    79,
      80,    81,    82,    83,     0,    84,    85,   -76,    86,    87,
       0,     0,    88,     0,    89,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,   101,     0,
       0,   102,     0,   103,   331,   104,     0,   105,     0,     4,
     106,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,   -76,     0,    39,    40,    41,     0,
      42,  -306,     0,    43,    44,    45,    46,    47,     0,    48,
      49,    50,   -47,    51,    52,     0,    53,    54,    55,     0,
    -306,     0,     0,    56,    57,    58,    59,    60,    61,    62,
    -306,   -47,    63,    64,    65,     0,    66,    67,    68,     0,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,    80,    81,    82,    83,     0,    84,    85,
     -76,    86,    87,     0,     0,    88,     0,    89,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,   101,     0,     0,   102,     0,   103,   743,   104,     0,
     105,     0,     4,   106,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,    38,   -76,     0,    39,
      40,    41,     0,    42,  -306,     0,    43,    44,    45,    46,
      47,     0,    48,    49,    50,   -47,    51,    52,     0,    53,
      54,    55,     0,  -306,     0,     0,    56,    57,    58,    59,
     346,    61,    62,  -306,   -47,    63,    64,    65,     0,    66,
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
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
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
      36,   648,    38,   -76,     0,    39,    40,    41,     0,    42,
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
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,   654,    38,   -76,     0,    39,    40,
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
      30,    31,    32,    33,    34,    35,    36,   984,    38,   -76,
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
     986,    38,   -76,     0,    39,    40,    41,     0,    42,  -306,
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
       4,   106,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,   991,    38,   -76,     0,    39,    40,    41,
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
      31,    32,    33,    34,    35,    36,   994,    38,   -76,     0,
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
      21,    22,    23,    24,    25,    26,     0,  1023,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
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
      35,    36,  1029,    38,   -76,     0,    39,    40,    41,     0,
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
      32,    33,    34,    35,    36,  1033,    38,   -76,     0,    39,
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
       0,   104,     0,   105,     0,  1219,   106,     5,   293,     6,
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
     102,     0,   191,     0,   104,     0,   192,  1220,   869,   106,
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
       0,     5,   106,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,   231,    23,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,    58,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,    66,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,    86,    87,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,     0,     0,     0,   102,     0,   191,     0,   104,     0,
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
     190,     0,    77,     0,     0,    79,   297,   298,     0,    83,
     335,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,   336,
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
      73,    74,    75,   190,     0,    77,     0,     0,    79,   297,
     298,     0,    83,   335,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,     0,   104,   727,   192,     0,     0,   106,
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
       0,    79,   297,   298,     0,    83,   335,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,   729,   192,
       0,     5,   106,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,   611,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,   612,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,   613,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,   614,    87,     0,     0,     0,     0,     0,     0,     0,
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
      77,     0,     0,    79,   297,   298,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,   299,     0,     0,   102,     0,   191,     0,   104,
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
     871,    53,    54,     0,     0,     0,     0,     0,     0,    57,
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
       7,     8,   218,    10,    11,   219,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     220,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,     0,
       0,   221,    83,     0,     0,    85,     0,     0,    87,     0,
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
       0,    57,   223,    59,     0,    61,     0,     0,     0,     0,
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
       0,    21,     0,   234,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,     0,    85,     0,   235,
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
      85,     0,     0,    87,     0,     0,     0,     0,     0,   224,
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
      98,    99,   100,     0,     0,     0,   102,     0,   191,     0,
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
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,   725,   104,     0,   192,     0,     0,   106,     5,
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
      79,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   102,     0,   191,     0,   104,     0,   192,   735,
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
       0,   192,  1151,     5,   106,     6,     7,     8,     9,    10,
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
       0,     0,     0,   273,    28,     0,   184,     0,     0,     0,
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
     102,     0,   191,     0,   104,     0,   192,     0,     5,   106,
       6,     7,     8,   218,    10,    11,     0,     0,     0,   182,
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
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   102,     0,   191,     0,   104,     0,   192,   740,
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
       0,    79,     0,     0,     0,    83,   897,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,   192,
       0,     0,   106,     5,   293,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   699,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,   700,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   189,     0,     0,     0,
       0,     0,     0,     0,    71,   701,    73,    74,    75,   702,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,     0,     0,     0,   102,     0,   191,     0,
     104,     0,  1090,     0,     5,   106,     6,     7,     8,     9,
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
    1115,     0,    85,     0,     0,    87,     0,     0,     0,     0,
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
     102,     0,   191,     0,   104,     0,  1090,     0,     0,   106,
       5,   293,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,  1071,    38,     0,     0,     0,    40,     0,     0,    42,
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
     192,     0,     5,   106,     6,     7,     8,   214,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   215,     0,     0,     0,     0,
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
      98,    99,   100,     0,     0,     0,   102,     0,   103,     0,
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
       0,    98,    99,   100,     0,     0,     0,   663,     0,   191,
       0,   104,     0,   192,     0,     5,   106,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,   699,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,   700,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,     0,    57,     0,
      59,     0,    61,     0,     0,     0,     0,    64,   189,     0,
       0,     0,     0,     0,     0,     0,    71,   701,    73,    74,
      75,   702,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,     0,     0,     0,   102,     0,
     191,     0,   104,     0,   703,     0,     5,   106,     6,     7,
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
       0,   191,     0,   104,     0,   703,     0,     5,   106,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   795,    38,
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
     184,     0,     0,     0,    32,   185,   186,     0,     0,   798,
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
    1065,    38,     0,     0,     0,    40,     0,     0,    42,     0,
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
       0,  1066,    38,     0,     0,     0,    40,     0,     0,    42,
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
       0,     0,  1068,    38,     0,     0,     0,    40,     0,     0,
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
     186,     0,     0,  1069,    38,     0,     0,     0,    40,     0,
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
     185,   186,     0,     0,  1070,    38,     0,     0,     0,    40,
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
      32,   185,   186,     0,     0,  1071,    38,     0,     0,     0,
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
       0,     0,     0,     0,     0,   699,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,   700,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,     0,    57,     0,
      59,     0,    61,     0,     0,     0,     0,    64,   189,     0,
       0,     0,     0,     0,     0,     0,    71,   701,    73,    74,
      75,   702,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,     0,     0,     0,   102,     0,
     191,     0,   104,     0,  1175,     0,     5,   106,     6,     7,
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
       0,     0,     0,     0,     0,     0,     0,     0,   910,     0,
     911,    91,    92,    93,    94,    95,     0,     0,     0,   912,
      96,     0,     0,     0,   913,   242,   914,   915,     0,     0,
      97,     0,   916,    98,    99,   100,     0,     0,     0,   102,
     245,     0,   184,   104,     0,  1175,   917,     0,   106,     0,
       0,     0,   918,     0,     0,     0,   248,     0,     0,   919,
       0,   920,     0,     0,     0,     0,     0,     0,     0,   921,
       0,     0,     0,     0,     0,   922,   923,     0,     0,     0,
       0,     0,     0,   253,     0,     0,     0,     0,     0,     0,
       0,     0,   924,     0,     0,     0,     0,     0,     0,     0,
       0,   255,   256,     0,   925,     0,   258,     0,   926,     0,
       0,   927,     0,     0,     0,   928,     0,     0,   262,     0,
       0,   929,     0,     0,     0,     0,     0,     0,     0,     0,
     476,   396,   397,   398,   399,   400,     0,     0,   403,   404,
     405,   406,     0,   408,   409,   930,   931,   932,   933,   934,
     935,     0,   936,     0,     0,     0,   937,   938,   939,   940,
     941,   942,   943,   944,   945,   946,   947,     0,   948,     0,
       0,   949,   950,   951,   952,     0,     5,   953,     6,     7,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,     0,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     5,   293,     6,     7,
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
       0,     0,     0,     0,     0,     0,  -472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,  -472,     0,     0,
       0,  -472,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,     0,     0,
       0,   191,     0,   104,  -472,  1090,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,  -454,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,  -454,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,  -454,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,  -454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,    93,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,   100,  -454,     0,     0,     0,
       0,  -454,    28,   104,   184,  1175,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,     5,    93,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,   100,     0,     0,     0,     0,     0,   191,    28,   104,
     184,   192,     0,     0,    32,   185,   186,     0,     0,   187,
      38,     0,     0,     0,    40,     0,     0,    42,     0,     0,
     188,     0,     0,    46,    47,     0,     0,    49,    50,     0,
      51,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,    93,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,   100,     0,     0,
       0,     0,     0,   191,    28,   104,   184,   703,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,  1128,    79,     0,     0,     0,    83,
       0,     0,    85,   912,     0,    87,     0,     0,   241,   242,
     914,   243,     0,     0,     0,     0,   244,     0,     0,     0,
       0,     0,     0,     0,   245,     0,     0,     0,     0,     0,
     917,    93,     0,     0,     0,     0,   247,     0,     0,     0,
     248,     0,     0,   249,     0,   920,     0,     0,     0,     0,
       0,     0,     0,   250,     0,     0,     0,     0,     0,   922,
     252,   104,     0,   192,     0,     0,     0,   253,     0,     0,
       0,     0,     0,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,     0,   365,   255,   256,     0,   257,     0,
     258,   366,  1129,     0,     0,   927,     0,     0,     0,   261,
       0,     0,   262,   367,     0,   263,     0,     0,     0,     0,
       0,     0,     0,     0,   476,   396,   397,   398,   399,   400,
       0,     0,   403,   404,   405,   406,     0,   408,   409,   930,
     931,   932,   933,   934,   935,     0,   936,     0,     0,     0,
     937,   938,   939,   940,   941,   942,   943,   944,   945,   946,
     947,     0,   948,     0,     0,   949,   950,   951,   952,   368,
     365,     0,     0,   369,     0,     0,     0,   366,     0,     0,
       0,     0,   475,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,   476,   396,   397,   398,   399,
     400,     0,     0,   403,   404,   405,   406,     0,   408,   409,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,   368,   391,   392,   393,   369,
     425,     0,   365,     0,     0,     0,     0,   394,   983,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,     0,     0,     0,     0,
     370,   476,   396,   397,   398,   399,   400,     0,     0,   403,
     404,   405,   406,     0,   408,   409,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,     0,   391,   392,   393,     0,   425,   368,   365,     0,
       0,   369,     0,   394,     0,   366,     0,     0,     0,     0,
     990,     0,     0,     0,     0,     0,     0,   367,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,   476,   396,   397,   398,   399,   400,     0,
       0,   403,   404,   405,   406,     0,   408,   409,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,     0,   368,   391,   392,   393,   369,   425,     0,
     365,     0,     0,     0,     0,   394,  1142,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,     0,     0,     0,   370,   476,
     396,   397,   398,   399,   400,     0,     0,   403,   404,   405,
     406,     0,   408,   409,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,     0,
     391,   392,   393,     0,   425,   368,   365,     0,     0,   369,
       0,   394,     0,   366,     0,     0,     0,     0,  1143,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     370,   476,   396,   397,   398,   399,   400,     0,     0,   403,
     404,   405,   406,     0,   408,   409,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,   368,   391,   392,   393,   369,   425,     0,   365,     0,
       0,     0,     0,   394,  1144,   366,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,     0,     0,
       0,     0,     0,     0,     0,     0,   370,   476,   396,   397,
     398,   399,   400,     0,     0,   403,   404,   405,   406,     0,
     408,   409,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,   425,   368,   365,     0,     0,   369,     0,   394,
       0,   366,     0,     0,     0,     0,  1145,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   370,   476,
     396,   397,   398,   399,   400,     0,     0,   403,   404,   405,
     406,     0,   408,   409,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   368,
     391,   392,   393,   369,   425,     0,   365,     0,     0,     0,
       0,   394,  1169,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,   370,   476,   396,   397,   398,   399,
     400,     0,     0,   403,   404,   405,   406,     0,   408,   409,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,     0,   391,   392,   393,     0,
     425,   368,     0,     0,     0,   369,     0,   394,   365,     0,
       0,     0,     0,     0,  1170,   366,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,   459,     0,
       0,     0,     0,     0,     0,     0,   370,   476,   396,   397,
     398,   399,   400,   460,     0,   403,   404,   405,   406,     0,
     408,   409,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,   425,   368,   365,     0,     0,   369,     0,   394,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   456,     0,     0,     0,   370,   465,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   368,
     391,   392,   393,   369,   425,     0,   365,     0,     0,     0,
       0,   394,     0,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,   453,     0,     0,     0,
     456,     0,     0,     0,   370,     0,     0,     0,     0,     0,
       0,   454,     0,     0,     0,     0,     0,     0,     0,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,     0,   391,   392,   393,     0,
     425,   368,     0,     0,     0,   369,     0,   394,     0,   365,
       0,     0,     0,     0,     0,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,   468,     0,     0,   370,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,   469,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,   425,     0,   368,     0,     0,     0,   369,   394,
       0,     0,   365,   751,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,   752,     0,     0,   370,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   753,   384,   385,   386,   387,   388,   389,   390,     0,
     470,   391,   392,   393,     0,     0,     0,   368,   365,     0,
       0,   369,   394,     0,     0,   366,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,   830,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,     0,   368,   391,   392,   393,   369,     0,     0,
     365,     0,     0,     0,     0,   394,     0,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
     273,     0,     0,     0,   456,     0,     0,     0,   370,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,     0,
     391,   392,   393,     0,   425,   368,   365,     0,     0,   369,
       0,   394,     0,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   456,     0,     0,     0,
     370,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,   368,   391,   392,   393,   369,   425,   365,     0,     0,
       0,     0,     0,   394,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,   273,     0,     0,
       0,     0,   177,     0,     0,     0,   370,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,   368,   365,     0,     0,   369,     0,     0,   394,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,   620,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   370,   621,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   368,   391,
     392,   393,   369,   425,   365,     0,     0,     0,     0,     0,
     394,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,   622,     0,     0,     0,     0,     0,
       0,     0,     0,   370,     0,     0,     0,     0,     0,   623,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,     0,   391,   392,   393,     0,   368,
     365,     0,     0,   369,     0,     0,   394,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
     822,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,   368,   391,   392,   393,   369,
       0,     0,   365,     0,     0,     0,     0,   394,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,     0,     0,     0,     0,
     370,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,     0,   391,   392,   393,     0,   425,   368,   365,     0,
       0,   369,     0,   394,     0,   366,     0,     0,     0,     0,
     848,     0,     0,     0,     0,     0,     0,   367,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,     0,   368,   391,   392,   393,   369,   425,     0,
       0,   365,     0,     0,     0,   394,     0,     0,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     367,     0,     0,     0,   456,  1042,     0,     0,   370,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
    1043,   384,   385,   386,   387,   388,   389,   390,     0,     0,
     391,   392,   393,     0,     0,     0,   368,   365,     0,   889,
     369,   394,     0,     0,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   370,     0,     0,     0,     0,     0,     0,   365,     0,
       0,     0,     0,     0,     0,   366,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,   367,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,     0,   368,   391,   392,   393,   369,     0,     0,     0,
       0,     0,     0,   365,   394,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,   370,     0,     0,
       0,     0,     0,   368,     0,     0,     0,   369,     0,     0,
       0,     0,     0,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   370,   391,
     392,   393,     0,   511,     0,     0,     0,     0,   368,     0,
     394,     0,   369,     0,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,   365,   515,
     391,   392,   393,   370,     0,   366,     0,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,   367,     0,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,   365,   517,   391,   392,   393,     0,     0,
     366,     0,     0,     0,     0,     0,   394,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   368,     0,     0,     0,   369,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,     0,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,   370,     0,
       0,   367,     0,     0,     0,     0,     0,     0,   368,     0,
       0,     0,   369,     0,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   519,
     391,   392,   393,   370,     0,     0,     0,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,   368,     0,   371,
     372,   369,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,   365,   525,   391,   392,   393,     0,     0,
     366,   542,   370,     0,     0,     0,   394,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,   365,     0,   391,   392,   393,     0,     0,   366,
       0,     0,     0,     0,     0,   394,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,   624,   365,   747,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,   370,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   368,     0,   371,
     372,   369,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,     0,   391,   392,   393,     0,     0,
       0,     0,   370,     0,     0,     0,   394,   368,     0,     0,
       0,   369,     0,     0,     0,     0,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,   370,     0,   391,   392,   393,     0,     0,   365,
       0,   726,     0,     0,     0,   394,   366,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   748,   378,   367,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,     0,     0,   391,   392,   393,     0,     0,     0,
       0,     0,     0,     0,   365,   394,     0,     0,     0,     0,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   368,     0,     0,     0,   369,     0,
       0,     0,     0,     0,   365,     0,     0,     0,     0,     0,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,     0,   370,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   368,
       0,     0,     0,   369,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,  -576,   384,   385,   386,   387,   388,   389,   390,     0,
     177,   391,   392,   393,   370,     0,     0,     0,     0,   368,
       0,     0,   394,   369,     0,     0,     0,     0,     0,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,   370,     0,   391,   392,   393,     0,
       0,   365,   839,     0,     0,     0,     0,   394,   366,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
     367,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,   807,   391,   392,   393,     0,
       0,   365,     0,     0,     0,     0,     0,   394,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     367,     0,     0,   846,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   368,     0,     0,     0,
     369,   365,     0,     0,     0,     0,     0,     0,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     367,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   370,     0,     0,     0,     0,   368,     0,     0,     0,
     369,     0,     0,     0,     0,     0,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,   370,     0,   391,   392,   393,   368,     0,     0,     0,
     369,     0,     0,     0,   394,     0,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,   370,     0,   391,   392,   393,     0,     0,   365,     0,
       0,     0,     0,     0,   394,   366,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,   367,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,     0,     0,   391,   392,   393,     0,     0,     0,   365,
     873,     0,     0,     0,   394,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   368,     0,     0,     0,   369,     0,   365,
       0,     0,     0,     0,     0,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,   968,
       0,     0,     0,     0,     0,     0,     0,     0,   370,     0,
       0,     0,     0,     0,   368,     0,     0,     0,   369,     0,
       0,     0,     0,     0,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,   892,   370,
     391,   392,   393,     0,   368,     0,     0,   874,   369,     0,
       0,   394,     0,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,   370,
       0,   391,   392,   393,     0,     0,     0,     0,     0,     0,
       0,     0,   394,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,   365,
     288,   391,   392,   393,    22,    23,   366,     0,     0,     0,
       0,     0,   394,   289,     0,    30,   290,     0,   367,   971,
       0,    36,     0,     0,   -77,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   365,
       0,     0,   -48,     0,     0,     0,   366,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,     0,   367,     0,
       0,   -48,     0,     0,   291,     0,    66,     0,     0,     0,
       0,     0,     0,     0,   368,     0,     0,     0,   369,   365,
    1039,     0,     0,     0,     0,    82,   366,     0,    84,     0,
     -77,    86,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   370,
       0,     0,     0,     0,   368,     0,     0,     0,   369,     0,
       0,     0,     0,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,   370,
       0,   391,   392,   393,   368,     0,     0,     0,   369,     0,
       0,     0,   394,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,   370,
       0,   391,   392,   393,     0,     0,   365,     0,     0,     0,
     974,     0,   394,   366,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,   367,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,     0,
       0,   391,   392,   393,     0,     0,   365,     0,     0,     0,
       0,     0,   394,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   368,     0,     0,     0,   369,   365,  1084,     0,     0,
       0,     0,     0,   366,  1064,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   370,     0,     0,     0,
       0,   368,     0,     0,     0,   369,     0,     0,     0,     0,
       0,     0,   371,   372,  1067,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,   370,     0,   391,   392,
     393,   368,     0,     0,     0,   369,     0,     0,     0,   394,
       0,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,   370,     0,   391,   392,
     393,     0,     0,   365,     0,     0,     0,     0,     0,   394,
     366,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,   367,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,     0,   365,     0,     0,     0,     0,     0,   394,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   370,     0,     0,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,   370,     0,   391,   392,   393,     0,     0,
     365,     0,  1127,     0,     0,     0,   394,   366,     0,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,   367,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,     0,   391,   392,   393,     0,     0,
     365,     0,     0,     0,  1165,     0,   394,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   368,     0,     0,     0,   369,
     365,     0,     0,     0,     0,     0,     0,   366,  1184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     370,     0,     0,     0,     0,   368,     0,     0,     0,   369,
       0,     0,     0,     0,     0,     0,   371,   372,  1185,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
     370,     0,   391,   392,   393,   368,     0,     0,     0,   369,
       0,     0,     0,   394,     0,     0,   371,   372,  1186,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
     370,     0,   391,   392,   393,     0,     0,   365,     0,     0,
       0,     0,     0,   394,   366,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,   367,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,     0,   391,   392,   393,     0,     0,   365,     0,     0,
       0,     0,     0,   394,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   368,     0,     0,     0,   369,   365,     0,     0,
       0,     0,     0,     0,   366,  1187,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   370,     0,     0,
       0,     0,   368,     0,     0,     0,   369,     0,     0,     0,
       0,     0,     0,   371,   372,  1188,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,   370,     0,   391,
     392,   393,   368,     0,     0,     0,   369,     0,     0,     0,
     394,     0,     0,   371,   372,  1189,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,   370,     0,   391,
     392,   393,     0,     0,   365,     0,     0,     0,     0,     0,
     394,   366,     0,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,   367,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,     0,   391,
     392,   393,     0,     0,   365,     0,     0,     0,     0,     0,
     394,   366,  1213,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,     0,     0,
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
       0,   365,     0,     0,     0,  1242,     0,   394,   366,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
     367,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,     0,   391,   392,   393,     0,
       0,     0,     0,     0,     0,     0,     0,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   793,     0,     0,     0,
     369,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   370,     0,     0,   368,     0,     0,     0,   369,     0,
       0,     0,     0,     0,     0,     0,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,     0,     0,   391,   392,   393,     0,     0,     0,     0,
       0,     0,     0,     0,   394,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,     0,   380,   381,   382,
       0,     0,   384,   385,   386,   387,     0,   758,   390,   240,
       0,   391,   392,   393,     0,     0,     0,     0,     0,     0,
       0,     0,   394,   241,   242,     0,   243,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,   245,
       0,     0,     0,     0,     0,   246,     0,     0,     0,     0,
    -297,   247,     0,     0,     0,   248,     0,     0,   249,     0,
       0,     0,     0,     0,  -297,  -297,     0,  -297,   250,     0,
       0,     0,  -297,     0,   251,   252,     0,     0,     0,     0,
    -297,     0,   253,     0,     0,     0,  -297,     0,     0,     0,
       0,   254,  -297,     0,     0,     0,  -297,     0,     0,  -297,
     255,   256,     0,   257,     0,   258,     0,   259,     0,  -297,
     260,     0,     0,     0,   261,  -297,  -297,   262,     0,     0,
     263,     0,     0,  -297,     0,     0,     0,     0,     0,     0,
       0,     0,  -297,     0,     0,   240,     0,     0,     0,     0,
       0,  -297,  -297,     0,  -297,     0,  -297,     0,  -297,   241,
     242,  -297,   243,     0,     0,  -297,     0,   244,  -297,     0,
       0,  -297,     0,     0,     0,   245,     0,     0,     0,     0,
       0,   246,     0,     0,     0,     0,  -288,   247,     0,     0,
       0,   248,     0,     0,   249,     0,     0,     0,     0,     0,
    -288,  -288,     0,  -288,   250,     0,     0,     0,  -288,     0,
     251,   252,     0,     0,     0,     0,  -288,     0,   253,     0,
       0,     0,  -288,     0,     0,     0,     0,   254,  -288,     0,
       0,     0,  -288,     0,     0,  -288,   255,   256,     0,   257,
       0,   258,     0,   259,     0,  -288,   260,     0,     0,     0,
     261,  -288,  -288,   262,     0,     0,   263,     0,     0,  -288,
       0,     0,     0,     0,     0,     0,     0,     0,  -288,     0,
       0,  -174,     0,     0,     0,     0,     0,  -288,  -288,     0,
    -288,     0,  -288,     0,  -288,  -174,  -174,  -288,  -174,     0,
       0,  -288,     0,  -174,  -288,     0,     0,  -288,     0,     0,
       0,  -174,     0,     0,     0,     0,     0,  -174,     0,     0,
       0,     0,  -170,  -174,     0,     0,     0,  -174,     0,     0,
    -174,     0,     0,     0,     0,     0,  -170,  -170,     0,  -170,
    -174,     0,     0,     0,  -170,     0,  -174,  -174,     0,     0,
       0,     0,  -170,     0,  -174,     0,     0,     0,  -170,     0,
       0,     0,     0,  -174,  -170,     0,     0,     0,  -170,     0,
       0,  -170,  -174,  -174,     0,  -174,     0,  -174,     0,  -174,
       0,  -170,  -174,     0,     0,     0,  -174,  -170,  -170,  -174,
       0,     0,  -174,     0,     0,  -170,     0,     0,     0,     0,
       0,     0,     0,     0,  -170,     0,     0,     0,     0,     0,
       0,     0,     0,  -170,  -170,     0,  -170,     0,  -170,     0,
    -170,     0,     0,  -170,     0,     0,     0,  -170,     0,     0,
    -170,     0,     0,  -170
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1103)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,    63,    67,   469,    16,     2,   520,   652,    20,   767,
     161,    33,    34,    35,    26,   205,   490,   487,    30,    31,
     239,    33,    34,    35,    36,    37,   753,   752,   416,   483,
     868,   221,   640,    19,    22,   643,    48,   495,    50,     1,
     105,   698,   903,    55,     1,    57,    27,    59,   213,   958,
     959,     1,    32,    32,     3,    67,    68,    69,    70,    71,
      72,  1013,   520,    32,    76,  1100,    78,   286,    80,    81,
      47,    32,    47,    85,   239,  1088,    88,    89,    32,    91,
      67,    93,    90,   105,    96,    97,    27,    99,    47,   308,
    1151,   103,   104,   105,   106,     1,    77,   144,     1,    90,
     144,  1203,     3,    60,   144,    47,   138,    88,   118,   147,
      47,    55,    68,  1010,   146,    36,   103,   104,   105,    47,
      47,   286,   169,    73,    55,   963,   107,   192,   170,   173,
     172,   171,     3,    90,   779,   792,    77,  1150,   118,   118,
    1242,   118,  1203,   308,    47,   157,     3,    88,   104,    20,
      71,    88,   101,    73,    60,   163,   228,   229,   230,   116,
     168,   123,    33,   173,   315,   115,   107,   144,   206,   144,
     182,   625,  1124,   185,   186,   187,  1014,   168,   190,   191,
     192,   143,   144,   144,    90,   144,   143,  1222,   168,   168,
    1051,    62,  1089,   412,   413,   115,   173,   103,   173,   168,
     101,   213,   144,   811,   191,   192,   814,   168,   865,   171,
     116,   168,   377,  1074,   168,  1076,   144,   144,   204,   169,
     123,  1130,  1131,   697,    60,   287,   170,   239,    60,   217,
     101,   173,   100,    90,   222,    18,   107,   143,    46,   170,
     143,   144,    47,   231,   101,   173,   173,   412,   413,   169,
    1088,   123,   163,   163,    90,    46,    64,   163,    27,   170,
     163,   116,   168,    53,   652,   118,   144,   103,   144,   341,
     342,   118,   144,   123,   286,    83,   148,  1138,   290,  1140,
     116,    50,    72,    88,   116,   297,   298,   753,   144,   144,
       3,   169,    82,    76,   513,   171,   308,   168,   148,   171,
     168,   755,  1060,   144,   112,   123,    75,    20,    77,     0,
     144,    94,  1150,   169,  1041,  1040,  1043,  1042,   483,    88,
      33,   112,   144,   979,   336,   981,   173,   163,   111,   163,
     171,   149,   168,   151,   152,    55,   333,   155,   156,   351,
     352,   163,    81,   988,   113,    84,   368,   369,   513,    62,
     804,   363,     3,   365,   366,   367,   368,   369,   370,    55,
     372,   373,   374,   375,   376,    31,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   144,    55,   859,   451,   857,   101,   401,
     402,   779,   414,   415,   107,   407,   144,   144,   410,   411,
     412,   413,   414,   415,   416,   427,   444,   144,   144,   144,
     171,   449,   144,    88,   170,   427,   172,  1073,   144,  1075,
     432,    87,   434,   171,   171,    91,  1220,    55,   170,   895,
     172,   163,   454,   903,   171,   171,   171,   163,   460,   451,
      60,   453,   454,   465,  1238,   401,   402,   459,   460,   144,
     144,   407,   170,   465,   410,   411,   468,   469,   163,   144,
     625,   123,   144,   475,   451,   170,   478,   172,   144,   163,
      90,   483,   138,   638,   144,    27,   171,   143,   163,   145,
     146,   163,   144,   103,   150,   497,   148,   144,    27,   501,
     502,   556,   158,   163,   506,   171,   116,   144,   164,   165,
     166,   513,   144,   138,   144,   138,  1020,    55,    47,   171,
      31,   146,    51,   146,   171,   163,   538,   529,    55,   506,
     532,   169,   534,   163,   171,    77,   538,   539,   540,   171,
     542,   543,    48,   163,   724,    74,    88,    55,    77,   169,
      79,  1156,   529,   163,   556,   532,    87,   534,   168,    88,
      91,   548,   539,   217,    70,  1013,    87,   170,   222,   172,
      91,   113,  1020,   170,    80,    81,    87,    31,   107,   556,
      91,   168,   170,   170,   172,  1041,   532,  1043,   534,   170,
      55,   172,   747,   748,   656,   657,  1056,    55,   660,   661,
     755,     8,  1207,  1208,  1209,  1210,  1211,  1212,    55,   621,
     988,   623,    80,    81,  1074,   144,  1076,    47,   620,   621,
     622,   623,   624,   625,   626,   627,   767,   138,    55,   532,
      60,   534,   143,    87,   636,   146,   638,    91,   170,   150,
      55,    98,   171,    55,   646,   170,   648,    55,   703,   804,
     652,    60,   654,   629,   630,    55,    55,   138,   168,   636,
      90,    83,   143,    55,   163,   146,   162,   669,   670,   150,
     163,   733,   734,   163,   154,   163,  1124,   163,  1138,   163,
    1140,    90,    55,   170,   138,   139,   116,   141,   142,   143,
     636,   145,   146,   123,   103,   168,   150,   203,   117,   701,
     702,   703,   163,  1151,   158,   707,    86,   116,   163,    86,
     164,   165,   166,   163,   144,   163,     8,  1165,   170,   225,
     226,   227,   228,   229,   230,    27,   703,    31,   170,   170,
     236,   237,   170,   163,   170,   170,   123,   163,   168,    64,
     143,   171,   146,   745,    31,   168,    67,   749,   123,   751,
     752,   753,   170,   755,   163,  1203,   144,   163,   171,   168,
     266,   707,   163,    47,   123,   170,   123,    47,    34,    34,
     276,   123,  1220,   171,   144,    77,   282,   779,   171,    21,
      60,   793,   794,    87,   144,   171,    88,    91,   171,   173,
    1238,   793,   794,   795,  1242,   173,   798,   168,   304,   163,
      87,   170,   804,   171,    91,   107,   173,   163,   163,   163,
      90,   113,  1021,  1022,   163,   163,   322,   323,   820,   170,
     822,   163,   877,   163,   144,   827,   163,    34,   830,    34,
       2,    64,   171,   169,   138,   837,   116,   839,   171,   143,
      12,   145,   146,   123,   846,    27,   150,   144,   123,    88,
     169,   138,   139,    25,   158,    27,   143,   171,   145,   146,
     173,    32,   166,   150,   144,    47,  1021,  1022,    32,    51,
     168,   158,   168,    34,   163,   877,   143,   164,   165,   166,
      88,   143,   173,   163,   170,   887,   169,   889,   168,   170,
     892,   171,    74,   895,    27,    77,   173,    79,   170,   173,
     877,     2,   163,   905,    76,   163,    88,   138,   146,   146,
     146,    12,   146,   138,    47,   168,   146,   146,    51,  1060,
     146,   138,   428,   146,    25,   107,    27,    47,   146,   138,
     146,   103,   146,   146,   138,    34,   108,   103,   173,   163,
      60,    74,   138,   171,    77,   138,    79,   168,   163,   171,
     144,   953,   458,   163,   348,    88,   462,   163,   464,   171,
     171,   467,   163,   163,   162,   967,   968,   162,   970,   971,
      90,   973,   974,   745,   107,    76,   481,    24,    88,    80,
      81,   983,   984,   852,   986,   316,   988,   881,   990,   991,
     906,  1060,   994,  1056,   857,   909,   116,  1049,   690,  1222,
    1213,  1180,   103,   123,  1206,    36,   178,   108,    30,   498,
     638,    -1,    -1,    -1,    -1,    -1,    -1,  1019,  1020,  1021,
    1022,    -1,    -1,    -1,   144,  1027,    -1,  1029,    -1,    -1,
      -1,  1033,    -1,    70,    -1,  1090,  1013,  1039,  1040,  1041,
    1042,  1043,    -1,   163,    -1,    -1,    -1,    -1,   168,    -1,
       1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,
      -1,    22,    -1,    -1,    -1,    26,    27,   178,    -1,    -1,
      -1,    -1,  1084,    -1,    35,    -1,    37,    38,  1090,    -1,
      -1,    -1,    43,  1095,    -1,   267,    -1,  1099,    -1,    50,
      -1,   273,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1090,    -1,    -1,    -1,    -1,  1095,    -1,
    1175,    72,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,
      81,    82,    -1,    84,    -1,    86,    -1,    88,    -1,    -1,
    1142,  1143,  1144,  1145,   650,   651,    -1,    -1,    -1,    -1,
     656,   657,    -1,    -1,   660,   661,   107,    -1,    -1,   110,
      -1,   333,   113,  1165,    -1,    -1,   267,  1169,  1170,    -1,
     676,    -1,   273,  1175,  1151,    -1,   348,    -1,    -1,    -1,
      -1,    -1,  1184,  1185,  1186,  1187,  1188,  1189,   225,   226,
     227,   228,   229,   230,    -1,    -1,    -1,    -1,  1175,    -1,
    1202,  1203,    -1,  1205,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,   720,    -1,    -1,    -1,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1203,    -1,    -1,    -1,
      -1,    -1,   333,    -1,    -1,    -1,  1238,    -1,    -1,   276,
    1242,    -1,    -1,    -1,    -1,   282,    -1,   348,    -1,    -1,
      -1,    -1,    -1,   425,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,  1242,    -1,   304,    35,    -1,
      37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,     2,
      -1,   453,    -1,    50,    -1,   322,   323,   459,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   805,
     806,   473,    25,   475,    27,    -1,    -1,    -1,    75,    -1,
      77,   817,    79,   819,    81,    -1,    -1,    84,    -1,    86,
     826,    88,    -1,   829,   425,    -1,   832,    -1,   834,    -1,
     836,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,   113,   853,    -1,    -1,
      -1,    -1,   453,    76,    -1,    -1,    -1,    -1,   459,    -1,
      -1,    -1,    -1,    -1,   536,    -1,    -1,     3,   540,    -1,
      -1,    -1,   473,    -1,   475,    -1,   548,    -1,   884,    -1,
     103,    17,    18,    -1,    20,   108,    -1,    -1,    -1,    25,
      -1,   428,    -1,    -1,    31,    -1,   163,    33,    -1,    -1,
      -1,    -1,   169,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,
      -1,   458,    -1,    -1,    -1,   462,    62,   464,    -1,    -1,
     467,    -1,    68,    69,    31,   536,    -1,    -1,    -1,   540,
      76,    -1,    -1,    -1,    -1,   617,    -1,   548,    -1,    85,
      87,    -1,    -1,    -1,    91,   178,    -1,    -1,    94,    95,
      -1,    97,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,   108,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
     672,   138,   139,   675,   141,   142,   143,    -1,   145,   146,
     147,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,  1025,
     157,   158,    -1,    -1,   161,    -1,    -1,   164,   165,   166,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,   175,    -1,
      -1,   138,   139,    -1,   267,   142,   143,    -1,   145,   146,
     273,    -1,    -1,   150,    -1,     2,    -1,    -1,    -1,    -1,
     732,   158,    -1,    -1,    -1,    12,   738,   164,   165,   166,
      -1,   672,  1078,  1079,   675,    -1,  1082,  1083,    25,    31,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1100,   767,    -1,    -1,    -1,    -1,
      -1,    -1,  1108,    -1,  1110,    -1,  1112,    -1,    -1,    -1,
     333,    -1,    -1,   650,   651,    -1,    -1,    -1,    -1,   656,
     657,   793,   794,   660,   661,   348,    -1,    -1,    -1,    76,
      -1,   732,    -1,    -1,    -1,    87,    -1,   738,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   820,    -1,
     822,    -1,    -1,    -1,    -1,   827,   103,    -1,   830,     2,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,   843,    -1,   845,    -1,    -1,   848,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,   425,    -1,   876,   157,   158,    -1,    -1,   161,
      -1,    -1,   164,   165,   166,   887,  1222,   889,    -1,   820,
      -1,   822,    -1,    -1,    -1,    -1,   827,    -1,    -1,   830,
     453,   178,    -1,    76,    -1,    -1,   459,    -1,    -1,    -1,
      -1,    -1,   843,    -1,   845,    -1,    -1,   848,    -1,    -1,
     473,    -1,   475,    -1,    -1,    -1,    -1,    22,    -1,    -1,
     103,    26,    27,    -1,    -1,   108,    -1,    -1,   805,   806,
      35,    -1,    37,    38,    -1,   876,    -1,    -1,    43,    -1,
     817,    -1,    -1,    -1,    -1,    50,   887,    -1,   889,    -1,
      -1,    -1,   829,    -1,    -1,   832,    -1,   834,    -1,   836,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,   983,    77,   536,    79,    -1,    81,   540,   990,    84,
     267,    86,    -1,    88,    -1,   548,   273,    -1,    -1,    -1,
      -1,    -1,    -1,  1005,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,  1023,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,
    1032,    -1,    -1,    -1,  1036,    35,    -1,    37,    38,    -1,
      -1,    -1,    22,    43,    -1,    -1,    26,    27,    -1,    -1,
      50,    -1,   983,    -1,    -1,    35,   333,    37,    38,   990,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,   163,    -1,
      50,   348,    -1,    -1,  1005,    75,    -1,    77,    -1,    79,
      -1,    81,    -1,  1085,    84,    -1,    86,    -1,    88,    -1,
      -1,    -1,  1023,    -1,   267,    75,    -1,    77,    -1,    -1,
     273,  1032,    -1,    -1,    -1,  1036,    86,   107,    88,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,   672,
      -1,    -1,   675,    -1,    -1,    -1,    -1,   107,    -1,    -1,
     110,    22,    -1,   113,    -1,    26,    27,    -1,    -1,    -1,
    1142,  1143,  1144,  1145,    35,    -1,    37,    38,   425,    -1,
      -1,    -1,    43,    -1,  1085,    -1,    -1,    -1,  1025,    50,
     333,    -1,    -1,   163,    -1,    -1,    -1,  1169,  1170,   169,
      -1,    -1,    -1,    -1,    -1,   348,   453,    -1,    -1,   732,
      -1,    -1,   459,    -1,    75,   738,    77,    -1,    79,    -1,
      81,    -1,    -1,    84,    -1,    86,   473,    88,   475,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       2,  1142,  1143,  1144,  1145,    -1,   107,    -1,    -1,   110,
      12,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,  1169,  1170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   425,    -1,    -1,    -1,    -1,    -1,    -1,   536,
      -1,    -1,    -1,   540,    -1,    -1,    -1,   820,    -1,   822,
      -1,   548,   163,    -1,   827,    -1,    -1,   830,   169,    -1,
     453,    -1,     3,    -1,    76,    -1,   459,    -1,    -1,    -1,
     843,    -1,   845,    -1,    -1,   848,    17,    18,    -1,    20,
     473,    -1,   475,    -1,    25,    -1,    -1,    -1,    -1,    -1,
       2,   103,    33,    -1,    -1,    -1,   108,    -1,    39,    -1,
      12,    -1,    -1,   876,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    25,   887,    27,   889,    -1,    -1,    -1,
       3,    62,    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    17,    76,    -1,    20,    -1,    -1,
      -1,    -1,    25,   536,    85,    -1,    -1,   540,    -1,    -1,
      -1,    -1,    35,    94,    95,   548,    97,    -1,    99,    -1,
     101,    -1,    45,   104,    76,    -1,   178,   108,    -1,    52,
     111,    -1,    -1,   114,    -1,   672,    -1,    -1,   675,    62,
      -1,    -1,    -1,    -1,    -1,    22,    69,    -1,    -1,    26,
      27,   103,    -1,    -1,    -1,    -1,   108,    -1,    35,    -1,
      37,    38,    85,    -1,    -1,    -1,    43,    -1,    -1,    -1,
     983,    -1,    -1,    50,    97,    -1,    -1,   990,   101,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,   168,    -1,   170,
      -1,   114,  1005,    -1,    -1,   732,    -1,    -1,    75,    -1,
      77,   738,    79,    -1,    81,    -1,    -1,    84,    -1,    86,
    1023,    88,    -1,    -1,    -1,   267,    -1,    -1,    -1,  1032,
      -1,   273,    -1,  1036,    -1,    -1,   178,     2,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,   113,    12,    -1,   672,
      -1,    -1,   675,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,  1085,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   333,    -1,   820,   154,   822,   163,    -1,    -1,    -1,
     827,    -1,   169,   830,    -1,    -1,   348,   167,    -1,   732,
     170,    76,   172,    -1,    -1,   738,   843,    -1,   845,    -1,
      -1,   848,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,
      -1,   273,    -1,    -1,    -1,    -1,     2,    -1,   103,  1142,
    1143,  1144,  1145,   108,    -1,    -1,    12,    -1,    -1,   876,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
     887,    27,   889,    -1,    -1,    -1,  1169,  1170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,   425,    -1,    -1,    -1,    -1,    -1,    -1,
      35,   333,    37,    38,    -1,    -1,    -1,   820,    43,   822,
      -1,    -1,    -1,    -1,   827,    50,   348,   830,    -1,    -1,
      76,   453,    -1,   178,    -1,    -1,    -1,   459,    -1,    -1,
     843,    -1,   845,    -1,    -1,   848,    -1,    -1,    -1,    31,
      75,   473,    77,   475,    79,    -1,    81,   103,    -1,    84,
      -1,    86,   108,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   876,    -1,    -1,   983,    -1,    -1,    -1,
      -1,    -1,   107,   990,   887,   110,   889,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1005,    -1,
      -1,    -1,    -1,   425,    -1,    87,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,   536,    -1,  1023,    -1,   540,    -1,
      -1,    -1,   267,    -1,    -1,  1032,   548,    -1,   273,  1036,
      -1,   453,   178,    -1,    -1,    -1,    -1,   459,   163,    -1,
     122,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,
      -1,   473,    -1,   475,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,    -1,    -1,   155,   156,   157,   158,   159,  1085,   161,
     983,    -1,   164,   165,   166,    -1,    -1,   990,   333,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1005,   348,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   536,    -1,    -1,    -1,   540,    -1,
    1023,   267,    -1,    -1,    -1,    -1,   548,   273,    -1,  1032,
      -1,    -1,    -1,  1036,    -1,  1142,  1143,  1144,  1145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,
     672,    -1,    -1,   675,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,  1169,  1170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
     425,    -1,  1085,    -1,    -1,    -1,    -1,   333,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   348,    -1,    -1,    -1,    -1,    -1,   453,    -1,
     732,    -1,    -1,    -1,   459,    -1,   738,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    31,    -1,    -1,   473,    -1,
     475,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1142,
    1143,  1144,  1145,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     672,    -1,    -1,   675,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1169,  1170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   425,
      -1,    87,    -1,    -1,    -1,    91,    43,    44,    45,    -1,
      -1,   536,    -1,    -1,    -1,   540,    -1,    -1,   820,    -1,
     822,    -1,    -1,   548,    -1,   827,    -1,   453,   830,    -1,
     732,    -1,    -1,   459,    -1,    -1,   738,    -1,    -1,    -1,
      -1,   843,    -1,   845,   178,    82,   848,   473,    -1,   475,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,    -1,    -1,   155,
     156,   157,   158,   159,   876,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    -1,    -1,   887,    -1,   889,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,   142,    -1,    -1,    -1,    -1,
     536,    -1,    -1,    -1,   540,    -1,    -1,    -1,   820,    -1,
     822,    -1,   548,    -1,    -1,   827,    -1,    -1,   830,    -1,
      -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,   273,
      31,   843,    -1,   845,    -1,    -1,   848,   672,    -1,    -1,
     675,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      -1,    -1,    -1,    -1,   876,    -1,    -1,    -1,    -1,    -1,
      -1,   983,    -1,    -1,   221,   887,   223,   889,   990,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,   234,   235,   333,
      91,    -1,    -1,  1005,    -1,    -1,    -1,   732,    -1,    -1,
      -1,    -1,    -1,   738,   348,    -1,    -1,    -1,    -1,    -1,
      -1,  1023,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1032,   122,   269,    -1,  1036,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   672,   138,   139,   675,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,    -1,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,   983,    -1,  1085,   175,    -1,    -1,    -1,   990,    -1,
      -1,   425,    -1,    -1,    -1,   820,    -1,   822,    -1,    -1,
      -1,    -1,   827,  1005,    -1,   830,   732,    -1,    -1,    -1,
      -1,    -1,   738,    -1,    -1,    -1,   353,    -1,   843,   453,
     845,  1023,    -1,   848,    -1,   459,    -1,    -1,    -1,    -1,
    1032,    -1,    -1,    -1,  1036,    -1,    -1,    -1,    -1,   473,
    1142,  1143,  1144,  1145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   876,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   887,    -1,   889,    -1,    -1,  1169,  1170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1085,    -1,    -1,    -1,   424,    -1,    -1,
      -1,    -1,    -1,    -1,   820,    -1,   822,    -1,    -1,    -1,
      -1,   827,   536,    -1,   830,   442,   540,    -1,    -1,   446,
      -1,    -1,    -1,    -1,   548,    -1,    -1,   843,    -1,   845,
      -1,    -1,   848,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1142,  1143,  1144,  1145,    -1,    -1,    -1,   484,    -1,    -1,
     876,    -1,    -1,   490,    -1,    -1,    -1,     3,   983,    -1,
      -1,   887,    -1,   889,    -1,   990,    -1,  1169,  1170,    -1,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
    1005,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,  1023,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,  1032,    -1,    -1,
      -1,  1036,   549,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,   558,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   672,    85,
      -1,   675,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      -1,    97,    -1,    99,    -1,   101,    -1,   983,   104,    -1,
    1085,    -1,   108,    -1,   990,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1005,
      -1,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,  1023,   732,    -1,
      -1,    -1,   639,    -1,   738,   642,  1032,    -1,    -1,    -1,
    1036,    -1,    -1,    -1,    -1,    -1,    -1,  1142,  1143,  1144,
    1145,    -1,    -1,    -1,   170,   167,    -1,    -1,   170,    -1,
     172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1169,  1170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   693,    -1,    -1,  1085,
     697,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   720,    -1,    -1,   820,   724,   822,    -1,
      -1,     3,    -1,   827,    -1,    -1,   830,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,   843,
      -1,   845,    -1,    25,   848,    -1,  1142,  1143,  1144,  1145,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
     767,    -1,     3,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,   876,  1169,  1170,    -1,    17,    18,    -1,    20,
      62,    -1,    -1,   887,    25,   889,    68,    69,    -1,    -1,
      -1,    -1,    33,    -1,    76,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    85,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    94,    95,    -1,    97,    -1,    99,    -1,   101,
      -1,    62,   104,    -1,    -1,    -1,   108,    68,    69,   111,
      -1,    -1,   114,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   850,    85,   852,    -1,    -1,    -1,   856,
      -1,    -1,   859,    94,    95,    -1,    97,    -1,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,   881,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   906,
      -1,  1005,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1023,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1032,    -1,
       0,     1,  1036,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,  1085,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    -1,    72,    -1,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,   101,   102,    -1,   104,   105,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
      -1,    -1,   122,  1060,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,     1,   175,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    -1,    -1,    75,    76,
      77,    78,    -1,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,   101,   102,    -1,   104,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,   168,   169,   170,    -1,   172,    -1,     1,   175,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
     168,   169,   170,    -1,   172,    -1,     1,   175,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    -1,    68,    69,    70,    -1,    72,    -1,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,   101,   102,    -1,   104,
     105,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,
      -1,   166,    -1,   168,   169,   170,    -1,   172,    -1,     1,
     175,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    -1,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    90,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,   101,
     102,    -1,   104,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,   163,    -1,    -1,   166,    -1,   168,   169,   170,    -1,
     172,    -1,     1,   175,     3,    -1,     5,     6,     7,     8,
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
      -1,   170,    -1,   172,    -1,     1,   175,     3,     4,     5,
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
     166,    -1,   168,    -1,   170,    -1,   172,   173,     1,   175,
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
      -1,     3,   175,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    77,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      99,    -1,   101,    -1,    -1,   104,   105,   106,    -1,   108,
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,   148,
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
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,   105,
     106,    -1,   108,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,   171,   172,    -1,    -1,   175,
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
      -1,   104,   105,   106,    -1,   108,   109,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,   171,   172,
      -1,     3,   175,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    47,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     101,    -1,    -1,   104,   105,   106,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,   170,
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
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
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
       6,     7,     8,     9,    10,    11,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,   107,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
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
      -1,    76,    77,    78,    -1,    80,    -1,    -1,    -1,    -1,
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
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,   113,
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
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,   120,
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
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
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
      97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,   169,   170,    -1,   172,    -1,    -1,   175,     3,
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
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,   173,
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
      -1,   172,   173,     3,   175,     5,     6,     7,     8,     9,
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
      -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,    -1,
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
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,   173,
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
      -1,   104,    -1,    -1,    -1,   108,   109,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
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
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,   138,   139,   140,   141,   142,    -1,    -1,    -1,    12,
     147,    -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,
     157,    -1,    25,   160,   161,   162,    -1,    -1,    -1,   166,
      33,    -1,    35,   170,    -1,   172,    39,    -1,   175,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    97,    -1,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,    -1,    -1,   131,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,    -1,   145,    -1,    -1,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,    -1,   161,    -1,
      -1,   164,   165,   166,   167,    -1,     3,   170,     5,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,    -1,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,     3,     4,     5,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,
      -1,   168,    -1,   170,   171,   172,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,   103,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,   140,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,   162,   163,    -1,    -1,    -1,
      -1,   168,    33,   170,    35,   172,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,     3,   140,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,   162,    -1,    -1,    -1,    -1,    -1,   168,    33,   170,
      35,   172,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,   140,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,   162,    -1,    -1,
      -1,    -1,    -1,   168,    33,   170,    35,   172,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,    -1,     3,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,    12,    -1,   114,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,   140,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    68,
      69,   170,    -1,   172,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    94,    95,    -1,    97,    -1,
      99,    19,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,    31,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
      -1,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,    -1,   145,    -1,    -1,    -1,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,    -1,   161,    -1,    -1,   164,   165,   166,   167,    87,
      12,    -1,    -1,    91,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    87,   164,   165,   166,    91,
     168,    -1,    12,    -1,    -1,    -1,    -1,   175,   100,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    -1,   168,    87,    12,    -1,
      -1,    91,    -1,   175,    -1,    19,    -1,    -1,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    -1,    87,   164,   165,   166,    91,   168,    -1,
      12,    -1,    -1,    -1,    -1,   175,   100,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,   131,   132,   133,
     134,    -1,   136,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,    -1,   168,    87,    12,    -1,    -1,    91,
      -1,   175,    -1,    19,    -1,    -1,    -1,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,    87,   164,   165,   166,    91,   168,    -1,    12,    -1,
      -1,    -1,    -1,   175,   100,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
     126,   127,   128,    -1,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,   168,    87,    12,    -1,    -1,    91,    -1,   175,
      -1,    19,    -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,   131,   132,   133,
     134,    -1,   136,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    -1,    87,
     164,   165,   166,    91,   168,    -1,    12,    -1,    -1,    -1,
      -1,   175,   100,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
     168,    87,    -1,    -1,    -1,    91,    -1,   175,    12,    -1,
      -1,    -1,    -1,    -1,   100,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
     126,   127,   128,    47,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,   168,    87,    12,    -1,    -1,    91,    -1,   175,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   122,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    -1,    87,
     164,   165,   166,    91,   168,    -1,    12,    -1,    -1,    -1,
      -1,   175,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
     118,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
     168,    87,    -1,    -1,    -1,    91,    -1,   175,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    36,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    71,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,   168,    -1,    87,    -1,    -1,    -1,    91,   175,
      -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    71,   155,   156,   157,   158,   159,   160,   161,    -1,
     163,   164,   165,   166,    -1,    -1,    -1,    87,    12,    -1,
      -1,    91,   175,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    -1,    87,   164,   165,   166,    91,    -1,    -1,
      12,    -1,    -1,    -1,    -1,   175,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,   118,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,    -1,   168,    87,    12,    -1,    -1,    91,
      -1,   175,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
     122,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,    87,   164,   165,   166,    91,   168,    12,    -1,    -1,
      -1,    -1,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,    87,    12,    -1,    -1,    91,    -1,    -1,   175,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,    87,   164,
     165,   166,    91,   168,    12,    -1,    -1,    -1,    -1,    -1,
     175,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,    87,
      12,    -1,    -1,    91,    -1,    -1,   175,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    87,   164,   165,   166,    91,
      -1,    -1,    12,    -1,    -1,    -1,    -1,   175,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    -1,   168,    87,    12,    -1,
      -1,    91,    -1,   175,    -1,    19,    -1,    -1,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    -1,    87,   164,   165,   166,    91,   168,    -1,
      -1,    12,    -1,    -1,    -1,   175,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   118,    36,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      71,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,    -1,    -1,    -1,    87,    12,    -1,   173,
      91,   175,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    31,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    87,   164,   165,   166,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    12,   175,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,   122,   164,
     165,   166,    -1,   168,    -1,    -1,    -1,    -1,    87,    -1,
     175,    -1,    91,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    12,   163,
     164,   165,   166,   122,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    31,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    12,   163,   164,   165,   166,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    -1,   163,
     164,   165,   166,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    87,    -1,   138,
     139,    91,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    12,   163,   164,   165,   166,    -1,    -1,
      19,   121,   122,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    12,    -1,   164,   165,   166,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,   138,
     139,    91,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,   175,    87,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    -1,    -1,    12,
      -1,   171,    -1,    -1,    -1,   175,    19,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    31,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,   175,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    91,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
     118,   164,   165,   166,   122,    -1,    -1,    -1,    -1,    87,
      -1,    -1,   175,    91,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   122,    -1,   164,   165,   166,    -1,
      -1,    12,    13,    -1,    -1,    -1,    -1,   175,    19,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      31,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,   164,   165,   166,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   175,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   175,    19,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    31,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,    12,
     171,    -1,    -1,    -1,   175,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   121,   122,
     164,   165,   166,    -1,    87,    -1,    -1,   171,    91,    -1,
      -1,   175,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,    12,
      22,   164,   165,   166,    26,    27,    19,    -1,    -1,    -1,
      -1,    -1,   175,    35,    -1,    37,    38,    -1,    31,    32,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    64,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    77,    -1,    -1,    31,    -1,
      -1,    83,    -1,    -1,    86,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    12,
      13,    -1,    -1,    -1,    -1,   107,    19,    -1,   110,    -1,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,   175,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,    -1,
     173,    -1,   175,    19,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    31,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   100,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,   175,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   175,
      19,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    31,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   175,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    -1,    -1,
      12,    -1,   171,    -1,    -1,    -1,   175,    19,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    31,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,
      12,    -1,    -1,    -1,   173,    -1,   175,    19,    -1,    -1,
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
      -1,    -1,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    91,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   100,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   122,    -1,   164,
     165,   166,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,
     175,    -1,    -1,   138,   139,   100,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   122,    -1,   164,
     165,   166,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     175,    19,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    31,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     175,    19,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    12,    -1,    -1,    -1,   173,    -1,   175,    19,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      31,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,    -1,   150,   151,   152,
      -1,    -1,   155,   156,   157,   158,    -1,     1,   161,     3,
      -1,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
       3,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,    62,    -1,
      -1,    -1,    25,    -1,    68,    69,    -1,    -1,    -1,    -1,
      33,    -1,    76,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    85,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      94,    95,    -1,    97,    -1,    99,    -1,   101,    -1,    62,
     104,    -1,    -1,    -1,   108,    68,    69,   111,    -1,    -1,
     114,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    97,    -1,    99,    -1,   101,    17,
      18,   104,    20,    -1,    -1,   108,    -1,    25,   111,    -1,
      -1,   114,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,     3,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    62,    -1,    -1,    -1,    25,    -1,
      68,    69,    -1,    -1,    -1,    -1,    33,    -1,    76,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    85,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    94,    95,    -1,    97,
      -1,    99,    -1,   101,    -1,    62,   104,    -1,    -1,    -1,
     108,    68,    69,   111,    -1,    -1,   114,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,
      97,    -1,    99,    -1,   101,    17,    18,   104,    20,    -1,
      -1,   108,    -1,    25,   111,    -1,    -1,   114,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,     3,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      62,    -1,    -1,    -1,    25,    -1,    68,    69,    -1,    -1,
      -1,    -1,    33,    -1,    76,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    85,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    94,    95,    -1,    97,    -1,    99,    -1,   101,
      -1,    62,   104,    -1,    -1,    -1,   108,    68,    69,   111,
      -1,    -1,   114,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    97,    -1,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114
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
     282,   323,   227,   323,   323,   323,   323,   323,   323,   323,
       1,   169,   182,   193,   298,   109,   148,   275,   300,   301,
     325,   225,   298,   323,   334,   323,    79,   184,   168,    83,
     188,    46,   112,    55,   207,   207,    53,    72,    82,   270,
     286,   162,   163,   154,   323,    12,    19,    31,    87,    91,
     122,   138,   139,   141,   142,   143,   145,   146,   147,   149,
     150,   151,   152,   153,   155,   156,   157,   158,   159,   160,
     161,   164,   165,   166,   175,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     167,   262,   170,   172,    87,    91,   170,   184,   163,   323,
     323,   323,   207,   298,    55,   168,   192,    47,   312,   282,
     286,   163,   144,   163,   117,   208,   275,   302,   303,   304,
     325,    86,   221,   256,   284,    86,   219,   282,   256,   284,
     207,   170,   212,    32,    47,   212,   118,   212,   315,    32,
      47,   212,   315,   212,   315,    47,   212,   315,    36,    71,
     163,   207,   207,   100,   192,   100,   123,   192,   262,    81,
      84,   189,   302,   170,   170,   192,   184,   170,   264,   109,
     170,   207,   288,   289,     1,   143,   293,    47,   144,   184,
     212,   170,   170,   212,   302,   216,   144,   163,   323,   323,
     163,   168,   212,   170,   302,   163,   237,   163,   237,   163,
     123,   283,   163,   212,   212,   163,   169,   169,   182,   144,
     169,   323,   144,   171,   144,   171,   173,   315,    47,   144,
     173,   315,   121,   144,   173,     8,     1,   169,   193,    64,
     201,   202,   323,   195,   323,   207,   143,   246,   168,   257,
     163,   323,   323,   323,   323,   225,   323,   225,   323,   323,
     323,   323,   323,   323,   323,     3,    20,    33,    62,   101,
     107,   208,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,    67,
     325,   325,   325,   325,   325,   302,   302,   225,   323,   225,
     323,    27,    47,    88,   113,   314,   317,   318,   323,   339,
      32,    47,    32,    47,   100,   170,    47,   173,   207,   225,
     323,   212,   163,   163,   323,   323,   123,   171,   144,   222,
     207,   286,   220,   207,   163,   286,    47,   298,    44,   323,
     225,   323,   170,   212,    44,   323,   225,   323,   212,   212,
     225,   323,   212,   166,   194,   197,   323,   194,   196,   123,
     123,   184,    34,   184,   323,    34,   323,   188,   171,   303,
     207,   229,   230,    27,    47,    51,    74,    77,    88,   107,
     183,   265,   266,   267,   268,   254,   289,   144,   171,    33,
      49,    95,    99,   172,   211,   294,   306,   123,   290,   323,
     287,   323,   323,   171,   275,   323,     1,   242,   302,   171,
      21,   238,   294,   306,   144,   169,   171,   171,   300,   171,
     300,   184,   173,   225,   323,   173,   184,   323,   173,   323,
     173,   323,   169,   169,   207,   144,   163,    13,   146,   144,
     163,    13,    36,    71,   163,   170,   298,   168,     1,   207,
     250,   251,    27,    77,    88,   107,   259,   269,   170,   163,
     163,   163,   163,   163,   163,   171,   173,    47,    88,   144,
     171,    17,    20,    25,    45,    52,    62,    69,    85,    97,
     108,   114,   305,    87,    87,    44,   225,   323,    44,   225,
     323,   303,   225,   323,   170,   312,   312,   163,   275,   325,
     304,   207,   246,   163,   207,   246,   163,   323,   171,   323,
      32,   212,    32,   212,   316,   317,   323,    32,   212,   315,
      32,   212,   315,   212,   315,   212,   315,   144,   163,    13,
     163,   323,   323,    34,   184,    34,    34,   184,   100,   192,
      64,   171,   144,   171,    47,    88,   267,   144,   171,   170,
     207,    27,    77,    88,   107,   271,   171,   289,   293,     1,
     298,    67,   325,   171,   171,   169,    73,   115,   169,   243,
     171,   170,   192,   207,   239,   282,   184,   173,   315,   173,
     315,   184,   121,   201,   208,   168,   323,   109,   323,   197,
     196,   303,     1,   247,   169,   123,   144,   169,    88,   258,
       1,     3,    12,    17,    19,    20,    25,    39,    45,    52,
      54,    62,    68,    69,    85,    97,   101,   104,   108,   114,
     138,   139,   140,   141,   142,   143,   145,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   161,   164,
     165,   166,   167,   170,   206,   207,   209,   260,   261,   262,
     305,   171,   317,   293,   305,   305,   323,    32,    32,   323,
      32,    32,   171,   173,   173,   303,   212,   212,   246,   168,
     246,   168,   212,   100,    44,   323,    44,   323,   144,   171,
     100,    44,   323,   212,    44,   323,   212,   212,   212,   323,
     323,   184,   184,   323,   184,    34,   207,   163,   230,   192,
     207,   266,   289,   143,   297,    88,   293,   290,   173,    47,
     173,   170,   170,    32,   184,   298,   239,   143,   192,    44,
     184,   323,   173,    44,   184,   323,   173,   323,   194,    13,
      36,    71,    36,    71,   163,   163,   171,   169,    81,    84,
     169,   183,   214,   323,   251,   271,   170,   263,   323,   138,
     146,   263,   263,   290,   100,    44,    44,   100,    44,    44,
      44,    44,   171,   168,   247,   168,   247,   323,   323,   323,
     317,   323,   323,   323,    13,    34,   184,   163,   297,   171,
     172,   211,   275,   296,   306,   148,   276,   290,    60,   116,
     277,   323,   294,   306,   302,   302,   184,   212,   171,   323,
     323,   184,   323,   184,   169,   109,   323,   197,   196,   197,
     196,   163,   214,     1,   143,   292,   265,   171,     3,   101,
     261,   262,   323,   323,   323,   323,   323,   323,   247,   169,
     247,   169,   100,   100,   100,   100,   323,   184,   276,   290,
     297,   173,   298,   275,   323,     3,    90,   101,   278,   279,
     280,   323,   192,   213,   274,   173,   171,   171,   192,   100,
     100,   163,   163,   163,   163,   172,   211,   291,   306,   103,
     272,   171,   263,   263,   100,   100,   100,   100,   100,   100,
     169,   169,   323,   323,   323,   323,   276,   290,   275,   295,
     296,   306,    47,   173,   280,   116,   144,   123,   149,   151,
     152,   155,   156,    60,   306,   162,   162,   323,   323,     1,
     173,   298,   277,   323,   295,   296,   323,   279,   280,   280,
     280,   280,   280,   280,   278,   173,   291,   306,   173,   163,
     273,   274,   173,   291,   306,   295
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
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1012 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), true, false); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), true, false); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
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
#line 1035 "chapel.ypp"
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1037 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1043 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1049 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1055 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1061 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1068 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1075 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1079 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1085 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1089 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1090 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1091 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1099 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1101 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1103 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1105 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pexpr) = buildIfVar((yyvsp[(2) - (4)].pch), (yyvsp[(4) - (4)].pexpr), false); }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pexpr) = buildIfVar((yyvsp[(2) - (4)].pch), (yyvsp[(4) - (4)].pexpr), true);  }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt))); }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1118 "chapel.ypp"
    { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (3)].pch), ls, (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1125 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1126 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[(1) - (1)].pch), INTENT_TYPE); }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1136 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1141 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1142 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1143 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1147 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1149 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[(1) - (1)].pch)); }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1155 "chapel.ypp"
    { (yyval.pch) = "none"; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1156 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1157 "chapel.ypp"
    { (yyval.pch) = "false"; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pch) = "true"; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1173 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), NULL)); }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1175 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (4)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (4)].pch), act, NULL)); }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1178 "chapel.ypp"
    { (yyvsp[(5) - (7)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (7)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (7)].pch), (yyvsp[(5) - (7)].pcallexpr), NULL)); }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.pexpr) = IfcConstraint::build((yyvsp[(2) - (5)].pch), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1186 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (3)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (3)].pch), act); }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyvsp[(5) - (6)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (6)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pcallexpr)); }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1194 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1199 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1201 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1207 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1226 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1228 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1232 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1242 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1249 "chapel.ypp"
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
#line 1259 "chapel.ypp"
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
#line 1272 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1277 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1305 "chapel.ypp"
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
#line 1321 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1328 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1337 "chapel.ypp"
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
#line 1345 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1349 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1361 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1371 "chapel.ypp"
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
#line 1393 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1396 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1400 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1408 "chapel.ypp"
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
#line 1418 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1425 "chapel.ypp"
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
#line 1458 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1462 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1467 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1471 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1476 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1492 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pch) = ":"; }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pexpr))); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1559 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1564 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1568 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1570 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1572 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1576 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1582 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_PROC; }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_ITER; }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_OP; }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1600 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1614 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1618 "chapel.ypp"
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1622 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1624 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1636 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1647 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1652 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1653 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1654 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1656 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1671 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1678 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1691 "chapel.ypp"
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

  case 428:
/* Line 1792 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1709 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1711 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1715 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1717 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1718 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1724 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1730 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1739 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1747 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1749 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1754 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1758 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1763 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1765 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1767 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1773 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1775 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1781 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1783 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1789 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1793 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1795 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1806 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1807 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1809 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1815 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1816 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1818 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1819 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1840 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1844 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1848 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1856 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1862 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1863 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1864 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1869 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1871 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1877 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1879 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1881 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1885 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1886 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1887 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1888 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1889 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1895 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1896 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1897 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1898 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1902 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1903 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1907 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1908 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1909 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1913 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1918 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1923 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1924 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1928 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1929 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1930 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1931 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1935 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1936 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1948 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1950 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1952 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1963 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1965 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1967 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1969 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1975 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1978 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1980 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1982 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1984 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1986 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1988 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1990 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1992 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 2002 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 2004 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 2006 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 2008 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 2010 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 2012 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 2014 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 2016 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 2018 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2020 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2022 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 2024 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2026 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2033 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 2039 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 2045 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2051 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2060 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2069 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2085 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2089 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2094 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2098 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2099 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2103 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2107 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2108 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2113 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2117 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2121 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2127 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2128 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2129 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2130 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2131 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2132 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2138 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2143 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2145 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2149 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2153 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2157 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2165 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2170 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2176 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2182 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2189 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2199 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2216 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2218 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2220 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2222 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), false, true); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2237 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2239 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2241 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr), true); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2243 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2247 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2248 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 607:
/* Line 1792 of yacc.c  */
#line 2252 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 608:
/* Line 1792 of yacc.c  */
#line 2253 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 609:
/* Line 1792 of yacc.c  */
#line 2254 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 614:
/* Line 1792 of yacc.c  */
#line 2270 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 615:
/* Line 1792 of yacc.c  */
#line 2271 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 616:
/* Line 1792 of yacc.c  */
#line 2272 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 617:
/* Line 1792 of yacc.c  */
#line 2273 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 619:
/* Line 1792 of yacc.c  */
#line 2278 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 620:
/* Line 1792 of yacc.c  */
#line 2279 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 621:
/* Line 1792 of yacc.c  */
#line 2280 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 622:
/* Line 1792 of yacc.c  */
#line 2284 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 623:
/* Line 1792 of yacc.c  */
#line 2285 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 624:
/* Line 1792 of yacc.c  */
#line 2286 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 625:
/* Line 1792 of yacc.c  */
#line 2287 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 626:
/* Line 1792 of yacc.c  */
#line 2288 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 627:
/* Line 1792 of yacc.c  */
#line 2296 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 628:
/* Line 1792 of yacc.c  */
#line 2297 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 629:
/* Line 1792 of yacc.c  */
#line 2298 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 630:
/* Line 1792 of yacc.c  */
#line 2299 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 631:
/* Line 1792 of yacc.c  */
#line 2303 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 632:
/* Line 1792 of yacc.c  */
#line 2304 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 633:
/* Line 1792 of yacc.c  */
#line 2308 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 634:
/* Line 1792 of yacc.c  */
#line 2309 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 637:
/* Line 1792 of yacc.c  */
#line 2315 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 638:
/* Line 1792 of yacc.c  */
#line 2316 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 639:
/* Line 1792 of yacc.c  */
#line 2317 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 640:
/* Line 1792 of yacc.c  */
#line 2318 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 641:
/* Line 1792 of yacc.c  */
#line 2319 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 642:
/* Line 1792 of yacc.c  */
#line 2320 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr),
                                            new SymExpr(gTrue)); }
    break;

  case 643:
/* Line 1792 of yacc.c  */
#line 2322 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr),
                                                   new SymExpr(gTrue)); }
    break;

  case 644:
/* Line 1792 of yacc.c  */
#line 2324 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 645:
/* Line 1792 of yacc.c  */
#line 2325 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 646:
/* Line 1792 of yacc.c  */
#line 2327 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 647:
/* Line 1792 of yacc.c  */
#line 2331 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 648:
/* Line 1792 of yacc.c  */
#line 2338 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 649:
/* Line 1792 of yacc.c  */
#line 2339 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 650:
/* Line 1792 of yacc.c  */
#line 2343 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 651:
/* Line 1792 of yacc.c  */
#line 2344 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 652:
/* Line 1792 of yacc.c  */
#line 2345 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 653:
/* Line 1792 of yacc.c  */
#line 2346 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 654:
/* Line 1792 of yacc.c  */
#line 2347 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 655:
/* Line 1792 of yacc.c  */
#line 2348 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 656:
/* Line 1792 of yacc.c  */
#line 2349 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 657:
/* Line 1792 of yacc.c  */
#line 2350 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 658:
/* Line 1792 of yacc.c  */
#line 2351 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 659:
/* Line 1792 of yacc.c  */
#line 2352 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 660:
/* Line 1792 of yacc.c  */
#line 2353 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 661:
/* Line 1792 of yacc.c  */
#line 2354 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 662:
/* Line 1792 of yacc.c  */
#line 2355 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 663:
/* Line 1792 of yacc.c  */
#line 2356 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 664:
/* Line 1792 of yacc.c  */
#line 2357 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 665:
/* Line 1792 of yacc.c  */
#line 2358 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 666:
/* Line 1792 of yacc.c  */
#line 2359 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 667:
/* Line 1792 of yacc.c  */
#line 2360 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 668:
/* Line 1792 of yacc.c  */
#line 2361 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 669:
/* Line 1792 of yacc.c  */
#line 2362 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 670:
/* Line 1792 of yacc.c  */
#line 2363 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 671:
/* Line 1792 of yacc.c  */
#line 2364 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 672:
/* Line 1792 of yacc.c  */
#line 2365 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr),
                                               new SymExpr(gTrue)); }
    break;

  case 673:
/* Line 1792 of yacc.c  */
#line 2370 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 674:
/* Line 1792 of yacc.c  */
#line 2371 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 675:
/* Line 1792 of yacc.c  */
#line 2372 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 676:
/* Line 1792 of yacc.c  */
#line 2373 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 677:
/* Line 1792 of yacc.c  */
#line 2374 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 678:
/* Line 1792 of yacc.c  */
#line 2375 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 679:
/* Line 1792 of yacc.c  */
#line 2376 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 680:
/* Line 1792 of yacc.c  */
#line 2380 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 681:
/* Line 1792 of yacc.c  */
#line 2381 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 682:
/* Line 1792 of yacc.c  */
#line 2382 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 683:
/* Line 1792 of yacc.c  */
#line 2383 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 684:
/* Line 1792 of yacc.c  */
#line 2387 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 685:
/* Line 1792 of yacc.c  */
#line 2388 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 686:
/* Line 1792 of yacc.c  */
#line 2389 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 687:
/* Line 1792 of yacc.c  */
#line 2390 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 688:
/* Line 1792 of yacc.c  */
#line 2395 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 689:
/* Line 1792 of yacc.c  */
#line 2396 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 690:
/* Line 1792 of yacc.c  */
#line 2397 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 691:
/* Line 1792 of yacc.c  */
#line 2398 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 692:
/* Line 1792 of yacc.c  */
#line 2399 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 693:
/* Line 1792 of yacc.c  */
#line 2400 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 694:
/* Line 1792 of yacc.c  */
#line 2401 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 11219 "bison-chapel.cpp"
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


