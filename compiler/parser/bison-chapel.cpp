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
     TDEPRECATED = 286,
     TDMAPPED = 287,
     TDO = 288,
     TDOMAIN = 289,
     TELSE = 290,
     TENUM = 291,
     TEXCEPT = 292,
     TEXPORT = 293,
     TEXTERN = 294,
     TFALSE = 295,
     TFOR = 296,
     TFORALL = 297,
     TFOREACH = 298,
     TFORWARDING = 299,
     TIF = 300,
     TIMAG = 301,
     TIMPORT = 302,
     TIN = 303,
     TINCLUDE = 304,
     TINDEX = 305,
     TINLINE = 306,
     TINOUT = 307,
     TINT = 308,
     TITER = 309,
     TINITEQUALS = 310,
     TIMPLEMENTS = 311,
     TINTERFACE = 312,
     TLABEL = 313,
     TLAMBDA = 314,
     TLET = 315,
     TLIFETIME = 316,
     TLOCAL = 317,
     TLOCALE = 318,
     TMANAGE = 319,
     TMINUSMINUS = 320,
     TMODULE = 321,
     TNEW = 322,
     TNIL = 323,
     TNOINIT = 324,
     TNONE = 325,
     TNOTHING = 326,
     TON = 327,
     TONLY = 328,
     TOPERATOR = 329,
     TOTHERWISE = 330,
     TOUT = 331,
     TOVERRIDE = 332,
     TOWNED = 333,
     TPARAM = 334,
     TPLUSPLUS = 335,
     TPRAGMA = 336,
     TPRIMITIVE = 337,
     TPRIVATE = 338,
     TPROC = 339,
     TPROTOTYPE = 340,
     TPUBLIC = 341,
     TREAL = 342,
     TRECORD = 343,
     TREDUCE = 344,
     TREF = 345,
     TREQUIRE = 346,
     TRETURN = 347,
     TSCAN = 348,
     TSELECT = 349,
     TSERIAL = 350,
     TSHARED = 351,
     TSINGLE = 352,
     TSPARSE = 353,
     TSTRING = 354,
     TSUBDOMAIN = 355,
     TSYNC = 356,
     TTHEN = 357,
     TTHIS = 358,
     TTHROW = 359,
     TTHROWS = 360,
     TTRUE = 361,
     TTRY = 362,
     TTRYBANG = 363,
     TTYPE = 364,
     TUINT = 365,
     TUNDERSCORE = 366,
     TUNION = 367,
     TUNMANAGED = 368,
     TUSE = 369,
     TVAR = 370,
     TVOID = 371,
     TWHEN = 372,
     TWHERE = 373,
     TWHILE = 374,
     TWITH = 375,
     TYIELD = 376,
     TZIP = 377,
     TALIAS = 378,
     TAND = 379,
     TASSIGN = 380,
     TASSIGNBAND = 381,
     TASSIGNBOR = 382,
     TASSIGNBXOR = 383,
     TASSIGNDIVIDE = 384,
     TASSIGNEXP = 385,
     TASSIGNLAND = 386,
     TASSIGNLOR = 387,
     TASSIGNMINUS = 388,
     TASSIGNMOD = 389,
     TASSIGNMULTIPLY = 390,
     TASSIGNPLUS = 391,
     TASSIGNREDUCE = 392,
     TASSIGNSL = 393,
     TASSIGNSR = 394,
     TBANG = 395,
     TBAND = 396,
     TBNOT = 397,
     TBOR = 398,
     TBXOR = 399,
     TCOLON = 400,
     TCOMMA = 401,
     TDIVIDE = 402,
     TDOT = 403,
     TDOTDOT = 404,
     TDOTDOTDOT = 405,
     TEQUAL = 406,
     TEXP = 407,
     TGREATER = 408,
     TGREATEREQUAL = 409,
     THASH = 410,
     TIO = 411,
     TLESS = 412,
     TLESSEQUAL = 413,
     TMINUS = 414,
     TMOD = 415,
     TNOTEQUAL = 416,
     TOR = 417,
     TPLUS = 418,
     TQUESTION = 419,
     TSEMI = 420,
     TSHIFTLEFT = 421,
     TSHIFTRIGHT = 422,
     TSTAR = 423,
     TSWAP = 424,
     TLCBR = 425,
     TRCBR = 426,
     TLP = 427,
     TRP = 428,
     TLSBR = 429,
     TRSBR = 430,
     TNOELSE = 431,
     TDOTDOTOPENHIGH = 432,
     TUMINUS = 433,
     TUPLUS = 434,
     TLNOT = 435
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
#line 500 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 507 "bison-chapel.cpp"
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
#define YYLAST   21112

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  174
/* YYNRULES -- Number of rules.  */
#define YYNRULES  715
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1289

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   435

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
     175,   176,   177,   178,   179,   180
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    17,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    45,    47,    50,    54,    58,    62,    66,    69,    71,
      73,    75,    77,    79,    81,    84,    88,    92,    96,   100,
     103,   107,   111,   114,   117,   119,   123,   126,   128,   130,
     135,   139,   144,   149,   150,   152,   154,   155,   157,   158,
     160,   162,   163,   171,   174,   178,   182,   184,   187,   189,
     191,   195,   199,   203,   209,   211,   215,   219,   223,   229,
     235,   236,   238,   239,   241,   243,   248,   255,   264,   273,
     280,   289,   298,   303,   305,   309,   313,   319,   321,   325,
     329,   334,   339,   344,   349,   354,   359,   360,   362,   364,
     366,   368,   370,   372,   374,   376,   378,   380,   382,   384,
     386,   388,   390,   392,   394,   396,   398,   400,   402,   404,
     406,   408,   410,   412,   414,   416,   418,   420,   422,   424,
     426,   428,   430,   432,   434,   436,   438,   440,   442,   444,
     446,   448,   450,   452,   454,   456,   458,   460,   463,   465,
     468,   472,   477,   481,   483,   485,   489,   493,   495,   499,
     502,   504,   506,   509,   510,   514,   516,   518,   520,   522,
     524,   526,   528,   532,   538,   544,   547,   548,   557,   558,
     568,   569,   578,   579,   589,   590,   599,   600,   610,   614,
     618,   624,   630,   633,   639,   643,   647,   654,   661,   666,
     672,   678,   682,   686,   693,   699,   706,   712,   719,   723,
     728,   732,   737,   743,   749,   753,   757,   764,   772,   779,
     787,   792,   798,   803,   809,   814,   819,   823,   830,   836,
     841,   845,   852,   858,   865,   871,   880,   888,   893,   898,
     905,   909,   911,   915,   917,   919,   921,   923,   925,   927,
     929,   931,   933,   935,   937,   939,   941,   943,   945,   947,
     949,   951,   953,   955,   962,   967,   975,   981,   985,   992,
     995,   997,   999,  1002,  1006,  1007,  1010,  1013,  1017,  1023,
    1025,  1029,  1033,  1039,  1045,  1046,  1049,  1053,  1056,  1060,
    1067,  1074,  1076,  1078,  1080,  1081,  1084,  1085,  1088,  1092,
    1098,  1104,  1106,  1108,  1111,  1115,  1117,  1121,  1124,  1126,
    1130,  1131,  1132,  1141,  1142,  1144,  1146,  1147,  1148,  1159,
    1163,  1167,  1173,  1179,  1183,  1185,  1189,  1191,  1194,  1196,
    1198,  1200,  1202,  1204,  1206,  1208,  1210,  1212,  1214,  1216,
    1218,  1220,  1222,  1224,  1226,  1228,  1230,  1232,  1234,  1236,
    1238,  1240,  1242,  1244,  1246,  1248,  1250,  1252,  1254,  1256,
    1258,  1260,  1262,  1264,  1266,  1268,  1270,  1272,  1274,  1276,
    1277,  1281,  1285,  1287,  1291,  1292,  1294,  1299,  1305,  1310,
    1316,  1323,  1330,  1331,  1333,  1335,  1337,  1339,  1341,  1344,
    1347,  1349,  1351,  1353,  1354,  1356,  1358,  1361,  1363,  1365,
    1367,  1369,  1371,  1372,  1374,  1377,  1379,  1381,  1383,  1384,
    1386,  1388,  1390,  1392,  1394,  1396,  1398,  1401,  1404,  1405,
    1408,  1411,  1416,  1421,  1423,  1427,  1431,  1435,  1439,  1443,
    1447,  1451,  1454,  1456,  1458,  1462,  1467,  1472,  1475,  1480,
    1481,  1484,  1487,  1489,  1491,  1493,  1496,  1498,  1503,  1507,
    1509,  1513,  1517,  1523,  1525,  1527,  1531,  1533,  1536,  1540,
    1541,  1544,  1547,  1551,  1554,  1559,  1563,  1567,  1572,  1576,
    1577,  1580,  1583,  1586,  1588,  1589,  1592,  1595,  1598,  1600,
    1605,  1610,  1617,  1621,  1622,  1624,  1626,  1630,  1635,  1639,
    1644,  1651,  1652,  1655,  1658,  1661,  1664,  1666,  1668,  1672,
    1676,  1678,  1682,  1684,  1686,  1688,  1692,  1696,  1697,  1699,
    1701,  1705,  1709,  1713,  1715,  1717,  1719,  1721,  1723,  1726,
    1728,  1730,  1732,  1734,  1736,  1738,  1741,  1746,  1751,  1756,
    1762,  1765,  1768,  1770,  1773,  1775,  1778,  1780,  1783,  1785,
    1788,  1790,  1792,  1799,  1806,  1811,  1821,  1831,  1839,  1846,
    1853,  1858,  1868,  1878,  1886,  1891,  1898,  1905,  1915,  1925,
    1932,  1934,  1936,  1938,  1940,  1942,  1944,  1946,  1948,  1952,
    1953,  1955,  1960,  1962,  1966,  1971,  1973,  1977,  1982,  1986,
    1990,  1992,  1994,  1997,  1999,  2002,  2004,  2006,  2010,  2012,
    2015,  2018,  2021,  2024,  2027,  2036,  2045,  2055,  2065,  2070,
    2072,  2074,  2076,  2078,  2080,  2082,  2084,  2086,  2088,  2093,
    2097,  2101,  2105,  2108,  2111,  2114,  2116,  2117,  2119,  2122,
    2125,  2127,  2129,  2131,  2133,  2135,  2137,  2140,  2143,  2145,
    2147,  2152,  2157,  2162,  2166,  2170,  2174,  2178,  2184,  2188,
    2193,  2197,  2202,  2204,  2206,  2208,  2210,  2212,  2214,  2216,
    2218,  2220,  2222,  2224,  2228,  2233,  2237,  2242,  2246,  2251,
    2255,  2261,  2265,  2269,  2273,  2277,  2281,  2285,  2289,  2293,
    2297,  2301,  2305,  2309,  2313,  2317,  2321,  2325,  2329,  2333,
    2337,  2341,  2345,  2349,  2353,  2356,  2359,  2362,  2365,  2368,
    2371,  2374,  2378,  2382,  2386,  2390,  2394,  2398,  2402,  2406,
    2408,  2410,  2412,  2414,  2416,  2418
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     182,     0,    -1,   183,    -1,    -1,   183,   184,    -1,   186,
      -1,   185,   186,    -1,    81,     8,    -1,   185,    81,     8,
      -1,   188,    -1,   195,    -1,   197,    -1,   187,    -1,   203,
      -1,   204,    -1,   207,    -1,   233,    -1,   243,    -1,   238,
      -1,   245,    -1,   247,    -1,   217,    -1,    14,   186,    -1,
      16,   209,   165,    -1,    28,   209,   165,    -1,    58,   211,
     186,    -1,   121,   338,   165,    -1,     1,   165,    -1,   208,
      -1,   236,    -1,   234,    -1,   220,    -1,   252,    -1,   251,
      -1,   326,   165,    -1,    15,   327,   186,    -1,    23,   327,
     197,    -1,    30,   314,   165,    -1,    62,   338,   216,    -1,
      62,   216,    -1,    72,   338,   216,    -1,    95,   338,   216,
      -1,    95,   216,    -1,   101,   186,    -1,   189,    -1,    31,
       8,   189,    -1,    31,   189,    -1,   191,    -1,   222,    -1,
     192,   193,    66,   211,    -1,   190,   170,   171,    -1,   190,
     170,   198,   171,    -1,   190,   170,     1,   171,    -1,    -1,
      86,    -1,    83,    -1,    -1,    85,    -1,    -1,    86,    -1,
      83,    -1,    -1,    49,   196,   194,   193,    66,   211,   165,
      -1,   170,   171,    -1,   170,   198,   171,    -1,   170,     1,
     171,    -1,   184,    -1,   198,   184,    -1,   338,    -1,   276,
      -1,   338,    13,   338,    -1,   199,   146,   338,    -1,   199,
     146,   276,    -1,   199,   146,   338,    13,   338,    -1,   338,
      -1,   338,    13,   338,    -1,   338,    13,   111,    -1,   200,
     146,   338,    -1,   200,   146,   338,    13,   338,    -1,   200,
     146,   338,    13,   111,    -1,    -1,   199,    -1,    -1,    86,
      -1,    83,    -1,   202,   114,   200,   165,    -1,   202,   114,
     338,    37,   199,   165,    -1,   202,   114,   338,    13,   338,
      37,   199,   165,    -1,   202,   114,   338,    13,   111,    37,
     199,   165,    -1,   202,   114,   338,    73,   201,   165,    -1,
     202,   114,   338,    13,   338,    73,   201,   165,    -1,   202,
     114,   338,    13,   111,    73,   201,   165,    -1,   202,    47,
     206,   165,    -1,   338,    -1,   338,   148,   276,    -1,   338,
      13,   212,    -1,   338,   148,   170,   199,   171,    -1,   205,
      -1,   206,   146,   205,    -1,    91,   313,   165,    -1,   341,
     275,   340,   165,    -1,   341,   169,   340,   165,    -1,   341,
     137,   340,   165,    -1,   341,   131,   340,   165,    -1,   341,
     132,   340,   165,    -1,   341,   125,    69,   165,    -1,    -1,
       3,    -1,     3,    -1,    70,    -1,   103,    -1,    40,    -1,
     106,    -1,   213,    -1,     3,    -1,    70,    -1,   103,    -1,
      40,    -1,   106,    -1,   213,    -1,     3,    -1,   103,    -1,
      17,    -1,    53,    -1,   110,    -1,    87,    -1,    46,    -1,
      25,    -1,    20,    -1,    99,    -1,   101,    -1,    97,    -1,
      78,    -1,    96,    -1,    18,    -1,   113,    -1,    34,    -1,
      50,    -1,    63,    -1,    71,    -1,   116,    -1,    17,    -1,
      36,    -1,    53,    -1,   110,    -1,    87,    -1,    46,    -1,
      25,    -1,    20,    -1,    99,    -1,    63,    -1,    71,    -1,
     116,    -1,   101,    -1,    97,    -1,    34,    -1,    50,    -1,
      33,   186,    -1,   197,    -1,    92,   165,    -1,    92,   340,
     165,    -1,   338,    13,   299,   211,    -1,   338,    13,   211,
      -1,   338,    -1,   218,    -1,   219,   146,   218,    -1,    64,
     219,   216,    -1,   222,    -1,    31,     8,   222,    -1,    31,
     222,    -1,   165,    -1,   224,    -1,    86,   224,    -1,    -1,
      83,   223,   224,    -1,   268,    -1,   300,    -1,   259,    -1,
     296,    -1,   255,    -1,   225,    -1,   226,    -1,    44,   338,
     165,    -1,    44,   338,    37,   199,   165,    -1,    44,   338,
      73,   201,   165,    -1,    44,   300,    -1,    -1,    39,    88,
     227,   211,   257,   170,   258,   171,    -1,    -1,    39,     8,
      88,   228,   211,   257,   170,   258,   171,    -1,    -1,    39,
     112,   229,   211,   257,   170,   258,   171,    -1,    -1,    39,
       8,   112,   230,   211,   257,   170,   258,   171,    -1,    -1,
      38,    88,   231,   211,   257,   170,   258,   171,    -1,    -1,
      38,     8,    88,   232,   211,   257,   170,   258,   171,    -1,
      39,   339,   268,    -1,    38,   339,   268,    -1,    39,   339,
     299,   301,   165,    -1,    38,   339,   299,   301,   165,    -1,
      39,    11,    -1,    33,   186,   119,   338,   165,    -1,   119,
     338,   216,    -1,   119,   237,   216,    -1,    24,   338,    48,
     338,   327,   216,    -1,    24,   338,    48,   235,   327,   216,
      -1,    24,   338,   327,   216,    -1,    41,   338,    48,   338,
     216,    -1,    41,   338,    48,   235,   216,    -1,    41,   338,
     216,    -1,    41,   235,   216,    -1,    41,    79,   211,    48,
     338,   216,    -1,    42,   338,    48,   338,   216,    -1,    42,
     338,    48,   338,   330,   216,    -1,    42,   338,    48,   235,
     216,    -1,    42,   338,    48,   235,   330,   216,    -1,    42,
     338,   216,    -1,    42,   338,   330,   216,    -1,    42,   235,
     216,    -1,    42,   235,   330,   216,    -1,    43,   338,    48,
     338,   216,    -1,    43,   338,    48,   235,   216,    -1,    43,
     338,   216,    -1,    43,   235,   216,    -1,   174,   313,    48,
     338,   175,   186,    -1,   174,   313,    48,   338,   330,   175,
     186,    -1,   174,   313,    48,   235,   175,   186,    -1,   174,
     313,    48,   235,   330,   175,   186,    -1,   174,   313,   175,
     186,    -1,   174,   313,   330,   175,   186,    -1,   174,   235,
     175,   186,    -1,   174,   235,   330,   175,   186,    -1,   122,
     172,   313,   173,    -1,    45,   338,   102,   186,    -1,    45,
     338,   197,    -1,    45,   338,   102,   186,    35,   186,    -1,
      45,   338,   197,    35,   186,    -1,    45,   237,   102,   186,
      -1,    45,   237,   197,    -1,    45,   237,   102,   186,    35,
     186,    -1,    45,   237,   197,    35,   186,    -1,    45,   338,
     275,   338,   102,   186,    -1,    45,   338,   275,   338,   197,
      -1,    45,   338,   275,   338,   102,   186,    35,   186,    -1,
      45,   338,   275,   338,   197,    35,   186,    -1,   115,   211,
     125,   338,    -1,    27,   211,   125,   338,    -1,    57,   211,
     172,   239,   173,   197,    -1,    57,   211,   197,    -1,   240,
      -1,   239,   146,   240,    -1,   211,    -1,     3,    -1,    17,
      -1,    53,    -1,   110,    -1,    87,    -1,    46,    -1,    25,
      -1,    20,    -1,    99,    -1,    63,    -1,    71,    -1,   116,
      -1,   242,    -1,    70,    -1,   103,    -1,    40,    -1,   106,
      -1,    34,    -1,    50,    -1,    56,   211,   172,   318,   173,
     165,    -1,   241,    56,   211,   165,    -1,   241,    56,   211,
     172,   318,   173,   165,    -1,    56,   211,   172,   318,   173,
      -1,   241,    56,   211,    -1,   241,    56,   211,   172,   318,
     173,    -1,    29,   186,    -1,   107,    -1,   108,    -1,   246,
     187,    -1,   246,   197,   248,    -1,    -1,   248,   249,    -1,
      21,   197,    -1,    21,   250,   197,    -1,    21,   172,   250,
     173,   197,    -1,   211,    -1,   211,   145,   338,    -1,   104,
     338,   165,    -1,    94,   338,   170,   253,   171,    -1,    94,
     338,   170,     1,   171,    -1,    -1,   253,   254,    -1,   117,
     313,   216,    -1,    75,   186,    -1,    75,    33,   186,    -1,
     256,   211,   257,   170,   258,   171,    -1,   256,   211,   257,
     170,     1,   171,    -1,    22,    -1,    88,    -1,   112,    -1,
      -1,   145,   313,    -1,    -1,   258,   221,    -1,   258,   185,
     221,    -1,   260,   211,   170,   261,   171,    -1,   260,   211,
     170,     1,   171,    -1,    36,    -1,   262,    -1,   261,   146,
      -1,   261,   146,   262,    -1,   263,    -1,    31,     8,   263,
      -1,    31,   263,    -1,   211,    -1,   211,   125,   338,    -1,
      -1,    -1,    59,   265,   278,   266,   286,   308,   292,   289,
      -1,    -1,    51,    -1,    77,    -1,    -1,    -1,   267,   285,
     269,   271,   270,   286,   307,   287,   292,   288,    -1,   284,
     273,   277,    -1,   284,   275,   277,    -1,   284,   272,   148,
     273,   277,    -1,   284,   272,   148,   275,   277,    -1,   284,
       1,   277,    -1,   320,    -1,   172,   338,   173,    -1,   210,
      -1,   211,   140,    -1,   274,    -1,   141,    -1,   143,    -1,
     144,    -1,   142,    -1,   151,    -1,   161,    -1,   158,    -1,
     154,    -1,   157,    -1,   153,    -1,   163,    -1,   159,    -1,
     168,    -1,   147,    -1,   166,    -1,   167,    -1,   160,    -1,
     152,    -1,   140,    -1,    19,    -1,   155,    -1,    12,    -1,
     169,    -1,   156,    -1,    55,    -1,   145,    -1,   125,    -1,
     136,    -1,   133,    -1,   135,    -1,   129,    -1,   134,    -1,
     130,    -1,   126,    -1,   127,    -1,   128,    -1,   139,    -1,
     138,    -1,   274,    -1,   275,    -1,    -1,   172,   280,   173,
      -1,   172,   280,   173,    -1,   281,    -1,   279,   146,   281,
      -1,    -1,   279,    -1,   282,   211,   312,   305,    -1,   185,
     282,   211,   312,   305,    -1,   282,   211,   312,   291,    -1,
     185,   282,   211,   312,   291,    -1,   282,   172,   304,   173,
     312,   305,    -1,   282,   172,   304,   173,   312,   291,    -1,
      -1,   283,    -1,    48,    -1,    52,    -1,    76,    -1,    27,
      -1,    27,    48,    -1,    27,    90,    -1,    79,    -1,    90,
      -1,   109,    -1,    -1,    79,    -1,    90,    -1,    27,    90,
      -1,    27,    -1,   109,    -1,    84,    -1,    54,    -1,    74,
      -1,    -1,    27,    -1,    27,    90,    -1,    90,    -1,    79,
      -1,   109,    -1,    -1,   105,    -1,   165,    -1,   289,    -1,
     197,    -1,   217,    -1,     4,    -1,   150,    -1,   150,   338,
      -1,   150,   290,    -1,    -1,   118,   338,    -1,    61,   293,
      -1,   118,   338,    61,   293,    -1,    61,   293,   118,   338,
      -1,   294,    -1,   293,   146,   294,    -1,   295,   125,   295,
      -1,   295,   157,   295,    -1,   295,   158,   295,    -1,   295,
     151,   295,    -1,   295,   153,   295,    -1,   295,   154,   295,
      -1,    92,   295,    -1,     3,    -1,   103,    -1,   109,   297,
     165,    -1,    26,   109,   297,   165,    -1,    39,   109,   297,
     165,    -1,   211,   298,    -1,   211,   298,   146,   297,    -1,
      -1,   125,   321,    -1,   125,   309,    -1,    79,    -1,    27,
      -1,    90,    -1,    27,    90,    -1,   115,    -1,    26,   299,
     301,   165,    -1,   299,   301,   165,    -1,   302,    -1,   301,
     146,   302,    -1,   211,   308,   305,    -1,   172,   304,   173,
     308,   305,    -1,   111,    -1,   211,    -1,   172,   304,   173,
      -1,   303,    -1,   303,   146,    -1,   303,   146,   304,    -1,
      -1,   125,    69,    -1,   125,   340,    -1,   174,   175,   321,
      -1,   174,   175,    -1,   174,   313,   175,   321,    -1,   174,
     313,   175,    -1,   174,   175,   306,    -1,   174,   313,   175,
     306,    -1,   174,     1,   175,    -1,    -1,   145,   321,    -1,
     145,   306,    -1,   145,   215,    -1,     1,    -1,    -1,   145,
     321,    -1,   145,   309,    -1,   145,   215,    -1,     1,    -1,
     174,   313,   175,   321,    -1,   174,   313,   175,   309,    -1,
     174,   313,    48,   338,   175,   321,    -1,   174,     1,   175,
      -1,    -1,   321,    -1,   290,    -1,   174,   175,   310,    -1,
     174,   313,   175,   310,    -1,   174,   175,   311,    -1,   174,
     313,   175,   311,    -1,   174,   313,    48,   338,   175,   310,
      -1,    -1,   145,   321,    -1,   145,   290,    -1,   145,   215,
      -1,   145,   311,    -1,   338,    -1,   290,    -1,   313,   146,
     338,    -1,   313,   146,   290,    -1,   338,    -1,   314,   146,
     338,    -1,   111,    -1,   340,    -1,   290,    -1,   315,   146,
     315,    -1,   316,   146,   315,    -1,    -1,   318,    -1,   319,
      -1,   318,   146,   319,    -1,   212,   125,   290,    -1,   212,
     125,   340,    -1,   290,    -1,   340,    -1,   212,    -1,   214,
      -1,   322,    -1,   322,   164,    -1,   164,    -1,   325,    -1,
     341,    -1,   324,    -1,   351,    -1,   350,    -1,    97,   338,
      -1,    50,   172,   317,   173,    -1,    34,   172,   317,   173,
      -1,   100,   172,   317,   173,    -1,    98,   100,   172,   319,
     173,    -1,    14,   338,    -1,   101,   338,    -1,    78,    -1,
      78,   338,    -1,   113,    -1,   113,   338,    -1,    96,    -1,
      96,   338,    -1,    18,    -1,    18,   338,    -1,    22,    -1,
      88,    -1,    41,   338,    48,   338,    33,   338,    -1,    41,
     338,    48,   235,    33,   338,    -1,    41,   338,    33,   338,
      -1,    41,   338,    48,   338,    33,    45,   338,   102,   338,
      -1,    41,   338,    48,   235,    33,    45,   338,   102,   338,
      -1,    41,   338,    33,    45,   338,   102,   338,    -1,    42,
     338,    48,   338,    33,   338,    -1,    42,   338,    48,   235,
      33,   338,    -1,    42,   338,    33,   338,    -1,    42,   338,
      48,   338,    33,    45,   338,   102,   338,    -1,    42,   338,
      48,   235,    33,    45,   338,   102,   338,    -1,    42,   338,
      33,    45,   338,   102,   338,    -1,   174,   313,   175,   338,
      -1,   174,   313,    48,   338,   175,   338,    -1,   174,   313,
      48,   235,   175,   338,    -1,   174,   313,    48,   338,   175,
      45,   338,   102,   338,    -1,   174,   313,    48,   235,   175,
      45,   338,   102,   338,    -1,    45,   338,   102,   338,    35,
     338,    -1,    68,    -1,   325,    -1,   320,    -1,   344,    -1,
     343,    -1,   264,    -1,   336,    -1,   337,    -1,   334,   156,
     338,    -1,    -1,   328,    -1,   120,   172,   329,   173,    -1,
     332,    -1,   329,   146,   332,    -1,   120,   172,   331,   173,
      -1,   332,    -1,   331,   146,   332,    -1,   333,   320,   308,
     305,    -1,   354,    89,   320,    -1,   338,    89,   320,    -1,
      27,    -1,    48,    -1,    27,    48,    -1,    90,    -1,    27,
      90,    -1,   115,    -1,   341,    -1,   334,   156,   338,    -1,
      67,    -1,    67,    78,    -1,    67,    96,    -1,    67,   113,
      -1,    67,    18,    -1,   335,   338,    -1,    67,    78,   172,
     338,   173,   172,   317,   173,    -1,    67,    96,   172,   338,
     173,   172,   317,   173,    -1,    67,    78,   172,   338,   173,
     172,   317,   173,   164,    -1,    67,    96,   172,   338,   173,
     172,   317,   173,   164,    -1,    60,   301,    48,   338,    -1,
     348,    -1,   321,    -1,   323,    -1,   352,    -1,   353,    -1,
     264,    -1,   336,    -1,   337,    -1,   244,    -1,   172,   150,
     338,   173,    -1,   338,   145,   338,    -1,   338,   149,   338,
      -1,   338,   177,   338,    -1,   338,   149,    -1,   149,   338,
      -1,   177,   338,    -1,   149,    -1,    -1,   338,    -1,   107,
     338,    -1,   108,   338,    -1,   338,    -1,   320,    -1,   343,
      -1,   344,    -1,   345,    -1,   341,    -1,   338,   140,    -1,
     322,   164,    -1,   264,    -1,   347,    -1,   342,   172,   317,
     173,    -1,   342,   174,   317,   175,    -1,    82,   172,   317,
     173,    -1,   338,   148,   212,    -1,   338,   148,   109,    -1,
     338,   148,    34,    -1,   338,   148,    63,    -1,   338,   148,
      20,   172,   173,    -1,   172,   315,   173,    -1,   172,   315,
     146,   173,    -1,   172,   316,   173,    -1,   172,   316,   146,
     173,    -1,    40,    -1,   106,    -1,     8,    -1,     9,    -1,
     346,    -1,   347,    -1,     5,    -1,     6,    -1,     7,    -1,
      10,    -1,    70,    -1,   170,   313,   171,    -1,   170,   313,
     146,   171,    -1,   174,   313,   175,    -1,   174,   313,   146,
     175,    -1,   174,   349,   175,    -1,   174,   349,   146,   175,
      -1,   338,   123,   338,    -1,   349,   146,   338,   123,   338,
      -1,   338,   163,   338,    -1,   338,   159,   338,    -1,   338,
     168,   338,    -1,   338,   147,   338,    -1,   338,   166,   338,
      -1,   338,   167,   338,    -1,   338,   160,   338,    -1,   338,
     151,   338,    -1,   338,   161,   338,    -1,   338,   158,   338,
      -1,   338,   154,   338,    -1,   338,   157,   338,    -1,   338,
     153,   338,    -1,   338,   141,   338,    -1,   338,   143,   338,
      -1,   338,   144,   338,    -1,   338,   124,   338,    -1,   338,
     162,   338,    -1,   338,   152,   338,    -1,   338,    19,   338,
      -1,   338,    12,   338,    -1,   338,   155,   338,    -1,   338,
      32,   338,    -1,   163,   338,    -1,   159,   338,    -1,    65,
     338,    -1,    80,   338,    -1,   140,   338,    -1,   338,   140,
      -1,   142,   338,    -1,   338,    89,   338,    -1,   338,    89,
     235,    -1,   354,    89,   338,    -1,   354,    89,   235,    -1,
     338,    93,   338,    -1,   338,    93,   235,    -1,   354,    93,
     338,    -1,   354,    93,   235,    -1,   163,    -1,   168,    -1,
     124,    -1,   162,    -1,   141,    -1,   143,    -1,   144,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   518,   518,   523,   524,   530,   531,   536,   537,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   564,   565,
     566,   567,   568,   569,   570,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   583,   584,   586,   591,   592,   596,
     609,   614,   619,   627,   628,   629,   633,   634,   638,   639,
     640,   645,   644,   665,   666,   667,   672,   673,   678,   683,
     688,   693,   697,   701,   710,   715,   720,   725,   729,   733,
     741,   746,   750,   751,   752,   756,   757,   758,   759,   760,
     761,   762,   766,   771,   772,   778,   779,   783,   784,   788,
     792,   794,   796,   798,   800,   802,   809,   810,   814,   815,
     816,   817,   818,   819,   822,   823,   824,   825,   826,   827,
     839,   840,   851,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,   862,   863,   864,   865,   866,   867,   868,
     869,   873,   874,   875,   876,   877,   878,   879,   880,   881,
     882,   883,   884,   891,   892,   893,   894,   898,   899,   903,
     904,   908,   909,   910,   914,   915,   919,   923,   924,   926,
     931,   932,   933,   943,   943,   948,   949,   950,   951,   952,
     953,   954,   958,   959,   960,   961,   966,   965,   982,   981,
     999,   998,  1015,  1014,  1032,  1031,  1047,  1046,  1062,  1066,
    1071,  1079,  1090,  1097,  1098,  1099,  1100,  1101,  1102,  1103,
    1104,  1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,
    1114,  1115,  1116,  1117,  1118,  1119,  1120,  1126,  1132,  1138,
    1144,  1151,  1158,  1162,  1169,  1173,  1174,  1175,  1176,  1178,
    1179,  1180,  1181,  1183,  1185,  1187,  1189,  1194,  1195,  1199,
    1201,  1209,  1210,  1215,  1220,  1221,  1222,  1223,  1224,  1225,
    1226,  1227,  1228,  1229,  1230,  1231,  1232,  1239,  1240,  1241,
    1242,  1251,  1252,  1256,  1258,  1261,  1267,  1269,  1272,  1278,
    1281,  1282,  1285,  1286,  1290,  1291,  1295,  1296,  1297,  1301,
    1302,  1306,  1309,  1311,  1316,  1317,  1321,  1323,  1325,  1332,
    1342,  1356,  1361,  1366,  1374,  1375,  1380,  1381,  1383,  1388,
    1404,  1411,  1420,  1428,  1432,  1439,  1440,  1442,  1447,  1448,
    1453,  1458,  1452,  1485,  1488,  1492,  1500,  1510,  1499,  1549,
    1553,  1558,  1562,  1567,  1574,  1575,  1579,  1580,  1581,  1585,
    1586,  1587,  1588,  1589,  1590,  1591,  1592,  1593,  1594,  1595,
    1596,  1597,  1598,  1599,  1600,  1601,  1602,  1603,  1604,  1605,
    1606,  1607,  1608,  1609,  1610,  1614,  1615,  1616,  1617,  1618,
    1619,  1620,  1621,  1622,  1623,  1624,  1625,  1629,  1630,  1634,
    1635,  1639,  1643,  1644,  1648,  1649,  1653,  1655,  1657,  1659,
    1661,  1663,  1668,  1669,  1673,  1674,  1675,  1676,  1677,  1678,
    1679,  1680,  1681,  1685,  1686,  1687,  1688,  1689,  1690,  1694,
    1695,  1696,  1700,  1701,  1702,  1703,  1704,  1705,  1709,  1710,
    1713,  1714,  1718,  1719,  1723,  1727,  1728,  1729,  1737,  1738,
    1740,  1742,  1744,  1749,  1751,  1756,  1757,  1758,  1759,  1760,
    1761,  1762,  1766,  1768,  1773,  1775,  1777,  1782,  1795,  1812,
    1813,  1815,  1820,  1821,  1822,  1823,  1824,  1828,  1834,  1842,
    1843,  1851,  1853,  1858,  1860,  1862,  1867,  1869,  1871,  1878,
    1879,  1880,  1885,  1887,  1889,  1893,  1897,  1899,  1903,  1911,
    1912,  1913,  1914,  1915,  1920,  1921,  1922,  1923,  1924,  1944,
    1948,  1952,  1960,  1967,  1968,  1969,  1973,  1975,  1981,  1983,
    1985,  1990,  1991,  1992,  1993,  1994,  2000,  2001,  2002,  2003,
    2007,  2008,  2012,  2013,  2014,  2018,  2019,  2023,  2024,  2028,
    2029,  2033,  2034,  2035,  2036,  2040,  2041,  2052,  2054,  2056,
    2062,  2063,  2064,  2065,  2066,  2067,  2069,  2071,  2073,  2075,
    2077,  2079,  2082,  2084,  2086,  2088,  2090,  2092,  2094,  2096,
    2099,  2101,  2106,  2108,  2110,  2112,  2114,  2116,  2118,  2120,
    2122,  2124,  2126,  2128,  2130,  2137,  2143,  2149,  2155,  2164,
    2174,  2182,  2183,  2184,  2185,  2186,  2187,  2188,  2189,  2194,
    2195,  2199,  2203,  2204,  2208,  2212,  2213,  2217,  2221,  2225,
    2232,  2233,  2234,  2235,  2236,  2237,  2241,  2242,  2247,  2249,
    2253,  2257,  2261,  2269,  2274,  2280,  2286,  2293,  2303,  2311,
    2312,  2313,  2314,  2315,  2316,  2317,  2318,  2319,  2320,  2322,
    2324,  2326,  2341,  2343,  2345,  2347,  2352,  2353,  2357,  2358,
    2359,  2363,  2364,  2365,  2366,  2375,  2376,  2377,  2378,  2379,
    2383,  2384,  2385,  2389,  2390,  2391,  2392,  2393,  2401,  2402,
    2403,  2404,  2408,  2409,  2413,  2414,  2418,  2419,  2420,  2421,
    2422,  2423,  2424,  2425,  2427,  2429,  2430,  2431,  2435,  2443,
    2444,  2448,  2449,  2450,  2451,  2452,  2453,  2454,  2455,  2456,
    2457,  2458,  2459,  2460,  2461,  2462,  2463,  2464,  2465,  2466,
    2467,  2468,  2469,  2470,  2475,  2476,  2477,  2478,  2479,  2480,
    2481,  2485,  2486,  2487,  2488,  2492,  2493,  2494,  2495,  2500,
    2501,  2502,  2503,  2504,  2505,  2506
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
  "TDEFER", "TDELETE", "TDEPRECATED", "TDMAPPED", "TDO", "TDOMAIN",
  "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE", "TFOR",
  "TFORALL", "TFOREACH", "TFORWARDING", "TIF", "TIMAG", "TIMPORT", "TIN",
  "TINCLUDE", "TINDEX", "TINLINE", "TINOUT", "TINT", "TITER",
  "TINITEQUALS", "TIMPLEMENTS", "TINTERFACE", "TLABEL", "TLAMBDA", "TLET",
  "TLIFETIME", "TLOCAL", "TLOCALE", "TMANAGE", "TMINUSMINUS", "TMODULE",
  "TNEW", "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY",
  "TOPERATOR", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM",
  "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE",
  "TPUBLIC", "TREAL", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN",
  "TSCAN", "TSELECT", "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE",
  "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS",
  "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE", "TUNION",
  "TUNMANAGED", "TUSE", "TVAR", "TVOID", "TWHEN", "TWHERE", "TWHILE",
  "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND",
  "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP",
  "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL",
  "TASSIGNSR", "TBANG", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON",
  "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP",
  "TGREATER", "TGREATEREQUAL", "THASH", "TIO", "TLESS", "TLESSEQUAL",
  "TMINUS", "TMOD", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP",
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TDOTDOTOPENHIGH", "TUMINUS",
  "TUPLUS", "TLNOT", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "tryable_stmt",
  "deprecated_decl_stmt", "deprecated_decl_base", "module_decl_start",
  "module_decl_stmt", "access_control", "opt_prototype",
  "include_access_control", "include_module_stmt", "$@1", "block_stmt",
  "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls",
  "use_access_control", "use_stmt", "import_stmt", "import_expr",
  "import_ls", "require_stmt", "assignment_stmt", "opt_label_ident",
  "ident_fn_def", "ident_def", "ident_use", "internal_type_ident_def",
  "scalar_type", "reserved_type_ident_use", "do_stmt", "return_stmt",
  "manager_expr", "manager_expr_ls", "manage_stmt",
  "deprecated_class_level_stmt", "class_level_stmt", "@2", "private_decl",
  "forwarding_stmt", "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6",
  "$@7", "$@8", "extern_block_stmt", "loop_stmt", "zippered_iterator",
  "if_stmt", "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "defer_stmt", "try_token",
  "try_stmt", "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt",
  "select_stmt", "when_stmt_ls", "when_stmt", "class_decl_stmt",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_header", "enum_ls", "deprecated_enum_item", "enum_item",
  "lambda_decl_expr", "$@9", "$@10", "linkage_spec", "fn_decl_stmt",
  "$@11", "$@12", "fn_decl_stmt_inner", "fn_decl_receiver_expr",
  "fn_ident", "op_ident", "assignop_ident", "all_op_name", "opt_formal_ls",
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
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   188,   188,   188,   189,   189,   190,
     191,   191,   191,   192,   192,   192,   193,   193,   194,   194,
     194,   196,   195,   197,   197,   197,   198,   198,   199,   199,
     199,   199,   199,   199,   200,   200,   200,   200,   200,   200,
     201,   201,   202,   202,   202,   203,   203,   203,   203,   203,
     203,   203,   204,   205,   205,   205,   205,   206,   206,   207,
     208,   208,   208,   208,   208,   208,   209,   209,   210,   210,
     210,   210,   210,   210,   211,   211,   211,   211,   211,   211,
     212,   212,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   215,   215,   215,   215,   216,   216,   217,
     217,   218,   218,   218,   219,   219,   220,   221,   221,   221,
     222,   222,   222,   223,   222,   224,   224,   224,   224,   224,
     224,   224,   225,   225,   225,   225,   227,   226,   228,   226,
     229,   226,   230,   226,   231,   226,   232,   226,   226,   226,
     226,   226,   233,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   235,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   237,   237,   238,
     238,   239,   239,   240,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   242,   242,   242,
     242,   242,   242,   243,   243,   243,   244,   244,   244,   245,
     246,   246,   247,   247,   248,   248,   249,   249,   249,   250,
     250,   251,   252,   252,   253,   253,   254,   254,   254,   255,
     255,   256,   256,   256,   257,   257,   258,   258,   258,   259,
     259,   260,   261,   261,   261,   262,   262,   262,   263,   263,
     265,   266,   264,   267,   267,   267,   269,   270,   268,   271,
     271,   271,   271,   271,   272,   272,   273,   273,   273,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   276,   276,   277,
     277,   278,   279,   279,   280,   280,   281,   281,   281,   281,
     281,   281,   282,   282,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   284,   284,   284,   284,   284,   284,   285,
     285,   285,   286,   286,   286,   286,   286,   286,   287,   287,
     288,   288,   289,   289,   290,   291,   291,   291,   292,   292,
     292,   292,   292,   293,   293,   294,   294,   294,   294,   294,
     294,   294,   295,   295,   296,   296,   296,   297,   297,   298,
     298,   298,   299,   299,   299,   299,   299,   300,   300,   301,
     301,   302,   302,   303,   303,   303,   304,   304,   304,   305,
     305,   305,   306,   306,   306,   306,   306,   306,   306,   307,
     307,   307,   307,   307,   308,   308,   308,   308,   308,   309,
     309,   309,   309,   310,   310,   310,   311,   311,   311,   311,
     311,   312,   312,   312,   312,   312,   313,   313,   313,   313,
     314,   314,   315,   315,   315,   316,   316,   317,   317,   318,
     318,   319,   319,   319,   319,   320,   320,   321,   321,   321,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   324,
     325,   326,   326,   326,   326,   326,   326,   326,   326,   327,
     327,   328,   329,   329,   330,   331,   331,   332,   332,   332,
     333,   333,   333,   333,   333,   333,   334,   334,   335,   335,
     335,   335,   335,   336,   336,   336,   336,   336,   337,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   339,   339,   340,   340,
     340,   341,   341,   341,   341,   342,   342,   342,   342,   342,
     343,   343,   343,   344,   344,   344,   344,   344,   345,   345,
     345,   345,   346,   346,   347,   347,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   349,
     349,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   351,   351,   351,   351,   351,   351,
     351,   352,   352,   352,   352,   353,   353,   353,   353,   354,
     354,   354,   354,   354,   354,   354
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     3,     3,     3,     3,     2,     1,     1,
       1,     1,     1,     1,     2,     3,     3,     3,     3,     2,
       3,     3,     2,     2,     1,     3,     2,     1,     1,     4,
       3,     4,     4,     0,     1,     1,     0,     1,     0,     1,
       1,     0,     7,     2,     3,     3,     1,     2,     1,     1,
       3,     3,     3,     5,     1,     3,     3,     3,     5,     5,
       0,     1,     0,     1,     1,     4,     6,     8,     8,     6,
       8,     8,     4,     1,     3,     3,     5,     1,     3,     3,
       4,     4,     4,     4,     4,     4,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       3,     4,     3,     1,     1,     3,     3,     1,     3,     2,
       1,     1,     2,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     5,     5,     2,     0,     8,     0,     9,
       0,     8,     0,     9,     0,     8,     0,     9,     3,     3,
       5,     5,     2,     5,     3,     3,     6,     6,     4,     5,
       5,     3,     3,     6,     5,     6,     5,     6,     3,     4,
       3,     4,     5,     5,     3,     3,     6,     7,     6,     7,
       4,     5,     4,     5,     4,     4,     3,     6,     5,     4,
       3,     6,     5,     6,     5,     8,     7,     4,     4,     6,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     4,     7,     5,     3,     6,     2,
       1,     1,     2,     3,     0,     2,     2,     3,     5,     1,
       3,     3,     5,     5,     0,     2,     3,     2,     3,     6,
       6,     1,     1,     1,     0,     2,     0,     2,     3,     5,
       5,     1,     1,     2,     3,     1,     3,     2,     1,     3,
       0,     0,     8,     0,     1,     1,     0,     0,    10,     3,
       3,     5,     5,     3,     1,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
       3,     0,     0,     1,     0,   120,   658,   659,   660,   654,
     655,   661,     0,   579,   106,   141,   548,   148,   550,   579,
       0,   147,     0,   453,   106,     0,     0,   323,     0,   271,
     142,   626,   626,   652,     0,     0,     0,     0,     0,   146,
      61,   272,   324,   143,     0,     0,     0,   320,     0,     0,
     150,     0,     0,   598,   570,   662,   151,     0,   325,   542,
     452,     0,     0,     0,   173,   323,   145,   551,   454,     0,
       0,     0,     0,   546,     0,     0,   149,     0,     0,   121,
       0,   653,   280,   281,     0,   144,   303,   544,   456,   152,
       0,     0,   711,     0,   713,     0,   714,   715,   625,     0,
     712,   709,   529,   170,   710,     0,     0,     0,     0,     4,
       0,     5,    12,     9,    44,     0,    47,    56,    10,    11,
       0,    13,    14,    15,    28,   525,   526,    21,    31,    48,
     171,   180,   181,    16,    30,    29,    18,     0,   266,    17,
     617,    19,     0,    20,    33,    32,   179,     0,   177,     0,
     614,     0,   175,   178,     0,   176,   631,   610,   527,   611,
     532,   530,     0,     0,     0,   615,   616,     0,   531,     0,
     632,   633,   634,   656,   657,   609,   534,   533,   612,   613,
       0,    27,    22,   540,     0,     0,   580,   107,     0,     0,
     550,   142,     0,     0,     0,     0,   551,     0,     0,     0,
       0,   614,   631,   530,   615,   616,   549,   531,   632,   633,
       0,   579,     0,     0,   455,     0,   279,     0,   510,   323,
     301,   311,   626,   173,   323,   302,    46,     0,   517,   654,
     551,   627,   323,   654,   202,   551,     0,   190,   323,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   185,     0,
       0,     0,     0,     0,    58,   517,   114,   122,   134,   128,
     127,   136,   117,   126,   137,   123,   138,   115,   139,   132,
     125,   133,   131,   129,   130,   116,   118,   124,   135,   140,
       0,   119,     0,     0,     0,     0,     0,     0,   459,     0,
     158,    39,     0,   164,     0,   163,   696,   602,   599,   600,
     601,     0,   543,   697,     7,   517,   323,   172,   424,   507,
       0,   506,     0,     0,   159,   630,     0,     0,    42,     0,
     547,   535,     0,   517,    43,   541,     0,   449,     0,   545,
       0,     0,     0,   698,   700,   623,   695,   694,     0,    63,
      66,     0,     0,   512,     0,   514,     0,     0,   513,     0,
       0,   506,     0,   624,     0,     6,     0,    57,     0,     0,
       0,     0,   282,   284,   304,     0,   410,   411,   409,   326,
       0,   528,    34,     0,   603,     0,     0,     0,     0,     0,
       0,   699,     0,     0,     0,     0,     0,     0,   622,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,   372,   373,   374,   369,
     371,     0,     0,   367,   370,   368,   366,     0,   376,   375,
       0,     0,   517,   517,     0,     0,     0,    35,    23,     0,
       0,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,    24,     0,    37,    45,     0,   525,   523,     0,   518,
     519,   524,   196,     0,   199,     0,   188,   192,     0,     0,
       0,   198,     0,     0,     0,   212,     0,     0,   211,     0,
     220,     0,     0,     0,   218,     0,   225,     0,   224,     0,
      80,   182,     0,     0,     0,   240,     0,   365,   236,     0,
      60,    59,    56,     0,     0,     0,   250,    25,   392,   321,
     463,     0,   464,   466,     0,   488,     0,   469,     0,     0,
     157,    38,     0,   166,     0,     0,     0,    40,     0,   174,
       0,    99,   628,   629,   160,     0,    41,     0,     0,   291,
       0,   447,   444,   205,   204,    26,    65,    64,    67,     0,
     663,     0,     0,   648,     0,   650,     0,     0,     0,     0,
       0,     0,     0,     0,   667,     8,     0,    50,     0,     0,
      97,     0,    93,     0,    74,   277,   283,     0,     0,     0,
     403,   458,   578,   691,   690,   693,   702,   701,   706,   705,
     687,   684,   685,   686,   619,   674,   120,     0,   645,   646,
     121,   644,   643,   620,   678,   689,   683,   681,   692,   682,
     680,   672,   677,   679,   688,   671,   675,   676,   673,   621,
       0,     0,     0,     0,     0,     0,     0,     0,   704,   703,
     708,   707,   590,   591,   593,   595,     0,   582,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   665,   277,
     579,   579,   208,   445,   457,   511,     0,     0,   537,     0,
       0,   304,     0,     0,     0,   304,   446,   304,     0,     0,
       0,     0,   554,     0,     0,     0,   221,     0,   560,     0,
       0,   219,     0,     0,   360,   358,   363,   357,   339,   342,
     340,   341,   364,   352,   343,   356,   348,   346,   359,   362,
     347,   345,   350,   355,   344,   349,   353,   354,   351,   361,
       0,   377,   378,    69,    68,    81,     0,     0,     0,   239,
       0,   235,     0,     0,     0,     0,   536,     0,   253,     0,
     251,   397,   394,   395,   396,   400,   401,   402,   392,   385,
       0,   382,     0,   393,   412,     0,   467,     0,   155,   156,
     154,   153,     0,   487,   486,   610,     0,   461,   608,   460,
     165,   162,     0,     0,     0,   642,   509,   508,     0,     0,
       0,   538,   451,   610,     0,   664,   618,   649,   515,   651,
     516,   232,     0,     0,     0,   666,   230,   564,     0,   669,
     668,     0,    52,    51,    49,     0,    92,     0,     0,     0,
      85,     0,     0,    80,   274,     0,     0,   285,   305,     0,
       0,     0,   318,     0,   312,   315,   407,   404,   405,   408,
     327,     0,     0,   105,   103,   104,   102,   101,   100,   640,
     641,   592,   594,     0,   581,   141,   148,   147,   146,   143,
     150,   151,   145,   149,   144,   152,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   203,   521,   522,   520,   304,     0,   201,   304,   304,
       0,     0,   200,     0,   234,     0,     0,   210,     0,   209,
       0,   585,     0,     0,   216,     0,     0,   214,     0,   223,
     222,     0,   183,     0,   184,   248,   247,     0,   242,     0,
       0,   238,     0,   244,     0,   276,     0,     0,   398,   399,
       0,   392,   381,     0,   501,   413,   416,   415,   417,     0,
     465,   468,   469,     0,     0,   470,   471,   161,     0,     0,
     293,     0,     0,   292,   295,   539,   448,   233,     0,     0,
       0,     0,   231,     0,    98,    95,   357,   339,   342,   340,
     341,   350,   349,   351,     0,    94,    77,    76,    75,     0,
       0,     0,     0,   286,   289,     0,     0,   323,   310,     0,
     317,     0,   313,   309,   406,   412,   379,   108,   122,   128,
     127,   111,   126,   123,   138,   109,   139,   125,   129,   110,
     112,   124,   140,     0,   336,     0,   113,     0,   379,   338,
     379,   334,   647,   583,   469,   631,   631,     0,     0,     0,
       0,     0,     0,   276,     0,     0,     0,   207,   206,     0,
     306,     0,     0,   306,   306,   213,     0,     0,   553,     0,
     552,     0,   584,     0,     0,   559,   217,     0,   558,   215,
      72,    71,    70,   241,   237,   569,   243,     0,     0,   273,
     252,   249,   501,   383,     0,     0,   469,   414,   428,   462,
     492,     0,   665,   517,   517,     0,   297,     0,     0,   228,
     566,     0,     0,   226,   565,     0,   670,     0,     0,     0,
      80,     0,    80,    86,    89,   278,     0,     0,   287,   300,
     323,   173,   323,   299,   323,   307,   167,   316,   319,   314,
       0,   392,   333,     0,   337,     0,   329,   330,   587,     0,
       0,     0,     0,     0,     0,     0,     0,   278,   306,   323,
     306,   306,   323,   323,   557,     0,     0,   586,   563,     0,
       0,     0,     0,   246,    62,   469,   501,     0,   504,   503,
     505,   610,   425,   388,   386,     0,     0,     0,     0,   490,
     610,     0,     0,   298,   296,     0,   229,     0,   227,    96,
      79,    78,     0,     0,     0,     0,   275,     0,   290,   323,
     169,   308,   483,     0,   418,     0,   335,   108,   110,   379,
     379,     0,     0,     0,     0,     0,     0,   323,   195,   323,
     323,   187,   191,     0,     0,     0,     0,    73,   245,   389,
     387,   469,   493,     0,   427,   426,   442,     0,   443,   430,
     433,     0,   429,   422,   423,   322,     0,   604,   605,     0,
       0,    88,    91,    87,    90,   288,   168,     0,   482,   481,
     610,   419,   428,   380,   331,   332,     0,     0,     0,     0,
       0,     0,   197,   189,   193,   556,   555,   562,   561,   391,
     390,   495,   496,   498,   610,     0,   665,   441,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   610,   606,   607,
     568,   567,     0,   473,     0,     0,     0,   497,   499,   432,
     434,   435,   438,   439,   440,   436,   437,   431,   478,   476,
     610,   665,   420,   328,   421,   493,   477,   610,   500
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   340,  1084,   711,   112,   113,   114,   115,
     116,   117,   358,   492,   118,   254,   119,   341,   705,   563,
     706,   120,   121,   122,   560,   561,   123,   124,   188,   984,
     286,   125,   281,   126,   743,   291,   127,   293,   294,   128,
    1085,   129,   306,   130,   131,   132,   458,   653,   460,   654,
     453,   650,   133,   134,   846,   135,   252,   136,   719,   720,
     200,   138,   139,   140,   141,   142,   143,   566,   797,   955,
     144,   145,   759,   924,   146,   147,   568,   957,   148,   149,
     803,   804,   805,   201,   284,   734,   151,   152,   570,   965,
     810,   987,   988,   701,   702,   703,  1092,   499,   729,   730,
     731,   732,   733,   811,   369,   909,  1222,  1283,  1205,   447,
    1133,  1137,  1199,  1200,  1201,   153,   328,   531,   154,   155,
     287,   288,   503,   504,   747,  1219,  1164,   507,   744,  1242,
    1130,  1046,   342,   217,   346,   347,   448,   449,   450,   202,
     157,   158,   159,   160,   203,   162,   185,   186,   626,   471,
     870,   627,   628,   163,   164,   204,   205,   167,   238,   451,
     207,   169,   208,   209,   172,   173,   174,   175,   352,   176,
     177,   178,   179,   180
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1142
static const yytype_int16 yypact[] =
{
   -1142,   153,  3771, -1142,    50,   178, -1142, -1142, -1142, -1142,
   -1142, -1142,  5173,   144,   284,   241, 15347,   286, 19928,   144,
   12312,   371,   342,   266,   284,  5173, 12312,  1509,  5173,   234,
   20648, 12485,  8488,   387,  9182, 10747, 10747,  7617,  9355,   442,
   -1142,   267, -1142,   450, 20735, 20735, 20735, -1142,  2525, 10920,
     464, 12312, 12312,   208, -1142,   496,   518, 12312, -1142, 15347,
   -1142, 12312,   579,   423,   350, 17229,   537, 20822, -1142, 11095,
    8661, 12312, 10920, 15347, 12312,   507,   561,   446,  5173,   575,
   12312,   576, -1142, -1142, 20735,   588, -1142, 15347, -1142,   592,
    9355, 12312, -1142, 12312, -1142, 12312, -1142, -1142, 14862, 12312,
   -1142, 12312, -1142, -1142, -1142,  4123,  7792,  9530, 12312, -1142,
    4998, -1142, -1142, -1142, -1142,   353, -1142,   566, -1142, -1142,
      26, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142, -1142, -1142, -1142, -1142,   601, -1142, -1142,
   -1142, -1142,  7444, -1142, -1142, -1142, -1142, 20735, -1142, 20735,
     264,   402, -1142, -1142,  2525, -1142,   494, -1142,   497, -1142,
   -1142,   499,   501,   513, 12312,   505,   506, 19998,  3577,   171,
     512,   516, -1142, -1142,   333, -1142, -1142, -1142, -1142, -1142,
      17, -1142, -1142, 19998,   510,  5173, -1142, -1142,   519, 12312,
   -1142, -1142, 12312, 12312, 12312, 20735, -1142, 12312, 11095, 11095,
     630,   420, -1142, -1142, -1142, -1142,   204,   467, -1142, -1142,
     517, 17006, 20735,  2525, -1142,   523, -1142,   -42, 19998,  1393,
   -1142, -1142,  9703,    47, 17492, -1142, -1142,   570,  8836,   603,
   20909, 19998,   239,    42, -1142, 20996, 20735, -1142,   239, 20735,
     520,    23, 16577,    18,  3255,    23, 16617,   238, -1142, 16769,
   20735, 20735,   -59, 15817,   224,  8836, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142,
     526, -1142,   223,  5173,   527,  1759,   107,    16, -1142,  5173,
   -1142, -1142, 17046, -1142,    20, 17555,  1197, -1142,   528,   529,
   -1142, 17046,   204,  1197, -1142,  8836,  2657, -1142, -1142, -1142,
     190, 19998, 12312, 12312, -1142, 19998,   525, 17662, -1142, 17046,
     204, 19998,   530,  8836, -1142, 19998, 17707,   568,   538,   204,
      23, 17046, 17747,   394,   394, 15026,  1197,  1197,   175, -1142,
   -1142,  4298,   -49, -1142, 12312, -1142,   136,   189, -1142,   -29,
      19, 17899,   -46, 15026,   705, -1142,  4473, -1142,   651, 12312,
   12312, 20735, -1142, -1142,   573,   552, -1142, -1142, -1142, -1142,
     277,   471, -1142, 12312,   571, 12312, 12312, 12312, 10747, 10747,
   12312,   480, 12312, 12312, 12312, 12312, 12312,   221, 14862, 12312,
   12312, 12312, 12312, 12312, 12312, 12312, 12312, 12312, 12312, 12312,
   12312, 12312, 12312, 12312, 12312,   654, -1142, -1142, -1142, -1142,
   -1142,  9876,  9876, -1142, -1142, -1142, -1142,  9876, -1142, -1142,
    9876,  9876,  8836,  8836, 10747, 10747,  8315, -1142, -1142, 17086,
   17238, 17939,   553,    76, 20735,  4648, -1142, 10747,    23,   567,
     344, -1142, 12312, -1142, -1142, 12312,   606, -1142,   560,   590,
   -1142, -1142, -1142, 20735, -1142,  2525, -1142, -1142, 20735,   572,
   20735, -1142,  2525,   690, 11095, -1142,  5348, 10747, -1142,   569,
   -1142,    23,  5523, 10747, -1142,    23, -1142, 10747, -1142,  7271,
    7271, -1142,   614,   615,  5173,   707,  5173, -1142,   709, 12312,
   -1142, -1142,   566,   574,  8836, 20735, -1142, -1142,   116, -1142,
   -1142,  1759, -1142,   600,   577, -1142, 12658,   624, 12312,  2525,
   -1142, -1142, 12312, -1142, 20547, 12312, 12312, -1142,   581, -1142,
   11095, -1142, 19998, 19998, -1142,    40, -1142,  8836,   585, -1142,
   12831,   613, -1142, -1142, -1142, -1142, -1142, -1142, -1142, 10051,
   -1142, 17979,  7967, -1142,  8142, -1142,  5173,   586, 10747, 10226,
    3948,   587, 12312, 11268, -1142, -1142,   389, -1142,  4823, 20735,
   -1142,   368, 18131,   382, 16809,   131,   731, 11095,   593, 20460,
     369, -1142, 18171, 20190, 20190,   458, -1142,   458, -1142,   458,
   20288,  1097,  2019,  1860,   204,   394, -1142,   598, -1142, -1142,
   -1142, -1142, -1142, 15026, 16515,   458,  2337,  2337, 20190,  2337,
    2337,  1570,   394, 16515, 20235,  1570,  1197,  1197,   394, 15026,
     609,   611,   620,   623,   626,   627,   605,   621, -1142,   458,
   -1142,   458,    69, -1142, -1142, -1142,   255, -1142,  2171, 20150,
     488, 13004, 10747, 13177, 10747, 12312,  8836, 10747, 15629,   625,
     144, 18216, -1142, -1142, -1142, 19998, 18323,  8836, -1142,  8836,
   20735,   573,   397, 20735, 20735,   573, -1142,   573,   405, 12312,
     258,  9355, 19998,    49, 17278,  8315, -1142,  9355, 19998,    21,
   16854, -1142,    23, 17046, -1142, -1142, -1142, 12312,   515, 12312,
     540,   541, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, 12312, -1142, -1142, 10399, -1142, -1142,   549, -1142,
     410, -1142, -1142, -1142, 18368,   619,   634, 12312, 12312,   760,
    5173,   767, 18408,  5173, 17318,   737, -1142,   272, -1142,   274,
   -1142,   247, -1142, -1142, -1142, -1142, -1142, -1142,   708,   658,
     632, -1142,  2912, -1142,   390,   633,  1759,   107,    45,    51,
   12312, 12312,  7098, -1142, -1142,   602,  9009, -1142, 19998, -1142,
   -1142, -1142, 20735, 18560, 18600, -1142, -1142, 19998,   636,   160,
     635, -1142, -1142,   417, 20735, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142,  5173,   -28, 17470, -1142, -1142, 19998,  5173, 19998,
   -1142, 18646, -1142, -1142, -1142, 12312, -1142,   109,  3520, 12312,
   -1142, 11441,  7271,  7271, -1142,  8836,  1865, -1142,   663,  1362,
     639, 19464,   687,   127, -1142, -1142,   723, -1142, -1142, -1142,
   -1142, 14694,   643, -1142, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142,  8315, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142, -1142, -1142, -1142,    84, 10747, 10747, 12312,
     785, 18752, 12312,   787, 18798,   300,   646, 18838,  8836,    23,
      23, -1142, -1142, -1142, -1142,   573,   652, -1142,   573,   573,
     653,   659, -1142, 17046, -1142, 15893,  5698, -1142,  5873, -1142,
     338, -1142, 15969,  6048, -1142,    23,  6223, -1142,    23, -1142,
   -1142,  7271, -1142, 12312, -1142, 19998, 19998,  5173, -1142,  5173,
   12312, -1142,  5173,   793, 20735,   666, 20735,   517, -1142, -1142,
   20735,   852, -1142,  1759,   688,   742, -1142, -1142, -1142,   113,
   -1142, -1142,   624,   660,   100, -1142, -1142, -1142,   662,   667,
   -1142,  6398, 11095, -1142, -1142, -1142, -1142, -1142,  6573,   668,
    6748,   669, -1142, 12312, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142, -1142,  7271, -1142, 18990,   232, 17510,   418,
     673,   345, 20735, -1142,   696,   517,   671,  2264, -1142, 20735,
   -1142, 12312, 19696, -1142, -1142,   390,   675,   341,   697,   700,
     701,   710,   704,   711,   712,   713,   714,   715,   718,   354,
     717,   722,   724, 12312, -1142,   733,   734,   727,   675, -1142,
     675, -1142, -1142, -1142,   624,   346,   349, 19030, 13350, 13523,
   19070, 13696, 13869, -1142, 14042, 14215,   351, -1142, -1142,   685,
   -1142,   706,   716, -1142, -1142, -1142,  5173,  9355, 19998,  9355,
   19998,  8315, -1142,  5173,  9355, 19998, -1142,  9355, 19998, -1142,
   -1142, 19222, 19998, -1142, -1142, 19998,   823,  5173,   719, -1142,
   -1142, -1142,   688, -1142,   720, 11616,   299, -1142,    15, -1142,
   -1142, 10747, 15488,  8836,  8836,  5173, -1142,    32,  9355, -1142,
   19998,  5173,  9355, -1142, 19998,  5173, 19998,   187, 11789,  7271,
    7271,  7271,  7271, -1142, -1142,   729,   725, 12312, -1142, -1142,
     869, -1142,  2657, -1142, 20383, -1142, -1142, -1142, 19998, -1142,
     115,   116, -1142, 19262, -1142, 15734, -1142, -1142, -1142, 12312,
   12312, 12312, 12312, 12312, 12312, 12312, 12312, -1142, -1142,  2626,
   -1142, -1142,  3083,  3436, 18408, 16045, 16121, -1142, 18408, 16197,
   16273, 12312,  5173, -1142, -1142,   299,   688, 10574, -1142, -1142,
   -1142,   177, 11095, -1142, -1142,   164, 12312,   -20, 19302, -1142,
     937,   726,   730,   570, -1142, 16349, -1142, 16425, -1142, -1142,
   -1142, 19998,   421,   732,   426,   744, -1142,   517, 19998,  1295,
   -1142, -1142, -1142, 14388,   775,   738, -1142,   745,   747,   675,
     675, 19454, 19494, 19534, 19686, 19726, 19766, 18135, -1142, 18571,
   20154, -1142, -1142,  5173,  5173,  5173,  5173, 19998, -1142, -1142,
   -1142,   299, 11964,   101, -1142, 19998, -1142,   176, -1142,   -15,
   -1142,   521, 19918, -1142, -1142, -1142, 14215,   728,   748,  5173,
    5173, -1142, -1142, -1142, -1142, -1142, -1142,  6923, -1142, -1142,
     233, -1142,    15, -1142, -1142, -1142, 12312, 12312, 12312, 12312,
   12312, 12312, -1142, -1142, -1142, 18408, 18408, 18408, 18408, -1142,
   -1142, -1142, -1142, -1142,   312, 10747, 15034, -1142, 12312,   164,
     176,   176,   176,   176,   176,   176,   164,  1019, -1142, -1142,
   18408, 18408,   740, 14561,   103,   169, 19958, -1142, -1142, 19998,
   -1142, -1142, -1142, -1142, -1142, -1142, -1142,   735, -1142, -1142,
     361, 15206, -1142, -1142, -1142, 12139, -1142,   383, -1142
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1142, -1142, -1142,     8,    78,  2505,   746, -1142,   -11, -1142,
   -1142, -1142,   414, -1142, -1142, -1142,   463,   533,  -472, -1142,
    -757, -1142, -1142, -1142,    97, -1142, -1142, -1142,   878, -1142,
    2798,  -186,  -783, -1142,  -990,  2895, -1093,   407, -1142, -1142,
    -161,  -909, -1142,   -64, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142, -1142,    92, -1142,   834, -1142, -1142,    31,
    2434, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142,   -27,
   -1142, -1142, -1142, -1142, -1142, -1142,  -567,  -751, -1142, -1142,
   -1142,   -26,  -770,   741, -1142, -1142, -1142,   352, -1142, -1142,
   -1142, -1142,  -160,  -781,  -151,  -725,  -956, -1142, -1142,  -154,
      38,   212, -1142, -1142, -1142,   -24, -1142, -1142,  -322,    29,
   -1007,  -277,  -309,  -299,  -639, -1142,  -203, -1142,     7,   916,
    -136,   447, -1142,  -488,  -885,  -982, -1142,  -691,  -527, -1141,
   -1121,  -968,   -57, -1142,   114, -1142,  -253,  -473,  -428,   812,
    -495, -1142, -1142, -1142,  1210, -1142,   -13, -1142, -1142,  -229,
   -1142,  -627, -1142, -1142, -1142,  1281,  1352,   -12,   927,   168,
    1894, -1142,  1965,  2036, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142,  -421
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -716
static const yytype_int16 yytable[] =
{
     183,   307,   493,   762,   206,   630,   210,   700,   211,   439,
     109,   745,   310,   735,   218,   475,   226,   421,   370,   231,
     231,   717,   242,   244,   246,   249,   253,  1049,   986,   213,
     989,   960,  1096,   459,  1097,   763,   950,   292,   871,   295,
     296,   758,   446,   484,  1204,   301,   912,   302,  1086,   303,
     350,   289,   518,   289,   873,  1128,   289,   311,   315,   317,
     319,   320,   321,   945,   508,   289,   325,   548,   326,   446,
     528,  1243,    70,   359,  1125,   329,  1135,   440,   331,   332,
     110,   333,   866,   334,   856,   505,   335,   336,   860,   337,
     861,   469,   469,   311,   315,   351,   353,   539,   309,   760,
     553,  -271,   489,  1248,   442,  1267,   424,  -272,   505,  1098,
     425,   435,   586,   -55,   505,  -294,  1162,   821,  1189,   446,
     547,   551,   540,   443,   637,  1268,   241,   243,   245,   554,
     456,  1249,   -55,  1136,   309,   345,   309,   446,   469,   469,
     360,   469,   433,   721,  1288,   994,   546,   928,  1051,  1245,
     435,   637,   374,     3,   457,  -484,  1030,  -294,  1191,   822,
     307,  1134,   509,   845,   722,   549,   512,  1196,   723,   616,
     617,  1160,  1204,  1218,  -484,  1086,  -479,   183,   520,  1196,
     429,   430,   431,   110,  1239,   325,   311,   351,   435,  1087,
     435,   435,   724,   435,   550,   725,   993,    62,   438,   349,
    1086,   592,   435,  1086,  1086,  -484,   726,  -479,   444,  -484,
     231,  -294,   590,  1224,  1225,   181,   315,   228,  1048,   435,
    -479,   854,   549,   255,   586,   727,   297,   309,   309,   506,
    -484,  -484,  -484,  -479,  -254,   921,   446,   446,   316,   455,
    1190,   587,   519,   315,   630,   462,   549,   549,   911,   549,
    1216,   638,   506,  -484,   213,   588,  1197,  -484,   506,  1109,
    1163,    70,  1112,  1113,   184,    23,    23,  1198,  1086,  1069,
    1086,  1086,  -484,   962,   348,  1052,  1246,   922,  1281,  1198,
    -479,  1279,   542,  -484,   589,  -479,   298,   187,  1009,  -384,
      42,  1011,  1012,   315,  -480,   898,   794,  -255,   963,  1286,
     522,   523,  -502,   795,   299,  1070,  1240,   490,   446,   543,
     491,   315,   986,  1153,   989,  1155,    58,    60,    60,   652,
     949,   300,   951,  -502,   590,  -480,   658,  -502,    68,    68,
     591,   923,   541,   881,  1282,   544,   520,   899,  -480,   435,
     181,   446,  -261,   422,   381,   423,   536,   562,   564,   538,
    -502,  -480,   387,    88,    88,   521,   214,  1177,  1149,  1179,
    1180,   572,   545,   573,   574,   575,   577,   579,   580,    23,
     581,   582,   583,   584,   585,  1006,   593,   594,   595,   596,
     597,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   435,  1117,   495,   806,   -84,  -480,   315,
     315,   823,   630,  -480,   520,   315,   228,   660,   315,   315,
     315,   315,   619,   621,   629,  1044,   -55,   905,   649,   110,
     896,    60,  -472,   509,   746,   641,   377,  -260,   824,  -575,
     645,   864,    68,   646,   110,   -55,  -638,  -494,  -638,   255,
     875,   878,   571,  -269,  -474,   895,   649,   897,   807,  1132,
     446,   212,   311,  -472,   662,   664,   366,    88,  -494,   808,
     668,   670,  -494,   446,   -84,   673,  -472,   704,   704,   906,
     576,   578,  1067,  1003,   712,  -474,   367,   714,   809,  -472,
     907,  -114,   315,   378,  1021,  -494,   368,   379,  -474,  -120,
     509,   649,  -589,   309,  -116,  -588,   748,   649,  -259,   908,
     295,  -474,  -121,   753,   754,  -639,  -256,  -639,   757,   644,
     798,  1022,   290,   110,   785,   315,   618,   620,  1075,  -589,
    -263,   752,  -588,   356,  1107,  1139,  -472,   757,   789,   640,
     315,  -472,   315,   786,   381,   290,   774,   757,   777,   385,
     779,   781,   387,   509,   929,   931,   390,   790,  -474,   756,
    1131,   509,  -267,  -474,   181,   311,   881,  1140,  1247,   663,
     782,   926,   857,  -450,   881,   669,   538,   881,   756,   672,
     862,   345,   881,   345,  -264,   882,   728,   838,   756,   611,
     612,   425,  -450,  1073,   454,   613,  1211,   304,   614,   615,
     461,  1213,  -638,  -258,  -638,   305,   309,  1152,   381,  1154,
     630,   935,   592,   385,  -713,   363,   387,   322,  -713,   446,
     390,  1271,  1272,  1273,  1274,  1275,  1276,  -262,   323,   662,
     841,   668,   844,   712,   315,   847,   777,   849,   850,  -714,
    -715,  -268,  -270,  -714,  -715,   315,   110,   315,  -710,  -635,
     773,  -635,  -710,  -637,  -257,  -637,  1250,   863,  -265,   865,
    -485,   357,  -636,   629,  -636,   872,   768,   361,   770,  -572,
     990,   371,   446,  -485,  -571,   333,   372,   334,  1220,   373,
    -576,  -577,  1251,   436,  1252,  1253,   852,  -574,  1254,  1255,
     336,  -573,   426,   337,   428,   914,   434,   435,   441,   445,
     524,   452,   464,   530,  -485,   885,   886,  1244,   494,   498,
     515,   516,   527,   532,   290,   290,   290,   290,   290,   290,
     348,  1257,   348,   555,   489,   485,   488,   559,   567,   387,
    -485,   489,   569,   610,   840,   636,   843,  -485,   321,   325,
     351,   647,   643,   648,   315,   721,   649,   656,   659,   707,
     708,   665,   710,   150,   713,   496,   736,   716,  -485,   746,
     737,  1244,   796,   150,   755,   290,   722,   290,   761,   764,
     723,   772,   778,   799,   290,   881,   150,  -485,  1280,   150,
     812,   309,  -485,   562,   813,  -485,   814,   946,   819,   948,
     704,   704,   290,   315,   724,   815,  1287,   725,   816,   354,
    1244,   817,   818,   290,   290,   887,   820,   848,   726,   884,
    1141,  1142,   889,   894,   901,   902,   910,   920,   925,   520,
     958,   629,   961,   964,   156,   853,   992,   727,   998,   150,
    1001,  1004,  1010,  1013,   156,   577,   619,   997,  1037,  1014,
    1000,  1039,  1047,  1045,  1053,  1050,   315,   156,  1074,  1054,
     156,  1077,  1079,  1061,  1065,  -141,   150,  1091,  -148,  -147,
    -117,   150,  -146,  -115,  1018,  1108,  1020,  -118,  1122,  -143,
    -150,  1025,  -151,  -145,  1028,  1057,  -149,   446,   446,  1031,
    -144,  1032,  -152,  1094,  -119,  1095,  1110,  1159,  1035,   721,
    1221,  1249,   934,   150,  1124,  -114,  1111,  -116,   362,   558,
     156,   220,  1258,  1126,  1156,    22,    23,  1212,  1157,  1207,
     722,   290,   215,  1208,   723,   221,   715,    31,   222,  1214,
     311,  1223,  1259,    37,   916,  1278,  1060,   156,  1064,   750,
      42,  1066,   156,  1161,   330,  1076,   150,  1040,   724,   576,
     618,   725,   704,    62,   290,  1169,  1089,  1165,   290,  1043,
     900,  1090,   726,  1284,  1170,  1265,    58,  1277,    60,  1088,
    1270,   309,  1081,   248,   156,  1082,   749,   225,   232,    68,
       0,   727,     0,     0,   489,   489,     0,     0,   489,   489,
       0,  1093,     0,     0,     0,     0,     0,     0,    84,   728,
       0,    86,     0,     0,    88,  -489,  1018,  1020,     0,  1025,
    1028,     0,  1060,  1064,   489,     0,   489,   156,  -489,     0,
       0,     0,     0,     0,  1114,  1115,     0,  1116,     0,   629,
       0,  1118,  1119,     0,     0,  1120,     0,     0,   307,     0,
       0,     0,     0,     0,   150,     0,     0,     0,     0,  -489,
     150,     0,     0,     0,   103,     0,     0,     0,     0,  1138,
     777,   315,   315,     0,     0,     0,  1145,     0,     0,     0,
    1147,     0,     0,     0,     0,  -489,  1151,   704,   704,   704,
     704,     0,  -489,     0,     0,  1158,     0,  -491,     0,     0,
    1193,     0,     0,     0,  1129,     0,     0,     0,     0,     0,
    -491,     0,   150,  -489,     0,     0,     0,  1114,  1171,  1172,
    1118,  1173,  1174,  1175,  1176,   156,     0,   150,     0,     0,
       0,   156,  -489,     0,     0,     0,     0,  -489,     0,  1187,
    -489,  -491,     0,     0,     0,   351,     0,     0,     0,     0,
    1195,     0,     0,     0,  1202,     0,   290,   290,     0,   377,
       0,     0,   290,   290,     0,   290,   290,  -491,     0,     0,
       0,     0,     0,     0,  -491,     0,     0,     0,     0,     0,
       0,     0,     0,   156,     0,     0,   309,     0,     0,     0,
    1264,  1194,     0,     0,     0,  -491,     0,     0,   156,   728,
       0,  1235,  1236,  1237,  1238,     0,   150,   893,     0,     0,
       0,     0,     0,     0,  -491,     0,   378,     0,     0,  -491,
     379,     0,  -491,     0,  1064,     0,     0,  1260,  1261,     0,
       0,     0,     0,     0,     0,   351,     0,   150,     0,     0,
       0,     0,   161,   150,  1235,  1236,  1237,  1238,  1260,  1261,
       0,  1241,   161,     0,     0,   150,     0,   150,     0,   377,
       0,     0,     0,  1266,   777,   161,  1269,   381,   161,     0,
       0,     0,   385,     0,   386,   387,   309,   156,     0,   390,
       0,     0,     0,     0,     0,     0,     0,   397,     0,   953,
       0,     0,     0,   401,   402,   403,     0,     0,     0,   777,
       0,     0,     0,  1064,     0,  1241,     0,     0,   156,     0,
       0,     0,     0,   165,   156,     0,   378,   150,   161,     0,
     379,   150,     0,   165,     0,     0,   156,     0,   156,   150,
       0,     0,     0,     0,     0,     0,   165,     0,     0,   165,
       0,     0,   290,   290,  1241,   161,     0,   220,     0,     0,
     161,    22,    23,     0,     0,     0,   290,     0,   488,     0,
       0,   221,     0,    31,   222,   488,     0,   381,   290,    37,
       0,   290,   385,     0,   386,   387,    42,     0,     0,   390,
       0,     0,   161,     0,   166,     0,     0,   397,   156,   165,
    1041,     0,   156,   956,   166,   403,     0,     0,     0,     0,
     156,     0,    58,     0,    60,     0,     0,   166,  1081,     0,
     166,  1082,     0,   225,  -306,    68,   165,     0,  -306,  -306,
       0,   165,     0,  -306,     0,   161,     0,     0,  -306,     0,
    -306,  -306,     0,     0,    84,     0,  -306,    86,     0,     0,
      88,     0,     0,  -306,     0,   220,  -306,     0,  1078,    22,
      23,     0,     0,   165,     0,     0,     0,     0,     0,   221,
     166,    31,   222,     0,     0,     0,  -306,    37,     0,  -306,
     836,  -306,     0,  -306,    42,  -306,  -306,     0,  -306,     0,
    -306,   150,  -306,     0,   150,     0,     0,   166,     0,   -53,
     103,     0,   166,     0,     0,     0,   165,     0,     0,     0,
      58,  -306,    60,     0,  -306,     0,   223,  -306,   -53,   224,
       0,   225,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   161,   166,     0,     0,     0,     0,   161,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,   150,     0,     0,     0,   219,     0,   150,
     290,     0,   156,     0,     0,   156,     0,  -306,     0,     0,
       0,   220,     0,  -306,     0,    22,    23,   166,     0,     0,
       0,     0,     0,     0,     0,   221,     0,    31,   222,     0,
       0,   161,     0,    37,     0,     0,     0,     0,   103,     0,
      42,     0,     0,     0,   165,     0,   161,     0,     0,     0,
     165,     0,     0,     0,     0,   -53,     0,     0,   488,   488,
       0,     0,   488,   488,   156,     0,    58,     0,    60,     0,
     156,     0,   223,     0,   -53,   224,     0,   225,     0,    68,
    1203,     0,   377,     0,     0,     0,     0,   150,   488,   150,
     488,     0,     0,     0,   150,     0,     0,   150,    84,     0,
    1215,    86,   165,   991,    88,     0,     0,     0,   150,     0,
     150,     0,     0,   150,     0,   166,     0,   165,     0,     0,
       0,   166,     0,     0,     0,   161,     0,     0,     0,   995,
     996,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,   150,   379,     0,     0,     0,     0,     0,   150,
       0,   150,     0,     0,   103,     0,   161,     0,   156,     0,
     156,     0,   161,     0,     0,   156,     0,     0,   156,     0,
       0,     0,     0,   166,   161,     0,   161,     0,     0,   156,
       0,   156,     0,     0,   156,     0,     0,     0,   166,     0,
     381,   382,     0,   383,   384,   385,   165,   386,   387,     0,
       0,     0,   390,     0,     0,     0,     0,     0,  1203,     0,
     397,     0,     0,   156,     0,     0,   401,   402,   403,     0,
     156,     0,   156,     0,     0,     0,     0,   165,     0,     0,
       0,     0,     0,   165,     0,     0,   161,   150,     0,     0,
     161,     0,   256,     0,   150,   165,     0,   165,   161,     0,
       0,     0,     0,     0,     0,     0,   257,   258,   150,   259,
       0,     0,     0,     0,   260,     0,     0,   166,     0,     0,
       0,     0,     0,   261,     0,     0,   150,     0,     0,   262,
       0,     0,   150,     0,     0,   263,   150,     0,     0,   264,
       0,     0,   265,     0,     0,     0,     0,     0,   166,     0,
       0,     0,   266,     0,   166,     0,     0,   165,   156,   267,
     268,   165,     0,     0,     0,   156,   166,   269,   166,   165,
       0,     0,     0,     0,     0,     0,   270,     0,     0,   156,
       0,     0,     0,     0,     0,   271,   272,     0,   273,     0,
     274,     0,   275,   150,     0,   276,     0,   156,   256,   277,
     500,     0,   278,   156,     0,   279,     0,   156,     0,     0,
       0,     0,   257,   258,     0,   259,     0,     0,     0,     0,
     260,     0,   377,     0,     0,     0,   168,     0,   166,   261,
       0,     0,   166,     0,     0,   262,   168,     0,     0,     0,
     166,   263,     0,     0,     0,   264,     0,     0,   265,   168,
     161,     0,   168,   161,   150,   150,   150,   150,   266,     0,
       0,   501,     0,     0,   156,   267,   268,     0,     0,     0,
       0,     0,     0,   269,     0,     0,     0,     0,     0,   378,
     150,   150,   270,   379,     0,     0,     0,     0,     0,     0,
       0,   271,   272,     0,   273,     0,   274,   170,   275,     0,
       0,   276,   168,     0,     0,   277,     0,   170,   278,     0,
       0,   279,   161,     0,     0,     0,     0,     0,   161,     0,
     170,   165,     0,   170,   165,   156,   156,   156,   156,   168,
     381,   382,     0,     0,   168,   385,     0,   386,   387,     0,
       0,     0,   390,     0,     0,     0,     0,     0,     0,     0,
     397,   156,   156,     0,     0,     0,   401,   402,   403,     0,
       0,     0,     0,     0,     0,   435,   168,   952,   171,     0,
       0,     0,     0,   170,     0,     0,     0,     0,   171,     0,
       0,   377,     0,   165,     0,     0,     0,     0,     0,   165,
       0,   171,   166,     0,   171,   166,     0,     0,     0,     0,
     170,     0,     0,     0,     0,   170,   161,     0,   161,   168,
       0,     0,     0,   161,     0,     0,   161,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   161,     0,   161,
       0,     0,   161,     0,     0,     0,     0,   170,   378,     0,
       0,     0,   379,     0,   171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   166,     0,     0,     0,     0,     0,
     166,   161,     0,     0,     0,     0,     0,     0,   161,     0,
     161,   171,     0,     0,     0,     0,   171,   165,     0,   165,
     170,     0,     0,     0,   165,     0,     0,   165,     0,   381,
     382,     0,     0,   384,   385,     0,   386,   387,   165,     0,
     165,   390,     0,   165,   586,     0,     0,   168,   171,   397,
       0,     0,     0,   168,     0,   401,   402,   403,   825,     0,
       0,   826,     0,     0,     0,     0,   827,     0,     0,     0,
       0,     0,   165,     0,     0,     0,     0,   191,     0,   165,
       0,   165,     0,     0,     0,     0,     0,   828,   166,     0,
     166,   171,     0,     0,   829,   166,   161,     0,   166,     0,
       0,     0,     0,   161,   830,   168,     0,     0,     0,   166,
       0,   166,   831,     0,   166,     0,     0,   161,   170,     0,
     168,     0,     0,     0,   170,     0,     0,     0,   832,     0,
       0,     0,     0,     0,     0,   161,     0,     0,     0,     0,
     833,   161,     0,   166,   590,   161,     0,     0,     0,     0,
     166,   834,   166,     0,     0,     0,   220,   835,     0,     0,
      22,    23,     0,     0,     0,  1080,     0,   165,     0,     0,
     221,     0,    31,   222,   165,     0,   170,     0,    37,     0,
       0,     0,     0,     0,     0,    42,     0,     0,   165,   171,
       0,   170,     0,     0,     0,   171,     0,     0,     0,   168,
       0,     0,   161,     0,     0,     0,   165,     0,     0,     0,
       0,    58,   165,    60,     0,    62,   165,  1081,     0,     0,
    1082,     0,   225,     0,    68,     0,     0,     0,     0,     0,
     168,     0,     0,     0,     0,     0,   168,     0,   166,   377,
       0,     0,     0,    84,     0,   166,    86,   171,   168,    88,
     168,     0,     0,     0,     0,     0,     0,     0,     0,   166,
       0,     0,   171,   161,   161,   161,   161,     0,     0,     0,
     170,     0,     0,   165,     0,     0,     0,   166,     0,     0,
       0,     0,     0,   166,     0,     0,     0,   166,     0,   161,
     161,     0,     0,     0,     0,     0,   378,     0,     0,   103,
     379,   170,     0,     0,     0,  1083,   137,   170,     0,     0,
     168,     0,     0,     0,   168,     0,   137,     0,     0,   170,
       0,   170,   168,     0,     0,     0,     0,     0,     0,   137,
       0,     0,   137,     0,   165,   165,   165,   165,     0,     0,
       0,   171,     0,     0,   166,     0,     0,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,     0,   390,
     165,   165,     0,     0,     0,     0,   396,   397,     0,     0,
     400,     0,   171,   401,   402,   403,     0,   111,   171,     0,
       0,   170,   137,     0,   404,   170,     0,   182,     0,     0,
     171,     0,   171,   170,     0,     0,     0,     0,   256,     0,
     216,     0,     0,   227,     0,   166,   166,   166,   166,   137,
       0,     0,   257,   258,   137,   259,     0,     0,     0,     0,
     260,     0,     0,     0,     0,     0,     0,     0,     0,   261,
       0,   166,   166,     0,     0,   262,     0,     0,     0,     0,
       0,   263,     0,     0,     0,   264,     0,     0,   265,     0,
       0,     0,   171,   324,     0,     0,   171,     0,   266,     0,
       0,     0,     0,     0,   171,   267,   268,     0,     0,     0,
       0,     0,     0,   269,   168,     0,     0,   168,     0,     0,
     111,     0,   270,     0,     0,   355,     0,     0,     0,   137,
       0,   271,   272,     0,   273,     0,   274,     0,   275,     0,
       0,   276,     0,     0,     0,   277,     0,     0,   278,     0,
       0,   279,     0,     0,     0,     0,     0,     0,   220,     0,
       0,     0,    22,    23,     0,     0,     0,  1080,     0,     0,
       0,     0,   221,     0,    31,   222,   168,     0,     0,     0,
      37,     0,   168,     0,     0,   170,     0,    42,   170,   220,
       0,     0,     0,    22,    23,     0,     0,     0,     0,     0,
     427,     0,     0,   221,     0,    31,   222,   285,     0,     0,
       0,    37,     0,    58,     0,    60,     0,    62,    42,  1081,
       0,     0,  1082,     0,   225,     0,    68,   137,     0,     0,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    84,    60,   170,    86,     0,
       0,    88,     0,   170,     0,   225,   171,    68,     0,   171,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     168,     0,   168,     0,     0,     0,    84,   168,     0,    86,
     168,     0,    88,     0,     0,   137,     0,     0,     0,     0,
       0,   168,     0,   168,     0,     0,   168,     0,   497,     0,
     137,   103,     0,     0,   510,     0,     0,  1178,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   171,     0,
       0,     0,     0,     0,   171,   168,     0,     0,     0,     0,
       0,     0,   168,     0,   168,     0,     0,     0,     0,     0,
       0,   170,     0,   170,     0,     0,     0,     0,   170,     0,
       0,   170,   280,   282,   283,     0,   111,     0,     0,     0,
       0,     0,   170,     0,   170,     0,     0,   170,     0,     0,
       0,   111,     0,     0,     0,     0,     0,     0,     0,   137,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   327,     0,     0,     0,   170,     0,     0,     0,
       0,     0,     0,   170,     0,   170,     0,     0,     0,     0,
     137,     0,   171,     0,   171,     0,   137,     0,     0,   171,
     168,     0,   171,     0,     0,   256,     0,   168,   137,     0,
     137,     0,     0,   171,     0,   171,     0,     0,   171,   257,
     258,   168,   259,     0,     0,     0,     0,   260,     0,     0,
     111,     0,     0,     0,     0,   364,   261,   365,     0,   168,
       0,     0,   262,     0,     0,   168,     0,   171,   263,   168,
       0,     0,   264,     0,   171,   265,   171,   318,     0,     0,
       0,   510,     0,     0,     0,   266,     0,   510,     0,     0,
     137,   170,   267,   268,   137,     0,     0,     0,   170,   709,
     269,     0,   137,   432,     0,     0,     0,     0,     0,   270,
       0,     0,   170,     0,     0,     0,     0,     0,   271,   272,
     327,   273,     0,   274,     0,   275,   168,     0,   276,     0,
     170,     0,   277,     0,     0,   278,   170,     0,   279,     0,
     170,     0,     0,     0,   327,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,   483,
       0,   771,   171,     0,     0,   776,     0,     0,     0,   171,
       0,     0,     0,   111,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,     0,     0,     0,   168,   168,   168,
     168,     0,     0,   502,   903,     0,     0,   170,     0,     0,
       0,   171,     0,     0,     0,     0,     0,   171,     0,     0,
       0,   171,     0,   168,   168,   220,     0,     0,     0,    22,
      23,     0,     0,     0,  1080,     0,     0,     0,     0,   221,
       0,    31,   222,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,    42,     0,   465,   468,   470,   474,
     476,   478,     0,     0,   137,     0,     0,   137,   170,   170,
     170,   170,     0,     0,     0,     0,     0,     0,   171,   565,
      58,     0,    60,     0,    62,     0,  1081,     0,     0,  1082,
       0,   225,     0,    68,   170,   170,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   511,     0,   513,
       0,     0,    84,     0,     0,    86,   517,     0,    88,     0,
       0,     0,     0,     0,     0,     0,   137,     0,     0,     0,
       0,     0,   137,     0,   526,   888,     0,     0,   891,   171,
     171,   171,   171,     0,     0,   533,   534,     0,     0,     0,
       0,     0,   639,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   171,     0,   103,     0,
       0,   651,     0,     0,  1181,     0,   655,     0,   657,     0,
       0,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,   376,     0,     0,   927,     0,     0,
       0,     0,     0,   932,     0,     0,     0,   377,   472,     0,
       0,     0,     0,   718,     0,     0,     0,     0,     0,   502,
     137,     0,   137,   473,     0,     0,     0,   137,     0,     0,
     137,     0,   751,     0,     0,     0,     0,     0,     0,     0,
       0,   137,     0,   137,     0,     0,   137,     0,     0,     0,
       0,     0,     0,   642,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,     0,   379,     0,
       0,     0,     0,     0,     0,   137,     0,   784,     0,     0,
       0,     0,   137,     0,   137,     0,   666,   802,     0,     0,
     671,   510,     0,   510,     0,   469,     0,     0,   510,   380,
       0,   510,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1033,     0,  1034,   381,   382,  1036,   383,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,   392,
     393,     0,   394,   395,   396,   397,   398,   399,   400,     0,
       0,   401,   402,   403,     0,   435,  1056,     0,     0,     0,
       0,     0,   404,  1059,     0,  1063,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   855,     0,
     137,   858,   859,     0,     0,     0,     0,   137,   220,     0,
       0,     0,    22,    23,     0,     0,     0,  1080,     0,     0,
       0,   137,   221,     0,    31,   222,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,    42,     0,   137,
       0,     0,     0,     0,     0,   137,     0,     0,     0,   137,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,  1081,
       0,     0,  1082,   586,   225,     0,    68,     0,     0,     0,
     904,     0,   674,     0,   502,     0,     0,     0,     0,   675,
     587,     0,  1123,     0,     0,    84,     0,     0,    86,     0,
     917,    88,     0,     0,   588,     0,   137,     0,   867,   869,
    1143,     0,   327,     0,   874,   877,  1146,   879,   880,     0,
    1148,     0,     0,     0,     0,   676,     0,     0,     0,     0,
       0,     0,     0,   589,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   954,     0,     0,     0,     0,   802,
       0,   103,     0,     0,     0,     0,     0,  1182,     0,   985,
       0,     0,     0,     0,     0,     0,     0,   137,   137,   137,
     137,     0,     0,   590,     0,     0,     0,  1188,     0,   591,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   137,   137,   487,   406,   407,   408,   409,
     410,     0,     0,   413,   414,   415,   416,     0,   418,   419,
     936,   937,   938,   939,   940,   682,     0,   683,     0,     0,
       0,   684,   685,   686,   687,   688,   689,   690,   691,   941,
     693,   694,     0,   942,     0,     0,   696,   697,   943,   699,
     944,     0,  1038,     0,   718,     0,     0,     0,  1042,     0,
       0,   502,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -596,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1007,  1008,   420,     0,     0,  -635,
     954,  -635,     0,     0,     0,     0,     0,   802,  1015,     0,
     802,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1026,    -2,     4,  1029,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -82,     0,
      40,    41,    42,     0,    43,  -323,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -53,    53,    54,
       0,    55,    56,    57,     0,  -323,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -323,   -53,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -82,    88,    89,     0,     0,
      90,     0,    91,   985,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     0,   108,     4,
       0,     5,  1144,     6,     7,     8,     9,    10,    11,     0,
    -665,     0,    12,    13,    14,    15,    16,  -665,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    -665,    28,    29,  -665,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -53,    65,    66,    67,  -665,    68,    69,
      70,  -665,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,  -665,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -665,  -665,
      95,  -665,  -665,  -665,  -665,  -665,  -665,  -665,     0,  -665,
    -665,  -665,  -665,  -665,     0,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,   103,  -665,  -665,  -665,     0,   105,  -665,
     106,     0,   107,     0,   338,  -665,     5,   308,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,   339,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,    62,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,   537,
     106,     0,   107,     0,   556,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,   557,   106,     0,   107,     0,   338,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,    62,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,   339,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,   783,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,   354,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,   661,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,   667,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1017,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1019,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1024,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1027,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,  1055,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1058,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1062,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,  1262,   108,     5,   308,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   189,     0,     0,
      15,    16,     0,    17,     0,   190,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   191,
       0,     0,     0,    33,   192,   193,     0,     0,   194,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   195,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   196,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   197,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   198,     0,   106,     0,   199,  1263,   913,
     108,     5,   308,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   189,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   199,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,   674,     0,   189,     0,     0,    15,    16,
     675,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,   676,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,   487,   406,   407,   408,
     409,   410,     0,     0,   413,   414,   415,   416,     0,   418,
     419,   677,   678,   679,   680,   681,   682,     0,   683,     0,
      98,     0,   684,   685,   686,   687,   688,   689,   690,   691,
     692,   693,   694,   100,   695,   102,     0,   696,   697,   698,
     699,   198,     0,   106,     0,   199,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   189,    13,
       0,    15,    16,     0,    17,     0,   190,    19,    20,    21,
       0,     0,     0,     0,    26,     0,     0,    28,    29,     0,
     191,     0,     0,     0,    33,    34,    35,    36,     0,    38,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,    49,    50,    51,    52,
       0,    53,    54,     0,    55,    56,    57,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,    90,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   105,     0,   106,     0,   107,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   189,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,   247,    23,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,    60,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,    68,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,    88,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
       0,   199,     0,     0,   108,     5,   308,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   189,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,   312,
     313,     0,    85,   343,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,   344,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     0,     0,   108,
       5,   308,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   189,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,   312,   313,     0,    85,   343,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
     767,   199,     0,     0,   108,     5,   308,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   189,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,   312,
     313,     0,    85,   343,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,   769,   199,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   189,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,   622,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,   623,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,   624,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
     625,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     5,   108,     6,     7,     8,   233,    10,    11,   234,
       0,     0,   189,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   235,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,   236,    85,     0,
     237,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   199,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   189,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,   312,   313,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   314,     0,     0,   104,
       0,   198,     0,   106,     0,   199,     0,     0,   108,     5,
     308,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     189,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,   312,   313,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   189,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,   915,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,   312,   313,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   189,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,   239,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,   240,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   189,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,   250,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
     251,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     0,   108,     5,   308,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   189,     0,     0,    15,    16,     0,
      17,     0,   190,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   191,     0,     0,     0,
      33,   192,   193,     0,     0,   194,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   197,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,   240,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     198,     0,   106,     0,   199,     0,     5,   108,     6,     7,
       8,   233,    10,    11,     0,     0,     0,   189,     0,     0,
      15,    16,     0,    17,     0,   190,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   191,
       0,     0,     0,    33,   192,   193,     0,     0,   194,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   195,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   235,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   197,     0,    79,     0,     0,    81,
       0,     0,   236,    85,     0,   237,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   198,     0,   106,     0,   199,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     189,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,   312,   313,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,     0,     0,   108,     5,   308,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   189,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   198,   765,   106,     0,   199,     0,     0,   108,     5,
     308,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     189,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,   775,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   189,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,  -709,     0,
       0,     0,  -709,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     0,   108,     5,   308,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   189,     0,
       0,    15,    16,     0,    17,     0,   190,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     191,     0,     0,     0,    33,   192,   193,     0,     0,   194,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   197,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   198,     0,   106,     0,   199,  1192,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   189,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,   240,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
       0,   199,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   189,     0,     0,    15,    16,     0,
      17,     0,   190,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   289,    29,     0,   191,     0,     0,     0,
      33,   192,   193,     0,     0,   194,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   197,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     105,     0,   106,     0,   199,     0,     0,   108,     5,   308,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   189,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   189,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   199,   780,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   189,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,   947,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   198,     0,   106,     0,   199,     0,     0,   108,     5,
     308,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     189,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
     738,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,   739,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,   740,    75,    76,    77,   741,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
    1127,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   189,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
    1150,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     0,   108,     5,   308,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   189,     0,
       0,    15,    16,     0,    17,     0,   190,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     191,     0,     0,     0,    33,   192,   193,     0,     0,   194,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   197,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   198,     0,   106,     0,  1127,     0,
       0,   108,     5,   308,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   189,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,  1106,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   189,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     0,     5,   108,
       6,     7,     8,   229,    10,    11,     0,     0,     0,   189,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   230,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   189,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,   738,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,   739,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,   740,    75,    76,    77,   741,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   742,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   189,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   198,     0,   106,     0,   742,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   189,     0,
       0,    15,    16,     0,    17,     0,   190,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     191,     0,     0,     0,    33,   192,   193,     0,     0,   839,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   197,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   198,     0,   106,     0,   199,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   189,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   842,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
       0,   199,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   189,     0,     0,    15,    16,     0,
      17,     0,   190,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   191,     0,     0,     0,
      33,   192,   193,     0,     0,  1100,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   197,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     198,     0,   106,     0,   199,     0,     5,   108,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   189,     0,     0,
      15,    16,     0,    17,     0,   190,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   191,
       0,     0,     0,    33,   192,   193,     0,     0,  1101,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   195,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   196,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   197,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   198,     0,   106,     0,   199,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     189,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,  1103,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   189,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,  1104,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   189,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,  1105,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   189,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
    1106,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   189,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,   738,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,   739,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,   740,    75,    76,    77,   741,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,  1217,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   189,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   966,     0,   967,     0,     0,
       0,    93,    94,    95,    96,    97,   674,     0,     0,     0,
      98,   968,   258,   675,   969,     0,     0,     0,     0,   970,
      99,     0,     0,   100,   101,   102,     0,     0,   261,   104,
     191,     0,     0,   106,   971,  1217,     0,     0,   108,     0,
     972,     0,     0,     0,   264,     0,     0,   973,     0,   676,
       0,     0,     0,     0,     0,     0,     0,   974,     0,     0,
       0,     0,     0,     0,   975,   976,     0,     0,     0,     0,
       0,     0,   269,     0,     0,     0,     0,     0,     0,     0,
       0,   977,     0,     0,     0,     0,     0,     0,     0,     0,
     271,   272,     0,   978,     0,   274,     0,   979,     0,     0,
     980,     0,     0,     0,   981,     0,     0,   278,     0,     0,
     982,     0,     0,     0,     0,     0,     0,     0,     0,   487,
     406,   407,   408,   409,   410,     0,     0,   413,   414,   415,
     416,     0,   418,   419,   936,   937,   938,   939,   940,   682,
       0,   683,     0,     0,     0,   684,   685,   686,   687,   688,
     689,   690,   691,   941,   693,   694,     0,   942,     0,     0,
     696,   697,   943,   699,     0,     5,   983,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   189,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,     0,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     5,   308,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   189,     0,
       0,    15,    16,     0,    17,     0,   190,     0,   377,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     191,     0,     0,     0,    33,   192,   193,     0,     0,   194,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,   378,    63,     0,     0,   379,
       0,    66,   196,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   197,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,  -493,
       0,     0,     0,     0,     0,     0,   381,   382,     0,   383,
     384,   385,     0,   386,   387,     0,    95,     0,   390,     0,
    -493,     0,     0,     0,  -493,   396,   397,     0,     0,   400,
       0,     0,   401,   402,   403,     0,     0,     0,   102,     0,
       0,     0,     0,     0,   198,     0,   106,  -493,  1127,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     189,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,  -475,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,  -475,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,  -475,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,  -475,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   189,     0,     0,    15,    16,     0,    17,     0,   190,
     102,  -475,    21,     0,     0,     0,  -475,     0,   106,     0,
    1217,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,    95,
       0,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   189,     0,     0,    15,    16,     0,    17,     0,
     190,   102,     0,    21,     0,     0,     0,   198,     0,   106,
       0,   199,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   189,     0,     0,    15,    16,     0,    17,
       0,   190,   102,     0,    21,     0,     0,     0,   198,     0,
     106,     0,   742,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,  1167,     0,    85,
       0,     0,    87,     0,     0,    89,   674,     0,     0,     0,
       0,   257,   258,   675,   259,     0,     0,     0,     0,   260,
       0,     0,     0,     0,     0,     0,     0,     0,   261,     0,
       0,    95,     0,     0,   971,     0,     0,     0,     0,     0,
     263,     0,     0,     0,   264,     0,     0,   265,     0,   676,
       0,     0,     0,     0,     0,     0,     0,   266,     0,     0,
       0,   106,     0,   199,   975,   268,     0,     0,     0,     0,
       0,     0,   269,     0,     0,     0,     0,     0,     0,     0,
       0,   270,     0,     0,     0,     0,     0,     0,     0,   375,
     271,   272,     0,   273,     0,   274,   376,  1168,     0,     0,
     980,     0,     0,     0,   277,     0,     0,   278,     0,   377,
     279,     0,     0,     0,     0,     0,     0,     0,     0,   487,
     406,   407,   408,   409,   410,     0,     0,   413,   414,   415,
     416,     0,   418,   419,   936,   937,   938,   939,   940,   682,
       0,   683,     0,     0,     0,   684,   685,   686,   687,   688,
     689,   690,   691,   941,   693,   694,     0,   942,     0,     0,
     696,   697,   943,   699,     0,   375,   378,     0,     0,     0,
     379,     0,   376,     0,     0,     0,     0,     0,     0,   486,
       0,     0,     0,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   380,   487,   406,   407,   408,   409,   410,     0,     0,
     413,   414,   415,   416,     0,   418,   419,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   398,   399,
     400,   375,   378,   401,   402,   403,   379,   435,   376,     0,
       0,     0,     0,     0,   404,  1016,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   380,   487,   406,
     407,   408,   409,   410,     0,     0,   413,   414,   415,   416,
       0,   418,   419,   381,   382,     0,   383,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,   392,   393,     0,
     394,   395,   396,   397,   398,   399,   400,   375,   378,   401,
     402,   403,   379,   435,   376,     0,     0,     0,     0,     0,
     404,  1023,     0,     0,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   380,   487,   406,   407,   408,   409,   410,
       0,     0,   413,   414,   415,   416,     0,   418,   419,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,   375,   378,   401,   402,   403,   379,   435,
     376,     0,     0,     0,     0,     0,   404,  1183,     0,     0,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   380,
     487,   406,   407,   408,   409,   410,     0,     0,   413,   414,
     415,   416,     0,   418,   419,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,   392,
     393,     0,   394,   395,   396,   397,   398,   399,   400,   375,
     378,   401,   402,   403,   379,   435,   376,     0,     0,     0,
       0,     0,   404,  1184,     0,     0,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   380,   487,   406,   407,   408,
     409,   410,     0,     0,   413,   414,   415,   416,     0,   418,
     419,   381,   382,     0,   383,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,   392,   393,     0,   394,   395,
     396,   397,   398,   399,   400,   375,   378,   401,   402,   403,
     379,   435,   376,     0,     0,     0,     0,     0,   404,  1185,
       0,     0,     0,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   380,   487,   406,   407,   408,   409,   410,     0,     0,
     413,   414,   415,   416,     0,   418,   419,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   398,   399,
     400,   375,   378,   401,   402,   403,   379,   435,   376,     0,
       0,     0,     0,     0,   404,  1186,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   380,   487,   406,
     407,   408,   409,   410,     0,     0,   413,   414,   415,   416,
       0,   418,   419,   381,   382,     0,   383,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,   392,   393,     0,
     394,   395,   396,   397,   398,   399,   400,   375,   378,   401,
     402,   403,   379,   435,   376,     0,     0,     0,     0,     0,
     404,  1209,     0,     0,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   380,   487,   406,   407,   408,   409,   410,
       0,     0,   413,   414,   415,   416,     0,   418,   419,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,     0,   378,   401,   402,   403,   379,   435,
       0,     0,     0,     0,     0,     0,   404,  1210,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   377,     0,   380,
     487,   406,   407,   408,   409,   410,     0,     0,   413,   414,
     415,   416,     0,   418,   419,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,   392,
     393,     0,   394,   395,   396,   397,   398,   399,   400,   375,
       0,   401,   402,   403,     0,   435,   376,     0,     0,     0,
       0,     0,   404,     0,   378,     0,     0,     0,   379,   377,
     466,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   377,
     289,     0,     0,     0,     0,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,   477,   378,   390,   391,   392,
     379,     0,   394,   395,   396,   397,     0,     0,   400,     0,
       0,   401,   402,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,     0,     0,     0,     0,     0,     0,     0,
       0,   380,     0,     0,     0,     0,   378,     0,     0,     0,
     379,     0,     0,     0,     0,     0,     0,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   398,   399,
     400,   380,     0,   401,   402,   403,     0,   435,     0,     0,
       0,     0,     0,     0,   404,     0,     0,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   398,   399,
     400,   375,     0,   401,   402,   403,     0,   435,   376,     0,
       0,     0,     0,     0,   404,     0,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,   791,     0,     0,     0,     0,     0,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   377,   480,     0,     0,     0,   792,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
       0,     0,   379,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,   376,     0,     0,     0,     0,     0,     0,
       0,     0,   793,     0,     0,     0,   377,   876,     0,     0,
       0,     0,     0,   380,     0,     0,     0,     0,   378,     0,
       0,     0,   379,     0,     0,     0,     0,     0,     0,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,   380,   481,   401,   402,   403,     0,     0,
       0,     0,     0,   378,     0,     0,   404,   379,     0,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,     0,   469,   401,   402,   403,   380,     0,
       0,     0,     0,     0,     0,     0,   404,     0,     0,     0,
       0,     0,     0,     0,   381,   382,     0,   383,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,   392,   393,
       0,   394,   395,   396,   397,   398,   399,   400,   375,     0,
     401,   402,   403,     0,   435,   376,     0,     0,     0,     0,
       0,   404,     0,     0,     0,     0,     0,     0,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,     0,     0,   375,     0,
       0,     0,     0,     0,     0,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   377,   289,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,   375,   379,
       0,     0,     0,     0,     0,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   377,   631,
       0,     0,     0,     0,     0,     0,   184,     0,     0,     0,
     380,     0,     0,     0,   632,   378,     0,     0,     0,   379,
       0,     0,     0,     0,     0,     0,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     380,     0,   401,   402,   403,   378,     0,     0,     0,   379,
       0,     0,     0,   404,     0,     0,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     380,     0,   401,   402,   403,     0,   435,     0,     0,     0,
       0,     0,     0,   404,     0,     0,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     375,   220,   401,   402,   403,    22,    23,   376,     0,     0,
       0,     0,     0,   404,     0,   221,     0,    31,   222,     0,
     377,   633,     0,    37,     0,     0,   -83,     0,     0,     0,
      42,     0,     0,     0,     0,     0,   634,     0,     0,     0,
     375,     0,     0,     0,     0,   -54,     0,   376,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
     377,   868,     0,     0,   -54,     0,     0,   225,     0,    68,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
     375,   379,     0,     0,     0,     0,     0,   376,    84,     0,
       0,    86,     0,   -83,    88,     0,     0,     0,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,   378,     0,     0,
       0,   379,     0,     0,     0,     0,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   380,     0,   401,   402,   403,   378,     0,     0,
       0,   379,     0,     0,     0,   404,     0,     0,   381,   382,
     892,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   380,     0,   401,   402,   403,     0,   435,     0,
       0,     0,     0,     0,     0,   404,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   375,     0,   401,   402,   403,     0,   435,   376,
       0,     0,     0,     0,     0,   404,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   220,     0,     0,     0,    22,    23,
       0,     0,   375,     0,     0,     0,     0,     0,   221,   376,
      31,   222,     0,     0,     0,     0,    37,     0,     0,     0,
       0,     0,   377,    42,     0,     0,     0,  1071,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -54,   378,
       0,     0,     0,   379,     0,     0,     0,   375,   514,    58,
       0,    60,     0,     0,   376,     0,     0,   -54,     0,     0,
     225,     0,    68,  1072,     0,     0,     0,   377,     0,     0,
     469,     0,     0,     0,   380,     0,     0,     0,     0,   378,
       0,    84,     0,   379,    86,     0,     0,    88,     0,     0,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   380,     0,   401,   402,   403,     0,
       0,     0,     0,     0,   378,   930,     0,   404,   379,     0,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   375,     0,   401,   402,   403,   380,
       0,   376,     0,     0,     0,     0,     0,   404,     0,     0,
       0,     0,     0,     0,   377,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,   392,
     393,     0,   394,   395,   396,   397,   398,   399,   400,   375,
       0,   401,   402,   403,     0,     0,   376,     0,     0,     0,
       0,     0,   404,     0,     0,     0,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,   379,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,     0,     0,     0,
     379,     0,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,   392,   393,     0,   394,
     395,   396,   397,   398,   399,   400,     0,     0,   401,   402,
     403,   380,   525,     0,     0,     0,   378,     0,     0,   404,
     379,     0,     0,     0,     0,     0,     0,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   398,   399,
     400,   380,   529,   401,   402,   403,     0,     0,     0,     0,
       0,     0,     0,     0,   404,     0,     0,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   398,   399,
     400,   375,   535,   401,   402,   403,     0,     0,   376,     0,
       0,     0,     0,     0,   404,     0,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
       0,   375,   379,     0,     0,     0,     0,     0,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   552,   380,     0,     0,     0,     0,   378,     0,
       0,     0,   379,     0,     0,     0,     0,     0,     0,   381,
     382,   635,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,   380,     0,   401,   402,   403,   378,     0,
       0,     0,   379,     0,     0,     0,   404,     0,     0,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,   380,     0,   401,   402,   403,     0,     0,
       0,     0,     0,     0,     0,     0,   404,     0,     0,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,   375,   787,   401,   402,   403,     0,     0,
     376,     0,   766,     0,     0,     0,   404,   220,     0,     0,
       0,    22,    23,   377,     0,     0,  1080,     0,     0,     0,
       0,   221,     0,    31,   222,     0,     0,     0,     0,    37,
       0,     0,     0,   375,     0,     0,    42,     0,     0,     0,
     376,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,  1081,     0,
     378,  1082,     0,   225,   379,    68,     0,     0,   375,     0,
       0,     0,     0,     0,     0,   376,     0,     0,     0,     0,
       0,     0,     0,     0,    84,     0,     0,    86,   377,     0,
      88,     0,     0,     0,     0,   380,     0,     0,     0,     0,
     378,     0,     0,     0,   379,     0,     0,     0,     0,     0,
       0,   381,   382,     0,   383,   384,   385,     0,   386,   788,
     388,     0,   389,   390,   391,   392,   393,     0,   394,   395,
     396,   397,   398,   399,   400,   380,     0,   401,   402,   403,
     103,     0,     0,     0,     0,   378,  1232,     0,   404,   379,
       0,   381,   382,     0,   383,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,   392,   393,  -597,   394,   395,
     396,   397,   398,   399,   400,   375,   184,   401,   402,   403,
     380,     0,   376,     0,     0,     0,     0,     0,   404,     0,
       0,     0,     0,     0,     0,   377,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     375,   883,   401,   402,   403,     0,     0,   376,     0,     0,
       0,     0,     0,   404,     0,     0,     0,     0,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,   379,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     377,     0,     0,   890,     0,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,   379,     0,   381,   382,     0,   383,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,   392,   393,     0,
     394,   395,   396,   397,   398,   399,   400,     0,   851,   401,
     402,   403,   380,     0,     0,     0,     0,   378,     0,     0,
     404,   379,     0,     0,     0,     0,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   380,     0,   401,   402,   403,     0,     0,     0,
       0,     0,     0,     0,     0,   404,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   375,     0,   401,   402,   403,     0,     0,   376,
       0,     0,     0,     0,     0,   404,     0,     0,     0,     0,
       0,     0,   377,   220,     0,     0,     0,    22,    23,     0,
       0,     0,  1080,     0,     0,     0,     0,   221,     0,    31,
     222,     0,   375,     0,     0,    37,     0,     0,     0,   376,
       0,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,   378,
      60,     0,    62,   379,  1081,     0,     0,  1082,   375,   225,
       0,    68,     0,     0,     0,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   377,     0,
      84,     0,     0,    86,   380,     0,    88,     0,     0,   378,
       0,     0,     0,   379,     0,     0,     0,     0,     0,     0,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   380,     0,   401,   402,   403,     0,
       0,     0,     0,   918,     0,   378,   103,   404,     0,   379,
     381,   382,  1233,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   375,     0,   401,   402,   403,   933,
     380,   376,     0,   919,     0,     0,     0,   404,     0,     0,
       0,     0,     0,     0,   377,   999,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     375,     0,   401,   402,   403,     0,     0,   376,     0,     0,
       0,     0,     0,   404,     0,     0,     0,     0,     0,     0,
     377,  1002,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,   379,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     377,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,   379,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,   392,   393,     0,   394,
     395,   396,   397,   398,   399,   400,     0,     0,   401,   402,
     403,     0,   380,     0,     0,     0,     0,   378,     0,   404,
       0,   379,     0,     0,     0,     0,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   380,     0,   401,   402,   403,     0,     0,     0,
       0,     0,     0,     0,     0,   404,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   375,  1068,   401,   402,   403,     0,     0,   376,
       0,     0,     0,  1005,     0,   404,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,   376,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,   375,   379,     0,     0,     0,     0,     0,   376,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   380,     0,     0,     0,     0,   378,
       0,     0,     0,   379,     0,     0,     0,     0,     0,     0,
     381,   382,  1099,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   380,     0,   401,   402,   403,   378,
       0,     0,     0,   379,     0,     0,     0,   404,     0,     0,
     381,   382,  1102,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   380,     0,   401,   402,   403,     0,
       0,     0,     0,     0,     0,     0,     0,   404,     0,     0,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   375,  1121,   401,   402,   403,     0,
       0,   376,     0,     0,     0,     0,     0,   404,     0,     0,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,   375,   379,     0,     0,     0,     0,
       0,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,   378,     0,     0,     0,   379,     0,     0,     0,     0,
       0,     0,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,   392,   393,     0,   394,
     395,   396,   397,   398,   399,   400,   380,     0,   401,   402,
     403,   378,     0,     0,     0,   379,     0,     0,     0,   404,
       0,     0,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,   392,   393,     0,   394,
     395,   396,   397,   398,   399,   400,   380,     0,   401,   402,
     403,     0,     0,     0,     0,  1166,     0,     0,     0,   404,
       0,     0,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,   392,   393,     0,   394,
     395,   396,   397,   398,   399,   400,   375,   256,   401,   402,
     403,     0,   959,   376,     0,     0,     0,  1206,     0,   404,
       0,   257,   258,     0,   259,     0,   377,     0,     0,   260,
       0,     0,     0,     0,     0,     0,     0,     0,   261,     0,
       0,     0,     0,     0,   262,     0,   375,     0,     0,     0,
     263,     0,     0,   376,   264,     0,     0,   265,     0,     0,
       0,     0,     0,     0,     0,     0,   377,   266,     0,     0,
       0,     0,     0,     0,   267,   268,     0,     0,     0,     0,
       0,     0,   269,   378,     0,     0,   375,   379,     0,     0,
       0,   270,     0,   376,     0,     0,  1226,     0,     0,     0,
     271,   272,     0,   273,     0,   274,   377,   275,     0,     0,
     276,     0,     0,     0,   277,     0,     0,   278,   380,     0,
     279,     0,     0,   378,     0,     0,     0,   379,     0,     0,
       0,     0,     0,     0,   381,   382,  1227,   383,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,   392,   393,
       0,   394,   395,   396,   397,   398,   399,   400,   380,     0,
     401,   402,   403,   378,     0,     0,     0,   379,     0,     0,
       0,   404,     0,     0,   381,   382,  1228,   383,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,   392,   393,
       0,   394,   395,   396,   397,   398,   399,   400,   380,     0,
     401,   402,   403,     0,     0,     0,     0,     0,     0,     0,
       0,   404,     0,     0,   381,   382,     0,   383,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,   392,   393,
       0,   394,   395,   396,   397,   398,   399,   400,   375,   256,
     401,   402,   403,     0,     0,   376,     0,     0,     0,     0,
       0,   404,     0,   257,   258,     0,   259,     0,   377,     0,
       0,   260,     0,     0,     0,     0,     0,   801,     0,     0,
     261,     0,     0,     0,     0,     0,   262,     0,   375,     0,
       0,     0,   263,     0,     0,   376,   264,     0,     0,   265,
       0,     0,     0,     0,     0,     0,     0,     0,   377,   266,
       0,     0,     0,     0,     0,     0,   267,   268,     0,     0,
       0,     0,     0,     0,   269,   378,     0,     0,   375,   379,
       0,     0,     0,   270,     0,   376,     0,     0,  1229,     0,
       0,     0,   271,   272,     0,   273,     0,   274,   377,   275,
       0,     0,   276,     0,     0,     0,   277,     0,     0,   278,
     380,     0,   279,     0,     0,   378,     0,     0,     0,   379,
       0,     0,     0,     0,     0,     0,   381,   382,  1230,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     380,     0,   401,   402,   403,   378,     0,     0,     0,   379,
       0,     0,     0,   404,     0,     0,   381,   382,  1231,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     380,     0,   401,   402,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,     0,     0,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     375,  -301,   401,   402,   403,     0,     0,   376,     0,     0,
       0,     0,     0,   404,     0,  -301,  -301,     0,  -301,     0,
     377,     0,     0,  -301,     0,     0,     0,     0,     0,     0,
       0,     0,  -301,     0,     0,     0,     0,     0,  -301,     0,
     375,     0,     0,     0,  -301,     0,     0,   376,  -301,  1256,
       0,  -301,     0,     0,     0,     0,     0,     0,     0,     0,
     377,  -301,     0,     0,     0,     0,     0,     0,  -301,  -301,
       0,     0,     0,     0,     0,     0,  -301,   378,     0,     0,
     375,   379,     0,     0,     0,  -301,     0,   376,     0,     0,
       0,     0,     0,     0,  -301,  -301,     0,  -301,     0,  -301,
     377,  -301,     0,     0,  -301,     0,     0,     0,  -301,     0,
       0,  -301,   380,     0,  -301,     0,     0,   378,     0,     0,
       0,   379,     0,     0,     0,     0,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   380,     0,   401,   402,   403,   378,     0,     0,
       0,   379,     0,     0,     0,   404,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   380,     0,   401,   402,   403,     0,     0,     0,
       0,     0,     0,  1285,     0,   404,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   375,     0,   401,   402,   403,     0,     0,   376,
       0,     0,     0,     0,     0,   404,   220,     0,     0,     0,
      22,    23,   377,     0,     0,  1080,     0,     0,     0,     0,
     221,     0,    31,   222,     0,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,  1081,     0,   837,
    1082,     0,   225,   379,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,     0,    86,   377,     0,    88,
       0,     0,     0,     0,   380,     0,     0,     0,     0,   378,
       0,     0,     0,   379,     0,     0,     0,     0,     0,     0,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   380,     0,   401,   402,   403,   103,
     377,     0,     0,     0,   378,  1234,     0,   404,   379,     0,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,     0,     0,   394,   395,   396,
     397,   398,   399,   400,     0,     0,   401,   402,   403,   380,
       0,     0,     0,     0,     0,     0,     0,   404,     0,     0,
       0,     0,     0,     0,     0,   381,   382,   378,   383,   384,
     385,   379,   386,   387,   388,     0,   389,   390,   391,   392,
       0,     0,   394,   395,   396,   397,   398,     0,   400,     0,
       0,   401,   402,   403,     0,   220,     0,     0,     0,    22,
      23,     0,   404,     0,  1080,     0,     0,     0,     0,   221,
       0,    31,   222,     0,     0,     0,     0,    37,   381,   382,
       0,   383,   384,   385,    42,   386,   387,   388,     0,   389,
     390,   391,   392,     0,     0,   394,   395,   396,   397,   398,
       0,   400,     0,     0,   401,   402,   403,     0,     0,     0,
      58,   800,    60,   256,   354,   404,  1081,     0,     0,  1082,
       0,   225,     0,    68,     0,     0,     0,   257,   258,     0,
     259,     0,     0,     0,     0,   260,     0,     0,     0,     0,
       0,   801,    84,     0,   261,    86,     0,     0,    88,     0,
     262,     0,     0,     0,     0,     0,   263,     0,     0,     0,
     264,     0,     0,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   266,     0,     0,     0,     0,     0,     0,
     267,   268,     0,     0,     0,     0,     0,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   103,     0,
     256,     0,     0,     0,     0,     0,   271,   272,     0,   273,
       0,   274,     0,   275,   257,   258,   276,   259,     0,     0,
     277,     0,   260,   278,    23,     0,   279,     0,     0,     0,
       0,   261,     0,     0,     0,     0,     0,   262,     0,     0,
       0,     0,     0,   263,     0,     0,     0,   264,     0,     0,
     265,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     266,     0,     0,     0,     0,     0,     0,   267,   268,     0,
       0,     0,     0,     0,     0,   269,    60,     0,     0,     0,
       0,     0,     0,     0,   270,     0,     0,    68,     0,     0,
       0,     0,     0,   271,   272,     0,   273,     0,   274,     0,
     275,  -311,     0,   276,     0,     0,     0,   277,     0,     0,
     278,     0,    88,   279,     0,  -311,  -311,     0,  -311,     0,
       0,     0,     0,  -311,     0,     0,     0,     0,     0,     0,
       0,     0,  -311,     0,     0,     0,     0,     0,  -311,     0,
       0,     0,     0,     0,  -311,     0,     0,     0,  -311,     0,
       0,  -311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -311,     0,     0,     0,     0,     0,     0,  -311,  -311,
       0,     0,     0,     0,     0,     0,  -311,     0,     0,     0,
       0,     0,     0,     0,     0,  -311,     0,     0,   256,     0,
       0,     0,     0,     0,  -311,  -311,     0,  -311,     0,  -311,
       0,  -311,   257,   258,  -311,   259,     0,     0,  -311,     0,
     260,  -311,     0,     0,  -311,     0,     0,     0,     0,   261,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,   263,     0,     0,     0,   264,     0,     0,   265,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   266,     0,
       0,     0,     0,     0,     0,   267,   268,     0,     0,     0,
       0,     0,     0,   269,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,  -302,     0,     0,     0,     0,
       0,   271,   272,     0,   273,     0,   274,     0,   275,  -302,
    -302,   276,  -302,     0,     0,   277,     0,  -302,   278,     0,
       0,   279,     0,     0,     0,     0,  -302,     0,     0,     0,
       0,     0,  -302,     0,     0,     0,     0,     0,  -302,     0,
       0,     0,  -302,     0,     0,  -302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -302,     0,     0,     0,     0,
       0,     0,  -302,  -302,     0,     0,     0,     0,     0,     0,
    -302,     0,     0,     0,     0,     0,     0,     0,     0,  -302,
       0,     0,  -194,     0,     0,     0,     0,     0,  -302,  -302,
       0,  -302,     0,  -302,     0,  -302,  -194,  -194,  -302,  -194,
       0,     0,  -302,     0,  -194,  -302,     0,     0,  -302,     0,
       0,     0,     0,  -194,     0,     0,     0,     0,     0,  -194,
       0,     0,     0,     0,     0,  -194,     0,     0,     0,  -194,
       0,     0,  -194,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -194,     0,     0,     0,     0,     0,     0,  -194,
    -194,     0,     0,     0,     0,     0,     0,  -194,     0,     0,
       0,     0,     0,     0,     0,     0,  -194,     0,     0,  -186,
       0,     0,     0,     0,     0,  -194,  -194,     0,  -194,     0,
    -194,     0,  -194,  -186,  -186,  -194,  -186,     0,     0,  -194,
       0,  -186,  -194,     0,     0,  -194,     0,     0,     0,     0,
    -186,     0,     0,     0,     0,     0,  -186,     0,     0,     0,
       0,     0,  -186,     0,     0,     0,  -186,     0,     0,  -186,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -186,
       0,     0,     0,     0,     0,     0,  -186,  -186,     0,     0,
       0,     0,     0,     0,  -186,     0,     0,     0,     0,     0,
       0,     0,     0,  -186,     0,     0,     0,     0,     0,     0,
       0,     0,  -186,  -186,     0,  -186,     0,  -186,     0,  -186,
       0,     0,  -186,     0,     0,     0,  -186,     0,     0,  -186,
       0,     0,  -186
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1142)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      12,    65,   255,   530,    16,   426,    19,   479,    20,   212,
       2,   506,    69,   501,    26,   244,    27,   168,   154,    31,
      32,   494,    34,    35,    36,    37,    38,   912,   811,    22,
     811,   801,   988,   236,   990,   530,   793,    49,   665,    51,
      52,     1,   228,   102,  1137,    57,   737,    59,   957,    61,
     107,    33,   305,    33,    33,  1045,    33,    69,    70,    71,
      72,    73,    74,   788,    48,    33,    78,    48,    80,   255,
     323,  1192,    92,    47,  1042,    87,    61,   213,    90,    91,
       2,    93,    33,    95,   651,     1,    98,    99,   655,   101,
     657,   120,   120,   105,   106,   107,   108,   146,    69,   527,
     146,    56,   253,   118,   146,  1246,    89,    56,     1,   994,
      93,   170,     3,    66,     1,    75,     1,    48,  1125,   305,
     349,   350,   171,   165,    48,  1246,    34,    35,    36,   175,
      88,   146,    85,   118,   105,   106,   107,   323,   120,   120,
     114,   120,   199,    27,  1285,   836,   175,   175,    48,    48,
     170,    48,   164,     0,   112,    48,   881,   117,  1126,    90,
     224,  1046,   146,   636,    48,   146,   146,     3,    52,   422,
     423,  1080,  1265,  1163,    61,  1084,    61,   189,   146,     3,
     192,   193,   194,   105,  1191,   197,   198,   199,   170,   959,
     170,   170,    76,   170,   175,    79,   823,    81,   211,   107,
    1109,   387,   170,  1112,  1113,    92,    90,    92,   219,   125,
     222,   171,   103,  1169,  1170,   165,   228,   172,   909,   170,
     105,   649,   146,   172,     3,   109,    18,   198,   199,   145,
     146,   118,   125,   118,    56,    75,   422,   423,    70,   232,
    1125,    20,   306,   255,   665,   238,   146,   146,   736,   146,
    1159,   175,   145,   146,   247,    34,    92,   173,   145,  1010,
     145,    92,  1013,  1014,   120,    27,    27,   103,  1177,    37,
    1179,  1180,   165,   146,   106,   175,   175,   117,   175,   103,
     165,  1263,   146,   170,    63,   170,    78,     3,   855,   173,
      51,   858,   859,   305,    61,    48,   165,    56,   171,  1281,
     312,   313,   125,   172,    96,    73,  1191,    83,   494,   173,
      86,   323,  1095,  1070,  1095,  1072,    77,    79,    79,   455,
     792,   113,   795,   146,   103,    92,   462,   150,    90,    90,
     109,   171,   344,   146,   165,   146,   146,    90,   105,   170,
     165,   527,    56,   172,   140,   174,   171,   359,   360,   341,
     173,   118,   148,   115,   115,   165,    90,  1108,   171,  1110,
    1111,   373,   173,   375,   376,   377,   378,   379,   380,    27,
     382,   383,   384,   385,   386,   848,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   170,  1021,   172,    27,    47,   165,   411,
     412,   146,   823,   170,   146,   417,   172,   464,   420,   421,
     422,   423,   424,   425,   426,   903,    66,    27,   146,   341,
     146,    79,    61,   146,   125,   437,    32,    56,   173,   165,
     442,   173,    90,   445,   356,    85,   172,   125,   174,   172,
     669,   670,   165,    56,    61,   173,   146,   173,    79,   150,
     636,   109,   464,    92,   466,   467,    54,   115,   146,    90,
     472,   473,   150,   649,   114,   477,   105,   479,   480,    79,
     378,   379,   944,   173,   486,    92,    74,   489,   109,   118,
      90,   140,   494,    89,   146,   173,    84,    93,   105,   148,
     146,   146,   146,   464,   140,   146,   508,   146,    56,   109,
     512,   118,   148,   515,   516,   172,    56,   174,   520,   165,
     567,   173,    49,   435,   146,   527,   424,   425,   173,   173,
      56,   514,   173,   170,   173,  1052,   165,   539,   146,   437,
     542,   170,   544,   165,   140,    72,   548,   549,   550,   145,
     552,   553,   148,   146,   773,   774,   152,   165,   165,   520,
    1045,   146,    56,   170,   165,   567,   146,  1052,  1197,   467,
     171,   764,   165,   146,   146,   473,   558,   146,   539,   477,
     165,   542,   146,   544,    56,   165,   498,    89,   549,   411,
     412,    93,   165,   165,   232,   417,   165,     8,   420,   421,
     238,   165,   172,    56,   174,   172,   567,  1069,   140,  1071,
    1021,   787,   788,   145,    89,   142,   148,   100,    93,   795,
     152,  1250,  1251,  1252,  1253,  1254,  1255,    56,   172,   631,
     632,   633,   634,   635,   636,   637,   638,   640,   641,    89,
      89,    56,    56,    93,    93,   647,   558,   649,    89,   172,
     548,   174,    93,   172,    56,   174,   125,   659,    56,   661,
      48,    85,   172,   665,   174,   667,   542,    56,   544,   165,
     811,   164,   848,    61,   165,   677,   165,   679,  1163,   156,
     165,   165,   151,   210,   153,   154,   647,   165,   157,   158,
     692,   165,   172,   695,   165,   742,    56,   170,   165,   119,
     165,    88,   172,   125,    92,   707,   708,  1192,   172,   172,
     172,   172,   172,   165,   241,   242,   243,   244,   245,   246,
     542,  1206,   544,     8,   865,   252,   253,    66,   145,   148,
     118,   872,   170,    69,   632,   172,   634,   125,   740,   741,
     742,   125,   165,   173,   746,    27,   146,   165,    48,   125,
     125,   172,    35,     2,    35,   282,   146,   173,   146,   125,
     173,  1246,    21,    12,   173,   292,    48,   294,   173,   146,
      52,   175,   175,   170,   301,   146,    25,   165,  1263,    28,
     172,   742,   170,   785,   165,   173,   165,   789,   173,   791,
     792,   793,   319,   795,    76,   165,  1281,    79,   165,    81,
    1285,   165,   165,   330,   331,    35,   175,   172,    90,   165,
    1053,  1054,    35,    66,   146,   173,   173,   171,   173,   146,
     171,   823,   125,    90,     2,   647,   173,   109,    33,    78,
      33,   175,   170,   170,    12,   837,   838,   839,    35,   170,
     842,   165,    90,   145,   172,   175,   848,    25,   165,   172,
      28,   145,   171,   175,   175,   148,   105,   172,   148,   148,
     140,   110,   148,   140,   866,   170,   868,   140,    35,   148,
     148,   873,   148,   148,   876,   922,   148,  1053,  1054,   881,
     148,   883,   148,   140,   140,   148,   170,     8,   890,    27,
     105,   146,   785,   142,   165,   140,   170,   140,   142,   356,
      78,    22,   164,   173,   165,    26,    27,   165,   173,   173,
      48,   438,    24,   173,    52,    36,   492,    38,    39,   165,
     922,   173,   164,    44,   746,   175,   928,   105,   930,   512,
      51,   933,   110,  1084,    90,   952,   185,   896,    76,   837,
     838,    79,   944,    81,   471,  1095,   962,  1091,   475,   901,
     728,   965,    90,  1265,  1095,  1222,    77,  1256,    79,   961,
    1249,   922,    83,    37,   142,    86,   509,    88,    31,    90,
      -1,   109,    -1,    -1,  1115,  1116,    -1,    -1,  1119,  1120,
      -1,   983,    -1,    -1,    -1,    -1,    -1,    -1,   109,   901,
      -1,   112,    -1,    -1,   115,    48,   998,   999,    -1,  1001,
    1002,    -1,  1004,  1005,  1145,    -1,  1147,   185,    61,    -1,
      -1,    -1,    -1,    -1,  1016,  1017,    -1,  1019,    -1,  1021,
      -1,  1023,  1024,    -1,    -1,  1027,    -1,    -1,  1082,    -1,
      -1,    -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,    92,
     289,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,  1051,
    1052,  1053,  1054,    -1,    -1,    -1,  1058,    -1,    -1,    -1,
    1062,    -1,    -1,    -1,    -1,   118,  1068,  1069,  1070,  1071,
    1072,    -1,   125,    -1,    -1,  1077,    -1,    48,    -1,    -1,
    1127,    -1,    -1,    -1,  1045,    -1,    -1,    -1,    -1,    -1,
      61,    -1,   341,   146,    -1,    -1,    -1,  1099,  1100,  1101,
    1102,  1103,  1104,  1105,  1106,   283,    -1,   356,    -1,    -1,
      -1,   289,   165,    -1,    -1,    -1,    -1,   170,    -1,  1121,
     173,    92,    -1,    -1,    -1,  1127,    -1,    -1,    -1,    -1,
    1132,    -1,    -1,    -1,  1136,    -1,   663,   664,    -1,    32,
      -1,    -1,   669,   670,    -1,   672,   673,   118,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   341,    -1,    -1,  1127,    -1,    -1,    -1,
    1217,  1132,    -1,    -1,    -1,   146,    -1,    -1,   356,  1091,
      -1,  1183,  1184,  1185,  1186,    -1,   435,   714,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    89,    -1,    -1,   170,
      93,    -1,   173,    -1,  1206,    -1,    -1,  1209,  1210,    -1,
      -1,    -1,    -1,    -1,    -1,  1217,    -1,   466,    -1,    -1,
      -1,    -1,     2,   472,  1226,  1227,  1228,  1229,  1230,  1231,
      -1,  1192,    12,    -1,    -1,   484,    -1,   486,    -1,    32,
      -1,    -1,    -1,  1245,  1246,    25,  1248,   140,    28,    -1,
      -1,    -1,   145,    -1,   147,   148,  1217,   435,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   796,
      -1,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,  1281,
      -1,    -1,    -1,  1285,    -1,  1246,    -1,    -1,   466,    -1,
      -1,    -1,    -1,     2,   472,    -1,    89,   546,    78,    -1,
      93,   550,    -1,    12,    -1,    -1,   484,    -1,   486,   558,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,
      -1,    -1,   849,   850,  1285,   105,    -1,    22,    -1,    -1,
     110,    26,    27,    -1,    -1,    -1,   863,    -1,   865,    -1,
      -1,    36,    -1,    38,    39,   872,    -1,   140,   875,    44,
      -1,   878,   145,    -1,   147,   148,    51,    -1,    -1,   152,
      -1,    -1,   142,    -1,     2,    -1,    -1,   160,   546,    78,
     897,    -1,   550,     1,    12,   168,    -1,    -1,    -1,    -1,
     558,    -1,    77,    -1,    79,    -1,    -1,    25,    83,    -1,
      28,    86,    -1,    88,    22,    90,   105,    -1,    26,    27,
      -1,   110,    -1,    31,    -1,   185,    -1,    -1,    36,    -1,
      38,    39,    -1,    -1,   109,    -1,    44,   112,    -1,    -1,
     115,    -1,    -1,    51,    -1,    22,    54,    -1,   955,    26,
      27,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    36,
      78,    38,    39,    -1,    -1,    -1,    74,    44,    -1,    77,
     628,    79,    -1,    81,    51,    83,    84,    -1,    86,    -1,
      88,   710,    90,    -1,   713,    -1,    -1,   105,    -1,    66,
     165,    -1,   110,    -1,    -1,    -1,   185,    -1,    -1,    -1,
      77,   109,    79,    -1,   112,    -1,    83,   115,    85,    86,
      -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   283,   142,    -1,    -1,    -1,    -1,   289,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,   772,    -1,    -1,    -1,     8,    -1,   778,
    1057,    -1,   710,    -1,    -1,   713,    -1,   165,    -1,    -1,
      -1,    22,    -1,   171,    -1,    26,    27,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,
      -1,   341,    -1,    44,    -1,    -1,    -1,    -1,   165,    -1,
      51,    -1,    -1,    -1,   283,    -1,   356,    -1,    -1,    -1,
     289,    -1,    -1,    -1,    -1,    66,    -1,    -1,  1115,  1116,
      -1,    -1,  1119,  1120,   772,    -1,    77,    -1,    79,    -1,
     778,    -1,    83,    -1,    85,    86,    -1,    88,    -1,    90,
    1137,    -1,    32,    -1,    -1,    -1,    -1,   866,  1145,   868,
    1147,    -1,    -1,    -1,   873,    -1,    -1,   876,   109,    -1,
    1157,   112,   341,   811,   115,    -1,    -1,    -1,   887,    -1,
     889,    -1,    -1,   892,    -1,   283,    -1,   356,    -1,    -1,
      -1,   289,    -1,    -1,    -1,   435,    -1,    -1,    -1,   837,
     838,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,   921,    93,    -1,    -1,    -1,    -1,    -1,   928,
      -1,   930,    -1,    -1,   165,    -1,   466,    -1,   866,    -1,
     868,    -1,   472,    -1,    -1,   873,    -1,    -1,   876,    -1,
      -1,    -1,    -1,   341,   484,    -1,   486,    -1,    -1,   887,
      -1,   889,    -1,    -1,   892,    -1,    -1,    -1,   356,    -1,
     140,   141,    -1,   143,   144,   145,   435,   147,   148,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,  1265,    -1,
     160,    -1,    -1,   921,    -1,    -1,   166,   167,   168,    -1,
     928,    -1,   930,    -1,    -1,    -1,    -1,   466,    -1,    -1,
      -1,    -1,    -1,   472,    -1,    -1,   546,  1016,    -1,    -1,
     550,    -1,     3,    -1,  1023,   484,    -1,   486,   558,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,  1037,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,   435,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,  1055,    -1,    -1,    40,
      -1,    -1,  1061,    -1,    -1,    46,  1065,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,   466,    -1,
      -1,    -1,    63,    -1,   472,    -1,    -1,   546,  1016,    70,
      71,   550,    -1,    -1,    -1,  1023,   484,    78,   486,   558,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,  1037,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,  1122,    -1,   106,    -1,  1055,     3,   110,
     111,    -1,   113,  1061,    -1,   116,    -1,  1065,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    32,    -1,    -1,    -1,     2,    -1,   546,    34,
      -1,    -1,   550,    -1,    -1,    40,    12,    -1,    -1,    -1,
     558,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    25,
     710,    -1,    28,   713,  1183,  1184,  1185,  1186,    63,    -1,
      -1,   172,    -1,    -1,  1122,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    89,
    1209,  1210,    87,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,     2,   103,    -1,
      -1,   106,    78,    -1,    -1,   110,    -1,    12,   113,    -1,
      -1,   116,   772,    -1,    -1,    -1,    -1,    -1,   778,    -1,
      25,   710,    -1,    28,   713,  1183,  1184,  1185,  1186,   105,
     140,   141,    -1,    -1,   110,   145,    -1,   147,   148,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,  1209,  1210,    -1,    -1,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,   170,   142,   172,     2,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    32,    -1,   772,    -1,    -1,    -1,    -1,    -1,   778,
      -1,    25,   710,    -1,    28,   713,    -1,    -1,    -1,    -1,
     105,    -1,    -1,    -1,    -1,   110,   866,    -1,   868,   185,
      -1,    -1,    -1,   873,    -1,    -1,   876,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   887,    -1,   889,
      -1,    -1,   892,    -1,    -1,    -1,    -1,   142,    89,    -1,
      -1,    -1,    93,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   772,    -1,    -1,    -1,    -1,    -1,
     778,   921,    -1,    -1,    -1,    -1,    -1,    -1,   928,    -1,
     930,   105,    -1,    -1,    -1,    -1,   110,   866,    -1,   868,
     185,    -1,    -1,    -1,   873,    -1,    -1,   876,    -1,   140,
     141,    -1,    -1,   144,   145,    -1,   147,   148,   887,    -1,
     889,   152,    -1,   892,     3,    -1,    -1,   283,   142,   160,
      -1,    -1,    -1,   289,    -1,   166,   167,   168,    17,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,   921,    -1,    -1,    -1,    -1,    36,    -1,   928,
      -1,   930,    -1,    -1,    -1,    -1,    -1,    46,   866,    -1,
     868,   185,    -1,    -1,    53,   873,  1016,    -1,   876,    -1,
      -1,    -1,    -1,  1023,    63,   341,    -1,    -1,    -1,   887,
      -1,   889,    71,    -1,   892,    -1,    -1,  1037,   283,    -1,
     356,    -1,    -1,    -1,   289,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,  1055,    -1,    -1,    -1,    -1,
      99,  1061,    -1,   921,   103,  1065,    -1,    -1,    -1,    -1,
     928,   110,   930,    -1,    -1,    -1,    22,   116,    -1,    -1,
      26,    27,    -1,    -1,    -1,    31,    -1,  1016,    -1,    -1,
      36,    -1,    38,    39,  1023,    -1,   341,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,  1037,   283,
      -1,   356,    -1,    -1,    -1,   289,    -1,    -1,    -1,   435,
      -1,    -1,  1122,    -1,    -1,    -1,  1055,    -1,    -1,    -1,
      -1,    77,  1061,    79,    -1,    81,  1065,    83,    -1,    -1,
      86,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,
     466,    -1,    -1,    -1,    -1,    -1,   472,    -1,  1016,    32,
      -1,    -1,    -1,   109,    -1,  1023,   112,   341,   484,   115,
     486,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1037,
      -1,    -1,   356,  1183,  1184,  1185,  1186,    -1,    -1,    -1,
     435,    -1,    -1,  1122,    -1,    -1,    -1,  1055,    -1,    -1,
      -1,    -1,    -1,  1061,    -1,    -1,    -1,  1065,    -1,  1209,
    1210,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,   165,
      93,   466,    -1,    -1,    -1,   171,     2,   472,    -1,    -1,
     546,    -1,    -1,    -1,   550,    -1,    12,    -1,    -1,   484,
      -1,   486,   558,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    28,    -1,  1183,  1184,  1185,  1186,    -1,    -1,
      -1,   435,    -1,    -1,  1122,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,    -1,   152,
    1209,  1210,    -1,    -1,    -1,    -1,   159,   160,    -1,    -1,
     163,    -1,   466,   166,   167,   168,    -1,     2,   472,    -1,
      -1,   546,    78,    -1,   177,   550,    -1,    12,    -1,    -1,
     484,    -1,   486,   558,    -1,    -1,    -1,    -1,     3,    -1,
      25,    -1,    -1,    28,    -1,  1183,  1184,  1185,  1186,   105,
      -1,    -1,    17,    18,   110,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,  1209,  1210,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    -1,   546,    78,    -1,    -1,   550,    -1,    63,    -1,
      -1,    -1,    -1,    -1,   558,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,   710,    -1,    -1,   713,    -1,    -1,
     105,    -1,    87,    -1,    -1,   110,    -1,    -1,    -1,   185,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,   772,    -1,    -1,    -1,
      44,    -1,   778,    -1,    -1,   710,    -1,    51,   713,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
     185,    -1,    -1,    36,    -1,    38,    39,   172,    -1,    -1,
      -1,    44,    -1,    77,    -1,    79,    -1,    81,    51,    83,
      -1,    -1,    86,    -1,    88,    -1,    90,   283,    -1,    -1,
      -1,    -1,    -1,   289,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,   109,    79,   772,   112,    -1,
      -1,   115,    -1,   778,    -1,    88,   710,    90,    -1,   713,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     866,    -1,   868,    -1,    -1,    -1,   109,   873,    -1,   112,
     876,    -1,   115,    -1,    -1,   341,    -1,    -1,    -1,    -1,
      -1,   887,    -1,   889,    -1,    -1,   892,    -1,   283,    -1,
     356,   165,    -1,    -1,   289,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   772,    -1,
      -1,    -1,    -1,    -1,   778,   921,    -1,    -1,    -1,    -1,
      -1,    -1,   928,    -1,   930,    -1,    -1,    -1,    -1,    -1,
      -1,   866,    -1,   868,    -1,    -1,    -1,    -1,   873,    -1,
      -1,   876,    44,    45,    46,    -1,   341,    -1,    -1,    -1,
      -1,    -1,   887,    -1,   889,    -1,    -1,   892,    -1,    -1,
      -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   435,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,   921,    -1,    -1,    -1,
      -1,    -1,    -1,   928,    -1,   930,    -1,    -1,    -1,    -1,
     466,    -1,   866,    -1,   868,    -1,   472,    -1,    -1,   873,
    1016,    -1,   876,    -1,    -1,     3,    -1,  1023,   484,    -1,
     486,    -1,    -1,   887,    -1,   889,    -1,    -1,   892,    17,
      18,  1037,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
     435,    -1,    -1,    -1,    -1,   147,    34,   149,    -1,  1055,
      -1,    -1,    40,    -1,    -1,  1061,    -1,   921,    46,  1065,
      -1,    -1,    50,    -1,   928,    53,   930,    72,    -1,    -1,
      -1,   466,    -1,    -1,    -1,    63,    -1,   472,    -1,    -1,
     546,  1016,    70,    71,   550,    -1,    -1,    -1,  1023,   484,
      78,    -1,   558,   195,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,  1037,    -1,    -1,    -1,    -1,    -1,    96,    97,
     212,    99,    -1,   101,    -1,   103,  1122,    -1,   106,    -1,
    1055,    -1,   110,    -1,    -1,   113,  1061,    -1,   116,    -1,
    1065,    -1,    -1,    -1,   236,    -1,    -1,   239,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   250,   251,
      -1,   546,  1016,    -1,    -1,   550,    -1,    -1,    -1,  1023,
      -1,    -1,    -1,   558,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1037,    -1,    -1,    -1,  1183,  1184,  1185,
    1186,    -1,    -1,   285,   172,    -1,    -1,  1122,    -1,    -1,
      -1,  1055,    -1,    -1,    -1,    -1,    -1,  1061,    -1,    -1,
      -1,  1065,    -1,  1209,  1210,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,   241,   242,   243,   244,
     245,   246,    -1,    -1,   710,    -1,    -1,   713,  1183,  1184,
    1185,  1186,    -1,    -1,    -1,    -1,    -1,    -1,  1122,   361,
      77,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    86,
      -1,    88,    -1,    90,  1209,  1210,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   292,    -1,   294,
      -1,    -1,   109,    -1,    -1,   112,   301,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   772,    -1,    -1,    -1,
      -1,    -1,   778,    -1,   319,   710,    -1,    -1,   713,  1183,
    1184,  1185,  1186,    -1,    -1,   330,   331,    -1,    -1,    -1,
      -1,    -1,   434,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1209,  1210,    -1,   165,    -1,
      -1,   453,    -1,    -1,   171,    -1,   458,    -1,   460,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,   772,    -1,    -1,
      -1,    -1,    -1,   778,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,   501,
     866,    -1,   868,    48,    -1,    -1,    -1,   873,    -1,    -1,
     876,    -1,   514,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   887,    -1,   889,    -1,    -1,   892,    -1,    -1,    -1,
      -1,    -1,    -1,   438,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,   921,    -1,   559,    -1,    -1,
      -1,    -1,   928,    -1,   930,    -1,   471,   569,    -1,    -1,
     475,   866,    -1,   868,    -1,   120,    -1,    -1,   873,   124,
      -1,   876,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   887,    -1,   889,   140,   141,   892,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,   170,   921,    -1,    -1,    -1,
      -1,    -1,   177,   928,    -1,   930,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   650,    -1,
    1016,   653,   654,    -1,    -1,    -1,    -1,  1023,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,  1037,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,  1055,
      -1,    -1,    -1,    -1,    -1,  1061,    -1,    -1,    -1,  1065,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    86,     3,    88,    -1,    90,    -1,    -1,    -1,
     732,    -1,    12,    -1,   736,    -1,    -1,    -1,    -1,    19,
      20,    -1,  1037,    -1,    -1,   109,    -1,    -1,   112,    -1,
     752,   115,    -1,    -1,    34,    -1,  1122,    -1,   663,   664,
    1055,    -1,   764,    -1,   669,   670,  1061,   672,   673,    -1,
    1065,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   796,    -1,    -1,    -1,    -1,   801,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,   811,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1183,  1184,  1185,
    1186,    -1,    -1,   103,    -1,    -1,    -1,  1122,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1209,  1210,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,   147,    -1,    -1,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,   163,    -1,    -1,   166,   167,   168,   169,
     170,    -1,   894,    -1,   896,    -1,    -1,    -1,   900,    -1,
      -1,   903,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   849,   850,   169,    -1,    -1,   172,
     952,   174,    -1,    -1,    -1,    -1,    -1,   959,   863,    -1,
     962,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     875,     0,     1,   878,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,  1095,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     1,
      -1,     3,  1057,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     3,   177,     5,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    -1,    -1,    17,    18,
      19,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,   147,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   166,   167,   168,
     169,   170,    -1,   172,    -1,   174,    -1,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      -1,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    64,    65,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,   119,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,   107,   108,    -1,   110,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
     173,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,   173,   174,    -1,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    48,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,   110,    -1,
     112,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     3,   177,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,   107,   108,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     3,   177,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     3,   177,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     3,   177,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,   110,    -1,   112,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,
     177,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,   175,     3,   177,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     3,   177,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,   175,     3,   177,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     3,   177,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     3,   177,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     3,   177,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     3,   177,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     3,   177,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,
     177,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     3,   177,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     3,   177,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     3,   177,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    12,    -1,    -1,    -1,
     149,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    34,   168,
      36,    -1,    -1,   172,    40,   174,    -1,    -1,   177,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,    -1,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,   147,    -1,    -1,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,    -1,    -1,
     166,   167,   168,   169,    -1,     3,   172,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    32,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    89,    82,    -1,    -1,    93,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,    -1,   142,    -1,   152,    -1,
     146,    -1,    -1,    -1,   150,   159,   160,    -1,    -1,   163,
      -1,    -1,   166,   167,   168,    -1,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    61,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,   105,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
     164,   165,    25,    -1,    -1,    -1,   170,    -1,   172,    -1,
     174,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,   164,    -1,    25,    -1,    -1,    -1,   170,    -1,   172,
      -1,   174,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,   164,    -1,    25,    -1,    -1,    -1,   170,    -1,
     172,    -1,   174,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,     3,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    12,    -1,    -1,    -1,
      -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,   142,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,   172,    -1,   174,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      96,    97,    -1,    99,    -1,   101,    19,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    32,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,    -1,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,   147,    -1,    -1,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,    -1,    -1,
     166,   167,   168,   169,    -1,    12,    89,    -1,    -1,    -1,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    -1,   138,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,
     177,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,   170,
      19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,   170,    19,    -1,    -1,    -1,
      -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    -1,   138,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,
     177,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,    89,   166,   167,   168,    93,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,   170,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    89,    -1,    -1,    -1,    93,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    48,    89,   152,   153,   154,
      93,    -1,   157,   158,   159,   160,    -1,    -1,   163,    -1,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    -1,   166,   167,   168,    -1,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    73,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,   165,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,   177,    93,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,   120,   166,   167,   168,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,    -1,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    12,    93,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    48,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    22,   166,   167,   168,    26,    27,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    36,    -1,    38,    39,    -1,
      32,    33,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    66,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      32,    33,    -1,    -1,    85,    -1,    -1,    88,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      12,    93,    -1,    -1,    -1,    -1,    -1,    19,   109,    -1,
      -1,   112,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
     102,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,   170,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    36,    19,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    32,    51,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    12,    13,    77,
      -1,    79,    -1,    -1,    19,    -1,    -1,    85,    -1,    -1,
      88,    -1,    90,    73,    -1,    -1,    -1,    32,    -1,    -1,
     120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,   109,    -1,    93,   112,    -1,    -1,   115,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    89,   175,    -1,   177,    93,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    -1,   166,   167,   168,   124,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   124,   170,    -1,    -1,    -1,    89,    -1,    -1,   177,
      93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,   165,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,   165,   166,   167,   168,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   102,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    13,   166,   167,   168,    -1,    -1,
      19,    -1,   173,    -1,    -1,    -1,   177,    22,    -1,    -1,
      -1,    26,    27,    32,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    12,    -1,    -1,    51,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,    -1,
      89,    86,    -1,    88,    93,    90,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    32,    -1,
     115,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
     165,    -1,    -1,    -1,    -1,    89,   171,    -1,   177,    93,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,   120,   166,   167,   168,
     124,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    13,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    35,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,   165,   166,
     167,   168,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
     177,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,
      39,    -1,    12,    -1,    -1,    44,    -1,    -1,    -1,    19,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    89,
      79,    -1,    81,    93,    83,    -1,    -1,    86,    12,    88,
      -1,    90,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
     109,    -1,    -1,   112,   124,    -1,   115,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,   173,    -1,    89,   165,   177,    -1,    93,
     140,   141,   171,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    -1,   166,   167,   168,   123,
     124,    19,    -1,   173,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,   177,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    13,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,   175,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   102,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,
     140,   141,   102,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    13,   166,   167,   168,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,   177,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,   177,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,     3,   166,   167,
     168,    -1,     8,    19,    -1,    -1,    -1,   175,    -1,   177,
      -1,    17,    18,    -1,    20,    -1,    32,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    12,    -1,    -1,    -1,
      46,    -1,    -1,    19,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    89,    -1,    -1,    12,    93,    -1,    -1,
      -1,    87,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    32,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,   124,    -1,
     116,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   102,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,   102,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,     3,
     166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    17,    18,    -1,    20,    -1,    32,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    12,    -1,
      -1,    -1,    46,    -1,    -1,    19,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    89,    -1,    -1,    12,    93,
      -1,    -1,    -1,    87,    -1,    19,    -1,    -1,   102,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    32,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
     124,    -1,   116,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   102,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,   102,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,     3,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    17,    18,    -1,    20,    -1,
      32,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      12,    -1,    -1,    -1,    46,    -1,    -1,    19,    50,    61,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    89,    -1,    -1,
      12,    93,    -1,    -1,    -1,    87,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      32,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,   124,    -1,   116,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    22,    -1,    -1,    -1,
      26,    27,    32,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    83,    -1,    89,
      86,    -1,    88,    93,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,   112,    32,    -1,   115,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,   165,
      32,    -1,    -1,    -1,    89,   171,    -1,   177,    93,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    89,   143,   144,
     145,    93,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,   157,   158,   159,   160,   161,    -1,   163,    -1,
      -1,   166,   167,   168,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,   177,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,    -1,    -1,    -1,    44,   140,   141,
      -1,   143,   144,   145,    51,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,    -1,   157,   158,   159,   160,   161,
      -1,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
      77,     1,    79,     3,    81,   177,    83,    -1,    -1,    86,
      -1,    88,    -1,    90,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,   109,    -1,    34,   112,    -1,    -1,   115,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,   165,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,
      -1,   101,    -1,   103,    17,    18,   106,    20,    -1,    -1,
     110,    -1,    25,   113,    27,    -1,   116,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,     3,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    17,    18,   106,    20,    -1,    -1,   110,    -1,
      25,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,    17,
      18,   106,    20,    -1,    -1,   110,    -1,    25,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    17,    18,   106,    20,
      -1,    -1,   110,    -1,    25,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    17,    18,   106,    20,    -1,    -1,   110,
      -1,    25,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   182,   183,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    33,    34,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      49,    50,    51,    53,    56,    57,    58,    59,    60,    62,
      63,    64,    65,    67,    68,    70,    71,    72,    77,    78,
      79,    80,    81,    82,    83,    86,    87,    88,    90,    91,
      92,    94,    95,    96,    97,    98,    99,   100,   101,   103,
     104,   106,   107,   108,   109,   110,   112,   113,   115,   116,
     119,   121,   124,   140,   141,   142,   143,   144,   149,   159,
     162,   163,   164,   165,   168,   170,   172,   174,   177,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   195,   197,
     202,   203,   204,   207,   208,   212,   214,   217,   220,   222,
     224,   225,   226,   233,   234,   236,   238,   241,   242,   243,
     244,   245,   246,   247,   251,   252,   255,   256,   259,   260,
     264,   267,   268,   296,   299,   300,   320,   321,   322,   323,
     324,   325,   326,   334,   335,   336,   337,   338,   341,   342,
     343,   344,   345,   346,   347,   348,   350,   351,   352,   353,
     354,   165,   186,   338,   120,   327,   328,     3,   209,    14,
      22,    36,    41,    42,    45,    56,    88,   101,   170,   174,
     241,   264,   320,   325,   336,   337,   338,   341,   343,   344,
     327,   338,   109,   299,    90,   209,   186,   314,   338,     8,
      22,    36,    39,    83,    86,    88,   189,   186,   172,     8,
      88,   338,   339,     8,    11,    88,   109,   112,   339,    79,
     122,   235,   338,   235,   338,   235,   338,    26,   300,   338,
      27,   115,   237,   338,   196,   172,     3,    17,    18,    20,
      25,    34,    40,    46,    50,    53,    63,    70,    71,    78,
      87,    96,    97,    99,   101,   103,   106,   110,   113,   116,
     211,   213,   211,   211,   265,   172,   211,   301,   302,    33,
     197,   216,   338,   218,   219,   338,   338,    18,    78,    96,
     113,   338,   338,   338,     8,   172,   223,   224,     4,   290,
     313,   338,   107,   108,   165,   338,   340,   338,   216,   338,
     338,   338,   100,   172,   186,   338,   338,   211,   297,   338,
     237,   338,   338,   338,   338,   338,   338,   338,     1,   171,
     184,   198,   313,   111,   150,   290,   315,   316,   340,   235,
     313,   338,   349,   338,    81,   186,   170,    85,   193,    47,
     114,    56,   187,   197,   211,   211,    54,    74,    84,   285,
     301,   164,   165,   156,   338,    12,    19,    32,    89,    93,
     124,   140,   141,   143,   144,   145,   147,   148,   149,   151,
     152,   153,   154,   155,   157,   158,   159,   160,   161,   162,
     163,   166,   167,   168,   177,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     169,   275,   172,   174,    89,    93,   172,   186,   165,   338,
     338,   338,   211,   313,    56,   170,   197,    48,   327,   297,
     301,   165,   146,   165,   189,   119,   212,   290,   317,   318,
     319,   340,    88,   231,   268,   299,    88,   112,   227,   297,
     229,   268,   299,   211,   172,   216,    33,    48,   216,   120,
     216,   330,    33,    48,   216,   330,   216,    48,   216,    37,
      73,   165,   211,   211,   102,   197,   102,   125,   197,   275,
      83,    86,   194,   317,   172,   172,   197,   186,   172,   278,
     111,   172,   211,   303,   304,     1,   145,   308,    48,   146,
     186,   216,   146,   216,    13,   172,   172,   216,   317,   224,
     146,   165,   338,   338,   165,   170,   216,   172,   317,   165,
     125,   298,   165,   216,   216,   165,   171,   171,   184,   146,
     171,   338,   146,   173,   146,   173,   175,   330,    48,   146,
     175,   330,   123,   146,   175,     8,     1,   171,   198,    66,
     205,   206,   338,   200,   338,   211,   248,   145,   257,   170,
     269,   165,   338,   338,   338,   338,   235,   338,   235,   338,
     338,   338,   338,   338,   338,   338,     3,    20,    34,    63,
     103,   109,   212,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
      69,   340,   340,   340,   340,   340,   317,   317,   235,   338,
     235,   338,    27,    48,    90,   115,   329,   332,   333,   338,
     354,    33,    48,    33,    48,   102,   172,    48,   175,   211,
     235,   338,   216,   165,   165,   338,   338,   125,   173,   146,
     232,   211,   301,   228,   230,   211,   165,   211,   301,    48,
     313,    45,   338,   235,   338,   172,   216,    45,   338,   235,
     338,   216,   235,   338,    12,    19,    55,   140,   141,   142,
     143,   144,   145,   147,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   163,   166,   167,   168,   169,
     199,   274,   275,   276,   338,   199,   201,   125,   125,   186,
      35,   186,   338,    35,   338,   193,   173,   318,   211,   239,
     240,    27,    48,    52,    76,    79,    90,   109,   185,   279,
     280,   281,   282,   283,   266,   304,   146,   173,    34,    50,
      97,   101,   174,   215,   309,   321,   125,   305,   338,   302,
     218,   211,   299,   338,   338,   173,   290,   338,     1,   253,
     319,   173,   309,   321,   146,   171,   173,   173,   315,   173,
     315,   186,   175,   235,   338,   175,   186,   338,   175,   338,
     175,   338,   171,   171,   211,   146,   165,    13,   148,   146,
     165,    13,    37,    73,   165,   172,    21,   249,   313,   170,
       1,    31,   211,   261,   262,   263,    27,    79,    90,   109,
     271,   284,   172,   165,   165,   165,   165,   165,   165,   173,
     175,    48,    90,   146,   173,    17,    20,    25,    46,    53,
      63,    71,    87,    99,   110,   116,   320,    89,    89,    45,
     235,   338,    45,   235,   338,   318,   235,   338,   172,   327,
     327,   165,   290,   340,   319,   211,   257,   165,   211,   211,
     257,   257,   165,   338,   173,   338,    33,   216,    33,   216,
     331,   332,   338,    33,   216,   330,    33,   216,   330,   216,
     216,   146,   165,    13,   165,   338,   338,    35,   186,    35,
      35,   186,   102,   197,    66,   173,   146,   173,    48,    90,
     282,   146,   173,   172,   211,    27,    79,    90,   109,   286,
     173,   304,   308,     1,   313,    69,   340,   211,   173,   173,
     171,    75,   117,   171,   254,   173,   297,   186,   175,   330,
     175,   330,   186,   123,   205,   212,   140,   141,   142,   143,
     144,   159,   163,   168,   170,   276,   338,   111,   338,   199,
     201,   318,   172,   197,   211,   250,     1,   258,   171,     8,
     263,   125,   146,   171,    90,   270,     1,     3,    17,    20,
      25,    40,    46,    53,    63,    70,    71,    87,    99,   103,
     106,   110,   116,   172,   210,   211,   213,   272,   273,   274,
     275,   320,   173,   332,   308,   320,   320,   338,    33,    33,
     338,    33,    33,   173,   175,   175,   318,   216,   216,   257,
     170,   257,   257,   170,   170,   216,   102,    45,   338,    45,
     338,   146,   173,   102,    45,   338,   216,    45,   338,   216,
     276,   338,   338,   186,   186,   338,   186,    35,   211,   165,
     240,   197,   211,   281,   304,   145,   312,    90,   308,   305,
     175,    48,   175,   172,   172,    33,   186,   313,    45,   186,
     338,   175,    45,   186,   338,   175,   338,   199,    13,    37,
      73,    37,    73,   165,   165,   173,   250,   145,   197,   171,
      31,    83,    86,   171,   185,   221,   222,   263,   338,   262,
     286,   172,   277,   338,   140,   148,   277,   277,   305,   102,
      45,    45,   102,    45,    45,    45,    45,   173,   170,   258,
     170,   170,   258,   258,   338,   338,   338,   332,   338,   338,
     338,    13,    35,   186,   165,   312,   173,   174,   215,   290,
     311,   321,   150,   291,   305,    61,   118,   292,   338,   309,
     321,   317,   317,   186,   216,   338,   186,   338,   186,   171,
     111,   338,   199,   201,   199,   201,   165,   173,   338,     8,
     222,   221,     1,   145,   307,   280,   173,     3,   103,   273,
     275,   338,   338,   338,   338,   338,   338,   258,   171,   258,
     258,   171,   171,   102,   102,   102,   102,   338,   186,   291,
     305,   312,   175,   313,   290,   338,     3,    92,   103,   293,
     294,   295,   338,   197,   217,   289,   175,   173,   173,   102,
     102,   165,   165,   165,   165,   197,   222,   174,   215,   306,
     321,   105,   287,   173,   277,   277,   102,   102,   102,   102,
     102,   102,   171,   171,   171,   338,   338,   338,   338,   291,
     305,   290,   310,   311,   321,    48,   175,   295,   118,   146,
     125,   151,   153,   154,   157,   158,    61,   321,   164,   164,
     338,   338,     1,   175,   313,   292,   338,   310,   311,   338,
     294,   295,   295,   295,   295,   295,   295,   293,   175,   306,
     321,   175,   165,   288,   289,   175,   306,   321,   310
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
#line 518 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 523 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 524 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 555 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 556 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 557 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 558 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 559 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 560 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 570 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeprecated((yyvsp[(3) - (3)].pblockstmt), (yyvsp[(2) - (3)].pch)); }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeprecated((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 597 "chapel.ypp"
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

  case 50:
/* Line 1792 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 615 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 620 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 627 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 628 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 629 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 633 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 634 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 638 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 645 "chapel.ypp"
    {
    (yylsp[(1) - (1)]).comment = context->latestComment;
    context->latestComment = NULL;
  }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 650 "chapel.ypp"
    {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[(6) - (7)].pch), (yyvsp[(3) - (7)].b), (yyvsp[(4) - (7)].b), (yylsp[(1) - (7)]).comment);
 }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 665 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 666 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 667 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 673 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 678 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 683 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr((yyvsp[(3) - (3)].pch));
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 710 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 715 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 720 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 725 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 733 "chapel.ypp"
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (4)].ponlylist), (yyvsp[(1) - (4)].b)); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), true, (yyvsp[(1) - (6)].b)); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 758 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), true, (yyvsp[(1) - (8)].b)); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), new UnresolvedSymExpr("_"), (yyvsp[(7) - (8)].ponlylist), true, (yyvsp[(1) - (8)].b)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 760 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (6)].pexpr), "", (yyvsp[(5) - (6)].ponlylist), false, (yyvsp[(1) - (6)].b)); }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 761 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), (yyvsp[(5) - (8)].pexpr), (yyvsp[(7) - (8)].ponlylist), false, (yyvsp[(1) - (8)].b)); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 762 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(3) - (8)].pexpr), new UnresolvedSymExpr("_"), (yyvsp[(7) - (8)].ponlylist), false, (yyvsp[(1) - (8)].b)); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(3) - (4)].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[(1) - (4)].b));}
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
    { std::vector<PotentialRename*>* renames = new std::vector<PotentialRename*>();
                          PotentialRename* nameInMod = new PotentialRename();
                          nameInMod->tag = PotentialRename::SINGLE;
                          nameInMod->elem = new UnresolvedSymExpr((yyvsp[(3) - (3)].pch));
                          renames->push_back(nameInMod);
                          (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (3)].pexpr), renames); }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 779 "chapel.ypp"
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[(1) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist)); }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pimportstmt)); }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 784 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (3)].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[(3) - (3)].pimportstmt)); }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 793 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 795 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 797 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 799 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 801 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 803 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), new SymExpr(gNoInit), "="); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 809 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 810 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 814 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 815 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 816 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 817 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 818 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 819 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 822 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 823 "chapel.ypp"
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 824 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 825 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 826 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 827 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 839 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 840 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 851 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 852 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 853 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 854 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 855 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 856 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 857 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 858 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 859 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 860 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 861 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 862 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 863 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 864 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 866 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 869 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 873 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 875 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 876 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 877 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 880 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 883 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 892 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 894 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 898 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 899 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 903 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 904 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 908 "chapel.ypp"
    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pflagset), (yyvsp[(4) - (4)].pch)); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 909 "chapel.ypp"
    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[(1) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pch)); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 910 "chapel.ypp"
    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[(1) - (1)].pexpr), NULL, NULL); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 914 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->insertAtTail((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 915 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (3)].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 919 "chapel.ypp"
    { (yyval.pblockstmt) = buildManageStmt((yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 925 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeprecated((yyvsp[(3) - (3)].pblockstmt), (yyvsp[(2) - (3)].pch)); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 927 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeprecated((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 944 "chapel.ypp"
    { parsingPrivate=(yyvsp[(2) - (3)].b); applyPrivateToBlock((yyvsp[(3) - (3)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(3) - (3)].pblockstmt); }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 958 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 959 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 960 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 961 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 966 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 971 "chapel.ypp"
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

  case 188:
/* Line 1792 of yacc.c  */
#line 982 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 987 "chapel.ypp"
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

  case 190:
/* Line 1792 of yacc.c  */
#line 999 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1004 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch),
                                             NULL,
                                             AGGREGATE_UNION,
                                             (yyvsp[(5) - (8)].pcallexpr),
                                             (yyvsp[(7) - (8)].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[(2) - (8)]).comment));
    }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1015 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1020 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(5) - (9)].pch),
                                             (yyvsp[(2) - (9)].pch),
                                             AGGREGATE_UNION,
                                             (yyvsp[(6) - (9)].pcallexpr),
                                             (yyvsp[(8) - (9)].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[(3) - (9)]).comment));
    }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1032 "chapel.ypp"
    {
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1037 "chapel.ypp"
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

  case 196:
/* Line 1792 of yacc.c  */
#line 1047 "chapel.ypp"
    {
      (yylsp[(3) - (3)]).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1052 "chapel.ypp"
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

  case 198:
/* Line 1792 of yacc.c  */
#line 1063 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1067 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt));
    }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1072 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1080 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), comment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1091 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1101 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1102 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1103 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), true, false); }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), true, false); }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1112 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1113 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), true, false); }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), true, false); }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1121 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1127 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1133 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1139 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1152 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1159 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1163 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1169 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1173 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1174 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1178 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1179 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1180 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1181 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1183 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1185 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1187 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1194 "chapel.ypp"
    { (yyval.pexpr) = buildIfVar((yyvsp[(2) - (4)].pch), (yyvsp[(4) - (4)].pexpr), false); }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1195 "chapel.ypp"
    { (yyval.pexpr) = buildIfVar((yyvsp[(2) - (4)].pch), (yyvsp[(4) - (4)].pexpr), true);  }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt))); }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1202 "chapel.ypp"
    { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[(2) - (3)].pch), ls, (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[(1) - (1)].pch), INTENT_TYPE); }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1221 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1226 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1227 "chapel.ypp"
    { (yyval.pch) = "bytes"; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1228 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1231 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[(1) - (1)].pch)); }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.pch) = "none"; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1241 "chapel.ypp"
    { (yyval.pch) = "false"; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1242 "chapel.ypp"
    { (yyval.pch) = "true"; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1251 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1252 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1257 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(2) - (6)].pch), (yyvsp[(4) - (6)].pcallexpr), NULL)); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1259 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (4)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (4)].pch), act, NULL)); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1262 "chapel.ypp"
    { (yyvsp[(5) - (7)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (7)].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[(3) - (7)].pch), (yyvsp[(5) - (7)].pcallexpr), NULL)); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1268 "chapel.ypp"
    { (yyval.pexpr) = IfcConstraint::build((yyvsp[(2) - (5)].pch), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1270 "chapel.ypp"
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[(1) - (3)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (3)].pch), act); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyvsp[(5) - (6)].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[(1) - (6)].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pcallexpr)); }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.b) = true; }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build((yyvsp[(1) - (2)].b), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build((yyvsp[(1) - (3)].b), (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1326 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1333 "chapel.ypp"
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

  case 300:
/* Line 1792 of yacc.c  */
#line 1343 "chapel.ypp"
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

  case 301:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1361 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1389 "chapel.ypp"
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

  case 310:
/* Line 1792 of yacc.c  */
#line 1405 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1412 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1421 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1429 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1433 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1441 "chapel.ypp"
    { (yyval.pdefexpr) = buildDeprecated((yyvsp[(3) - (3)].pdefexpr), (yyvsp[(2) - (3)].pch)); }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1443 "chapel.ypp"
    { (yyval.pdefexpr) = buildDeprecated((yyvsp[(2) - (2)].pdefexpr)); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1447 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1448 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1453 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1458 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1463 "chapel.ypp"
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

  case 323:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1488 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1492 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1500 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[(2) - (2)]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1510 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1517 "chapel.ypp"
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

  case 329:
/* Line 1792 of yacc.c  */
#line 1550 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1554 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1559 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1563 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1568 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1586 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1587 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1588 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1592 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1593 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1600 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1602 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pch) = ":"; }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1614 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1615 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1616 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1617 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1618 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1619 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1620 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1621 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1622 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1625 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1635 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1639 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1643 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1644 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1648 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1649 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(1) - (1)].pfnsymbol); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1654 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1658 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1660 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pexpr))); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1664 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1668 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1673 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1674 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1676 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1680 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1685 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1686 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1687 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1688 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1690 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1694 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_PROC; }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_ITER; }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1696 "chapel.ypp"
    { (yyval.procIterOp) = ProcIterOp_OP; }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1703 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1705 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1709 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1713 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 423:
/* Line 1792 of yacc.c  */
#line 1719 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 424:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 425:
/* Line 1792 of yacc.c  */
#line 1727 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1728 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1729 "chapel.ypp"
    { if (DefExpr* def = toDefExpr((yyvsp[(2) - (2)].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
                         }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1737 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1743 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1750 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1756 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1757 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1758 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1759 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1760 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1761 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));}
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1767 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1769 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1783 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1796 "chapel.ypp"
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

  case 449:
/* Line 1792 of yacc.c  */
#line 1812 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1816 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1820 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1821 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1822 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1824 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1829 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1835 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1844 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1852 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1854 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1859 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1861 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1863 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1868 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1870 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1872 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1878 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1879 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1880 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1886 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1888 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1890 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1894 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1898 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1900 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1904 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1911 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1912 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1913 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1914 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1915 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1920 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1921 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1922 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1923 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1924 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1945 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1949 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1953 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1961 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1967 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1968 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1969 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1974 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1976 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1982 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1984 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1986 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1990 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1992 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1994 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 2000 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 2001 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 2002 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 509:
/* Line 1792 of yacc.c  */
#line 2003 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 510:
/* Line 1792 of yacc.c  */
#line 2007 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 511:
/* Line 1792 of yacc.c  */
#line 2008 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 512:
/* Line 1792 of yacc.c  */
#line 2012 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 2013 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 2014 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 2018 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 2019 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 2023 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 2028 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 2029 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 2033 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 2034 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 2035 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 2036 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 2040 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 2041 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 2053 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 2055 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 529:
/* Line 1792 of yacc.c  */
#line 2057 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 2070 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 2072 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 2076 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeTypeForParentDomain", (yyvsp[(4) - (5)].pexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 2078 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 2080 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 2085 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 2087 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 2089 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 2091 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 2093 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 2095 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 2097 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 2100 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 2102 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 2107 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 2109 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 2111 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 2113 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 2115 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 2117 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 2119 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 2121 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 2123 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2125 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2127 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2129 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2131 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2138 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2144 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2150 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2156 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2165 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2174 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2190 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2194 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2199 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2203 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2204 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2208 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2212 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2213 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2218 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2222 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 589:
/* Line 1792 of yacc.c  */
#line 2226 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 590:
/* Line 1792 of yacc.c  */
#line 2232 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 591:
/* Line 1792 of yacc.c  */
#line 2233 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 592:
/* Line 1792 of yacc.c  */
#line 2234 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 593:
/* Line 1792 of yacc.c  */
#line 2235 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 594:
/* Line 1792 of yacc.c  */
#line 2236 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 595:
/* Line 1792 of yacc.c  */
#line 2237 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 597:
/* Line 1792 of yacc.c  */
#line 2243 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 598:
/* Line 1792 of yacc.c  */
#line 2248 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
    break;

  case 599:
/* Line 1792 of yacc.c  */
#line 2250 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
    break;

  case 600:
/* Line 1792 of yacc.c  */
#line 2254 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
    break;

  case 601:
/* Line 1792 of yacc.c  */
#line 2258 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
    break;

  case 602:
/* Line 1792 of yacc.c  */
#line 2262 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
    break;

  case 603:
/* Line 1792 of yacc.c  */
#line 2270 "chapel.ypp"
    { (yyvsp[(1) - (2)].pcallexpr)->insertAtTail((yyvsp[(2) - (2)].pexpr));
      (yyval.pexpr) = (yyvsp[(1) - (2)].pcallexpr); }
    break;

  case 604:
/* Line 1792 of yacc.c  */
#line 2275 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 605:
/* Line 1792 of yacc.c  */
#line 2281 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[(4) - (8)].pexpr), (yyvsp[(7) - (8)].pcallexpr)));
    }
    break;

  case 606:
/* Line 1792 of yacc.c  */
#line 2287 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 607:
/* Line 1792 of yacc.c  */
#line 2294 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[(4) - (9)].pexpr), (yyvsp[(7) - (9)].pcallexpr))));
    }
    break;

  case 608:
/* Line 1792 of yacc.c  */
#line 2304 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 618:
/* Line 1792 of yacc.c  */
#line 2321 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 619:
/* Line 1792 of yacc.c  */
#line 2323 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 620:
/* Line 1792 of yacc.c  */
#line 2325 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 621:
/* Line 1792 of yacc.c  */
#line 2327 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), false, true); }
    break;

  case 622:
/* Line 1792 of yacc.c  */
#line 2342 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 623:
/* Line 1792 of yacc.c  */
#line 2344 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 624:
/* Line 1792 of yacc.c  */
#line 2346 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[(2) - (2)].pexpr), true); }
    break;

  case 625:
/* Line 1792 of yacc.c  */
#line 2348 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
    break;

  case 626:
/* Line 1792 of yacc.c  */
#line 2352 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 627:
/* Line 1792 of yacc.c  */
#line 2353 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 628:
/* Line 1792 of yacc.c  */
#line 2357 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 629:
/* Line 1792 of yacc.c  */
#line 2358 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 630:
/* Line 1792 of yacc.c  */
#line 2359 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 635:
/* Line 1792 of yacc.c  */
#line 2375 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 636:
/* Line 1792 of yacc.c  */
#line 2376 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 637:
/* Line 1792 of yacc.c  */
#line 2377 "chapel.ypp"
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[(1) - (2)].pexpr));}
    break;

  case 638:
/* Line 1792 of yacc.c  */
#line 2378 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 640:
/* Line 1792 of yacc.c  */
#line 2383 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 641:
/* Line 1792 of yacc.c  */
#line 2384 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 642:
/* Line 1792 of yacc.c  */
#line 2385 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 643:
/* Line 1792 of yacc.c  */
#line 2389 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 644:
/* Line 1792 of yacc.c  */
#line 2390 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 645:
/* Line 1792 of yacc.c  */
#line 2391 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 646:
/* Line 1792 of yacc.c  */
#line 2392 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 647:
/* Line 1792 of yacc.c  */
#line 2393 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[(1) - (5)].pexpr), "chpl_bytes")); }
    break;

  case 648:
/* Line 1792 of yacc.c  */
#line 2401 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 649:
/* Line 1792 of yacc.c  */
#line 2402 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 650:
/* Line 1792 of yacc.c  */
#line 2403 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 651:
/* Line 1792 of yacc.c  */
#line 2404 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 652:
/* Line 1792 of yacc.c  */
#line 2408 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 653:
/* Line 1792 of yacc.c  */
#line 2409 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 654:
/* Line 1792 of yacc.c  */
#line 2413 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 655:
/* Line 1792 of yacc.c  */
#line 2414 "chapel.ypp"
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 658:
/* Line 1792 of yacc.c  */
#line 2420 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 659:
/* Line 1792 of yacc.c  */
#line 2421 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 660:
/* Line 1792 of yacc.c  */
#line 2422 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 661:
/* Line 1792 of yacc.c  */
#line 2423 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 662:
/* Line 1792 of yacc.c  */
#line 2424 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 663:
/* Line 1792 of yacc.c  */
#line 2425 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr),
                                            new SymExpr(gTrue)); }
    break;

  case 664:
/* Line 1792 of yacc.c  */
#line 2427 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr),
                                                   new SymExpr(gTrue)); }
    break;

  case 665:
/* Line 1792 of yacc.c  */
#line 2429 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 666:
/* Line 1792 of yacc.c  */
#line 2430 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 667:
/* Line 1792 of yacc.c  */
#line 2432 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 668:
/* Line 1792 of yacc.c  */
#line 2436 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 669:
/* Line 1792 of yacc.c  */
#line 2443 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 670:
/* Line 1792 of yacc.c  */
#line 2444 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 671:
/* Line 1792 of yacc.c  */
#line 2448 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 672:
/* Line 1792 of yacc.c  */
#line 2449 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 673:
/* Line 1792 of yacc.c  */
#line 2450 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 674:
/* Line 1792 of yacc.c  */
#line 2451 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 675:
/* Line 1792 of yacc.c  */
#line 2452 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 676:
/* Line 1792 of yacc.c  */
#line 2453 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 677:
/* Line 1792 of yacc.c  */
#line 2454 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 678:
/* Line 1792 of yacc.c  */
#line 2455 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 679:
/* Line 1792 of yacc.c  */
#line 2456 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 680:
/* Line 1792 of yacc.c  */
#line 2457 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 681:
/* Line 1792 of yacc.c  */
#line 2458 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 682:
/* Line 1792 of yacc.c  */
#line 2459 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 683:
/* Line 1792 of yacc.c  */
#line 2460 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 684:
/* Line 1792 of yacc.c  */
#line 2461 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 685:
/* Line 1792 of yacc.c  */
#line 2462 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 686:
/* Line 1792 of yacc.c  */
#line 2463 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 687:
/* Line 1792 of yacc.c  */
#line 2464 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 688:
/* Line 1792 of yacc.c  */
#line 2465 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 689:
/* Line 1792 of yacc.c  */
#line 2466 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 690:
/* Line 1792 of yacc.c  */
#line 2467 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 691:
/* Line 1792 of yacc.c  */
#line 2468 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 692:
/* Line 1792 of yacc.c  */
#line 2469 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 693:
/* Line 1792 of yacc.c  */
#line 2470 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr),
                                               new SymExpr(gTrue)); }
    break;

  case 694:
/* Line 1792 of yacc.c  */
#line 2475 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 695:
/* Line 1792 of yacc.c  */
#line 2476 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 696:
/* Line 1792 of yacc.c  */
#line 2477 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 697:
/* Line 1792 of yacc.c  */
#line 2478 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 698:
/* Line 1792 of yacc.c  */
#line 2479 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 699:
/* Line 1792 of yacc.c  */
#line 2480 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 700:
/* Line 1792 of yacc.c  */
#line 2481 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 701:
/* Line 1792 of yacc.c  */
#line 2485 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 702:
/* Line 1792 of yacc.c  */
#line 2486 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 703:
/* Line 1792 of yacc.c  */
#line 2487 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 704:
/* Line 1792 of yacc.c  */
#line 2488 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 705:
/* Line 1792 of yacc.c  */
#line 2492 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 706:
/* Line 1792 of yacc.c  */
#line 2493 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 707:
/* Line 1792 of yacc.c  */
#line 2494 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 708:
/* Line 1792 of yacc.c  */
#line 2495 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 709:
/* Line 1792 of yacc.c  */
#line 2500 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 710:
/* Line 1792 of yacc.c  */
#line 2501 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 711:
/* Line 1792 of yacc.c  */
#line 2502 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 712:
/* Line 1792 of yacc.c  */
#line 2503 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 713:
/* Line 1792 of yacc.c  */
#line 2504 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 714:
/* Line 1792 of yacc.c  */
#line 2505 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 715:
/* Line 1792 of yacc.c  */
#line 2506 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 11538 "bison-chapel.cpp"
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


