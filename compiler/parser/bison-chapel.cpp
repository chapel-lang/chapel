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
     TOPERATOR = 326,
     TOTHERWISE = 327,
     TOUT = 328,
     TOVERRIDE = 329,
     TOWNED = 330,
     TPARAM = 331,
     TPLUSPLUS = 332,
     TPRAGMA = 333,
     TPRIMITIVE = 334,
     TPRIVATE = 335,
     TPROC = 336,
     TPROTOTYPE = 337,
     TPUBLIC = 338,
     TREAL = 339,
     TRECORD = 340,
     TREDUCE = 341,
     TREF = 342,
     TREQUIRE = 343,
     TRETURN = 344,
     TSCAN = 345,
     TSELECT = 346,
     TSERIAL = 347,
     TSHARED = 348,
     TSINGLE = 349,
     TSPARSE = 350,
     TSTRING = 351,
     TSUBDOMAIN = 352,
     TSYNC = 353,
     TTHEN = 354,
     TTHIS = 355,
     TTHROW = 356,
     TTHROWS = 357,
     TTRUE = 358,
     TTRY = 359,
     TTRYBANG = 360,
     TTYPE = 361,
     TUINT = 362,
     TUNDERSCORE = 363,
     TUNION = 364,
     TUNMANAGED = 365,
     TUSE = 366,
     TVAR = 367,
     TVOID = 368,
     TWHEN = 369,
     TWHERE = 370,
     TWHILE = 371,
     TWITH = 372,
     TYIELD = 373,
     TZIP = 374,
     TALIAS = 375,
     TAND = 376,
     TASSIGN = 377,
     TASSIGNBAND = 378,
     TASSIGNBOR = 379,
     TASSIGNBXOR = 380,
     TASSIGNDIVIDE = 381,
     TASSIGNEXP = 382,
     TASSIGNLAND = 383,
     TASSIGNLOR = 384,
     TASSIGNMINUS = 385,
     TASSIGNMOD = 386,
     TASSIGNMULTIPLY = 387,
     TASSIGNPLUS = 388,
     TASSIGNREDUCE = 389,
     TASSIGNSL = 390,
     TASSIGNSR = 391,
     TBANG = 392,
     TBAND = 393,
     TBNOT = 394,
     TBOR = 395,
     TBXOR = 396,
     TCOLON = 397,
     TCOMMA = 398,
     TDIVIDE = 399,
     TDOT = 400,
     TDOTDOT = 401,
     TDOTDOTDOT = 402,
     TEQUAL = 403,
     TEXP = 404,
     TGREATER = 405,
     TGREATEREQUAL = 406,
     THASH = 407,
     TIO = 408,
     TLESS = 409,
     TLESSEQUAL = 410,
     TMINUS = 411,
     TMOD = 412,
     TNOTEQUAL = 413,
     TOR = 414,
     TPLUS = 415,
     TQUESTION = 416,
     TSEMI = 417,
     TSHIFTLEFT = 418,
     TSHIFTRIGHT = 419,
     TSTAR = 420,
     TSWAP = 421,
     TLCBR = 422,
     TRCBR = 423,
     TLP = 424,
     TRP = 425,
     TLSBR = 426,
     TRSBR = 427,
     TNOELSE = 428,
     TDOTDOTOPENHIGH = 429,
     TUMINUS = 430,
     TUPLUS = 431,
     TLNOT = 432
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
#line 497 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 504 "bison-chapel.cpp"
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
#line 582 "bison-chapel.cpp"

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
#define YYLAST   19639

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  178
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  161
/* YYNRULES -- Number of rules.  */
#define YYNRULES  685
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1226

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   432

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
     175,   176,   177
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
    1135,  1137,  1139,  1141,  1143,  1145,  1147,  1149,  1152,  1154,
    1156,  1158,  1160,  1162,  1164,  1166,  1168,  1170,  1172,  1174,
    1176,  1177,  1181,  1185,  1186,  1188,  1192,  1197,  1203,  1208,
    1214,  1221,  1228,  1229,  1231,  1233,  1235,  1237,  1239,  1242,
    1245,  1247,  1249,  1251,  1252,  1254,  1256,  1259,  1261,  1263,
    1265,  1267,  1269,  1270,  1272,  1275,  1277,  1279,  1281,  1282,
    1284,  1286,  1288,  1290,  1292,  1294,  1296,  1299,  1302,  1303,
    1306,  1309,  1314,  1319,  1321,  1325,  1329,  1333,  1337,  1341,
    1345,  1349,  1352,  1354,  1356,  1360,  1365,  1370,  1373,  1378,
    1379,  1382,  1385,  1387,  1389,  1391,  1394,  1396,  1401,  1405,
    1407,  1411,  1415,  1421,  1423,  1425,  1429,  1431,  1434,  1438,
    1439,  1442,  1445,  1449,  1452,  1457,  1461,  1465,  1470,  1474,
    1475,  1478,  1481,  1484,  1486,  1487,  1490,  1493,  1496,  1498,
    1503,  1508,  1515,  1519,  1520,  1522,  1524,  1528,  1533,  1537,
    1542,  1549,  1550,  1553,  1556,  1559,  1562,  1564,  1566,  1570,
    1574,  1576,  1580,  1582,  1584,  1586,  1590,  1594,  1595,  1597,
    1599,  1603,  1607,  1611,  1613,  1615,  1617,  1619,  1621,  1624,
    1626,  1628,  1630,  1632,  1634,  1636,  1639,  1644,  1649,  1654,
    1660,  1663,  1666,  1668,  1671,  1673,  1676,  1678,  1681,  1683,
    1686,  1688,  1690,  1697,  1704,  1709,  1719,  1729,  1737,  1744,
    1751,  1756,  1766,  1776,  1784,  1789,  1796,  1803,  1813,  1823,
    1830,  1832,  1834,  1836,  1838,  1840,  1842,  1844,  1846,  1850,
    1851,  1853,  1858,  1860,  1864,  1869,  1871,  1875,  1880,  1884,
    1888,  1890,  1892,  1895,  1897,  1900,  1902,  1904,  1908,  1910,
    1913,  1916,  1919,  1922,  1925,  1934,  1943,  1953,  1963,  1968,
    1970,  1972,  1974,  1976,  1978,  1980,  1982,  1984,  1986,  1991,
    1995,  1999,  2003,  2006,  2009,  2012,  2014,  2015,  2017,  2020,
    2023,  2025,  2027,  2029,  2031,  2033,  2035,  2038,  2041,  2043,
    2045,  2050,  2055,  2060,  2064,  2068,  2072,  2076,  2082,  2086,
    2091,  2095,  2100,  2102,  2104,  2106,  2108,  2110,  2112,  2114,
    2116,  2118,  2120,  2122,  2126,  2131,  2135,  2140,  2144,  2149,
    2153,  2159,  2163,  2167,  2171,  2175,  2179,  2183,  2187,  2191,
    2195,  2199,  2203,  2207,  2211,  2215,  2219,  2223,  2227,  2231,
    2235,  2239,  2243,  2247,  2251,  2254,  2257,  2260,  2263,  2266,
    2269,  2272,  2276,  2280,  2284,  2288,  2292,  2296,  2300,  2304,
    2306,  2308,  2310,  2312,  2314,  2316
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     179,     0,    -1,   180,    -1,    -1,   180,   181,    -1,   183,
      -1,   182,   183,    -1,    78,     8,    -1,   182,    78,     8,
      -1,   185,    -1,   189,    -1,   191,    -1,   198,    -1,   199,
      -1,   202,    -1,   213,    -1,   203,    -1,   222,    -1,   225,
      -1,   232,    -1,   227,    -1,   223,    -1,   240,    -1,   234,
      -1,   235,    -1,   239,    -1,   212,    -1,   310,   162,    -1,
      14,   183,    -1,    15,   311,   183,    -1,    16,   204,   162,
      -1,    23,   311,   191,    -1,    28,   204,   162,    -1,    30,
     298,   162,    -1,    56,   206,   183,    -1,    60,   322,   211,
      -1,    60,   211,    -1,    69,   322,   211,    -1,    92,   322,
     211,    -1,    92,   211,    -1,    98,   183,    -1,   118,   322,
     162,    -1,     1,   162,    -1,   186,   187,    63,   206,    -1,
     184,   167,   168,    -1,   184,   167,   192,   168,    -1,   184,
     167,     1,   168,    -1,    -1,    83,    -1,    80,    -1,    -1,
      82,    -1,    -1,    83,    -1,    80,    -1,    -1,    47,   190,
     188,   187,    63,   206,   162,    -1,   167,   168,    -1,   167,
     192,   168,    -1,   167,     1,   168,    -1,   181,    -1,   192,
     181,    -1,   322,    -1,   322,    13,   322,    -1,   193,   143,
     322,    -1,   193,   143,   322,    13,   322,    -1,   322,    -1,
     322,    13,   322,    -1,   322,    13,   108,    -1,   194,   143,
     322,    -1,   194,   143,   322,    13,   322,    -1,   194,   143,
     322,    13,   108,    -1,    -1,   193,    -1,   165,    -1,   193,
      -1,    -1,    83,    -1,    80,    -1,   197,   111,   194,   162,
      -1,   197,   111,   322,    36,   196,   162,    -1,   197,   111,
     322,    13,   322,    36,   196,   162,    -1,   197,   111,   322,
      13,   108,    36,   196,   162,    -1,   197,   111,   322,    70,
     195,   162,    -1,   197,   111,   322,    13,   322,    70,   195,
     162,    -1,   197,   111,   322,    13,   108,    70,   195,   162,
      -1,   197,    45,   201,   162,    -1,   322,    -1,   322,    13,
     207,    -1,   322,   145,   167,   193,   168,    -1,   200,    -1,
     201,   143,   200,    -1,    88,   297,   162,    -1,   325,   261,
     324,   162,    -1,   325,   166,   324,   162,    -1,   325,   134,
     324,   162,    -1,   325,   128,   324,   162,    -1,   325,   129,
     324,   162,    -1,   325,   122,    66,   162,    -1,    -1,     3,
      -1,     3,    -1,    67,    -1,   100,    -1,    39,    -1,   103,
      -1,   208,    -1,     3,    -1,    67,    -1,   100,    -1,    39,
      -1,   103,    -1,   208,    -1,     3,    -1,   100,    -1,    17,
      -1,    51,    -1,   107,    -1,    84,    -1,    44,    -1,    25,
      -1,    20,    -1,    96,    -1,    98,    -1,    94,    -1,    75,
      -1,    93,    -1,    18,    -1,   110,    -1,    33,    -1,    48,
      -1,    61,    -1,    68,    -1,   113,    -1,    17,    -1,    35,
      -1,    51,    -1,   107,    -1,    84,    -1,    44,    -1,    25,
      -1,    20,    -1,    96,    -1,    61,    -1,    68,    -1,   113,
      -1,    98,    -1,    94,    -1,    33,    -1,    48,    -1,    32,
     183,    -1,   191,    -1,    89,   162,    -1,    89,   324,   162,
      -1,   162,    -1,   215,    -1,    83,   215,    -1,    -1,    80,
     214,   215,    -1,   255,    -1,   284,    -1,   247,    -1,   280,
      -1,   243,    -1,   216,    -1,   217,    -1,    42,   322,   162,
      -1,    42,   322,    36,   196,   162,    -1,    42,   322,    70,
     195,   162,    -1,    42,   284,    -1,    -1,    38,    85,   218,
     206,   245,   167,   246,   168,    -1,    -1,    38,     8,    85,
     219,   206,   245,   167,   246,   168,    -1,    -1,    37,    85,
     220,   206,   245,   167,   246,   168,    -1,    -1,    37,     8,
      85,   221,   206,   245,   167,   246,   168,    -1,    38,   323,
     255,    -1,    37,   323,   255,    -1,    38,   323,   283,   285,
     162,    -1,    37,   323,   283,   285,   162,    -1,    38,    11,
      -1,    32,   183,   116,   322,   162,    -1,   116,   322,   211,
      -1,    24,   322,    46,   322,   311,   211,    -1,    24,   322,
      46,   224,   311,   211,    -1,    24,   322,   311,   211,    -1,
      40,   322,    46,   322,   211,    -1,    40,   322,    46,   224,
     211,    -1,    40,   322,   211,    -1,    40,   224,   211,    -1,
      40,    76,   206,    46,   322,   211,    -1,    41,   322,    46,
     322,   211,    -1,    41,   322,    46,   322,   314,   211,    -1,
      41,   322,    46,   224,   211,    -1,    41,   322,    46,   224,
     314,   211,    -1,    41,   322,   211,    -1,    41,   322,   314,
     211,    -1,    41,   224,   211,    -1,    41,   224,   314,   211,
      -1,   171,   297,    46,   322,   172,   183,    -1,   171,   297,
      46,   322,   314,   172,   183,    -1,   171,   297,    46,   224,
     172,   183,    -1,   171,   297,    46,   224,   314,   172,   183,
      -1,   171,   297,   172,   183,    -1,   171,   297,   314,   172,
     183,    -1,   171,   224,   172,   183,    -1,   171,   224,   314,
     172,   183,    -1,   119,   169,   297,   170,    -1,    43,   322,
      99,   183,    -1,    43,   322,   191,    -1,    43,   322,    99,
     183,    34,   183,    -1,    43,   322,   191,    34,   183,    -1,
      43,   226,    99,   183,    -1,    43,   226,   191,    -1,    43,
     226,    99,   183,    34,   183,    -1,    43,   226,   191,    34,
     183,    -1,    43,   322,   261,   322,    99,   183,    -1,    43,
     322,   261,   322,   191,    -1,    43,   322,   261,   322,    99,
     183,    34,   183,    -1,    43,   322,   261,   322,   191,    34,
     183,    -1,   112,   206,   122,   322,    -1,    27,   206,   122,
     322,    -1,    55,   206,   169,   228,   170,   191,    -1,    55,
     206,   191,    -1,   229,    -1,   228,   143,   229,    -1,   206,
      -1,     3,    -1,    17,    -1,    51,    -1,   107,    -1,    84,
      -1,    44,    -1,    25,    -1,    20,    -1,    96,    -1,    61,
      -1,    68,    -1,   113,    -1,   231,    -1,    67,    -1,   100,
      -1,    39,    -1,   103,    -1,    33,    -1,    48,    -1,    54,
     206,   169,   302,   170,   162,    -1,   230,    54,   206,   162,
      -1,   230,    54,   206,   169,   302,   170,   162,    -1,    54,
     206,   169,   302,   170,    -1,   230,    54,   206,    -1,   230,
      54,   206,   169,   302,   170,    -1,    29,   183,    -1,   104,
     322,   162,    -1,   105,   322,   162,    -1,   104,   203,    -1,
     105,   203,    -1,   104,   191,   236,    -1,   105,   191,   236,
      -1,    -1,   236,   237,    -1,    21,   191,    -1,    21,   238,
     191,    -1,    21,   169,   238,   170,   191,    -1,   206,    -1,
     206,   142,   322,    -1,   101,   322,   162,    -1,    91,   322,
     167,   241,   168,    -1,    91,   322,   167,     1,   168,    -1,
      -1,   241,   242,    -1,   114,   297,   211,    -1,    72,   183,
      -1,    72,    32,   183,    -1,   244,   206,   245,   167,   246,
     168,    -1,   244,   206,   245,   167,     1,   168,    -1,    22,
      -1,    85,    -1,   109,    -1,    -1,   142,   297,    -1,    -1,
     246,   213,    -1,   246,   182,   213,    -1,   248,   206,   167,
     249,   168,    -1,   248,   206,   167,     1,   168,    -1,    35,
      -1,   250,    -1,   249,   143,    -1,   249,   143,   250,    -1,
     206,    -1,   206,   122,   322,    -1,    -1,    -1,    57,   252,
     263,   253,   270,   292,   276,   273,    -1,    -1,    49,    -1,
      74,    -1,    -1,    -1,   254,   269,   256,   258,   257,   270,
     291,   271,   276,   272,    -1,   268,   260,   262,    -1,   268,
     261,   262,    -1,   268,   259,   145,   260,   262,    -1,   268,
     259,   145,   261,   262,    -1,   268,     1,   262,    -1,   304,
      -1,   169,   322,   170,    -1,   205,    -1,   138,    -1,   140,
      -1,   141,    -1,   139,    -1,   148,    -1,   158,    -1,   155,
      -1,   151,    -1,   154,    -1,   150,    -1,   160,    -1,   156,
      -1,   165,    -1,   144,    -1,   163,    -1,   164,    -1,   157,
      -1,   149,    -1,   137,    -1,    19,    -1,   152,    -1,    12,
      -1,   166,    -1,   153,    -1,    53,    -1,   142,    -1,   206,
     137,    -1,   122,    -1,   133,    -1,   130,    -1,   132,    -1,
     126,    -1,   131,    -1,   127,    -1,   123,    -1,   124,    -1,
     125,    -1,   136,    -1,   135,    -1,    -1,   169,   264,   170,
      -1,   169,   264,   170,    -1,    -1,   265,    -1,   264,   143,
     265,    -1,   266,   206,   296,   289,    -1,   182,   266,   206,
     296,   289,    -1,   266,   206,   296,   275,    -1,   182,   266,
     206,   296,   275,    -1,   266,   169,   288,   170,   296,   289,
      -1,   266,   169,   288,   170,   296,   275,    -1,    -1,   267,
      -1,    46,    -1,    50,    -1,    73,    -1,    27,    -1,    27,
      46,    -1,    27,    87,    -1,    76,    -1,    87,    -1,   106,
      -1,    -1,    76,    -1,    87,    -1,    27,    87,    -1,    27,
      -1,   106,    -1,    81,    -1,    52,    -1,    71,    -1,    -1,
      27,    -1,    27,    87,    -1,    87,    -1,    76,    -1,   106,
      -1,    -1,   102,    -1,   162,    -1,   273,    -1,   191,    -1,
     212,    -1,     4,    -1,   147,    -1,   147,   322,    -1,   147,
     274,    -1,    -1,   115,   322,    -1,    59,   277,    -1,   115,
     322,    59,   277,    -1,    59,   277,   115,   322,    -1,   278,
      -1,   277,   143,   278,    -1,   279,   122,   279,    -1,   279,
     154,   279,    -1,   279,   155,   279,    -1,   279,   148,   279,
      -1,   279,   150,   279,    -1,   279,   151,   279,    -1,    89,
     279,    -1,     3,    -1,   100,    -1,   106,   281,   162,    -1,
      26,   106,   281,   162,    -1,    38,   106,   281,   162,    -1,
     206,   282,    -1,   206,   282,   143,   281,    -1,    -1,   122,
     305,    -1,   122,   293,    -1,    76,    -1,    27,    -1,    87,
      -1,    27,    87,    -1,   112,    -1,    26,   283,   285,   162,
      -1,   283,   285,   162,    -1,   286,    -1,   285,   143,   286,
      -1,   206,   292,   289,    -1,   169,   288,   170,   292,   289,
      -1,   108,    -1,   206,    -1,   169,   288,   170,    -1,   287,
      -1,   287,   143,    -1,   287,   143,   288,    -1,    -1,   122,
      66,    -1,   122,   324,    -1,   171,   172,   305,    -1,   171,
     172,    -1,   171,   297,   172,   305,    -1,   171,   297,   172,
      -1,   171,   172,   290,    -1,   171,   297,   172,   290,    -1,
     171,     1,   172,    -1,    -1,   142,   305,    -1,   142,   290,
      -1,   142,   210,    -1,     1,    -1,    -1,   142,   305,    -1,
     142,   293,    -1,   142,   210,    -1,     1,    -1,   171,   297,
     172,   305,    -1,   171,   297,   172,   293,    -1,   171,   297,
      46,   322,   172,   305,    -1,   171,     1,   172,    -1,    -1,
     305,    -1,   274,    -1,   171,   172,   294,    -1,   171,   297,
     172,   294,    -1,   171,   172,   295,    -1,   171,   297,   172,
     295,    -1,   171,   297,    46,   322,   172,   294,    -1,    -1,
     142,   305,    -1,   142,   274,    -1,   142,   210,    -1,   142,
     295,    -1,   322,    -1,   274,    -1,   297,   143,   322,    -1,
     297,   143,   274,    -1,   322,    -1,   298,   143,   322,    -1,
     108,    -1,   324,    -1,   274,    -1,   299,   143,   299,    -1,
     300,   143,   299,    -1,    -1,   302,    -1,   303,    -1,   302,
     143,   303,    -1,   207,   122,   274,    -1,   207,   122,   324,
      -1,   274,    -1,   324,    -1,   207,    -1,   209,    -1,   306,
      -1,   306,   161,    -1,   161,    -1,   309,    -1,   325,    -1,
     308,    -1,   335,    -1,   334,    -1,    94,   322,    -1,    48,
     169,   301,   170,    -1,    33,   169,   301,   170,    -1,    97,
     169,   301,   170,    -1,    95,    97,   169,   301,   170,    -1,
      14,   322,    -1,    98,   322,    -1,    75,    -1,    75,   322,
      -1,   110,    -1,   110,   322,    -1,    93,    -1,    93,   322,
      -1,    18,    -1,    18,   322,    -1,    22,    -1,    85,    -1,
      40,   322,    46,   322,    32,   322,    -1,    40,   322,    46,
     224,    32,   322,    -1,    40,   322,    32,   322,    -1,    40,
     322,    46,   322,    32,    43,   322,    99,   322,    -1,    40,
     322,    46,   224,    32,    43,   322,    99,   322,    -1,    40,
     322,    32,    43,   322,    99,   322,    -1,    41,   322,    46,
     322,    32,   322,    -1,    41,   322,    46,   224,    32,   322,
      -1,    41,   322,    32,   322,    -1,    41,   322,    46,   322,
      32,    43,   322,    99,   322,    -1,    41,   322,    46,   224,
      32,    43,   322,    99,   322,    -1,    41,   322,    32,    43,
     322,    99,   322,    -1,   171,   297,   172,   322,    -1,   171,
     297,    46,   322,   172,   322,    -1,   171,   297,    46,   224,
     172,   322,    -1,   171,   297,    46,   322,   172,    43,   322,
      99,   322,    -1,   171,   297,    46,   224,   172,    43,   322,
      99,   322,    -1,    43,   322,    99,   322,    34,   322,    -1,
      65,    -1,   309,    -1,   304,    -1,   328,    -1,   327,    -1,
     251,    -1,   320,    -1,   321,    -1,   318,   153,   322,    -1,
      -1,   312,    -1,   117,   169,   313,   170,    -1,   316,    -1,
     313,   143,   316,    -1,   117,   169,   315,   170,    -1,   316,
      -1,   315,   143,   316,    -1,   317,   304,   292,   289,    -1,
     338,    86,   304,    -1,   322,    86,   304,    -1,    27,    -1,
      46,    -1,    27,    46,    -1,    87,    -1,    27,    87,    -1,
     112,    -1,   325,    -1,   318,   153,   322,    -1,    64,    -1,
      64,    75,    -1,    64,    93,    -1,    64,   110,    -1,    64,
      18,    -1,   319,   322,    -1,    64,    75,   169,   322,   170,
     169,   301,   170,    -1,    64,    93,   169,   322,   170,   169,
     301,   170,    -1,    64,    75,   169,   322,   170,   169,   301,
     170,   161,    -1,    64,    93,   169,   322,   170,   169,   301,
     170,   161,    -1,    58,   285,    46,   322,    -1,   332,    -1,
     305,    -1,   307,    -1,   336,    -1,   337,    -1,   251,    -1,
     320,    -1,   321,    -1,   233,    -1,   169,   147,   322,   170,
      -1,   322,   142,   322,    -1,   322,   146,   322,    -1,   322,
     174,   322,    -1,   322,   146,    -1,   146,   322,    -1,   174,
     322,    -1,   146,    -1,    -1,   322,    -1,   104,   322,    -1,
     105,   322,    -1,   322,    -1,   304,    -1,   327,    -1,   328,
      -1,   329,    -1,   325,    -1,   322,   137,    -1,   306,   161,
      -1,   251,    -1,   331,    -1,   326,   169,   301,   170,    -1,
     326,   171,   301,   172,    -1,    79,   169,   301,   170,    -1,
     322,   145,   207,    -1,   322,   145,   106,    -1,   322,   145,
      33,    -1,   322,   145,    61,    -1,   322,   145,    20,   169,
     170,    -1,   169,   299,   170,    -1,   169,   299,   143,   170,
      -1,   169,   300,   170,    -1,   169,   300,   143,   170,    -1,
      39,    -1,   103,    -1,     8,    -1,     9,    -1,   330,    -1,
     331,    -1,     5,    -1,     6,    -1,     7,    -1,    10,    -1,
      67,    -1,   167,   297,   168,    -1,   167,   297,   143,   168,
      -1,   171,   297,   172,    -1,   171,   297,   143,   172,    -1,
     171,   333,   172,    -1,   171,   333,   143,   172,    -1,   322,
     120,   322,    -1,   333,   143,   322,   120,   322,    -1,   322,
     160,   322,    -1,   322,   156,   322,    -1,   322,   165,   322,
      -1,   322,   144,   322,    -1,   322,   163,   322,    -1,   322,
     164,   322,    -1,   322,   157,   322,    -1,   322,   148,   322,
      -1,   322,   158,   322,    -1,   322,   155,   322,    -1,   322,
     151,   322,    -1,   322,   154,   322,    -1,   322,   150,   322,
      -1,   322,   138,   322,    -1,   322,   140,   322,    -1,   322,
     141,   322,    -1,   322,   121,   322,    -1,   322,   159,   322,
      -1,   322,   149,   322,    -1,   322,    19,   322,    -1,   322,
      12,   322,    -1,   322,   152,   322,    -1,   322,    31,   322,
      -1,   160,   322,    -1,   156,   322,    -1,    62,   322,    -1,
      77,   322,    -1,   137,   322,    -1,   322,   137,    -1,   139,
     322,    -1,   322,    86,   322,    -1,   322,    86,   224,    -1,
     338,    86,   322,    -1,   338,    86,   224,    -1,   322,    90,
     322,    -1,   322,    90,   224,    -1,   338,    90,   322,    -1,
     338,    90,   224,    -1,   160,    -1,   165,    -1,   121,    -1,
     159,    -1,   138,    -1,   140,    -1,   141,    -1
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
    1026,  1027,  1033,  1039,  1045,  1051,  1058,  1065,  1069,  1076,
    1080,  1081,  1082,  1083,  1085,  1086,  1087,  1088,  1090,  1092,
    1094,  1096,  1101,  1102,  1106,  1108,  1116,  1117,  1122,  1127,
    1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,
    1138,  1139,  1146,  1147,  1148,  1149,  1158,  1159,  1163,  1165,
    1168,  1174,  1176,  1179,  1185,  1188,  1189,  1190,  1191,  1192,
    1193,  1197,  1198,  1202,  1203,  1204,  1208,  1209,  1213,  1216,
    1218,  1223,  1224,  1228,  1230,  1232,  1239,  1249,  1263,  1268,
    1273,  1281,  1282,  1287,  1288,  1290,  1295,  1311,  1318,  1327,
    1335,  1339,  1346,  1347,  1352,  1357,  1351,  1384,  1387,  1391,
    1399,  1409,  1398,  1448,  1452,  1457,  1461,  1466,  1473,  1474,
    1478,  1479,  1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,
    1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,
    1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1509,  1510,
    1511,  1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,  1520,
    1524,  1525,  1529,  1533,  1534,  1535,  1539,  1541,  1543,  1545,
    1547,  1549,  1554,  1555,  1559,  1560,  1561,  1562,  1563,  1564,
    1565,  1566,  1567,  1571,  1572,  1573,  1574,  1575,  1576,  1580,
    1581,  1582,  1586,  1587,  1588,  1589,  1590,  1591,  1595,  1596,
    1599,  1600,  1604,  1605,  1609,  1613,  1614,  1615,  1623,  1624,
    1626,  1628,  1630,  1635,  1637,  1642,  1643,  1644,  1645,  1646,
    1647,  1648,  1652,  1654,  1659,  1661,  1663,  1668,  1681,  1698,
    1699,  1701,  1706,  1707,  1708,  1709,  1710,  1714,  1720,  1728,
    1729,  1737,  1739,  1744,  1746,  1748,  1753,  1755,  1757,  1764,
    1765,  1766,  1771,  1773,  1775,  1779,  1783,  1785,  1789,  1797,
    1798,  1799,  1800,  1801,  1806,  1807,  1808,  1809,  1810,  1830,
    1834,  1838,  1846,  1853,  1854,  1855,  1859,  1861,  1867,  1869,
    1871,  1876,  1877,  1878,  1879,  1880,  1886,  1887,  1888,  1889,
    1893,  1894,  1898,  1899,  1900,  1904,  1905,  1909,  1910,  1914,
    1915,  1919,  1920,  1921,  1922,  1926,  1927,  1938,  1940,  1942,
    1948,  1949,  1950,  1951,  1952,  1953,  1955,  1957,  1959,  1961,
    1963,  1965,  1968,  1970,  1972,  1974,  1976,  1978,  1980,  1982,
    1985,  1987,  1992,  1994,  1996,  1998,  2000,  2002,  2004,  2006,
    2008,  2010,  2012,  2014,  2016,  2023,  2029,  2035,  2041,  2050,
    2060,  2068,  2069,  2070,  2071,  2072,  2073,  2074,  2075,  2080,
    2081,  2085,  2089,  2090,  2094,  2098,  2099,  2103,  2107,  2111,
    2118,  2119,  2120,  2121,  2122,  2123,  2127,  2128,  2133,  2135,
    2139,  2143,  2147,  2155,  2160,  2166,  2172,  2179,  2189,  2197,
    2198,  2199,  2200,  2201,  2202,  2203,  2204,  2205,  2206,  2208,
    2210,  2212,  2227,  2229,  2231,  2233,  2238,  2239,  2243,  2244,
    2245,  2249,  2250,  2251,  2252,  2261,  2262,  2263,  2264,  2265,
    2269,  2270,  2271,  2275,  2276,  2277,  2278,  2279,  2287,  2288,
    2289,  2290,  2294,  2295,  2299,  2300,  2304,  2305,  2306,  2307,
    2308,  2309,  2310,  2311,  2313,  2315,  2316,  2317,  2321,  2329,
    2330,  2334,  2335,  2336,  2337,  2338,  2339,  2340,  2341,  2342,
    2343,  2344,  2345,  2346,  2347,  2348,  2349,  2350,  2351,  2352,
    2353,  2354,  2355,  2356,  2361,  2362,  2363,  2364,  2365,  2366,
    2367,  2371,  2372,  2373,  2374,  2378,  2379,  2380,  2381,  2386,
    2387,  2388,  2389,  2390,  2391,  2392
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
  "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE", "TFOR", "TFORALL",
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
     425,   426,   427,   428,   429,   430,   431,   432
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   178,   179,   180,   180,   181,   181,   182,   182,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   184,   185,   185,   185,   186,   186,   186,
     187,   187,   188,   188,   188,   190,   189,   191,   191,   191,
     192,   192,   193,   193,   193,   193,   194,   194,   194,   194,
     194,   194,   195,   195,   196,   196,   197,   197,   197,   198,
     198,   198,   198,   198,   198,   198,   199,   200,   200,   200,
     201,   201,   202,   203,   203,   203,   203,   203,   203,   204,
     204,   205,   205,   205,   205,   205,   205,   206,   206,   206,
     206,   206,   206,   207,   207,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   210,   210,   210,   210,
     211,   211,   212,   212,   213,   213,   213,   214,   213,   215,
     215,   215,   215,   215,   215,   215,   216,   216,   216,   216,
     218,   217,   219,   217,   220,   217,   221,   217,   217,   217,
     217,   217,   222,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   224,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   226,   226,   227,   227,   228,   228,   229,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   231,   231,   231,   231,   231,   231,   232,   232,
     232,   233,   233,   233,   234,   235,   235,   235,   235,   235,
     235,   236,   236,   237,   237,   237,   238,   238,   239,   240,
     240,   241,   241,   242,   242,   242,   243,   243,   244,   244,
     244,   245,   245,   246,   246,   246,   247,   247,   248,   249,
     249,   249,   250,   250,   252,   253,   251,   254,   254,   254,
     256,   257,   255,   258,   258,   258,   258,   258,   259,   259,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     262,   262,   263,   264,   264,   264,   265,   265,   265,   265,
     265,   265,   266,   266,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   268,   268,   268,   268,   268,   268,   269,
     269,   269,   270,   270,   270,   270,   270,   270,   271,   271,
     272,   272,   273,   273,   274,   275,   275,   275,   276,   276,
     276,   276,   276,   277,   277,   278,   278,   278,   278,   278,
     278,   278,   279,   279,   280,   280,   280,   281,   281,   282,
     282,   282,   283,   283,   283,   283,   283,   284,   284,   285,
     285,   286,   286,   287,   287,   287,   288,   288,   288,   289,
     289,   289,   290,   290,   290,   290,   290,   290,   290,   291,
     291,   291,   291,   291,   292,   292,   292,   292,   292,   293,
     293,   293,   293,   294,   294,   294,   295,   295,   295,   295,
     295,   296,   296,   296,   296,   296,   297,   297,   297,   297,
     298,   298,   299,   299,   299,   300,   300,   301,   301,   302,
     302,   303,   303,   303,   303,   304,   304,   305,   305,   305,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   308,
     309,   310,   310,   310,   310,   310,   310,   310,   310,   311,
     311,   312,   313,   313,   314,   315,   315,   316,   316,   316,
     317,   317,   317,   317,   317,   317,   318,   318,   319,   319,
     319,   319,   319,   320,   320,   320,   320,   320,   321,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   323,   323,   324,   324,
     324,   325,   325,   325,   325,   326,   326,   326,   326,   326,
     327,   327,   327,   328,   328,   328,   328,   328,   329,   329,
     329,   329,   330,   330,   331,   331,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   333,
     333,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   335,   335,   335,   335,   335,   335,
     335,   336,   336,   336,   336,   337,   337,   337,   337,   338,
     338,   338,   338,   338,   338,   338
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
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     3,     0,     1,     3,     4,     5,     4,     5,
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
       3,     0,     0,     1,     0,   113,   628,   629,   630,   624,
     625,   631,     0,   549,    99,   134,   518,   141,   520,   549,
       0,   140,     0,   423,    99,     0,     0,     0,   246,   135,
     596,   596,   622,     0,     0,     0,     0,   139,    55,   247,
     298,   136,     0,     0,     0,   294,     0,     0,   143,     0,
     568,   540,   632,   144,     0,   299,   512,   422,     0,     0,
       0,   157,   297,   138,   521,   424,     0,     0,     0,     0,
     516,     0,     0,   142,     0,     0,   114,     0,   623,     0,
       0,     0,   137,   280,   514,   426,   145,     0,     0,   681,
       0,   683,     0,   684,   685,   595,     0,   682,   679,   499,
     154,   680,     0,     0,     0,     0,     4,     0,     5,     0,
       9,    50,    10,    11,     0,    12,    13,    14,    16,   495,
     496,    26,    15,   155,   164,   165,    17,    21,    18,    20,
       0,   241,    19,   587,    23,    24,    25,    22,   163,     0,
     161,     0,   584,     0,   159,   162,     0,   160,   601,   580,
     497,   581,   502,   500,     0,     0,     0,   585,   586,     0,
     501,     0,   602,   603,   604,   626,   627,   579,   504,   503,
     582,   583,     0,    42,    28,   510,     0,     0,   550,   100,
       0,     0,   520,   135,     0,     0,     0,     0,   521,     0,
       0,     0,     0,   584,   601,   500,   585,   586,   519,   501,
     602,   603,     0,   549,     0,     0,   425,     0,   254,     0,
     480,     0,   487,   624,   521,   597,   297,   624,   182,   521,
       0,   297,     0,     0,     0,     0,     0,     0,     0,   169,
       0,     0,     0,     0,     0,    52,   487,   107,   115,   127,
     121,   120,   129,   110,   119,   130,   116,   131,   108,   132,
     125,   118,   126,   124,   122,   123,   109,   111,   117,   128,
     133,     0,   112,     0,     0,     0,     0,     0,     0,   429,
       0,   151,    36,     0,   666,   572,   569,   570,   571,     0,
     513,   667,     7,   487,   297,   278,   288,   596,   279,   156,
     394,   477,     0,   476,     0,     0,   152,   600,     0,     0,
      39,     0,   517,   505,     0,   487,    40,   511,     0,   261,
     257,     0,   501,   261,   258,     0,   419,     0,   515,     0,
       0,   668,   670,   593,   665,   664,     0,    57,    60,     0,
       0,   482,     0,   484,     0,     0,   483,     0,     0,   476,
       0,   594,     0,     6,     0,    51,     0,     0,     0,     0,
     281,     0,   380,   381,   379,   300,     0,   498,    27,     0,
     573,     0,     0,     0,     0,     0,     0,   669,     0,     0,
       0,     0,     0,     0,   592,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   338,   345,   346,   347,   342,   344,     0,     0,   340,
     343,   341,   339,     0,   349,   348,     0,     0,   487,   487,
       0,     0,     0,    29,    30,     0,     0,     0,     0,     0,
       0,     0,    31,     0,     0,     0,     0,    32,     0,    33,
       0,   495,   493,     0,   488,   489,   494,   176,     0,   179,
       0,   172,     0,     0,   178,     0,     0,     0,   191,     0,
       0,   190,     0,   199,     0,     0,     0,   197,     0,     0,
      72,   166,     0,     0,     0,   215,     0,   338,   211,     0,
      54,    53,    50,     0,     0,     0,   225,    34,   362,   295,
     433,     0,   434,   436,     0,   458,     0,   439,     0,     0,
     150,    35,     0,     0,    37,     0,   158,     0,    92,   598,
     599,   153,     0,    38,   487,     0,   268,   259,   255,   260,
     256,     0,   417,   414,   184,    41,    59,    58,    61,     0,
     633,     0,     0,   618,     0,   620,     0,     0,     0,     0,
       0,     0,     0,     0,   637,     8,     0,    44,     0,     0,
      90,     0,    87,     0,    66,   252,     0,     0,     0,   373,
     428,   548,   661,   660,   663,   672,   671,   676,   675,   657,
     654,   655,   656,   589,   644,   113,     0,   615,   616,   114,
     614,   613,   590,   648,   659,   653,   651,   662,   652,   650,
     642,   647,   649,   658,   641,   645,   646,   643,   591,     0,
       0,     0,     0,     0,     0,     0,     0,   674,   673,   678,
     677,   560,   561,   563,   565,     0,   552,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   635,   252,   549,
     549,   187,   415,   427,   481,     0,     0,   507,     0,     0,
     281,     0,     0,   281,   416,     0,     0,     0,     0,   524,
       0,     0,     0,   200,     0,   530,     0,     0,   198,   680,
      75,     0,    62,    73,     0,     0,     0,   214,     0,   210,
       0,     0,     0,     0,   506,     0,   228,     0,   226,   367,
     364,   365,   366,   370,   371,   372,   362,     0,   354,     0,
     363,   382,     0,   437,     0,   148,   149,   147,   146,     0,
     457,   456,   580,     0,   431,   578,   430,     0,     0,   612,
     479,   478,     0,     0,     0,   508,     0,   262,   421,   580,
       0,   634,   588,   619,   485,   621,   486,   207,     0,     0,
       0,   636,   205,   534,     0,   639,   638,     0,    46,    45,
      43,     0,    86,     0,     0,     0,    79,     0,     0,    72,
     249,     0,   282,     0,     0,   292,     0,   289,   377,   374,
     375,   378,   301,     0,     0,    98,    96,    97,    95,    94,
      93,   610,   611,   562,   564,     0,   551,   134,   141,   140,
     139,   136,   143,   144,   138,   142,   137,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   183,   491,   492,   490,   281,     0,   181,
     281,     0,   180,     0,   209,     0,     0,   189,     0,   188,
       0,   555,     0,     0,   195,     0,     0,   193,     0,     0,
     167,     0,   168,   223,   222,     0,   217,     0,     0,   213,
       0,   219,     0,   251,     0,     0,   368,   369,     0,   362,
     352,     0,   471,   383,   386,   385,   387,     0,   435,   438,
     439,     0,     0,   440,   441,     0,     0,   270,     0,     0,
     269,   272,   509,     0,   263,   266,     0,   418,   208,     0,
       0,     0,     0,   206,     0,    91,    88,     0,    69,    68,
      67,     0,     0,     0,     0,   297,   287,     0,   290,   286,
     376,   382,   350,   101,   332,   115,   330,   121,   120,   104,
     119,   116,   335,   131,   102,   132,   118,   122,   103,   105,
     117,   133,   329,   311,   314,   312,   313,   336,   324,   315,
     328,   320,   318,   331,   334,   319,   317,   322,   327,   316,
     321,   325,   326,   323,   333,     0,   310,     0,   106,     0,
     350,   350,   308,   617,   553,   439,   601,   601,     0,     0,
       0,     0,     0,     0,   251,     0,     0,     0,   186,   185,
       0,   283,     0,   283,   192,     0,     0,   523,     0,   522,
       0,   554,     0,     0,   529,   196,     0,   528,   194,    64,
      63,   216,   212,   539,   218,     0,     0,   248,   227,   224,
     471,   355,     0,     0,   439,   384,   398,   432,   462,     0,
     635,   487,   487,     0,   274,     0,     0,     0,   264,     0,
     203,   536,     0,     0,   201,   535,     0,   640,     0,     0,
       0,    72,     0,    72,    80,    83,   253,   277,   157,   297,
     276,   297,   284,   293,   291,     0,   362,   307,     0,   337,
       0,   303,   304,   557,     0,     0,     0,     0,     0,     0,
       0,     0,   253,   283,   297,   283,   297,   527,     0,     0,
     556,   533,     0,     0,     0,     0,   221,    56,   439,   471,
       0,   474,   473,   475,   580,   395,   358,   356,     0,     0,
       0,     0,   460,   580,     0,     0,   275,   273,     0,   267,
       0,   204,     0,   202,    89,    71,    70,     0,     0,     0,
       0,   250,   285,   453,     0,   388,     0,   309,   101,   103,
     350,   350,     0,     0,     0,     0,     0,     0,   297,   175,
     297,   171,     0,     0,     0,     0,    65,   220,   359,   357,
     439,   463,     0,   397,   396,   412,     0,   413,   400,   403,
       0,   399,   392,   393,   296,     0,   574,   575,   265,     0,
       0,    82,    85,    81,    84,     0,   452,   451,   580,   389,
     398,   351,   305,   306,     0,     0,     0,     0,     0,     0,
     177,   173,   526,   525,   532,   531,   361,   360,   465,   466,
     468,   580,     0,   635,   411,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   580,   576,   577,   538,   537,     0,
     443,     0,     0,     0,   467,   469,   402,   404,   405,   408,
     409,   410,   406,   407,   401,   448,   446,   580,   635,   390,
     302,   391,   463,   447,   580,   470
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   328,   107,   659,   109,   110,   111,   346,
     472,   112,   235,   113,   329,   650,   543,   654,   651,   114,
     115,   116,   540,   541,   117,   118,   180,   936,   267,   119,
     262,   120,   690,   272,   121,   122,   284,   123,   124,   125,
     442,   632,   438,   629,   126,   127,   788,   128,   233,   129,
     667,   668,   192,   131,   132,   133,   134,   135,   507,   707,
     866,   136,   137,   703,   861,   138,   139,   547,   885,   140,
     141,   746,   747,   193,   265,   681,   143,   144,   549,   891,
     752,   939,   940,   469,  1037,   479,   677,   678,   679,   680,
     753,   355,   847,  1160,  1220,  1144,   432,  1076,  1080,  1138,
    1139,  1140,   145,   317,   512,   146,   147,   268,   269,   483,
     484,   694,  1157,  1105,   487,   691,  1179,  1073,   994,   330,
     209,   334,   335,   433,   434,   435,   194,   149,   150,   151,
     152,   195,   154,   177,   178,   605,   454,   810,   606,   607,
     155,   156,   196,   197,   159,   221,   436,   199,   161,   200,
     201,   164,   165,   166,   167,   340,   168,   169,   170,   171,
     172
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1072
static const yytype_int16 yypact[] =
{
   -1072,   128,  3609, -1072,   -26,   105, -1072, -1072, -1072, -1072,
   -1072, -1072,  4985,   116,   263,   149, 14812,   223,  2365,   116,
   11491,   241,   383,   168,   263,  4985, 11491,  4985,   192, 19314,
   11661,  8415,   330,  8585,  9783,  7047,  8755,   350, -1072,   258,
   -1072,   353, 19367, 19367, 19367, -1072,  3166,  9953,   367, 11491,
     148, -1072,   448,   457, 11491, -1072, 14812, -1072, 11491,   467,
     309,   233,  1964,   460, 19420, -1072, 10125,  7903, 11491,  9953,
   14812, 11491,   439,   487,   377,  4985,   505, 11491,   507, 11831,
   11831, 19367,   512, -1072, 14812, -1072,   524, 11491, 11491, -1072,
   11491, -1072, 11491, -1072, -1072, 14339, 11491, -1072, 11491, -1072,
   -1072, -1072,  3953,  7219,  8927, 11491, -1072,  4813, -1072,   414,
   -1072,   514, -1072, -1072,    21, -1072, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
     533, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, 19367,
   -1072, 19367,   243,   351, -1072, -1072,  3166, -1072,   450, -1072,
     461, -1072, -1072,   459,   466,   489, 11491,   492,   493, 18831,
    2490,   356,   494,   496, -1072, -1072,   421, -1072, -1072, -1072,
   -1072, -1072,   483, -1072, -1072, 18831,   490,  4985, -1072, -1072,
     498, 11491, -1072, -1072, 11491, 11491, 11491, 19367, -1072, 11491,
   10125, 10125,   591,   424, -1072, -1072, -1072, -1072,   200,   444,
   -1072, -1072,   495, 16218, 19367,  3166, -1072,   499, -1072,   -45,
   18831,   548,  8075,   580, 19473, 18831,   412,   582, -1072, 19526,
   19367,   412, 19367,   501,    37, 15896,     1,  3001,   213, -1072,
   15979, 19367, 19367,     3,  3341,   244,  8075, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
   -1072,   502, -1072,   462,  4985,   503,  2123,   106,    18, -1072,
    4985, -1072, -1072, 16294,  1078, -1072,   504,   509, -1072, 16294,
     200,  1078, -1072,  8075,  2267, -1072, -1072, 10295, -1072, -1072,
   -1072, -1072,   147, 18831, 11491, 11491, -1072, 18831,   518, 16765,
   -1072, 16294,   200, 18831,   513,  8075, -1072, 18831, 16805, -1072,
   -1072, 16845,  2820, -1072, -1072, 16961,   552,   521,   200, 16294,
   17001,   411,   411,  1424,  1078,  1078,    99, -1072, -1072,  4125,
      88, -1072, 11491, -1072,   -35,    95, -1072,   -17,   119, 17047,
      92,  1424,   676, -1072,  4297, -1072,   623, 11491, 11491, 19367,
     545,   522, -1072, -1072, -1072, -1072,   312,   464, -1072, 11491,
     543, 11491, 11491, 11491,  9783,  9783, 11491,   470, 11491, 11491,
   11491, 11491, 11491,   226, 14339, 11491, 11491, 11491, 11491, 11491,
   11491, 11491, 11491, 11491, 11491, 11491, 11491, 11491, 11491, 11491,
   11491,   624, -1072, -1072, -1072, -1072, -1072,  9097,  9097, -1072,
   -1072, -1072, -1072,  9097, -1072, -1072,  9097,  9097,  8075,  8075,
    9783,  9783,  7733, -1072, -1072, 16374, 16450, 17150,   525,    70,
   19367,  4469, -1072,  9783,    37,   529,   322, -1072, 11491, -1072,
   11491,   571, -1072,   526,   554, -1072, -1072, -1072, 19367, -1072,
    3166, -1072, 19367,   539, -1072,  3166,   649, 10125, -1072,  5157,
    9783, -1072,   536, -1072,    37,  5329,  9783, -1072,    37, 12001,
   11491, -1072,   588,   590,  4985,   679,  4985, -1072,   680, 11491,
   -1072, -1072,   514,   546,  8075, 19367, -1072, -1072,   476, -1072,
   -1072,  2123, -1072,   572,   547, -1072, 12171,   596, 11491,  3166,
   -1072, -1072, 11491, 11491, -1072,   558, -1072, 10125, -1072, 18831,
   18831, -1072,    50, -1072,  8075,   560, -1072,   710, -1072,   710,
   -1072, 12341,   589, -1072, -1072, -1072, -1072, -1072, -1072,  9269,
   -1072, 17196,  7391, -1072,  7563, -1072,  4985,   561,  9783,  9441,
    3781,   562, 11491, 10465, -1072, -1072,   402, -1072,  4641, 19367,
   -1072,   336, 17241,   347, 16062,   161, 10125,   570, 19261,   242,
   -1072, 17345, 18715, 18715,   413, -1072,   413, -1072,   413, 19081,
    1734,  1371,   755,   200,   411, -1072,   573, -1072, -1072, -1072,
   -1072, -1072,  1424,  1231,   413, 14496, 14496, 18715, 14496, 14496,
     931,   411,  1231, 19043,   931,  1078,  1078,   411,  1424,   578,
     579,   581,   584,   586,   587,   592,   594, -1072,   413, -1072,
     413,   199, -1072, -1072, -1072,   127, -1072,  1246, 18871,   485,
   12511,  9783, 12681,  9783, 11491,  8075,  9783, 15082,   583,   116,
   17390, -1072, -1072, -1072, 18831, 17430,  8075, -1072,  8075, 19367,
     545,   357, 19367,   545, -1072,   380, 11491,   212,  8755, 18831,
      53, 16530,  7733, -1072,  8755, 18831,    17, 16138, -1072,   597,
     607,   601, 17546,   607,   608, 11491, 11491,   722,  4985,   739,
   17586,  4985, 16490,   711, -1072,   236, -1072,   238, -1072,   201,
   -1072, -1072, -1072, -1072, -1072, -1072,  1071,   249, -1072,  3289,
   -1072,   390,   605,  2123,   106,    -9,     6, 11491, 11491,  6877,
   -1072, -1072,   577,  8245, -1072, 18831, -1072, 17626, 17702, -1072,
   -1072, 18831,   612,    29,   611, -1072,  2654, -1072, -1072,   385,
   19367, -1072, -1072, -1072, -1072, -1072, -1072, -1072,  4985,     4,
   16606, -1072, -1072, 18831,  4985, 18831, -1072, 17783, -1072, -1072,
   -1072, 11491, -1072,    85,    86, 11491, -1072, 10635, 12001, 11491,
   -1072,  8075,   640,  1199,   616,   663,   130, -1072,   701, -1072,
   -1072, -1072, -1072, 14174,   619, -1072, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072,  7733, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,    73,  9783,
    9783, 11491,   758, 17859, 11491,   761, 17939,   259,   625, 17899,
    8075,    37,    37, -1072, -1072, -1072, -1072,   545,   631, -1072,
     545,   635, -1072, 16294, -1072, 15264,  5501, -1072,  5673, -1072,
     266, -1072, 15340,  5845, -1072,    37,  6017, -1072,    37, 11491,
   -1072, 11491, -1072, 18831, 18831,  4985, -1072,  4985, 11491, -1072,
    4985,   772, 19367,   648, 19367,   495, -1072, -1072, 19367,  1242,
   -1072,  2123,   669,   725, -1072, -1072, -1072,    52, -1072, -1072,
     596,   641,    78, -1072, -1072,   645,   646, -1072,  6189, 10125,
   -1072, -1072, -1072, 19367, -1072,   677,   495, -1072, -1072,  6361,
     650,  6533,   651, -1072, 11491, -1072, -1072, 11491, 18015,    87,
   16689,   656,   658,   281,   657,  1631, -1072, 11491, 19367, -1072,
   -1072,   390,   659,   368, -1072,   681, -1072,   682,   685,   695,
     688,   690, -1072,   691,   700,   694,   698,   703,   392,   712,
     705,   708, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072, 11491, -1072,   717,   718,   713,
     659,   659, -1072, -1072, -1072,   596,   282,   313, 18055, 12851,
   13021, 18095, 13191, 13361, -1072, 13531, 13701,   317, -1072, -1072,
     673, -1072,   693, -1072, -1072,  4985,  8755, 18831,  8755, 18831,
    7733, -1072,  4985,  8755, 18831, -1072,  8755, 18831, -1072, 18171,
   18831, -1072, -1072, 18831,   827,  4985,   702, -1072, -1072, -1072,
     669, -1072,   696, 10807,   298, -1072,    35, -1072, -1072,  9783,
   14947,  8075,  8075,  4985, -1072,    38,   697, 11491, -1072,  8755,
   -1072, 18831,  4985,  8755, -1072, 18831,  4985, 18831,   178, 10977,
   12001, 11491, 12001, 11491, -1072, -1072,   707, -1072, -1072,  2267,
   -1072, 19221, -1072, 18831, -1072,   137,   476, -1072, 18211, -1072,
   15181, -1072, -1072, -1072, 11491, 11491, 11491, 11491, 11491, 11491,
   11491, 11491, -1072, -1072,  1720, -1072,  2754, 17586, 15422, 15498,
   -1072, 17586, 15580, 15656, 11491,  4985, -1072, -1072,   298,   669,
    9613, -1072, -1072, -1072,   138, 10125, -1072, -1072,    40, 11491,
     -28, 18251, -1072,  1378,   704,   706,   548, -1072,   495, 18831,
   15738, -1072, 15814, -1072, -1072, -1072, 18831,   709,   715,   716,
     720, -1072, -1072, -1072, 13871,   770,   324, -1072,   726,   736,
     659,   659, 18327, 18367, 18443, 18483, 18559, 18599, 19011, -1072,
   19028, -1072,  4985,  4985,  4985,  4985, 18831, -1072, -1072, -1072,
     298, 11149,    79, -1072, 18831, -1072,    93, -1072,   -10, -1072,
     429, 18675, -1072, -1072, -1072, 13701,   719,   724, -1072,  4985,
    4985, -1072, -1072, -1072, -1072,  6705, -1072, -1072,   609, -1072,
      35, -1072, -1072, -1072, 11491, 11491, 11491, 11491, 11491, 11491,
   -1072, -1072, 17586, 17586, 17586, 17586, -1072, -1072, -1072, -1072,
   -1072,   158,  9783, 14508, -1072, 11491,    40,    93,    93,    93,
      93,    93,    93,    40,  1389, -1072, -1072, 17586, 17586,   729,
   14041,    91,   155, 18755, -1072, -1072, 18831, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072,   740, -1072, -1072,   620, 14677, -1072,
   -1072, -1072, 11321, -1072,   662, -1072
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1072, -1072, -1072,     5,  -416,  2563, -1072, -1072, -1072,   407,
   -1072, -1072, -1072,   384,   544,  -447, -1072,  -711,  -709, -1072,
   -1072, -1072,   156, -1072, -1072,   227,   865, -1072,   664,  -202,
    -737, -1072,  -922,   114, -1048,  -844, -1072,   -57, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072, -1072,   -22, -1072, -1072, -1072,
   -1072,    56,  1157, -1072, -1072, -1072, -1072, -1072,   585, -1072,
      28, -1072, -1072, -1072, -1072, -1072, -1072,  -583,  -864, -1072,
   -1072, -1072,    14,  1381, -1072, -1072, -1072,   252, -1072, -1072,
   -1072, -1072,  -137,  -159,  -902, -1072,  -129,    69,   234, -1072,
   -1072, -1072,    20, -1072, -1072,  -293,    11,  -981,  -247,  -279,
    -271,  -539, -1072,  -195, -1072,     9,   881,  -126,   433, -1072,
    -451,  -814, -1070, -1072,  -636,  -496, -1071, -1027,  -881,   -64,
   -1072,   -31, -1072,  -210,  -457,   296,   819,  -366, -1072, -1072,
   -1072,  1517, -1072,   -13, -1072, -1072,  -175, -1072,  -639, -1072,
   -1072, -1072,  1595,  2070,   -12,   895,    43,   943, -1072,  2107,
    2472, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
    -385
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -636
static const yytype_int16 yytable[] =
{
     175,   407,   292,   811,   198,   289,   202,   106,   203,   425,
     431,   224,   226,   653,   210,   708,   938,   665,   215,   215,
     356,   225,   227,   230,   234,   443,   473,   609,   882,   881,
     682,   205,  1143,   270,   431,   273,   997,   274,  1041,  1042,
     338,  1032,   279,  1135,   280,  -246,   281,   798,   850,   813,
     801,   702,   458,   485,   293,   297,   299,   301,   302,   303,
    -247,    67,   676,   307,   488,   308,   347,   311,   315,   270,
     270,  1071,   318,   495,   485,   319,   320,   291,   321,   426,
     322,   431,   337,   323,   324,   806,   325,  1128,   565,   565,
     293,   297,   339,   341,  1078,   505,  1135,  1054,   428,  1056,
     452,   858,   464,   431,  1180,  1185,   566,   485,   522,  1068,
     298,  -454,  1204,   291,   333,   291,   616,   429,   452,   567,
     692,   452,  -271,  1020,   999,  1182,   944,   419,     3,  1136,
    1216,  1043,   348,  1186,   452,   523,   173,   616,  1103,   421,
    1137,  -454,   945,   859,   360,   709,   336,   568,  1223,  1176,
    1079,  1225,  -454,   407,  1143,   526,  1205,  1021,   787,  -229,
     212,   489,   527,   531,  -271,   528,   275,  -454,   421,   175,
     421,   571,   415,   416,   417,   236,   869,   307,   293,   339,
    1077,   497,  1156,   300,   421,   569,   569,  1102,  1130,  1118,
     424,  1120,   570,  1137,   486,  -454,  -449,   860,   595,   596,
     297,   291,   291,  -230,   421,   421,   431,   431,  1162,  1163,
    1032,   996,  1032,   529,   960,   486,  -454,   962,  -271,  -454,
     421,   529,   529,   276,   297,   440,  -449,   496,  -454,   565,
     445,   519,   849,   176,   529,   533,   452,   205,   524,  -449,
      23,   277,   617,  -454,    67,   763,   566,   836,   486,  -454,
    1000,  1183,  -449,   877,  1129,   206,   520,   609,   278,   567,
    -472,   173,   529,  1218,   534,   525,   179,   516,  -454,   748,
     765,   297,   431,   888,  1032,   215,  1032,  -236,   -78,  1104,
    -464,  -472,   499,   500,   883,  -472,   764,   568,   837,    57,
     497,   530,   653,   297,   704,  -235,   -49,   766,   889,  -449,
      65,  -464,   431,   938,  -449,  -464,   310,   314,  -472,   498,
    1098,  1097,  1100,  1099,   631,   -49,  1177,  1219,   749,   635,
     521,   819,   421,   740,   470,    85,   569,   471,  -464,   750,
     741,  1060,   570,   957,   518,   542,   544,   367,   448,   451,
     453,   457,   555,   557,   -78,   373,  1094,   551,   751,   552,
     553,   554,   556,   558,   559,   497,   560,   561,   562,   563,
     564,   212,   572,   573,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   587,   588,   628,
     609,   834,   804,   637,  -244,   297,   297,   491,   597,   599,
     992,   297,   839,   494,   297,   297,   297,   297,   598,   600,
     608,   619,   628,   352,  -234,  -545,   833,  -231,   835,   970,
      23,   620,  -608,   431,  -608,   503,   624,   843,   625,   840,
     693,  -238,   353,   676,   628,  -559,   431,   236,   640,   954,
    1018,   271,   354,   514,   646,   293,   971,   639,   641,    23,
     590,   591,   363,   645,   647,  1075,   592,   652,   652,   593,
     594,  1026,  -559,   271,   660,   489,  -558,   662,   291,    57,
     628,    40,   297,   309,   313,   489,   844,   839,   439,  1031,
      65,   815,   818,   444,   550,   282,   695,   845,   283,   731,
     697,   698,   742,  -558,   623,   701,    55,  1052,    57,   204,
     735,   714,   297,   716,  1161,    85,   846,   364,   732,    65,
     489,   365,  -242,   669,  1082,  -107,   719,   701,   700,   736,
     297,  -239,   297,  -113,  -233,   867,   720,   701,   723,   799,
     725,   727,   670,   489,    85,   408,   671,   409,  -420,  -109,
     700,   876,   571,   333,   293,   333,   304,  -114,   621,   431,
     700,  -237,   802,   518,   870,   872,   305,  -420,   367,   672,
     367,  1187,   673,   371,    59,   371,   373,   291,   373,  -243,
     376,  -245,   376,   674,   173,   336,  -232,   336,   643,   410,
     728,   780,   648,   411,   653,   411,   653,  1188,  -240,  1189,
    1190,   344,   675,  1191,  1192,   609,   422,   349,   431,   782,
    -609,   785,  -609,  -608,   941,  -608,   345,  1184,   639,   783,
     645,   786,   660,   297,   789,   723,   791,   792,   271,   271,
     271,   271,  -542,  -605,   297,  -605,   297,   465,   468,  -353,
     676,  -541,   357,  -455,   803,   852,   805,  1074,   358,   421,
     608,   475,   812,  -607,  1083,  -607,  -455,   794,  1031,  -606,
    1031,  -606,   359,   823,   824,   420,  -353,   476,  1208,  1209,
    1210,  1211,  1212,  1213,  -546,  -547,  -544,   271,  -543,   412,
     414,   427,   421,   271,   430,   437,  -455,   441,  -450,   795,
     447,   474,   478,   492,   511,   303,   307,   339,   493,  -442,
     501,   297,   504,   513,   535,   271,   539,   546,   373,   548,
     589,   622,  -455,   626,   615,   636,   627,   628,  -450,  -455,
     291,   634,  1031,   271,  1031,   642,   261,   263,   264,  -442,
     655,  -450,   656,   658,   661,   683,   664,   684,   693,   542,
    -455,  -444,  -442,   878,  -450,   880,   652,   652,   699,   297,
     705,   706,   710,   718,   724,  -442,   854,   743,  1158,  -455,
     755,   756,   754,   757,  -455,   316,   758,  -455,   759,   760,
     819,  -444,   790,   608,   807,   809,   825,   555,   597,   -74,
     814,   817,   761,   820,  -444,  1181,   762,   556,   598,   948,
     822,  -450,   951,   827,   832,   848,  -450,  -444,   297,  1194,
     857,   862,  -442,   497,   886,   887,   363,  -442,   890,   943,
     949,  1084,  1085,   952,   967,  1005,   969,   955,   961,   431,
     431,   974,   963,   350,   977,   351,   985,   979,   271,   980,
     987,   993,   995,   998,  1001,  1002,   983,  1181,  1024,  1007,
    1025,   148,  1012,  1016,  -444,  1027,  -134,  -141,  1036,  -444,
    -140,   148,  -110,  -139,  1217,  -136,  -143,  -108,   271,  -144,
    1053,   364,   271,  -138,   148,   365,   148,   293,  -142,  -111,
    -137,   418,  1224,  -145,  1039,  -112,  1181,  1011,  1040,  1015,
    1055,  1065,  1017,  -107,  1067,   652,  1069,  1088,   316,  1101,
     291,  1151,  1159,  -109,  1146,  1033,  1147,  1152,  1153,   663,
    1195,  1111,  1154,  1186,   316,  1196,   446,   875,   538,   207,
     988,  1006,   367,   368,   148,   462,   463,   371,   509,   372,
     373,  1215,  1034,  1110,   376,   958,   959,  1106,   991,  1221,
     838,  1035,   383,  1202,  1214,  1207,   229,   964,   387,   388,
     389,   148,   696,  1038,   796,   216,   148,     0,     0,   975,
     482,     0,   978,     0,     0,     0,     0,   967,   969,     0,
     974,   977,     0,  1011,  1015,   160,     0,     0,     0,     0,
       0,     0,     0,  1057,  1058,   160,  1059,     0,   608,     0,
    1061,  1062,   363,     0,  1063,     0,     0,     0,   160,     0,
     160,     0,   289,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1081,   723,   297,
     297,     0,     0,     0,     0,  1089,   148,  1090,     0,     0,
       0,  1092,     0,     0,  1072,     0,  1132,  1096,   652,   652,
     652,   652,     0,   545,     0,     0,     0,   364,   160,     0,
       0,   365,   312,   312,   271,   271,     0,     0,     0,     0,
     271,   271,  1057,  1112,  1113,  1061,  1114,  1115,  1116,  1117,
       0,     0,     0,     0,     0,   160,   831,     0,     0,     0,
     160,     0,  1126,     0,     0,     0,     0,     0,   339,     0,
       0,     0,     0,  1134,     0,     0,     0,  1141,   367,   368,
       0,   369,   370,   371,     0,   372,   373,     0,     0,     0,
     376,   291,     0,   148,   618,     0,  1133,     0,   383,   148,
     864,  1201,     0,     0,   387,   388,   389,     0,   669,     0,
       0,     0,   630,     0,     0,     0,   633,     0,     0,   363,
    1172,  1173,  1174,  1175,     0,     0,     0,   670,     0,  1087,
     160,   671,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1015,     0,     0,     0,  1197,  1198,   666,
       0,     0,  1178,   339,   672,   482,     0,   673,   148,   342,
       0,     0,  1172,  1173,  1174,  1175,  1197,  1198,   674,   130,
       0,     0,     0,   148,   364,     0,   291,     0,   365,   130,
    1203,   723,     0,  1206,     0,   271,   271,   675,     0,     0,
       0,     0,   130,     0,   130,     0,     0,   271,     0,   468,
       0,     0,     0,     0,  1178,     0,   468,     0,     0,   271,
     884,     0,   271,   730,     0,     0,   723,   160,     0,     0,
    1015,     0,   745,   160,     0,   367,     0,     0,     0,   989,
     371,  -283,   372,   373,     0,  -283,  -283,   376,     0,     0,
       0,     0,   130,  1178,  -283,   383,  -283,  -283,     0,     0,
     148,  -283,     0,   389,     0,     0,     0,     0,  -283,   565,
    1008,  -283,     0,     0,     0,     0,     0,     0,     0,   130,
       0,     0,   363,   767,   130,     0,   768,     0,   148,   669,
    -283,   769,   160,  -283,   148,  -283,     0,  -283,     0,  -283,
    -283,   183,  -283,   148,  -283,   148,  -283,   160,   670,     0,
     770,     0,   671,   797,     0,     0,   800,   771,     0,     0,
       0,     0,     0,     0,     0,  -283,     0,   772,  -283,     0,
       0,  -283,     0,     0,   773,   672,     0,   364,   673,     0,
      59,   365,     0,     0,     0,     0,     0,     0,     0,   674,
     774,     0,     0,     0,   130,     0,     0,     0,     0,     0,
       0,     0,   775,   842,     0,   148,   569,   482,   675,   148,
       0,     0,     0,   776,     0,     0,     0,   148,     0,   777,
       0,  -283,     0,     0,   160,     0,     0,  -283,   367,   368,
     865,   369,   370,   371,   316,   372,   373,   374,     0,     0,
     376,   377,   378,   142,     0,   380,   381,   382,   383,   271,
       0,   386,   160,   142,   387,   388,   389,     0,   160,     0,
       0,     0,   363,     0,     0,   390,   142,   160,   142,   160,
       0,     0,     0,     0,     0,     0,     0,   937,     0,     0,
       0,   130,     0,     0,  -459,     0,   778,   130,     0,     0,
       0,     0,     0,     0,     0,  -461,     0,  -459,     0,     0,
       0,     0,   468,   468,     0,     0,   468,   468,  -461,     0,
       0,     0,     0,     0,     0,   363,   142,   364,     0,     0,
       0,   365,     0,     0,  1142,     0,     0,  -459,     0,   160,
       0,     0,  1148,   160,   468,     0,   468,   148,  -461,     0,
     148,   160,     0,   142,     0,     0,   130,     0,   142,     0,
       0,     0,     0,  -459,     0,     0,   986,     0,   666,     0,
    -459,   130,   990,     0,  -461,   482,     0,     0,   367,   368,
     364,  -461,   370,   371,   365,   372,   373,     0,     0,   153,
     376,  -459,     0,     0,     0,     0,     0,   865,   383,   153,
       0,     0,  -461,     0,   387,   388,   389,   148,     0,     0,
    -459,     0,   153,   148,   153,  -459,     0,     0,  -459,     0,
       0,  -461,   745,     0,     0,     0,  -461,     0,   142,  -461,
       0,   367,   368,     0,   369,   370,   371,     0,   372,   373,
       0,     0,   942,   376,     0,     0,     0,     0,   130,     0,
     382,   383,     0,     0,   386,     0,  1142,   387,   388,   389,
       0,     0,   153,     0,     0,     0,     0,   157,   946,   947,
       0,   160,     0,     0,   160,     0,   130,   157,     0,     0,
       0,     0,   130,     0,     0,     0,     0,     0,     0,   153,
     157,   130,   157,   130,   153,   148,     0,   148,     0,     0,
       0,     0,   148,     0,     0,   148,     0,     0,     0,     0,
       0,     0,     0,     0,   148,   142,   148,     0,     0,   148,
       0,   142,     0,   285,     0,     0,     0,    22,    23,     0,
       0,   160,     0,     0,     0,     0,   286,   160,    30,   287,
     157,     0,     0,    35,     0,     0,     0,   148,     0,     0,
      40,     0,     0,   130,     0,     0,     0,   130,   148,     0,
     148,     0,     0,     0,   153,   130,     0,   157,     0,     0,
       0,     0,   157,     0,   937,    55,     0,    57,     0,    59,
     142,  1028,     0,     0,  1029,     0,   288,     0,    65,     0,
       0,     0,     0,     0,     0,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    81,     0,     0,
      83,     0,   285,    85,     0,     0,    22,    23,     0,   160,
       0,   160,     0,     0,     0,   286,   160,    30,   287,   160,
       0,     0,    35,     0,     0,   363,     0,     0,   160,    40,
     160,     0,   157,   160,     0,     0,     0,     0,     0,     0,
       0,   153,     0,     0,   148,     0,     0,   153,     0,     0,
       0,   148,     0,   100,    55,     0,    57,     0,    59,  1030,
    1028,   160,   142,  1029,   148,   288,     0,    65,     0,     0,
       0,     0,   160,     0,   160,   130,     0,     0,   130,     0,
     364,     0,   148,     0,   365,     0,    81,     0,     0,    83,
     142,   148,    85,     0,     0,   148,   142,     0,     0,     0,
       0,     0,     0,     0,     0,   142,   153,   142,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   157,
       0,   153,     0,     0,     0,   157,     0,     0,     0,     0,
       0,   367,     0,     0,     0,   130,   371,     0,   372,   373,
       0,   130,   100,   376,   148,     0,     0,     0,  1119,     0,
       0,   383,     0,     0,     0,     0,     0,   387,   388,   389,
       0,     0,     0,     0,     0,     0,     0,   142,   160,     0,
       0,   142,     0,     0,     0,   160,     0,     0,     0,   142,
       0,     0,     0,     0,   157,     0,     0,     0,   160,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   153,   157,
       0,   148,   148,   148,   148,     0,   160,     0,     0,     0,
       0,     0,     0,     0,     0,   160,     0,     0,     0,   160,
       0,     0,     0,   130,     0,   130,   153,     0,   148,   148,
     130,     0,   153,   130,     0,     0,     0,     0,     0,     0,
       0,   153,   130,   153,   130,     0,   285,   130,     0,     0,
      22,    23,     0,     0,     0,     0,     0,     0,     0,   286,
       0,    30,   287,     0,     0,     0,    35,     0,   160,   -77,
       0,     0,     0,    40,     0,   130,   157,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   130,   -48,   130,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,   142,
      57,     0,   142,   153,   157,     0,   -48,   153,     0,   288,
     157,    65,     0,     0,     0,   153,     0,     0,     0,   157,
       0,   157,     0,     0,     0,   160,   160,   160,   160,     0,
      81,     0,   158,    83,     0,   -77,    85,     0,     0,     0,
       0,     0,   158,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   160,   160,     0,   158,     0,   158,     0,   142,
       0,     0,     0,     0,     0,   142,     0,     0,     0,   162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   162,
       0,   157,   130,     0,     0,   157,   237,     0,     0,   130,
       0,     0,   162,   157,   162,     0,     0,     0,     0,     0,
     238,   239,   130,   240,     0,   158,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
     130,     0,   243,     0,     0,     0,     0,   244,     0,   130,
       0,   245,   158,   130,   246,   153,     0,   158,   153,     0,
       0,     0,   162,     0,   247,     0,     0,   142,     0,   142,
     248,   249,     0,     0,   142,     0,     0,   142,   250,     0,
       0,     0,     0,     0,     0,     0,   142,   251,   142,   162,
       0,   142,     0,     0,   162,     0,   252,   253,     0,   254,
       0,   255,   130,   256,     0,     0,   257,     0,     0,     0,
     258,   480,     0,   259,     0,   153,   260,     0,     0,   142,
       0,   153,     0,     0,     0,     0,     0,   158,     0,     0,
     142,     0,   142,   157,     0,     0,   157,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   130,
     130,   130,   130,     0,   162,     0,     0,     0,     0,   285,
       0,     0,   481,    22,    23,     0,     0,     0,     0,     0,
       0,     0,   286,     0,    30,   287,   130,   130,     0,    35,
       0,     0,     0,   157,     0,     0,    40,     0,     0,   157,
       0,     0,     0,   153,     0,   153,     0,     0,     0,     0,
     153,     0,     0,   153,   158,     0,     0,     0,     0,     0,
     158,    55,   153,    57,   153,     0,   142,   153,     0,     0,
       0,     0,   288,   142,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   142,     0,  -278,     0,
       0,   162,     0,    81,     0,   153,    83,   162,     0,    85,
       0,     0,  -278,  -278,   142,  -278,   153,     0,   153,     0,
    -278,     0,     0,   142,     0,     0,     0,   142,  -278,   158,
       0,   157,     0,   157,  -278,     0,     0,     0,   157,  -278,
       0,   157,     0,  -278,   158,     0,  -278,     0,     0,     0,
     157,     0,   157,     0,     0,   157,  -278,     0,     0,     0,
       0,     0,  -278,  -278,     0,     0,   162,     0,     0,     0,
    -278,     0,     0,     0,     0,     0,   142,     0,     0,  -278,
       0,   162,     0,   157,     0,     0,     0,     0,  -278,  -278,
       0,  -278,     0,  -278,   157,  -278,   157,     0,  -278,     0,
       0,     0,  -278,     0,   163,  -278,     0,     0,  -278,     0,
       0,     0,   153,     0,   163,     0,     0,     0,     0,   153,
       0,   158,     0,     0,     0,     0,     0,   163,     0,   163,
       0,     0,   153,   142,   142,   142,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   158,
     153,     0,     0,     0,     0,   158,     0,     0,   162,   153,
     142,   142,     0,   153,   158,     0,   158,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   163,     0,     0,
       0,     0,     0,     0,     0,     0,   162,     0,     0,     0,
     157,     0,   162,     0,     0,   108,     0,   157,     0,     0,
       0,   162,     0,   162,   163,   174,     0,     0,     0,   163,
     157,     0,   153,     0,     0,     0,     0,     0,   208,     0,
     211,     0,     0,     0,     0,     0,   158,     0,   157,     0,
     158,     0,     0,     0,     0,     0,     0,   157,   158,     0,
       0,   157,   391,   392,   393,   394,   395,   396,   397,   398,
     399,   400,   401,   402,   403,   404,   405,     0,     0,     0,
       0,     0,     0,   162,     0,     0,     0,   162,   306,   153,
     153,   153,   153,  -566,     0,   162,     0,     0,     0,   163,
       0,     0,     0,     0,     0,     0,   406,   237,     0,  -605,
     157,  -605,     0,     0,     0,   108,   153,   153,     0,     0,
     343,   238,   239,     0,   240,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
       0,     0,     0,   243,     0,     0,     0,     0,   244,     0,
       0,     0,   245,     0,     0,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   247,     0,   157,   157,   157,
     157,   248,   249,     0,     0,     0,     0,     0,   158,   250,
       0,   158,     0,     0,     0,     0,   163,     0,   251,     0,
     413,     0,   163,     0,   157,   157,     0,   252,   253,     0,
     254,     0,   255,     0,   256,     0,     0,   257,     0,     0,
       0,   258,     0,     0,   259,   162,     0,   260,   162,     0,
       0,     0,     0,     0,     0,     0,   285,     0,     0,     0,
      22,    23,     0,     0,     0,     0,     0,     0,   158,   286,
       0,    30,   287,     0,   158,     0,    35,     0,     0,     0,
       0,   163,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   163,     0,     0,     0,
       0,   421,     0,   863,     0,   162,     0,   477,    55,     0,
      57,   162,    59,   490,  1028,     0,     0,  1029,     0,   288,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   158,     0,   158,     0,
       0,     0,     0,   158,     0,     0,   158,     0,     0,     0,
       0,     0,   108,   163,     0,   158,     0,   158,     0,     0,
     158,     0,     0,     0,     0,     0,     0,   108,     0,     0,
       0,     0,     0,   162,     0,   162,   100,     0,     0,     0,
     162,   163,  1121,   162,     0,     0,     0,   163,   158,     0,
       0,     0,   162,     0,   162,     0,   163,   162,   163,   158,
       0,   158,   391,   392,   393,   394,   395,   396,   397,   398,
     399,   400,   401,   402,   403,   404,   405,     0,     0,     0,
       0,     0,     0,     0,     0,   162,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   162,     0,   162,     0,
       0,     0,     0,     0,   108,     0,   406,     0,     0,  -605,
       0,  -605,     0,     0,     0,     0,     0,     0,   163,     0,
       0,     0,   163,     0,     0,     0,     0,     0,     0,     0,
     163,     0,   490,   361,     0,     0,     0,     0,   490,     0,
     362,     0,     0,     0,     0,     0,     0,   657,     0,     0,
       0,     0,   363,   455,     0,   158,     0,     0,     0,     0,
       0,     0,   158,     0,     0,     0,     0,   456,     0,     0,
       0,     0,     0,     0,     0,   158,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,   158,     0,     0,     0,     0,     0,   162,
       0,     0,   158,     0,     0,     0,   158,   364,     0,   717,
       0,   365,   162,   722,     0,     0,     0,     0,     0,     0,
       0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
     162,     0,     0,     0,     0,     0,     0,     0,   452,   162,
       0,     0,   366,   162,     0,     0,     0,     0,     0,     0,
     163,     0,     0,   163,     0,   158,     0,     0,   367,   368,
       0,   369,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,     0,     0,   387,   388,   389,     0,   421,   237,
       0,     0,   162,     0,     0,   390,     0,     0,     0,     0,
       0,     0,     0,   238,   239,     0,   240,     0,     0,     0,
     163,   241,   158,   158,   158,   158,   163,     0,     0,   242,
       0,     0,     0,     0,     0,   243,     0,     0,     0,     0,
     244,     0,     0,     0,   245,     0,     0,   246,     0,   158,
     158,   826,     0,     0,   829,     0,     0,   247,     0,   162,
     162,   162,   162,   248,   249,     0,     0,     0,     0,     0,
       0,   250,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,     0,     0,     0,     0,   162,   162,     0,   252,
     253,     0,   254,     0,   255,     0,   256,     0,     0,   257,
       0,     0,     0,   258,     0,     0,   259,     0,   163,   260,
     163,   868,     0,     0,     0,   163,     0,   873,   163,     0,
       0,     0,   237,     0,     0,     0,     0,   163,     0,   163,
       0,     0,   163,     0,     0,     0,   238,   239,     0,   240,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,   242,     0,     0,     0,     0,     0,   243,     0,
     163,     0,     0,   244,     0,   266,     0,   245,     0,     0,
     246,   163,     0,   163,     0,     0,     0,     0,     0,     0,
     247,     0,     0,   361,     0,     0,   248,   249,     0,     0,
     362,     0,     0,     0,   250,     0,     0,     0,     0,   490,
       0,   490,   363,   251,     0,     0,   490,     0,     0,   490,
       0,     0,   252,   253,     0,   254,     0,   255,   981,   256,
     982,     0,   257,   984,     0,     0,   258,     0,     0,   259,
       0,     0,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1004,     0,     0,     0,     0,     0,   364,     0,     0,
       0,   365,  1010,     0,  1014,     0,     0,   163,     0,     0,
     466,     0,     0,     0,   163,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   163,   841,     0,
       0,     0,   366,   467,   392,   393,   394,   395,   396,     0,
       0,   399,   400,   401,   402,   163,   404,   405,   367,   368,
       0,   369,   370,   371,   163,   372,   373,   374,   163,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,     0,     0,   387,   388,   389,     0,   421,     0,
       0,     0,     0,     0,     0,   390,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   163,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1066,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1086,     0,     0,     0,
       0,     0,     0,     0,     0,  1091,     0,     0,     0,  1093,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   163,   163,   163,   163,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    -2,
       4,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,   163,   163,    12,    13,    14,    15,    16,  1127,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,   -76,     0,    38,    39,    40,     0,
      41,  -297,     0,    42,    43,    44,    45,    46,     0,    47,
      48,    49,   -47,    50,    51,     0,    52,    53,    54,     0,
    -297,     0,     0,    55,    56,    57,    58,    59,    60,    61,
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
      10,    11,     0,  -635,     0,    12,    13,    14,    15,    16,
    -635,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,  -635,    27,    28,  -635,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,   -76,     0,    38,    39,
      40,     0,    41,  -297,     0,    42,    43,    44,    45,    46,
       0,    47,    48,    49,   -47,    50,    51,     0,    52,    53,
      54,     0,  -297,     0,     0,    55,    56,    57,    58,     0,
      60,    61,  -297,   -47,    62,    63,    64,  -635,    65,    66,
      67,  -635,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,     0,
      83,    84,   -76,    85,    86,     0,     0,    87,     0,    88,
       0,     0,  -635,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -635,  -635,
      92,  -635,  -635,  -635,  -635,  -635,  -635,  -635,     0,  -635,
    -635,  -635,  -635,  -635,     0,  -635,  -635,  -635,  -635,  -635,
    -635,  -635,  -635,   100,  -635,  -635,  -635,     0,   102,  -635,
     103,     0,   104,     0,   326,  -635,     5,   290,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -76,     0,
      38,    39,    40,     0,    41,  -297,     0,    42,    43,    44,
      45,    46,     0,    47,    48,    49,   -47,    50,    51,     0,
      52,    53,    54,     0,  -297,     0,     0,    55,    56,    57,
      58,    59,    60,    61,  -297,   -47,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -76,    85,    86,     0,     0,    87,
       0,    88,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,   327,   103,     0,   104,     0,     4,   105,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     -76,     0,    38,    39,    40,     0,    41,  -297,     0,    42,
      43,    44,    45,    46,     0,    47,    48,    49,   -47,    50,
      51,     0,    52,    53,    54,     0,  -297,     0,     0,    55,
      56,    57,    58,    59,    60,    61,  -297,   -47,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,     0,    83,    84,   -76,    85,    86,     0,
       0,    87,     0,    88,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,   100,     0,     0,
     101,     0,   102,   517,   103,     0,   104,     0,   536,   105,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,   -76,     0,    38,    39,    40,     0,    41,  -297,
       0,    42,    43,    44,    45,    46,     0,    47,    48,    49,
     -47,    50,    51,     0,    52,    53,    54,     0,  -297,     0,
       0,    55,    56,    57,    58,    59,    60,    61,  -297,   -47,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,    75,     0,    76,    77,     0,
      78,    79,    80,    81,    82,     0,    83,    84,   -76,    85,
      86,     0,     0,    87,     0,    88,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,   100,
       0,     0,   101,     0,   102,   537,   103,     0,   104,     0,
     326,   105,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,   -76,     0,    38,    39,    40,     0,
      41,  -297,     0,    42,    43,    44,    45,    46,     0,    47,
      48,    49,   -47,    50,    51,     0,    52,    53,    54,     0,
    -297,     0,     0,    55,    56,    57,    58,    59,    60,    61,
    -297,   -47,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,     0,    83,    84,
     -76,    85,    86,     0,     0,    87,     0,    88,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   100,     0,     0,   101,     0,   102,   327,   103,     0,
     104,     0,     4,   105,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,   -76,     0,    38,    39,
      40,     0,    41,  -297,     0,    42,    43,    44,    45,    46,
       0,    47,    48,    49,   -47,    50,    51,     0,    52,    53,
      54,     0,  -297,     0,     0,    55,    56,    57,    58,    59,
      60,    61,  -297,   -47,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,     0,
      83,    84,   -76,    85,    86,     0,     0,    87,     0,    88,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,   100,     0,     0,   101,     0,   102,   729,
     103,     0,   104,     0,     4,   105,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -76,     0,
      38,    39,    40,     0,    41,  -297,     0,    42,    43,    44,
      45,    46,     0,    47,    48,    49,   -47,    50,    51,     0,
      52,    53,    54,     0,  -297,     0,     0,    55,    56,    57,
      58,   342,    60,    61,  -297,   -47,    62,    63,    64,     0,
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
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     -76,     0,    38,    39,    40,     0,    41,  -297,     0,    42,
      43,    44,    45,    46,     0,    47,    48,    49,   -47,    50,
      51,     0,    52,    53,    54,     0,  -297,     0,     0,    55,
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
     638,    37,   -76,     0,    38,    39,    40,     0,    41,  -297,
       0,    42,    43,    44,    45,    46,     0,    47,    48,    49,
     -47,    50,    51,     0,    52,    53,    54,     0,  -297,     0,
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
      34,    35,   644,    37,   -76,     0,    38,    39,    40,     0,
      41,  -297,     0,    42,    43,    44,    45,    46,     0,    47,
      48,    49,   -47,    50,    51,     0,    52,    53,    54,     0,
    -297,     0,     0,    55,    56,    57,    58,     0,    60,    61,
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
      32,    33,    34,    35,   966,    37,   -76,     0,    38,    39,
      40,     0,    41,  -297,     0,    42,    43,    44,    45,    46,
       0,    47,    48,    49,   -47,    50,    51,     0,    52,    53,
      54,     0,  -297,     0,     0,    55,    56,    57,    58,     0,
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
      30,    31,    32,    33,    34,    35,   968,    37,   -76,     0,
      38,    39,    40,     0,    41,  -297,     0,    42,    43,    44,
      45,    46,     0,    47,    48,    49,   -47,    50,    51,     0,
      52,    53,    54,     0,  -297,     0,     0,    55,    56,    57,
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
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,   973,    37,
     -76,     0,    38,    39,    40,     0,    41,  -297,     0,    42,
      43,    44,    45,    46,     0,    47,    48,    49,   -47,    50,
      51,     0,    52,    53,    54,     0,  -297,     0,     0,    55,
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
     976,    37,   -76,     0,    38,    39,    40,     0,    41,  -297,
       0,    42,    43,    44,    45,    46,     0,    47,    48,    49,
     -47,    50,    51,     0,    52,    53,    54,     0,  -297,     0,
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
       0,  1003,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,   -76,     0,    38,    39,    40,     0,
      41,  -297,     0,    42,    43,    44,    45,    46,     0,    47,
      48,    49,   -47,    50,    51,     0,    52,    53,    54,     0,
    -297,     0,     0,    55,    56,    57,    58,     0,    60,    61,
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
      32,    33,    34,    35,  1009,    37,   -76,     0,    38,    39,
      40,     0,    41,  -297,     0,    42,    43,    44,    45,    46,
       0,    47,    48,    49,   -47,    50,    51,     0,    52,    53,
      54,     0,  -297,     0,     0,    55,    56,    57,    58,     0,
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
      30,    31,    32,    33,    34,    35,  1013,    37,   -76,     0,
      38,    39,    40,     0,    41,  -297,     0,    42,    43,    44,
      45,    46,     0,    47,    48,    49,   -47,    50,    51,     0,
      52,    53,    54,     0,  -297,     0,     0,    55,    56,    57,
      58,     0,    60,    61,  -297,   -47,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -76,    85,    86,     0,     0,    87,
       0,    88,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,     0,   103,     0,   104,     0,  1199,   105,     5,   290,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,  1200,   851,   105,
       5,   290,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
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
       0,     0,    21,   228,    23,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
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
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,   294,   295,     0,    82,   331,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,   332,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,     0,
     191,     0,     0,   105,     5,   290,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,   294,   295,     0,    82,   331,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,   713,   191,     0,     0,   105,     5,   290,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,   186,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,   294,   295,     0,
      82,   331,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     190,     0,   103,   715,   191,     0,     5,   105,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
     601,     0,     0,     0,     0,     0,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,   186,    37,     0,   602,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
     603,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,   604,    86,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     190,     0,   103,     0,   191,     0,     5,   105,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,   186,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,   294,   295,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   296,     0,     0,   101,     0,
     190,     0,   103,     0,   191,     0,     0,   105,     5,   290,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,   294,
     295,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,     0,     5,   105,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,   853,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,   294,
     295,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,     0,     5,   105,
       6,     7,     8,   217,    10,    11,   218,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     219,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,   220,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,     0,     5,   105,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,   222,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,   223,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,     0,     5,   105,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,   231,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
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
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,   294,   295,     0,    82,     0,     0,    84,     0,     0,
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
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   190,   711,   103,     0,
     191,     0,     0,   105,     5,   290,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,   721,     0,   105,     5,   290,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,   186,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     190,     0,   103,     0,   191,  1131,     5,   105,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,   186,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,   223,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     190,     0,   103,     0,   191,     0,     5,   105,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,   270,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,   186,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     102,     0,   103,     0,   191,     0,     0,   105,     5,   290,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
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
       6,     7,     8,   217,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     219,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,   220,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,     0,     5,   105,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,   726,     5,   105,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   181,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     183,     0,     0,     0,    32,   184,   185,     0,   186,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,   187,
       0,     0,    45,    46,     0,     0,    48,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
     188,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   189,     0,    76,     0,     0,    78,     0,
       0,     0,    82,   879,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   190,     0,   103,     0,   191,     0,     0,   105,
       5,   290,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     685,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,   686,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,   687,    72,    73,    74,   688,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   190,     0,   103,     0,  1070,     0,
       5,   105,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,  1095,     0,    84,     0,     0,
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
    1070,     0,     0,   105,     5,   290,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,  1051,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,   213,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   214,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   102,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   649,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   685,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,   686,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,   687,    72,    73,    74,   688,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   689,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   689,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   781,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   784,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,  1045,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,  1046,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,  1048,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,  1049,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,  1050,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,  1051,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,   191,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   685,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,   686,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,   687,    72,    73,    74,   688,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   190,     0,
     103,     0,  1155,     0,     5,   105,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   181,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   183,     0,     0,     0,
      32,   184,   185,     0,   186,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,   187,     0,     0,    45,    46,
       0,     0,    48,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,   188,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   189,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   892,     0,   893,    90,    91,
      92,    93,    94,     0,     0,     0,   894,    95,     0,     0,
       0,   895,   239,   896,   897,     0,     0,    96,     0,   898,
      97,    98,    99,     0,     0,     0,   101,   242,     0,   183,
     103,     0,  1155,   899,     0,   105,     0,     0,   900,     0,
       0,     0,   245,     0,     0,   901,     0,   902,     0,     0,
       0,     0,     0,     0,     0,   903,     0,     0,     0,     0,
       0,   904,   905,     0,     0,     0,     0,     0,     0,   250,
       0,     0,     0,     0,     0,     0,     0,     0,   906,     0,
       0,     0,     0,     0,     0,     0,     0,   252,   253,     0,
     907,     0,   255,     0,   908,     0,     0,   909,     0,     0,
       0,   910,     0,     0,   259,     0,     0,   911,     0,     0,
       0,     0,     0,     0,     0,     0,   467,   392,   393,   394,
     395,   396,     0,     0,   399,   400,   401,   402,     0,   404,
     405,   912,   913,   914,   915,   916,   917,     0,   918,     0,
       0,     0,   919,   920,   921,   922,   923,   924,   925,   926,
     927,   928,   929,     0,   930,     0,     0,   931,   932,   933,
     934,     0,     5,   935,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   181,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   183,     0,     0,     0,    32,   184,
     185,     0,   186,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,   187,     0,     0,    45,    46,     0,     0,
      48,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,   188,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   189,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,     0,    98,
      99,     0,     0,     0,   101,     0,   190,     0,   103,     0,
     191,     5,   290,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,   363,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,   364,    56,     0,    58,   365,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
    -463,     0,     0,   367,   368,     0,   369,   370,   371,     0,
     372,   373,   374,     0,     0,   376,     0,    92,     0,     0,
       0,  -463,   382,   383,     0,  -463,   386,     0,     0,   387,
     388,   389,     0,     0,     0,     0,     0,     0,     0,    99,
     390,     0,     0,     0,     0,   190,     0,   103,  -463,  1070,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   183,     0,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,  -445,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,  -445,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,  -445,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,  -445,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,    92,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,    99,  -445,
       0,     0,     0,     0,  -445,    28,   103,   183,  1155,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
       5,    92,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,    99,     0,     0,     0,     0,     0,   190,
      28,   103,   183,   191,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,    92,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,    99,     0,
       0,     0,     0,     0,   190,    28,   103,   183,   689,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,  1108,    78,     0,     0,     0,    82,
       0,     0,    84,   894,     0,    86,     0,     0,   238,   239,
     896,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
     899,    92,     0,     0,     0,   244,     0,     0,     0,   245,
       0,     0,   246,     0,   902,     0,     0,     0,     0,     0,
       0,     0,   247,     0,     0,     0,     0,     0,   904,   249,
       0,   103,     0,   191,     0,     0,   250,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,   253,   361,   254,     0,   255,
       0,  1109,     0,   362,   909,     0,     0,     0,   258,     0,
       0,   259,     0,     0,   260,   363,     0,     0,     0,     0,
       0,     0,     0,   467,   392,   393,   394,   395,   396,     0,
       0,   399,   400,   401,   402,     0,   404,   405,   912,   913,
     914,   915,   916,   917,     0,   918,     0,     0,     0,   919,
     920,   921,   922,   923,   924,   925,   926,   927,   928,   929,
       0,   930,     0,     0,   931,   932,   933,   934,     0,     0,
     364,     0,   361,     0,   365,     0,     0,     0,     0,   362,
       0,     0,     0,   965,     0,     0,     0,     0,     0,     0,
       0,   363,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   467,   392,   393,   394,
     395,   396,     0,     0,   399,   400,   401,   402,     0,   404,
     405,   367,   368,     0,   369,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,     0,   364,   387,   388,   389,
     365,   421,     0,     0,   361,     0,     0,     0,   390,   972,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,     0,     0,     0,     0,     0,     0,
       0,   366,   467,   392,   393,   394,   395,   396,     0,     0,
     399,   400,   401,   402,     0,   404,   405,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,     0,     0,   387,   388,   389,     0,   421,   364,     0,
     361,     0,   365,     0,   390,     0,     0,   362,     0,     0,
       0,  1122,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   467,   392,   393,   394,   395,   396,
       0,     0,   399,   400,   401,   402,     0,   404,   405,   367,
     368,     0,   369,   370,   371,     0,   372,   373,   374,     0,
     375,   376,   377,   378,   379,     0,   380,   381,   382,   383,
     384,   385,   386,     0,   364,   387,   388,   389,   365,   421,
       0,     0,   361,     0,     0,     0,   390,  1123,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,     0,     0,     0,     0,     0,     0,     0,   366,
     467,   392,   393,   394,   395,   396,     0,     0,   399,   400,
     401,   402,     0,   404,   405,   367,   368,     0,   369,   370,
     371,     0,   372,   373,   374,     0,   375,   376,   377,   378,
     379,     0,   380,   381,   382,   383,   384,   385,   386,     0,
       0,   387,   388,   389,     0,   421,   364,     0,   361,     0,
     365,     0,   390,     0,     0,   362,     0,     0,     0,  1124,
       0,     0,     0,     0,     0,     0,     0,   363,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   467,   392,   393,   394,   395,   396,     0,     0,
     399,   400,   401,   402,     0,   404,   405,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,     0,   364,   387,   388,   389,   365,   421,     0,     0,
     361,     0,     0,     0,   390,  1125,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,     0,     0,     0,     0,   366,   467,   392,
     393,   394,   395,   396,     0,     0,   399,   400,   401,   402,
       0,   404,   405,   367,   368,     0,   369,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,   378,   379,     0,
     380,   381,   382,   383,   384,   385,   386,     0,     0,   387,
     388,   389,     0,   421,   364,     0,   361,     0,   365,     0,
     390,     0,     0,   362,     0,     0,     0,  1149,     0,     0,
       0,     0,     0,     0,     0,   363,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     467,   392,   393,   394,   395,   396,     0,     0,   399,   400,
     401,   402,     0,   404,   405,   367,   368,     0,   369,   370,
     371,     0,   372,   373,   374,     0,   375,   376,   377,   378,
     379,     0,   380,   381,   382,   383,   384,   385,   386,     0,
     364,   387,   388,   389,   365,   421,     0,     0,   361,     0,
       0,     0,   390,  1150,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,   449,     0,
       0,     0,     0,     0,     0,   366,   467,   392,   393,   394,
     395,   396,   450,     0,   399,   400,   401,   402,     0,   404,
     405,   367,   368,     0,   369,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,     0,     0,   387,   388,   389,
       0,   421,   364,     0,     0,     0,   365,     0,   390,     0,
       0,   361,     0,     0,     0,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,     0,     0,     0,     0,   459,     0,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,   368,     0,   369,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,   378,   379,   460,
     380,   381,   382,   383,   384,   385,   386,     0,     0,   387,
     388,   389,     0,   421,     0,   364,     0,     0,     0,   365,
     390,     0,     0,     0,   361,   737,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,     0,     0,     0,     0,   738,     0,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,     0,   375,   376,   377,
     378,   379,   739,   380,   381,   382,   383,   384,   385,   386,
       0,   461,   387,   388,   389,     0,     0,     0,   364,     0,
     361,     0,   365,   390,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     816,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
     368,     0,   369,   370,   371,     0,   372,   373,   374,     0,
     375,   376,   377,   378,   379,     0,   380,   381,   382,   383,
     384,   385,   386,     0,   364,   387,   388,   389,   365,     0,
     361,     0,     0,     0,     0,     0,   390,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,     0,     0,   452,     0,     0,     0,   366,
       0,     0,     0,     0,   423,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,   368,     0,   369,   370,
     371,     0,   372,   373,   374,     0,   375,   376,   377,   378,
     379,     0,   380,   381,   382,   383,   384,   385,   386,     0,
       0,   387,   388,   389,   364,   421,   361,     0,   365,     0,
       0,     0,   390,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   270,     0,     0,     0,
       0,     0,     0,     0,     0,   176,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,   368,     0,   369,   370,
     371,     0,   372,   373,   374,     0,   375,   376,   377,   378,
     379,     0,   380,   381,   382,   383,   384,   385,   386,     0,
     364,   387,   388,   389,   365,     0,   361,     0,     0,     0,
       0,     0,   390,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   610,     0,     0,     0,
       0,     0,     0,     0,     0,   366,     0,     0,     0,     0,
     611,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,   368,     0,   369,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,     0,     0,   387,   388,   389,
     364,   421,   361,     0,   365,     0,     0,     0,   390,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   612,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   613,     0,     0,     0,
       0,     0,   361,     0,     0,     0,     0,     0,     0,   362,
       0,   367,   368,     0,   369,   370,   371,     0,   372,   373,
     374,   363,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,     0,   364,   387,   388,   389,
     365,     0,   361,     0,     0,     0,     0,     0,   390,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   808,     0,     0,     0,     0,     0,     0,     0,
       0,   366,     0,     0,     0,     0,   364,     0,     0,     0,
     365,     0,     0,     0,     0,     0,     0,   367,   368,   830,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   366,     0,   387,   388,   389,   364,     0,   361,     0,
     365,     0,     0,     0,   390,   362,     0,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,   363,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   366,     0,   387,   388,   389,     0,   421,     0,     0,
       0,     0,     0,     0,   390,     0,     0,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,     0,   364,   387,   388,   389,   365,   421,     0,     0,
       0,   361,     0,     0,   390,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,     0,     0,   452,     0,  1022,     0,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,   368,     0,   369,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,   378,   379,  1023,
     380,   381,   382,   383,   384,   385,   386,     0,     0,   387,
     388,   389,     0,     0,     0,   364,     0,   361,   871,   365,
     390,     0,     0,     0,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,     0,     0,   361,     0,     0,
       0,     0,     0,     0,   362,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,   363,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
       0,   364,   387,   388,   389,   365,     0,   361,     0,     0,
       0,     0,     0,   390,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,     0,     0,     0,
       0,   364,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   366,     0,   387,   388,
     389,   364,   502,     0,     0,   365,     0,     0,     0,   390,
       0,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   366,   506,   387,   388,
     389,     0,     0,   361,     0,     0,     0,     0,     0,   390,
     362,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,   363,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,     0,   508,   387,   388,
     389,     0,     0,   361,     0,     0,     0,     0,     0,   390,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   363,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   364,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,   361,
       0,     0,     0,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   363,     0,
       0,     0,   366,     0,     0,     0,     0,   364,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,   367,   368,
       0,   369,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   366,   510,   387,   388,   389,     0,     0,     0,
       0,     0,     0,   364,     0,   390,     0,   365,   367,   368,
       0,   369,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   361,   515,   387,   388,   389,   532,   366,   362,
       0,     0,     0,     0,     0,   390,     0,     0,     0,     0,
       0,   363,     0,     0,   367,   368,     0,   369,   370,   371,
       0,   372,   373,   374,     0,   375,   376,   377,   378,   379,
       0,   380,   381,   382,   383,   384,   385,   386,   361,     0,
     387,   388,   389,     0,     0,   362,     0,     0,     0,     0,
       0,   390,     0,     0,     0,     0,     0,   363,     0,     0,
       0,     0,     0,     0,     0,     0,   364,     0,     0,     0,
     365,     0,     0,     0,     0,     0,     0,     0,     0,   614,
       0,     0,     0,   361,   733,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   363,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   364,     0,     0,     0,   365,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,     0,     0,   387,   388,   389,     0,   366,     0,     0,
       0,     0,     0,     0,   390,     0,     0,   364,     0,     0,
       0,   365,     0,   367,   368,     0,   369,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,   378,   379,     0,
     380,   381,   382,   383,   384,   385,   386,   361,     0,   387,
     388,   389,   366,     0,   362,     0,   712,     0,     0,     0,
     390,     0,     0,     0,     0,     0,   363,     0,   367,   368,
       0,   369,   370,   371,     0,   372,   734,   374,     0,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   361,     0,   387,   388,   389,     0,     0,   362,
       0,     0,     0,     0,     0,   390,     0,     0,     0,     0,
       0,   363,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   364,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,   361,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,     0,     0,     0,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   364,     0,     0,     0,
     365,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,   378,   379,  -567,   380,
     381,   382,   383,   384,   385,   386,     0,   176,   387,   388,
     389,   366,     0,     0,     0,     0,   364,     0,     0,   390,
     365,     0,     0,     0,     0,     0,     0,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   366,     0,   387,   388,   389,     0,     0,   361,   821,
       0,     0,     0,     0,   390,   362,     0,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,   363,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,     0,   793,   387,   388,   389,     0,     0,   361,     0,
       0,     0,     0,     0,   390,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,     0,     0,
     828,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   364,     0,     0,     0,   365,     0,   361,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,     0,     0,
       0,     0,   364,     0,     0,     0,   365,     0,     0,     0,
       0,     0,     0,   367,   368,     0,   369,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,   378,   379,     0,
     380,   381,   382,   383,   384,   385,   386,   366,     0,   387,
     388,   389,   364,     0,   361,     0,   365,     0,     0,     0,
     390,   362,     0,   367,   368,     0,   369,   370,   371,     0,
     372,   373,   374,   363,   375,   376,   377,   378,   379,     0,
     380,   381,   382,   383,   384,   385,   386,   366,     0,   387,
     388,   389,     0,     0,     0,     0,     0,     0,     0,     0,
     390,     0,     0,   367,   368,     0,   369,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,   378,   379,     0,
     380,   381,   382,   383,   384,   385,   386,     0,   364,   387,
     388,   389,   365,     0,     0,   361,   855,     0,     0,     0,
     390,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,     0,     0,     0,     0,     0,
       0,     0,     0,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
     368,     0,   369,   370,   371,     0,   372,   373,   374,     0,
     375,   376,   377,   378,   379,     0,   380,   381,   382,   383,
     384,   385,   386,     0,     0,   387,   388,   389,     0,   364,
       0,   361,   856,   365,     0,     0,   390,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,   950,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   874,   366,     0,     0,     0,     0,     0,
       0,   361,     0,     0,     0,     0,     0,     0,   362,     0,
     367,   368,     0,   369,   370,   371,     0,   372,   373,   374,
     363,   375,   376,   377,   378,   379,     0,   380,   381,   382,
     383,   384,   385,   386,     0,   364,   387,   388,   389,   365,
       0,   361,     0,     0,     0,     0,     0,   390,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,   953,     0,     0,     0,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,   364,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,     0,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     366,     0,   387,   388,   389,   364,     0,   361,  1019,   365,
       0,     0,     0,   390,   362,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,   363,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     366,     0,   387,   388,   389,     0,     0,   361,     0,     0,
       0,   956,     0,   390,   362,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,   363,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
       0,   364,   387,   388,   389,   365,     0,   361,     0,     0,
       0,     0,     0,   390,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,     0,     0,     0,
       0,   364,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,   367,   368,  1044,   369,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   366,     0,   387,   388,
     389,   364,     0,   361,  1064,   365,     0,     0,     0,   390,
     362,     0,   367,   368,  1047,   369,   370,   371,     0,   372,
     373,   374,   363,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   366,     0,   387,   388,
     389,     0,     0,   361,     0,     0,     0,     0,     0,   390,
     362,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,   363,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,     0,   364,   387,   388,
     389,   365,     0,   361,     0,     0,     0,     0,     0,   390,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   363,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,     0,     0,     0,     0,   364,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,   367,   368,
       0,   369,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   366,     0,   387,   388,   389,   364,     0,   361,
       0,   365,     0,     0,     0,   390,   362,     0,   367,   368,
       0,   369,   370,   371,     0,   372,   373,   374,   363,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   366,     0,   387,   388,   389,     0,     0,   361,
       0,  1107,     0,     0,     0,   390,   362,     0,   367,   368,
       0,   369,   370,   371,     0,   372,   373,   374,   363,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,     0,   364,   387,   388,   389,   365,     0,     0,
       0,     0,     0,  1145,     0,   390,  1164,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,     0,
       0,     0,     0,   364,     0,   361,     0,   365,     0,     0,
       0,     0,   362,     0,   367,   368,  1165,   369,   370,   371,
       0,   372,   373,   374,   363,   375,   376,   377,   378,   379,
       0,   380,   381,   382,   383,   384,   385,   386,   366,     0,
     387,   388,   389,     0,     0,   361,     0,     0,     0,     0,
       0,   390,   362,     0,   367,   368,     0,   369,   370,   371,
       0,   372,   373,   374,   363,   375,   376,   377,   378,   379,
       0,   380,   381,   382,   383,   384,   385,   386,     0,   364,
     387,   388,   389,   365,     0,     0,     0,     0,     0,     0,
       0,   390,  1166,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,   364,
       0,   361,     0,   365,     0,     0,     0,     0,   362,     0,
     367,   368,  1167,   369,   370,   371,     0,   372,   373,   374,
     363,   375,   376,   377,   378,   379,     0,   380,   381,   382,
     383,   384,   385,   386,   366,     0,   387,   388,   389,     0,
       0,   361,     0,     0,     0,     0,     0,   390,   362,     0,
     367,   368,     0,   369,   370,   371,     0,   372,   373,   374,
     363,   375,   376,   377,   378,   379,     0,   380,   381,   382,
     383,   384,   385,   386,     0,   364,   387,   388,   389,   365,
       0,     0,     0,     0,     0,     0,     0,   390,  1168,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,   364,     0,   361,     0,   365,
       0,     0,     0,     0,   362,     0,   367,   368,  1169,   369,
     370,   371,     0,   372,   373,   374,   363,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     366,     0,   387,   388,   389,     0,     0,     0,     0,     0,
       0,     0,     0,   390,  1193,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,   363,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
       0,   364,   387,   388,   389,   365,     0,   361,     0,     0,
       0,     0,     0,   390,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,     0,     0,     0,
       0,   364,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   366,     0,   387,   388,
     389,   364,     0,   361,     0,   365,     0,     0,     0,   390,
     362,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,   363,   375,   376,   377,   378,     0,     0,   380,
     381,   382,   383,   384,   385,   386,   366,     0,   387,   388,
     389,     0,     0,   361,     0,     0,     0,     0,     0,   390,
     362,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,   363,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,     0,   364,   387,   388,
     389,   365,     0,     0,     0,     0,     0,  1222,     0,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,     0,     0,     0,     0,   779,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,   367,   368,
       0,   369,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   366,     0,   387,   388,   389,     0,     0,     0,
       0,     0,     0,     0,     0,   390,     0,     0,   367,   368,
       0,   369,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,     0,   285,   387,   388,   389,    22,    23,     0,
       0,     0,     0,     0,     0,   390,   286,     0,    30,   287,
     285,     0,     0,    35,    22,    23,     0,     0,     0,     0,
      40,     0,     0,   286,     0,    30,   287,     0,     0,     0,
      35,     0,     0,     0,   363,     0,     0,    40,     0,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,  1028,     0,     0,  1029,     0,   288,     0,    65,     0,
       0,     0,    55,     0,    57,     0,    59,     0,  1028,     0,
       0,  1029,   363,   288,     0,    65,     0,    81,     0,     0,
      83,     0,     0,    85,     0,     0,     0,     0,     0,   364,
       0,     0,     0,   365,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,     0,     0,   364,     0,     0,
       0,   365,     0,   100,     0,     0,     0,     0,     0,  1170,
     367,   368,     0,   369,   370,   371,     0,   372,   373,   374,
     100,   375,   376,   377,   378,     0,  1171,   380,   381,   382,
     383,   384,     0,   386,     0,     0,   387,   388,   389,     0,
       0,     0,     0,     0,     0,     0,     0,   390,   367,   368,
       0,   369,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,   378,     0,     0,   380,   381,   382,   383,   384,
       0,   386,     0,   285,   387,   388,   389,    22,    23,     0,
       0,     0,     0,     0,     0,   390,   286,     0,    30,   287,
       0,     0,   744,    35,   237,     0,     0,     0,     0,     0,
      40,     0,     0,     0,     0,     0,     0,     0,   238,   239,
       0,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,    55,     0,    57,     0,   342,
     243,  1028,     0,     0,  1029,   244,   288,     0,    65,   245,
       0,     0,   246,     0,     0,     0,     0,  -288,     0,     0,
       0,     0,   247,     0,     0,     0,     0,    81,   248,   249,
      83,  -288,  -288,    85,  -288,     0,   250,     0,     0,  -288,
       0,     0,     0,     0,     0,   251,     0,  -288,     0,     0,
       0,     0,     0,  -288,   252,   253,     0,   254,  -288,   255,
       0,   256,  -288,     0,   257,  -288,     0,     0,   258,     0,
     237,   259,     0,     0,   260,  -288,     0,     0,     0,     0,
       0,  -288,  -288,   100,   238,   239,     0,   240,     0,  -288,
       0,     0,   241,     0,     0,     0,     0,     0,  -288,     0,
     242,     0,     0,     0,     0,     0,   243,  -288,  -288,     0,
    -288,   244,  -288,     0,  -288,   245,     0,  -288,   246,     0,
       0,  -288,     0,  -279,  -288,     0,     0,  -288,   247,     0,
       0,     0,     0,     0,   248,   249,     0,  -279,  -279,     0,
    -279,     0,   250,     0,     0,  -279,     0,     0,     0,     0,
       0,   251,     0,  -279,     0,     0,     0,     0,     0,  -279,
     252,   253,     0,   254,  -279,   255,     0,   256,  -279,     0,
     257,  -279,     0,     0,   258,     0,  -174,   259,     0,     0,
     260,  -279,     0,     0,     0,     0,     0,  -279,  -279,     0,
    -174,  -174,     0,  -174,     0,  -279,     0,     0,  -174,     0,
       0,     0,     0,     0,  -279,     0,  -174,     0,     0,     0,
       0,     0,  -174,  -279,  -279,     0,  -279,  -174,  -279,     0,
    -279,  -174,     0,  -279,  -174,     0,     0,  -279,     0,  -170,
    -279,     0,     0,  -279,  -174,     0,     0,     0,     0,     0,
    -174,  -174,     0,  -170,  -170,     0,  -170,     0,  -174,     0,
       0,  -170,     0,     0,     0,     0,     0,  -174,     0,  -170,
       0,     0,     0,     0,     0,  -170,  -174,  -174,     0,  -174,
    -170,  -174,     0,  -174,  -170,     0,  -174,  -170,     0,     0,
    -174,     0,     0,  -174,     0,     0,  -174,  -170,     0,     0,
       0,     0,     0,  -170,  -170,     0,     0,     0,     0,     0,
       0,  -170,     0,     0,     0,     0,     0,     0,     0,     0,
    -170,     0,     0,     0,     0,     0,     0,     0,     0,  -170,
    -170,     0,  -170,     0,  -170,     0,  -170,     0,     0,  -170,
       0,     0,     0,  -170,     0,     0,  -170,     0,     0,  -170
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1072)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,   160,    66,   642,    16,    62,    19,     2,    20,   204,
     212,    33,    34,   460,    26,   511,   753,   474,    30,    31,
     146,    33,    34,    35,    36,   220,   236,   412,   739,   738,
     481,    22,  1080,    32,   236,    47,   850,    49,   940,   941,
     104,   885,    54,     3,    56,    54,    58,   630,   684,    32,
     633,     1,   227,     1,    66,    67,    68,    69,    70,    71,
      54,    89,   478,    75,    46,    77,    45,    79,    80,    32,
      32,   993,    84,   283,     1,    87,    88,    66,    90,   205,
      92,   283,   104,    95,    96,    32,    98,  1068,     3,     3,
     102,   103,   104,   105,    59,   305,     3,   961,   143,   963,
     117,    72,    99,   305,  1131,   115,    20,     1,   143,   990,
      67,    59,  1183,   102,   103,   104,    46,   162,   117,    33,
     486,   117,    72,    36,    46,    46,   765,   191,     0,    89,
    1200,   945,   111,   143,   117,   170,   162,    46,     1,   167,
     100,    89,   778,   114,   156,   511,   103,    61,  1218,  1130,
     115,  1222,    46,   312,  1202,   172,  1183,    70,   615,    54,
     169,   143,   337,   338,   114,    46,    18,   115,   167,   181,
     167,   373,   184,   185,   186,   169,   172,   189,   190,   191,
     994,   143,  1104,    69,   167,   100,   100,  1031,  1069,  1053,
     203,  1055,   106,   100,   142,   122,    59,   168,   408,   409,
     212,   190,   191,    54,   167,   167,   408,   409,  1110,  1111,
    1054,   847,  1056,   143,   797,   142,   143,   800,   168,   167,
     167,   143,   143,    75,   236,   216,    89,   284,   122,     3,
     221,   143,   683,   117,   143,   143,   117,   228,   143,   102,
      27,    93,   172,   170,    89,    46,    20,    46,   142,   143,
     172,   172,   115,   167,  1068,    87,   168,   642,   110,    33,
     122,   162,   143,   172,   172,   170,     3,   168,   162,    27,
     143,   283,   474,   143,  1118,   287,  1120,    54,    45,   142,
     122,   143,   294,   295,   741,   147,    87,    61,    87,    76,
     143,   172,   739,   305,   504,    54,    63,   170,   168,   162,
      87,   143,   504,  1040,   167,   147,    79,    80,   170,   162,
    1021,  1020,  1023,  1022,   440,    82,  1130,   162,    76,   445,
     332,   143,   167,   162,    80,   112,   100,    83,   170,    87,
     169,   970,   106,   790,   329,   347,   348,   137,   224,   225,
     226,   227,   364,   365,   111,   145,   168,   359,   106,   361,
     362,   363,   364,   365,   366,   143,   368,   369,   370,   371,
     372,   169,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   143,
     765,   143,   170,   447,    54,   397,   398,   273,   410,   411,
     841,   403,   143,   279,   406,   407,   408,   409,   410,   411,
     412,   423,   143,    52,    54,   162,   170,    54,   170,   143,
      27,   423,   169,   615,   171,   301,   428,    27,   430,   170,
     122,    54,    71,   839,   143,   143,   628,   169,   450,   170,
     877,    47,    81,   319,   456,   447,   170,   449,   450,    27,
     397,   398,    31,   455,   456,   147,   403,   459,   460,   406,
     407,   170,   170,    69,   466,   143,   143,   469,   447,    76,
     143,    49,   474,    79,    80,   143,    76,   143,   216,   885,
      87,   646,   647,   221,   162,     8,   488,    87,   169,   143,
     492,   493,   546,   170,   162,   497,    74,   170,    76,   106,
     143,   522,   504,   524,   170,   112,   106,    86,   162,    87,
     143,    90,    54,    27,  1000,   137,   528,   519,   497,   162,
     522,    54,   524,   145,    54,   710,   528,   529,   530,   162,
     532,   533,    46,   143,   112,   169,    50,   171,   143,   137,
     519,   733,   734,   522,   546,   524,    97,   145,   424,   741,
     529,    54,   162,   538,   719,   720,   169,   162,   137,    73,
     137,   122,    76,   142,    78,   142,   145,   546,   145,    54,
     149,    54,   149,    87,   162,   522,    54,   524,   454,    86,
     168,    86,   458,    90,  1021,    90,  1023,   148,    54,   150,
     151,   167,   106,   154,   155,   970,   202,    54,   790,   611,
     169,   613,   171,   169,   753,   171,    82,  1136,   610,   611,
     612,   613,   614,   615,   616,   617,   619,   620,   224,   225,
     226,   227,   162,   169,   626,   171,   628,   233,   234,   143,
    1036,   162,   161,    46,   636,   689,   638,   993,   162,   167,
     642,   169,   644,   169,  1000,   171,    59,   626,  1054,   169,
    1056,   171,   153,   655,   656,    54,   170,   263,  1187,  1188,
    1189,  1190,  1191,  1192,   162,   162,   162,   273,   162,   169,
     162,   162,   167,   279,   116,    85,    89,    85,    59,   626,
     169,   169,   169,   169,   122,   687,   688,   689,   169,    59,
     162,   693,   169,   162,     8,   301,    63,   142,   145,   167,
      66,   162,   115,   122,   169,    46,   170,   143,    89,   122,
     689,   162,  1118,   319,  1120,   169,    42,    43,    44,    89,
     122,   102,   122,    34,    34,   143,   170,   170,   122,   731,
     143,    59,   102,   735,   115,   737,   738,   739,   170,   741,
     170,    21,   143,   172,   172,   115,   693,   167,  1104,   162,
     162,   162,   169,   162,   167,    81,   162,   170,   162,   162,
     143,    89,   169,   765,   640,   641,    34,   779,   780,   162,
     646,   647,   170,   162,   102,  1131,   172,   779,   780,   781,
     162,   162,   784,    34,    63,   170,   167,   115,   790,  1145,
     168,   170,   162,   143,   168,   122,    31,   167,    87,   170,
      32,  1001,  1002,    32,   806,   859,   808,   172,   167,  1001,
    1002,   813,   167,   139,   816,   141,    34,   819,   424,   821,
     162,   142,    87,   172,   169,   169,   828,  1183,   162,   142,
     162,     2,   172,   172,   162,   168,   145,   145,   169,   167,
     145,    12,   137,   145,  1200,   145,   145,   137,   454,   145,
     167,    86,   458,   145,    25,    90,    27,   859,   145,   137,
     145,   187,  1218,   145,   137,   137,  1222,   869,   145,   871,
     167,    34,   874,   137,   162,   877,   170,   170,   204,   162,
     859,   162,   102,   137,   170,   887,   170,   162,   162,   472,
     161,  1040,   162,   143,   220,   161,   222,   731,   344,    24,
     834,   863,   137,   138,    75,   231,   232,   142,   313,   144,
     145,   172,   888,  1040,   149,   791,   792,  1036,   839,  1202,
     676,   891,   157,  1160,  1193,  1186,    35,   803,   163,   164,
     165,   102,   489,   935,   628,    30,   107,    -1,    -1,   815,
     266,    -1,   818,    -1,    -1,    -1,    -1,   949,   950,    -1,
     952,   953,    -1,   955,   956,     2,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   965,   966,    12,   968,    -1,   970,    -1,
     972,   973,    31,    -1,   976,    -1,    -1,    -1,    25,    -1,
      27,    -1,  1029,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   999,  1000,  1001,
    1002,    -1,    -1,    -1,    -1,  1007,   177,  1009,    -1,    -1,
      -1,  1013,    -1,    -1,   993,    -1,  1070,  1019,  1020,  1021,
    1022,  1023,    -1,   349,    -1,    -1,    -1,    86,    75,    -1,
      -1,    90,    79,    80,   640,   641,    -1,    -1,    -1,    -1,
     646,   647,  1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,
      -1,    -1,    -1,    -1,    -1,   102,   662,    -1,    -1,    -1,
     107,    -1,  1064,    -1,    -1,    -1,    -1,    -1,  1070,    -1,
      -1,    -1,    -1,  1075,    -1,    -1,    -1,  1079,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,    -1,    -1,    -1,
     149,  1070,    -1,   264,   420,    -1,  1075,    -1,   157,   270,
     706,  1155,    -1,    -1,   163,   164,   165,    -1,    27,    -1,
      -1,    -1,   438,    -1,    -1,    -1,   442,    -1,    -1,    31,
    1122,  1123,  1124,  1125,    -1,    -1,    -1,    46,    -1,  1005,
     177,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1145,    -1,    -1,    -1,  1149,  1150,   475,
      -1,    -1,  1131,  1155,    73,   481,    -1,    76,   329,    78,
      -1,    -1,  1164,  1165,  1166,  1167,  1168,  1169,    87,     2,
      -1,    -1,    -1,   344,    86,    -1,  1155,    -1,    90,    12,
    1182,  1183,    -1,  1185,    -1,   791,   792,   106,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,   803,    -1,   805,
      -1,    -1,    -1,    -1,  1183,    -1,   812,    -1,    -1,   815,
       1,    -1,   818,   539,    -1,    -1,  1218,   264,    -1,    -1,
    1222,    -1,   548,   270,    -1,   137,    -1,    -1,    -1,   835,
     142,    22,   144,   145,    -1,    26,    27,   149,    -1,    -1,
      -1,    -1,    75,  1222,    35,   157,    37,    38,    -1,    -1,
     421,    42,    -1,   165,    -1,    -1,    -1,    -1,    49,     3,
     866,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    31,    17,   107,    -1,    20,    -1,   449,    27,
      71,    25,   329,    74,   455,    76,    -1,    78,    -1,    80,
      81,    35,    83,   464,    85,   466,    87,   344,    46,    -1,
      44,    -1,    50,   629,    -1,    -1,   632,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    61,   109,    -1,
      -1,   112,    -1,    -1,    68,    73,    -1,    86,    76,    -1,
      78,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      84,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,   679,    -1,   526,   100,   683,   106,   530,
      -1,    -1,    -1,   107,    -1,    -1,    -1,   538,    -1,   113,
      -1,   162,    -1,    -1,   421,    -1,    -1,   168,   137,   138,
     706,   140,   141,   142,   710,   144,   145,   146,    -1,    -1,
     149,   150,   151,     2,    -1,   154,   155,   156,   157,  1005,
      -1,   160,   449,    12,   163,   164,   165,    -1,   455,    -1,
      -1,    -1,    31,    -1,    -1,   174,    25,   464,    27,   466,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   753,    -1,    -1,
      -1,   264,    -1,    -1,    46,    -1,   607,   270,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    59,    -1,    -1,
      -1,    -1,  1058,  1059,    -1,    -1,  1062,  1063,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    75,    86,    -1,    -1,
      -1,    90,    -1,    -1,  1080,    -1,    -1,    89,    -1,   526,
      -1,    -1,  1088,   530,  1090,    -1,  1092,   658,    89,    -1,
     661,   538,    -1,   102,    -1,    -1,   329,    -1,   107,    -1,
      -1,    -1,    -1,   115,    -1,    -1,   832,    -1,   834,    -1,
     122,   344,   838,    -1,   115,   841,    -1,    -1,   137,   138,
      86,   122,   141,   142,    90,   144,   145,    -1,    -1,     2,
     149,   143,    -1,    -1,    -1,    -1,    -1,   863,   157,    12,
      -1,    -1,   143,    -1,   163,   164,   165,   718,    -1,    -1,
     162,    -1,    25,   724,    27,   167,    -1,    -1,   170,    -1,
      -1,   162,   888,    -1,    -1,    -1,   167,    -1,   177,   170,
      -1,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
      -1,    -1,   753,   149,    -1,    -1,    -1,    -1,   421,    -1,
     156,   157,    -1,    -1,   160,    -1,  1202,   163,   164,   165,
      -1,    -1,    75,    -1,    -1,    -1,    -1,     2,   779,   780,
      -1,   658,    -1,    -1,   661,    -1,   449,    12,    -1,    -1,
      -1,    -1,   455,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      25,   464,    27,   466,   107,   806,    -1,   808,    -1,    -1,
      -1,    -1,   813,    -1,    -1,   816,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   825,   264,   827,    -1,    -1,   830,
      -1,   270,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,   718,    -1,    -1,    -1,    -1,    35,   724,    37,    38,
      75,    -1,    -1,    42,    -1,    -1,    -1,   858,    -1,    -1,
      49,    -1,    -1,   526,    -1,    -1,    -1,   530,   869,    -1,
     871,    -1,    -1,    -1,   177,   538,    -1,   102,    -1,    -1,
      -1,    -1,   107,    -1,  1040,    74,    -1,    76,    -1,    78,
     329,    80,    -1,    -1,    83,    -1,    85,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,   344,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
     109,    -1,    22,   112,    -1,    -1,    26,    27,    -1,   806,
      -1,   808,    -1,    -1,    -1,    35,   813,    37,    38,   816,
      -1,    -1,    42,    -1,    -1,    31,    -1,    -1,   825,    49,
     827,    -1,   177,   830,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   264,    -1,    -1,   965,    -1,    -1,   270,    -1,    -1,
      -1,   972,    -1,   162,    74,    -1,    76,    -1,    78,   168,
      80,   858,   421,    83,   985,    85,    -1,    87,    -1,    -1,
      -1,    -1,   869,    -1,   871,   658,    -1,    -1,   661,    -1,
      86,    -1,  1003,    -1,    90,    -1,   106,    -1,    -1,   109,
     449,  1012,   112,    -1,    -1,  1016,   455,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   464,   329,   466,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   264,
      -1,   344,    -1,    -1,    -1,   270,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,   718,   142,    -1,   144,   145,
      -1,   724,   162,   149,  1065,    -1,    -1,    -1,   168,    -1,
      -1,   157,    -1,    -1,    -1,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   526,   965,    -1,
      -1,   530,    -1,    -1,    -1,   972,    -1,    -1,    -1,   538,
      -1,    -1,    -1,    -1,   329,    -1,    -1,    -1,   985,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   421,   344,
      -1,  1122,  1123,  1124,  1125,    -1,  1003,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1012,    -1,    -1,    -1,  1016,
      -1,    -1,    -1,   806,    -1,   808,   449,    -1,  1149,  1150,
     813,    -1,   455,   816,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   464,   825,   466,   827,    -1,    22,   830,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    -1,    -1,    42,    -1,  1065,    45,
      -1,    -1,    -1,    49,    -1,   858,   421,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   869,    63,   871,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,   658,
      76,    -1,   661,   526,   449,    -1,    82,   530,    -1,    85,
     455,    87,    -1,    -1,    -1,   538,    -1,    -1,    -1,   464,
      -1,   466,    -1,    -1,    -1,  1122,  1123,  1124,  1125,    -1,
     106,    -1,     2,   109,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1149,  1150,    -1,    25,    -1,    27,    -1,   718,
      -1,    -1,    -1,    -1,    -1,   724,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,   526,   965,    -1,    -1,   530,     3,    -1,    -1,   972,
      -1,    -1,    25,   538,    27,    -1,    -1,    -1,    -1,    -1,
      17,    18,   985,    20,    -1,    75,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
    1003,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,  1012,
      -1,    48,   102,  1016,    51,   658,    -1,   107,   661,    -1,
      -1,    -1,    75,    -1,    61,    -1,    -1,   806,    -1,   808,
      67,    68,    -1,    -1,   813,    -1,    -1,   816,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   825,    84,   827,   102,
      -1,   830,    -1,    -1,   107,    -1,    93,    94,    -1,    96,
      -1,    98,  1065,   100,    -1,    -1,   103,    -1,    -1,    -1,
     107,   108,    -1,   110,    -1,   718,   113,    -1,    -1,   858,
      -1,   724,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
     869,    -1,   871,   658,    -1,    -1,   661,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1122,
    1123,  1124,  1125,    -1,   177,    -1,    -1,    -1,    -1,    22,
      -1,    -1,   169,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,  1149,  1150,    -1,    42,
      -1,    -1,    -1,   718,    -1,    -1,    49,    -1,    -1,   724,
      -1,    -1,    -1,   806,    -1,   808,    -1,    -1,    -1,    -1,
     813,    -1,    -1,   816,   264,    -1,    -1,    -1,    -1,    -1,
     270,    74,   825,    76,   827,    -1,   965,   830,    -1,    -1,
      -1,    -1,    85,   972,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   985,    -1,     3,    -1,
      -1,   264,    -1,   106,    -1,   858,   109,   270,    -1,   112,
      -1,    -1,    17,    18,  1003,    20,   869,    -1,   871,    -1,
      25,    -1,    -1,  1012,    -1,    -1,    -1,  1016,    33,   329,
      -1,   806,    -1,   808,    39,    -1,    -1,    -1,   813,    44,
      -1,   816,    -1,    48,   344,    -1,    51,    -1,    -1,    -1,
     825,    -1,   827,    -1,    -1,   830,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    -1,    -1,   329,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,  1065,    -1,    -1,    84,
      -1,   344,    -1,   858,    -1,    -1,    -1,    -1,    93,    94,
      -1,    96,    -1,    98,   869,   100,   871,    -1,   103,    -1,
      -1,    -1,   107,    -1,     2,   110,    -1,    -1,   113,    -1,
      -1,    -1,   965,    -1,    12,    -1,    -1,    -1,    -1,   972,
      -1,   421,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,   985,  1122,  1123,  1124,  1125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   449,
    1003,    -1,    -1,    -1,    -1,   455,    -1,    -1,   421,  1012,
    1149,  1150,    -1,  1016,   464,    -1,   466,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   449,    -1,    -1,    -1,
     965,    -1,   455,    -1,    -1,     2,    -1,   972,    -1,    -1,
      -1,   464,    -1,   466,   102,    12,    -1,    -1,    -1,   107,
     985,    -1,  1065,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,   526,    -1,  1003,    -1,
     530,    -1,    -1,    -1,    -1,    -1,    -1,  1012,   538,    -1,
      -1,  1016,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,    -1,    -1,   526,    -1,    -1,    -1,   530,    75,  1122,
    1123,  1124,  1125,   153,    -1,   538,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,   166,     3,    -1,   169,
    1065,   171,    -1,    -1,    -1,   102,  1149,  1150,    -1,    -1,
     107,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,  1122,  1123,  1124,
    1125,    67,    68,    -1,    -1,    -1,    -1,    -1,   658,    75,
      -1,   661,    -1,    -1,    -1,    -1,   264,    -1,    84,    -1,
     177,    -1,   270,    -1,  1149,  1150,    -1,    93,    94,    -1,
      96,    -1,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
      -1,   107,    -1,    -1,   110,   658,    -1,   113,   661,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   718,    35,
      -1,    37,    38,    -1,   724,    -1,    42,    -1,    -1,    -1,
      -1,   329,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   344,    -1,    -1,    -1,
      -1,   167,    -1,   169,    -1,   718,    -1,   264,    74,    -1,
      76,   724,    78,   270,    80,    -1,    -1,    83,    -1,    85,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   806,    -1,   808,    -1,
      -1,    -1,    -1,   813,    -1,    -1,   816,    -1,    -1,    -1,
      -1,    -1,   329,   421,    -1,   825,    -1,   827,    -1,    -1,
     830,    -1,    -1,    -1,    -1,    -1,    -1,   344,    -1,    -1,
      -1,    -1,    -1,   806,    -1,   808,   162,    -1,    -1,    -1,
     813,   449,   168,   816,    -1,    -1,    -1,   455,   858,    -1,
      -1,    -1,   825,    -1,   827,    -1,   464,   830,   466,   869,
      -1,   871,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   858,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   869,    -1,   871,    -1,
      -1,    -1,    -1,    -1,   421,    -1,   166,    -1,    -1,   169,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,   526,    -1,
      -1,    -1,   530,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     538,    -1,   449,    12,    -1,    -1,    -1,    -1,   455,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,   464,    -1,    -1,
      -1,    -1,    31,    32,    -1,   965,    -1,    -1,    -1,    -1,
      -1,    -1,   972,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   985,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   965,  1003,    -1,    -1,    -1,    -1,    -1,   972,
      -1,    -1,  1012,    -1,    -1,    -1,  1016,    86,    -1,   526,
      -1,    90,   985,   530,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   538,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1003,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,  1012,
      -1,    -1,   121,  1016,    -1,    -1,    -1,    -1,    -1,    -1,
     658,    -1,    -1,   661,    -1,  1065,    -1,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,   163,   164,   165,    -1,   167,     3,
      -1,    -1,  1065,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
     718,    25,  1122,  1123,  1124,  1125,   724,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,  1149,
    1150,   658,    -1,    -1,   661,    -1,    -1,    61,    -1,  1122,
    1123,  1124,  1125,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,  1149,  1150,    -1,    93,
      94,    -1,    96,    -1,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,   806,   113,
     808,   718,    -1,    -1,    -1,   813,    -1,   724,   816,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,   825,    -1,   827,
      -1,    -1,   830,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
     858,    -1,    -1,    44,    -1,   169,    -1,    48,    -1,    -1,
      51,   869,    -1,   871,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    12,    -1,    -1,    67,    68,    -1,    -1,
      19,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,   806,
      -1,   808,    31,    84,    -1,    -1,   813,    -1,    -1,   816,
      -1,    -1,    93,    94,    -1,    96,    -1,    98,   825,   100,
     827,    -1,   103,   830,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   858,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    90,   869,    -1,   871,    -1,    -1,   965,    -1,    -1,
      99,    -1,    -1,    -1,   972,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   985,   169,    -1,
      -1,    -1,   121,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,  1003,   135,   136,   137,   138,
      -1,   140,   141,   142,  1012,   144,   145,   146,  1016,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,   163,   164,   165,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1065,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   985,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1003,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1012,    -1,    -1,    -1,  1016,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1122,  1123,  1124,  1125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
       1,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,  1149,  1150,    14,    15,    16,    17,    18,  1065,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    69,    -1,
      71,    -1,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      91,    92,    93,    94,    95,    96,    97,    98,    -1,   100,
     101,    -1,   103,   104,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,
     141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,
     161,   162,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,
     171,    -1,     1,   174,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    71,    -1,    -1,    74,    75,    76,    77,    -1,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      -1,   100,   101,    -1,   103,   104,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,    -1,    -1,   116,    -1,   118,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,   167,   168,
     169,    -1,   171,    -1,     1,   174,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    71,    -1,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,   100,   101,    -1,   103,   104,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,    -1,    -1,   116,
      -1,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,
     167,   168,   169,    -1,   171,    -1,     1,   174,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    71,    -1,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    91,    92,    93,    94,
      95,    96,    97,    98,    -1,   100,   101,    -1,   103,   104,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,    -1,
      -1,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,   162,    -1,    -1,
     165,    -1,   167,   168,   169,    -1,   171,    -1,     1,   174,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    71,    -1,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,   100,   101,    -1,
     103,   104,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,   162,
      -1,    -1,   165,    -1,   167,   168,   169,    -1,   171,    -1,
       1,   174,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    69,    -1,
      71,    -1,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      91,    92,    93,    94,    95,    96,    97,    98,    -1,   100,
     101,    -1,   103,   104,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,
     141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,
     161,   162,    -1,    -1,   165,    -1,   167,   168,   169,    -1,
     171,    -1,     1,   174,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    71,    -1,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    91,    92,    93,    94,    95,    96,    97,    98,
      -1,   100,   101,    -1,   103,   104,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,    -1,    -1,   116,    -1,   118,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,   162,    -1,    -1,   165,    -1,   167,   168,
     169,    -1,   171,    -1,     1,   174,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    71,    -1,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,   100,   101,    -1,   103,   104,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,    -1,    -1,   116,
      -1,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,
     167,    -1,   169,    -1,   171,    -1,     1,   174,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    71,    -1,    -1,    74,
      75,    76,    77,    -1,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    91,    92,    93,    94,
      95,    96,    97,    98,    -1,   100,   101,    -1,   103,   104,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,    -1,
      -1,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,   162,    -1,    -1,
     165,    -1,   167,    -1,   169,    -1,   171,    -1,     1,   174,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    71,    -1,
      -1,    74,    75,    76,    77,    -1,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,   100,   101,    -1,
     103,   104,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,   162,
      -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,
       1,   174,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    69,    -1,
      71,    -1,    -1,    74,    75,    76,    77,    -1,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      91,    92,    93,    94,    95,    96,    97,    98,    -1,   100,
     101,    -1,   103,   104,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,
     141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,
     161,   162,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,
     171,    -1,     1,   174,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    71,    -1,    -1,    74,    75,    76,    77,    -1,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    91,    92,    93,    94,    95,    96,    97,    98,
      -1,   100,   101,    -1,   103,   104,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,    -1,    -1,   116,    -1,   118,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,   162,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     1,   174,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    71,    -1,    -1,    74,    75,    76,
      77,    -1,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,   100,   101,    -1,   103,   104,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,    -1,    -1,   116,
      -1,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,
     167,    -1,   169,    -1,   171,    -1,     1,   174,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    71,    -1,    -1,    74,
      75,    76,    77,    -1,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    91,    92,    93,    94,
      95,    96,    97,    98,    -1,   100,   101,    -1,   103,   104,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,    -1,
      -1,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,   162,    -1,    -1,
     165,    -1,   167,    -1,   169,    -1,   171,    -1,     1,   174,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    71,    -1,
      -1,    74,    75,    76,    77,    -1,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,   100,   101,    -1,
     103,   104,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,   162,
      -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,
       1,   174,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    68,    69,    -1,
      71,    -1,    -1,    74,    75,    76,    77,    -1,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      91,    92,    93,    94,    95,    96,    97,    98,    -1,   100,
     101,    -1,   103,   104,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,
     141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,
     161,   162,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,
     171,    -1,     1,   174,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    71,    -1,    -1,    74,    75,    76,    77,    -1,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    91,    92,    93,    94,    95,    96,    97,    98,
      -1,   100,   101,    -1,   103,   104,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,    -1,    -1,   116,    -1,   118,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,   162,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     1,   174,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    69,    -1,    71,    -1,    -1,    74,    75,    76,
      77,    -1,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,   100,   101,    -1,   103,   104,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,    -1,    -1,   116,
      -1,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,
     167,    -1,   169,    -1,   171,    -1,     1,   174,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
     165,    -1,   167,    -1,   169,    -1,   171,   172,     1,   174,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,
      -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,
       3,   174,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    -1,    79,    -1,    -1,    -1,
      -1,    84,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,   112,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,
      -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,
      -1,   174,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    97,    98,    -1,   100,
      -1,    -1,   103,   104,   105,    -1,   107,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,
     141,    -1,    -1,    -1,    -1,   146,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,
     171,    -1,    -1,   174,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,   104,   105,    -1,   107,   108,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,   170,   171,    -1,    -1,   174,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      97,    98,    -1,   100,    -1,    -1,   103,   104,   105,    -1,
     107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,
     167,    -1,   169,   170,   171,    -1,     3,   174,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    46,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,
     167,    -1,   169,    -1,   171,    -1,     3,   174,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      97,    98,    -1,   100,    -1,    -1,   103,   104,   105,    -1,
     107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,
     167,    -1,   169,    -1,   171,    -1,    -1,   174,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,   103,   104,
     105,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
     165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,   174,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,   103,   104,
     105,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
     165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,   174,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,
      -1,   106,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
     165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,   174,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
     165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,   174,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,
      -1,    -1,   107,    -1,    -1,   110,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
     165,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,   174,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,
      -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,
       3,   174,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
     103,   104,   105,    -1,   107,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,
      -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,
      -1,   174,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    97,    98,    -1,   100,
      -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,
     141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    -1,   165,    -1,   167,   168,   169,    -1,
     171,    -1,    -1,   174,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,   172,    -1,   174,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,
     167,    -1,   169,    -1,   171,   172,     3,   174,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,   119,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,
     167,    -1,   169,    -1,   171,    -1,     3,   174,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,
     167,    -1,   169,    -1,   171,    -1,    -1,   174,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
     165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,   174,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,
      -1,   106,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
     165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,   174,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
     165,    -1,   167,    -1,   169,    -1,   171,   172,     3,   174,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,
      -1,    -1,   107,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
     165,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,   174,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,
      -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,
       3,   174,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
     103,    -1,    -1,    -1,   107,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,
      -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,
      -1,   174,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    97,    98,    -1,   100,
      -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,
     141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,
     171,    -1,    -1,   174,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,
     169,    -1,   171,    -1,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    12,   146,    -1,    -1,
      -1,    17,    18,    19,    20,    -1,    -1,   156,    -1,    25,
     159,   160,   161,    -1,    -1,    -1,   165,    33,    -1,    35,
     169,    -1,   171,    39,    -1,   174,    -1,    -1,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      96,    -1,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
     126,   127,    -1,    -1,   130,   131,   132,   133,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,    -1,   144,    -1,
      -1,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,     3,   169,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,
      61,    62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    97,    98,    -1,   100,
      -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,   160,
     161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,
     171,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    31,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    86,    75,    -1,    77,    90,    79,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    -1,    -1,   149,    -1,   139,    -1,    -1,
      -1,   143,   156,   157,    -1,   147,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
     174,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    -1,   100,    -1,   102,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
     113,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,   139,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,   161,   162,
      -1,    -1,    -1,    -1,   167,    33,   169,    35,   171,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
       3,   139,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,   161,    -1,    -1,    -1,    -1,    -1,   167,
      33,   169,    35,   171,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,   139,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,   161,    -1,
      -1,    -1,    -1,    -1,   167,    33,   169,    35,   171,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,     3,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    12,    -1,   113,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,   139,    -1,    -1,    -1,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,
      -1,   169,    -1,   171,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    12,    96,    -1,    98,
      -1,   100,    -1,    19,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,   113,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,    -1,   144,    -1,    -1,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,
      86,    -1,    12,    -1,    90,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,   122,   123,   124,   125,
     126,   127,    -1,    -1,   130,   131,   132,   133,    -1,   135,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,    -1,    86,   163,   164,   165,
      90,   167,    -1,    -1,    12,    -1,    -1,    -1,   174,    99,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
     130,   131,   132,   133,    -1,   135,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,   163,   164,   165,    -1,   167,    86,    -1,
      12,    -1,    90,    -1,   174,    -1,    -1,    19,    -1,    -1,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,   130,   131,   132,   133,    -1,   135,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,    -1,    86,   163,   164,   165,    90,   167,
      -1,    -1,    12,    -1,    -1,    -1,   174,    99,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,   130,   131,
     132,   133,    -1,   135,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,   163,   164,   165,    -1,   167,    86,    -1,    12,    -1,
      90,    -1,   174,    -1,    -1,    19,    -1,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
     130,   131,   132,   133,    -1,   135,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,    -1,    86,   163,   164,   165,    90,   167,    -1,    -1,
      12,    -1,    -1,    -1,   174,    99,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,   130,   131,   132,   133,
      -1,   135,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,    -1,    -1,   163,
     164,   165,    -1,   167,    86,    -1,    12,    -1,    90,    -1,
     174,    -1,    -1,    19,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,   130,   131,
     132,   133,    -1,   135,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,    -1,
      86,   163,   164,   165,    90,   167,    -1,    -1,    12,    -1,
      -1,    -1,   174,    99,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   121,   122,   123,   124,   125,
     126,   127,    46,    -1,   130,   131,   132,   133,    -1,   135,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,   163,   164,   165,
      -1,   167,    86,    -1,    -1,    -1,    90,    -1,   174,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    70,
     154,   155,   156,   157,   158,   159,   160,    -1,    -1,   163,
     164,   165,    -1,   167,    -1,    86,    -1,    -1,    -1,    90,
     174,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,    70,   154,   155,   156,   157,   158,   159,   160,
      -1,   162,   163,   164,   165,    -1,    -1,    -1,    86,    -1,
      12,    -1,    90,   174,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,    -1,    86,   163,   164,   165,    90,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   174,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,   163,   164,   165,    86,   167,    12,    -1,    90,    -1,
      -1,    -1,   174,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,    -1,
      86,   163,   164,   165,    90,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   174,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,   163,   164,   165,
      86,   167,    12,    -1,    90,    -1,    -1,    -1,   174,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    46,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    31,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,    -1,    86,   163,   164,   165,
      90,    -1,    12,    -1,    -1,    -1,    -1,    -1,   174,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    99,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   121,    -1,   163,   164,   165,    86,    -1,    12,    -1,
      90,    -1,    -1,    -1,   174,    19,    -1,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    31,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   121,    -1,   163,   164,   165,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,    -1,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,    -1,    86,   163,   164,   165,    90,   167,    -1,    -1,
      -1,    12,    -1,    -1,   174,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,   117,    -1,    36,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    70,
     154,   155,   156,   157,   158,   159,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,    86,    -1,    12,   172,    90,
     174,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
      -1,    86,   163,   164,   165,    90,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   174,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   121,    -1,   163,   164,
     165,    86,   167,    -1,    -1,    90,    -1,    -1,    -1,   174,
      -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   121,   162,   163,   164,
     165,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   174,
      19,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    -1,   162,   163,   164,
     165,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   174,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   121,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,   174,    -1,    90,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    12,   162,   163,   164,   165,   120,   121,    19,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    12,    -1,
     163,   164,   165,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    90,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,   163,   164,   165,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,    -1,    86,    -1,    -1,
      -1,    90,    -1,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,    12,    -1,   163,
     164,   165,   121,    -1,    19,    -1,   170,    -1,    -1,    -1,
     174,    -1,    -1,    -1,    -1,    -1,    31,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    12,    -1,   163,   164,   165,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      90,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,   117,   163,   164,
     165,   121,    -1,    -1,    -1,    -1,    86,    -1,    -1,   174,
      90,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   121,    -1,   163,   164,   165,    -1,    -1,    12,    13,
      -1,    -1,    -1,    -1,   174,    19,    -1,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    31,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,    -1,   162,   163,   164,   165,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   174,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    90,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   121,    -1,   163,
     164,   165,    86,    -1,    12,    -1,    90,    -1,    -1,    -1,
     174,    19,    -1,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    31,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   121,    -1,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,    -1,    86,   163,
     164,   165,    90,    -1,    -1,    12,   170,    -1,    -1,    -1,
     174,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,    -1,    -1,   163,   164,   165,    -1,    86,
      -1,    12,   170,    90,    -1,    -1,   174,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    -1,    86,   163,   164,   165,    90,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   174,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     121,    -1,   163,   164,   165,    86,    -1,    12,    13,    90,
      -1,    -1,    -1,   174,    19,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     121,    -1,   163,   164,   165,    -1,    -1,    12,    -1,    -1,
      -1,   172,    -1,   174,    19,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
      -1,    86,   163,   164,   165,    90,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   174,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,    99,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   121,    -1,   163,   164,
     165,    86,    -1,    12,    13,    90,    -1,    -1,    -1,   174,
      19,    -1,   137,   138,    99,   140,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   121,    -1,   163,   164,
     165,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   174,
      19,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    -1,    86,   163,   164,
     165,    90,    -1,    12,    -1,    -1,    -1,    -1,    -1,   174,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   121,    -1,   163,   164,   165,    86,    -1,    12,
      -1,    90,    -1,    -1,    -1,   174,    19,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    31,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   121,    -1,   163,   164,   165,    -1,    -1,    12,
      -1,   170,    -1,    -1,    -1,   174,    19,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    31,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    -1,    86,   163,   164,   165,    90,    -1,    -1,
      -1,    -1,    -1,   172,    -1,   174,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    86,    -1,    12,    -1,    90,    -1,    -1,
      -1,    -1,    19,    -1,   137,   138,    99,   140,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   121,    -1,
     163,   164,   165,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   174,    19,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    -1,    86,
     163,   164,   165,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    86,
      -1,    12,    -1,    90,    -1,    -1,    -1,    -1,    19,    -1,
     137,   138,    99,   140,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   121,    -1,   163,   164,   165,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   174,    19,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    -1,    86,   163,   164,   165,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    86,    -1,    12,    -1,    90,
      -1,    -1,    -1,    -1,    19,    -1,   137,   138,    99,   140,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     121,    -1,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    59,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
      -1,    86,   163,   164,   165,    90,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   174,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   121,    -1,   163,   164,
     165,    86,    -1,    12,    -1,    90,    -1,    -1,    -1,   174,
      19,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,   151,    -1,    -1,   154,
     155,   156,   157,   158,   159,   160,   121,    -1,   163,   164,
     165,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   174,
      19,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    -1,    86,   163,   164,
     165,    90,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   121,    -1,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    -1,    22,   163,   164,   165,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    35,    -1,    37,    38,
      22,    -1,    -1,    42,    26,    27,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    31,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    83,    -1,    85,    -1,    87,    -1,
      -1,    -1,    74,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    83,    31,    85,    -1,    87,    -1,   106,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    90,   106,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    86,    -1,    -1,
      -1,    90,    -1,   162,    -1,    -1,    -1,    -1,    -1,   168,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     162,   148,   149,   150,   151,    -1,   168,   154,   155,   156,
     157,   158,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,    -1,    -1,   154,   155,   156,   157,   158,
      -1,   160,    -1,    22,   163,   164,   165,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    35,    -1,    37,    38,
      -1,    -1,     1,    42,     3,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    74,    -1,    76,    -1,    78,
      39,    80,    -1,    -1,    83,    44,    85,    -1,    87,    48,
      -1,    -1,    51,    -1,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    -1,   106,    67,    68,
     109,    17,    18,   112,    20,    -1,    75,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    93,    94,    -1,    96,    44,    98,
      -1,   100,    48,    -1,   103,    51,    -1,    -1,   107,    -1,
       3,   110,    -1,    -1,   113,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,   162,    17,    18,    -1,    20,    -1,    75,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    93,    94,    -1,
      96,    44,    98,    -1,   100,    48,    -1,   103,    51,    -1,
      -1,   107,    -1,     3,   110,    -1,    -1,   113,    61,    -1,
      -1,    -1,    -1,    -1,    67,    68,    -1,    17,    18,    -1,
      20,    -1,    75,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      93,    94,    -1,    96,    44,    98,    -1,   100,    48,    -1,
     103,    51,    -1,    -1,   107,    -1,     3,   110,    -1,    -1,
     113,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,
      17,    18,    -1,    20,    -1,    75,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    93,    94,    -1,    96,    44,    98,    -1,
     100,    48,    -1,   103,    51,    -1,    -1,   107,    -1,     3,
     110,    -1,    -1,   113,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    -1,    17,    18,    -1,    20,    -1,    75,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    84,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    93,    94,    -1,    96,
      44,    98,    -1,   100,    48,    -1,   103,    51,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,   113,    61,    -1,    -1,
      -1,    -1,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    96,    -1,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   179,   180,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    47,    48,
      49,    51,    54,    55,    56,    57,    58,    60,    61,    62,
      64,    65,    67,    68,    69,    74,    75,    76,    77,    78,
      79,    80,    83,    84,    85,    87,    88,    89,    91,    92,
      93,    94,    95,    96,    97,    98,   100,   101,   103,   104,
     105,   106,   107,   109,   110,   112,   113,   116,   118,   121,
     137,   138,   139,   140,   141,   146,   156,   159,   160,   161,
     162,   165,   167,   169,   171,   174,   181,   182,   183,   184,
     185,   186,   189,   191,   197,   198,   199,   202,   203,   207,
     209,   212,   213,   215,   216,   217,   222,   223,   225,   227,
     230,   231,   232,   233,   234,   235,   239,   240,   243,   244,
     247,   248,   251,   254,   255,   280,   283,   284,   304,   305,
     306,   307,   308,   309,   310,   318,   319,   320,   321,   322,
     325,   326,   327,   328,   329,   330,   331,   332,   334,   335,
     336,   337,   338,   162,   183,   322,   117,   311,   312,     3,
     204,    14,    22,    35,    40,    41,    43,    54,    85,    98,
     167,   171,   230,   251,   304,   309,   320,   321,   322,   325,
     327,   328,   311,   322,   106,   283,    87,   204,   183,   298,
     322,   183,   169,     8,    85,   322,   323,     8,    11,    85,
     106,   323,    76,   119,   224,   322,   224,   322,    26,   284,
     322,    27,   112,   226,   322,   190,   169,     3,    17,    18,
      20,    25,    33,    39,    44,    48,    51,    61,    67,    68,
      75,    84,    93,    94,    96,    98,   100,   103,   107,   110,
     113,   206,   208,   206,   206,   252,   169,   206,   285,   286,
      32,   191,   211,   322,   322,    18,    75,    93,   110,   322,
     322,   322,     8,   169,   214,    22,    35,    38,    85,   215,
       4,   274,   297,   322,   104,   105,   162,   322,   324,   322,
     211,   322,   322,   322,    97,   169,   183,   322,   322,   191,
     203,   322,   325,   191,   203,   322,   206,   281,   322,   322,
     322,   322,   322,   322,   322,   322,     1,   168,   181,   192,
     297,   108,   147,   274,   299,   300,   324,   224,   297,   322,
     333,   322,    78,   183,   167,    82,   187,    45,   111,    54,
     206,   206,    52,    71,    81,   269,   285,   161,   162,   153,
     322,    12,    19,    31,    86,    90,   121,   137,   138,   140,
     141,   142,   144,   145,   146,   148,   149,   150,   151,   152,
     154,   155,   156,   157,   158,   159,   160,   163,   164,   165,
     174,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   166,   261,   169,   171,
      86,    90,   169,   183,   162,   322,   322,   322,   206,   297,
      54,   167,   191,    46,   311,   281,   285,   162,   143,   162,
     116,   207,   274,   301,   302,   303,   324,    85,   220,   255,
     283,    85,   218,   281,   255,   283,   206,   169,   211,    32,
      46,   211,   117,   211,   314,    32,    46,   211,   314,    36,
      70,   162,   206,   206,    99,   191,    99,   122,   191,   261,
      80,    83,   188,   301,   169,   169,   191,   183,   169,   263,
     108,   169,   206,   287,   288,     1,   142,   292,    46,   143,
     183,   211,   169,   169,   211,   301,   215,   143,   162,   322,
     322,   162,   167,   211,   169,   301,   162,   236,   162,   236,
     162,   122,   282,   162,   211,   162,   168,   168,   181,   143,
     168,   322,   143,   170,   143,   170,   172,   314,    46,   143,
     172,   314,   120,   143,   172,     8,     1,   168,   192,    63,
     200,   201,   322,   194,   322,   206,   142,   245,   167,   256,
     162,   322,   322,   322,   322,   224,   322,   224,   322,   322,
     322,   322,   322,   322,   322,     3,    20,    33,    61,   100,
     106,   207,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,    66,
     324,   324,   324,   324,   324,   301,   301,   224,   322,   224,
     322,    27,    46,    87,   112,   313,   316,   317,   322,   338,
      32,    46,    32,    46,    99,   169,    46,   172,   206,   224,
     322,   211,   162,   162,   322,   322,   122,   170,   143,   221,
     206,   285,   219,   206,   162,   285,    46,   297,    43,   322,
     224,   322,   169,   211,    43,   322,   224,   322,   211,   165,
     193,   196,   322,   193,   195,   122,   122,   183,    34,   183,
     322,    34,   322,   187,   170,   302,   206,   228,   229,    27,
      46,    50,    73,    76,    87,   106,   182,   264,   265,   266,
     267,   253,   288,   143,   170,    33,    48,    94,    98,   171,
     210,   293,   305,   122,   289,   322,   286,   322,   322,   170,
     274,   322,     1,   241,   301,   170,    21,   237,   293,   305,
     143,   168,   170,   170,   299,   170,   299,   183,   172,   224,
     322,   172,   183,   322,   172,   322,   172,   322,   168,   168,
     206,   143,   162,    13,   145,   143,   162,    13,    36,    70,
     162,   169,   297,   167,     1,   206,   249,   250,    27,    76,
      87,   106,   258,   268,   169,   162,   162,   162,   162,   162,
     162,   170,   172,    46,    87,   143,   170,    17,    20,    25,
      44,    51,    61,    68,    84,    96,   107,   113,   304,    86,
      86,    43,   224,   322,    43,   224,   322,   302,   224,   322,
     169,   311,   311,   162,   274,   324,   303,   206,   245,   162,
     206,   245,   162,   322,   170,   322,    32,   211,    32,   211,
     315,   316,   322,    32,   211,   314,    32,   211,   314,   143,
     162,    13,   162,   322,   322,    34,   183,    34,    34,   183,
      99,   191,    63,   170,   143,   170,    46,    87,   266,   143,
     170,   169,   206,    27,    76,    87,   106,   270,   170,   288,
     292,     1,   297,    66,   324,   170,   170,   168,    72,   114,
     168,   242,   170,   169,   191,   206,   238,   281,   183,   172,
     314,   172,   314,   183,   120,   200,   207,   167,   322,   108,
     322,   196,   195,   302,     1,   246,   168,   122,   143,   168,
      87,   257,     1,     3,    12,    17,    19,    20,    25,    39,
      44,    51,    53,    61,    67,    68,    84,    96,   100,   103,
     107,   113,   137,   138,   139,   140,   141,   142,   144,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     160,   163,   164,   165,   166,   169,   205,   206,   208,   259,
     260,   261,   304,   170,   316,   292,   304,   304,   322,    32,
      32,   322,    32,    32,   170,   172,   172,   302,   211,   211,
     245,   167,   245,   167,   211,    99,    43,   322,    43,   322,
     143,   170,    99,    43,   322,   211,    43,   322,   211,   322,
     322,   183,   183,   322,   183,    34,   206,   162,   229,   191,
     206,   265,   288,   142,   296,    87,   292,   289,   172,    46,
     172,   169,   169,    32,   183,   297,   238,   142,   191,    43,
     183,   322,   172,    43,   183,   322,   172,   322,   193,    13,
      36,    70,    36,    70,   162,   162,   170,   168,    80,    83,
     168,   182,   213,   322,   250,   270,   169,   262,   322,   137,
     145,   262,   262,   289,    99,    43,    43,    99,    43,    43,
      43,    43,   170,   167,   246,   167,   246,   322,   322,   322,
     316,   322,   322,   322,    13,    34,   183,   162,   296,   170,
     171,   210,   274,   295,   305,   147,   275,   289,    59,   115,
     276,   322,   293,   305,   301,   301,   183,   211,   170,   322,
     322,   183,   322,   183,   168,   108,   322,   196,   195,   196,
     195,   162,   213,     1,   142,   291,   264,   170,     3,   100,
     260,   261,   322,   322,   322,   322,   322,   322,   246,   168,
     246,   168,    99,    99,    99,    99,   322,   183,   275,   289,
     296,   172,   297,   274,   322,     3,    89,   100,   277,   278,
     279,   322,   191,   212,   273,   172,   170,   170,   191,    99,
      99,   162,   162,   162,   162,   171,   210,   290,   305,   102,
     271,   170,   262,   262,    99,    99,    99,    99,    99,    99,
     168,   168,   322,   322,   322,   322,   275,   289,   274,   294,
     295,   305,    46,   172,   279,   115,   143,   122,   148,   150,
     151,   154,   155,    59,   305,   161,   161,   322,   322,     1,
     172,   297,   276,   322,   294,   295,   322,   278,   279,   279,
     279,   279,   279,   279,   277,   172,   290,   305,   172,   162,
     272,   273,   172,   290,   305,   294
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
#line 1028 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1034 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1040 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1046 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1052 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1059 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1066 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1070 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1076 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1080 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1082 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1083 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1085 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1090 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1092 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1094 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1101 "chapel.ypp"
    { (yyval.pexpr) = buildIfVar((yyvsp[(2) - (4)].pch), (yyvsp[(4) - (4)].pexpr), false); }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1102 "chapel.ypp"
    { (yyval.pexpr) = buildIfVar((yyvsp[(2) - (4)].pch), (yyvsp[(4) - (4)].pexpr), true);  }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt))); }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1109 "chapel.ypp"
    { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (3)].pch), ls, (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[(1) - (1)].pch), INTENT_TYPE); }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1129 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1134 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1135 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1136 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[(1) - (1)].pch)); }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.pch) = "none"; }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1147 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1148 "chapel.ypp"
    { (yyval.pch) = "false"; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1149 "chapel.ypp"
    { (yyval.pch) = "true"; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1159 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), NULL)); }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (4)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (4)].pch), act, NULL)); }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1169 "chapel.ypp"
    { (yyvsp[(5) - (7)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (7)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (7)].pch), (yyvsp[(5) - (7)].pcallexpr), NULL)); }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.pexpr) = IfcConstraint::build((yyvsp[(2) - (5)].pch), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1177 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (3)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (3)].pch), act); }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1180 "chapel.ypp"
    { (yyvsp[(5) - (6)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (6)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pcallexpr)); }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1185 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1188 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1190 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1191 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1192 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1193 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1208 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1219 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1231 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1240 "chapel.ypp"
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
/* Line 1792 of yacc.c  */
#line 1250 "chapel.ypp"
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
/* Line 1792 of yacc.c  */
#line 1263 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1268 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
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
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1319 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1328 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1336 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1340 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1347 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1352 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1357 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
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
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1387 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1391 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1399 "chapel.ypp"
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
/* Line 1792 of yacc.c  */
#line 1409 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1416 "chapel.ypp"
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

  case 303:
/* Line 1792 of yacc.c  */
#line 1449 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1453 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1458 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1462 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1467 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1480 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1492 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pch) = ":"; }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pexpr))); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1548 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1550 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1564 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1565 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1566 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1572 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1576 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_PROC; }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_ITER; }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1582 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_OP; }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1586 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1587 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1588 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1613 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1614 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1615 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1625 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1627 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1636 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1642 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1643 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1644 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1645 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1646 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1647 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1648 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1653 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1660 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1682 "chapel.ypp"
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

  case 419:
/* Line 1792 of yacc.c  */
#line 1698 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 422:
/* Line 1792 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1707 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1709 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1715 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1721 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1730 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1738 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1740 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1747 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1749 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1754 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1756 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1758 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1765 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1772 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1797 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1798 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1801 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1806 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1807 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1809 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1831 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1835 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1839 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1847 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1853 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1854 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1855 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1860 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1862 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1868 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1870 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1872 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1876 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1877 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1878 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1879 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1880 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1886 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1887 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1888 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1889 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1893 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1894 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1898 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1899 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1900 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1904 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1905 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1909 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1915 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1919 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1920 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1921 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1922 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1926 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1927 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1939 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1941 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1943 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1954 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1956 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1958 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1960 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 1962 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 1964 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 1966 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 1969 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1975 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1977 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1979 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1981 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1986 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1988 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1999 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 2001 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 2003 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 2005 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 2007 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 2009 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 2011 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 2013 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 2015 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 2017 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 2024 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 2030 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 2036 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 2042 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 2051 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2060 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2076 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2080 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2085 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2089 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2090 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2094 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2098 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2099 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2104 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2108 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2112 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2118 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2119 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2120 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2121 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2122 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2123 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2129 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2134 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2136 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2140 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2144 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2148 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2156 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2161 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2167 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2173 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2180 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2190 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2207 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2209 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2211 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2213 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), false, true); }
    break;

  case 592:
/* Line 1792 of yacc.c  */
#line 2228 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 593:
/* Line 1792 of yacc.c  */
#line 2230 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 594:
/* Line 1792 of yacc.c  */
#line 2232 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr), true); }
    break;

  case 595:
/* Line 1792 of yacc.c  */
#line 2234 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
    break;

  case 596:
/* Line 1792 of yacc.c  */
#line 2238 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2239 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2243 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2244 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2245 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2261 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2262 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 607:
/* Line 1792 of yacc.c  */
#line 2263 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 608:
/* Line 1792 of yacc.c  */
#line 2264 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 610:
/* Line 1792 of yacc.c  */
#line 2269 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 611:
/* Line 1792 of yacc.c  */
#line 2270 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 612:
/* Line 1792 of yacc.c  */
#line 2271 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 613:
/* Line 1792 of yacc.c  */
#line 2275 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 614:
/* Line 1792 of yacc.c  */
#line 2276 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 615:
/* Line 1792 of yacc.c  */
#line 2277 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 616:
/* Line 1792 of yacc.c  */
#line 2278 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 617:
/* Line 1792 of yacc.c  */
#line 2279 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 618:
/* Line 1792 of yacc.c  */
#line 2287 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 619:
/* Line 1792 of yacc.c  */
#line 2288 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 620:
/* Line 1792 of yacc.c  */
#line 2289 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 621:
/* Line 1792 of yacc.c  */
#line 2290 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 622:
/* Line 1792 of yacc.c  */
#line 2294 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 623:
/* Line 1792 of yacc.c  */
#line 2295 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 624:
/* Line 1792 of yacc.c  */
#line 2299 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 625:
/* Line 1792 of yacc.c  */
#line 2300 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 628:
/* Line 1792 of yacc.c  */
#line 2306 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 629:
/* Line 1792 of yacc.c  */
#line 2307 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 630:
/* Line 1792 of yacc.c  */
#line 2308 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 631:
/* Line 1792 of yacc.c  */
#line 2309 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 632:
/* Line 1792 of yacc.c  */
#line 2310 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 633:
/* Line 1792 of yacc.c  */
#line 2311 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr),
                                            new SymExpr(gTrue)); }
    break;

  case 634:
/* Line 1792 of yacc.c  */
#line 2313 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr),
                                                   new SymExpr(gTrue)); }
    break;

  case 635:
/* Line 1792 of yacc.c  */
#line 2315 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 636:
/* Line 1792 of yacc.c  */
#line 2316 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 637:
/* Line 1792 of yacc.c  */
#line 2318 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 638:
/* Line 1792 of yacc.c  */
#line 2322 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 639:
/* Line 1792 of yacc.c  */
#line 2329 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 640:
/* Line 1792 of yacc.c  */
#line 2330 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 641:
/* Line 1792 of yacc.c  */
#line 2334 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 642:
/* Line 1792 of yacc.c  */
#line 2335 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 643:
/* Line 1792 of yacc.c  */
#line 2336 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 644:
/* Line 1792 of yacc.c  */
#line 2337 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 645:
/* Line 1792 of yacc.c  */
#line 2338 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 646:
/* Line 1792 of yacc.c  */
#line 2339 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 647:
/* Line 1792 of yacc.c  */
#line 2340 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 648:
/* Line 1792 of yacc.c  */
#line 2341 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 649:
/* Line 1792 of yacc.c  */
#line 2342 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 650:
/* Line 1792 of yacc.c  */
#line 2343 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 651:
/* Line 1792 of yacc.c  */
#line 2344 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 652:
/* Line 1792 of yacc.c  */
#line 2345 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 653:
/* Line 1792 of yacc.c  */
#line 2346 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 654:
/* Line 1792 of yacc.c  */
#line 2347 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 655:
/* Line 1792 of yacc.c  */
#line 2348 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 656:
/* Line 1792 of yacc.c  */
#line 2349 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 657:
/* Line 1792 of yacc.c  */
#line 2350 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 658:
/* Line 1792 of yacc.c  */
#line 2351 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 659:
/* Line 1792 of yacc.c  */
#line 2352 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 660:
/* Line 1792 of yacc.c  */
#line 2353 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 661:
/* Line 1792 of yacc.c  */
#line 2354 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 662:
/* Line 1792 of yacc.c  */
#line 2355 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 663:
/* Line 1792 of yacc.c  */
#line 2356 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr),
                                               new SymExpr(gTrue)); }
    break;

  case 664:
/* Line 1792 of yacc.c  */
#line 2361 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 665:
/* Line 1792 of yacc.c  */
#line 2362 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 666:
/* Line 1792 of yacc.c  */
#line 2363 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 667:
/* Line 1792 of yacc.c  */
#line 2364 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 668:
/* Line 1792 of yacc.c  */
#line 2365 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 669:
/* Line 1792 of yacc.c  */
#line 2366 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 670:
/* Line 1792 of yacc.c  */
#line 2367 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 671:
/* Line 1792 of yacc.c  */
#line 2371 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 672:
/* Line 1792 of yacc.c  */
#line 2372 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 673:
/* Line 1792 of yacc.c  */
#line 2373 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 674:
/* Line 1792 of yacc.c  */
#line 2374 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 675:
/* Line 1792 of yacc.c  */
#line 2378 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 676:
/* Line 1792 of yacc.c  */
#line 2379 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 677:
/* Line 1792 of yacc.c  */
#line 2380 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 678:
/* Line 1792 of yacc.c  */
#line 2381 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 679:
/* Line 1792 of yacc.c  */
#line 2386 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 680:
/* Line 1792 of yacc.c  */
#line 2387 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 681:
/* Line 1792 of yacc.c  */
#line 2388 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 682:
/* Line 1792 of yacc.c  */
#line 2389 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 683:
/* Line 1792 of yacc.c  */
#line 2390 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 684:
/* Line 1792 of yacc.c  */
#line 2391 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 685:
/* Line 1792 of yacc.c  */
#line 2392 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 11011 "bison-chapel.cpp"
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


