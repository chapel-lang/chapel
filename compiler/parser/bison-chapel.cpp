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
#line 32 "chapel.ypp"

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;

/* Line 387 of yacc.c  */
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

/* Line 387 of yacc.c  */
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
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<OnlyRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif

/* Line 387 of yacc.c  */
#line 145 "chapel.ypp"

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
#line 168 "chapel.ypp"

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
#line 250 "bison-chapel.cpp"

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
     TBOOL = 270,
     TBORROWED = 271,
     TBY = 272,
     TCATCH = 273,
     TCLASS = 274,
     TCOBEGIN = 275,
     TCOFORALL = 276,
     TCOMPLEX = 277,
     TCONFIG = 278,
     TCONST = 279,
     TCONTINUE = 280,
     TDEFER = 281,
     TDELETE = 282,
     TDMAPPED = 283,
     TDO = 284,
     TDOMAIN = 285,
     TELSE = 286,
     TENUM = 287,
     TEXCEPT = 288,
     TEXPORT = 289,
     TEXTERN = 290,
     TFALSE = 291,
     TFOR = 292,
     TFORALL = 293,
     TFORWARDING = 294,
     TIF = 295,
     TIMAG = 296,
     TIN = 297,
     TINDEX = 298,
     TINLINE = 299,
     TINOUT = 300,
     TINT = 301,
     TITER = 302,
     TINITEQUALS = 303,
     TLABEL = 304,
     TLAMBDA = 305,
     TLET = 306,
     TLIFETIME = 307,
     TLOCAL = 308,
     TLOCALE = 309,
     TMINUSMINUS = 310,
     TMODULE = 311,
     TNEW = 312,
     TNIL = 313,
     TNOINIT = 314,
     TNONE = 315,
     TNOTHING = 316,
     TON = 317,
     TONLY = 318,
     TOTHERWISE = 319,
     TOUT = 320,
     TOVERRIDE = 321,
     TOWNED = 322,
     TPARAM = 323,
     TPLUSPLUS = 324,
     TPRAGMA = 325,
     TPRIMITIVE = 326,
     TPRIVATE = 327,
     TPROC = 328,
     TPROTOTYPE = 329,
     TPUBLIC = 330,
     TREAL = 331,
     TRECORD = 332,
     TREDUCE = 333,
     TREF = 334,
     TREQUIRE = 335,
     TRETURN = 336,
     TSCAN = 337,
     TSELECT = 338,
     TSERIAL = 339,
     TSHARED = 340,
     TSINGLE = 341,
     TSPARSE = 342,
     TSTRING = 343,
     TSUBDOMAIN = 344,
     TSYNC = 345,
     TTHEN = 346,
     TTHIS = 347,
     TTHROW = 348,
     TTHROWS = 349,
     TTRUE = 350,
     TTRY = 351,
     TTRYBANG = 352,
     TTYPE = 353,
     TUINT = 354,
     TUNDERSCORE = 355,
     TUNION = 356,
     TUNMANAGED = 357,
     TUSE = 358,
     TVAR = 359,
     TVOID = 360,
     TWHEN = 361,
     TWHERE = 362,
     TWHILE = 363,
     TWITH = 364,
     TYIELD = 365,
     TZIP = 366,
     TALIAS = 367,
     TAND = 368,
     TASSIGN = 369,
     TASSIGNBAND = 370,
     TASSIGNBOR = 371,
     TASSIGNBXOR = 372,
     TASSIGNDIVIDE = 373,
     TASSIGNEXP = 374,
     TASSIGNLAND = 375,
     TASSIGNLOR = 376,
     TASSIGNMINUS = 377,
     TASSIGNMOD = 378,
     TASSIGNMULTIPLY = 379,
     TASSIGNPLUS = 380,
     TASSIGNREDUCE = 381,
     TASSIGNSL = 382,
     TASSIGNSR = 383,
     TBANG = 384,
     TBAND = 385,
     TBNOT = 386,
     TBOR = 387,
     TBXOR = 388,
     TCOLON = 389,
     TCOMMA = 390,
     TDIVIDE = 391,
     TDOT = 392,
     TDOTDOT = 393,
     TDOTDOTDOT = 394,
     TEQUAL = 395,
     TEXP = 396,
     TGREATER = 397,
     TGREATEREQUAL = 398,
     THASH = 399,
     TIO = 400,
     TLESS = 401,
     TLESSEQUAL = 402,
     TMINUS = 403,
     TMOD = 404,
     TNOTEQUAL = 405,
     TOR = 406,
     TPLUS = 407,
     TQUESTION = 408,
     TSEMI = 409,
     TSHIFTLEFT = 410,
     TSHIFTRIGHT = 411,
     TSTAR = 412,
     TSWAP = 413,
     TLCBR = 414,
     TRCBR = 415,
     TLP = 416,
     TRP = 417,
     TLSBR = 418,
     TRSBR = 419,
     TNOELSE = 420,
     TUMINUS = 421,
     TUPLUS = 422
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
#line 199 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 387 of yacc.c  */
#line 482 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 489 "bison-chapel.cpp"
/* Unqualified %code blocks.  */
/* Line 391 of yacc.c  */
#line 38 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;

/* Line 391 of yacc.c  */
#line 207 "chapel.ypp"

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
#line 568 "bison-chapel.cpp"

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
#define YYLAST   16453

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  168
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  139
/* YYNRULES -- Number of rules.  */
#define YYNRULES  588
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1045

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   422

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
     165,   166,   167
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
     112,   116,   121,   126,   127,   129,   131,   132,   134,   137,
     141,   145,   147,   150,   152,   156,   160,   166,   167,   169,
     171,   173,   177,   183,   189,   193,   198,   203,   208,   213,
     218,   219,   221,   223,   225,   227,   229,   231,   233,   235,
     237,   239,   241,   243,   245,   247,   249,   251,   253,   255,
     257,   259,   261,   263,   265,   267,   269,   271,   273,   275,
     277,   279,   281,   283,   285,   287,   289,   291,   293,   295,
     297,   299,   301,   303,   305,   307,   309,   310,   312,   315,
     317,   320,   324,   326,   328,   331,   334,   336,   338,   340,
     342,   344,   346,   350,   356,   362,   365,   368,   374,   378,
     385,   392,   397,   403,   409,   413,   417,   424,   430,   437,
     443,   450,   454,   459,   463,   468,   475,   483,   490,   498,
     503,   509,   514,   520,   525,   530,   534,   541,   547,   554,
     560,   569,   577,   580,   584,   588,   591,   594,   598,   602,
     603,   606,   609,   613,   619,   621,   625,   629,   635,   641,
     642,   645,   649,   652,   656,   663,   672,   679,   688,   690,
     692,   694,   695,   698,   699,   702,   706,   712,   718,   720,
     722,   725,   729,   731,   735,   736,   737,   746,   747,   749,
     752,   755,   757,   758,   759,   770,   774,   778,   784,   790,
     794,   796,   800,   802,   805,   807,   809,   811,   813,   815,
     817,   819,   821,   823,   825,   827,   829,   831,   833,   835,
     837,   839,   841,   843,   845,   847,   849,   851,   853,   855,
     858,   860,   862,   864,   866,   868,   870,   872,   874,   876,
     878,   880,   882,   883,   887,   891,   892,   894,   898,   903,
     909,   914,   921,   928,   929,   931,   933,   935,   937,   939,
     942,   945,   947,   949,   951,   952,   954,   956,   959,   961,
     963,   965,   967,   968,   970,   973,   975,   977,   979,   980,
     982,   984,   986,   988,   990,   993,   995,   996,   998,  1001,
    1004,  1005,  1008,  1011,  1016,  1021,  1023,  1027,  1031,  1035,
    1039,  1043,  1047,  1051,  1054,  1056,  1058,  1062,  1067,  1072,
    1075,  1080,  1081,  1084,  1087,  1089,  1091,  1093,  1096,  1098,
    1103,  1109,  1113,  1115,  1119,  1123,  1129,  1131,  1133,  1137,
    1139,  1142,  1146,  1147,  1150,  1153,  1157,  1160,  1165,  1169,
    1173,  1178,  1182,  1183,  1186,  1189,  1192,  1194,  1195,  1198,
    1201,  1204,  1206,  1211,  1216,  1223,  1227,  1228,  1230,  1232,
    1236,  1241,  1245,  1250,  1257,  1258,  1261,  1264,  1267,  1270,
    1272,  1274,  1278,  1282,  1284,  1288,  1290,  1292,  1294,  1298,
    1302,  1303,  1305,  1307,  1311,  1315,  1319,  1321,  1323,  1325,
    1327,  1329,  1332,  1334,  1336,  1338,  1340,  1342,  1345,  1350,
    1355,  1360,  1366,  1369,  1372,  1374,  1377,  1379,  1382,  1384,
    1387,  1389,  1392,  1399,  1406,  1411,  1421,  1431,  1439,  1446,
    1453,  1458,  1468,  1478,  1486,  1491,  1498,  1505,  1515,  1525,
    1532,  1534,  1536,  1538,  1540,  1542,  1544,  1546,  1548,  1552,
    1553,  1555,  1560,  1562,  1566,  1571,  1573,  1577,  1582,  1586,
    1590,  1592,  1594,  1597,  1599,  1602,  1604,  1606,  1610,  1613,
    1618,  1620,  1622,  1624,  1626,  1628,  1630,  1632,  1634,  1639,
    1643,  1647,  1650,  1653,  1655,  1656,  1658,  1661,  1664,  1666,
    1668,  1670,  1672,  1674,  1676,  1679,  1681,  1686,  1691,  1696,
    1700,  1704,  1708,  1712,  1716,  1721,  1725,  1730,  1732,  1734,
    1736,  1738,  1740,  1742,  1744,  1746,  1748,  1752,  1757,  1761,
    1766,  1770,  1775,  1779,  1785,  1789,  1793,  1797,  1801,  1805,
    1809,  1813,  1817,  1821,  1825,  1829,  1833,  1837,  1841,  1845,
    1849,  1853,  1857,  1861,  1865,  1869,  1873,  1877,  1880,  1883,
    1886,  1889,  1892,  1895,  1898,  1902,  1906,  1910,  1914,  1918,
    1922,  1926,  1930,  1932,  1934,  1936,  1938,  1940,  1942
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     169,     0,    -1,   170,    -1,    -1,   170,   171,    -1,   173,
      -1,   172,   173,    -1,    70,     7,    -1,   172,    70,     7,
      -1,   175,    -1,   178,    -1,   183,    -1,   184,    -1,   196,
      -1,   185,    -1,   199,    -1,   202,    -1,   200,    -1,   209,
      -1,   203,    -1,   204,    -1,   208,    -1,   195,    -1,   280,
     154,    -1,    12,   173,    -1,    13,   281,   173,    -1,    14,
     186,   154,    -1,    20,   281,   178,    -1,    25,   186,   154,
      -1,    27,   268,   154,    -1,    49,   188,   173,    -1,    53,
     291,   194,    -1,    53,   194,    -1,    62,   291,   194,    -1,
      84,   291,   194,    -1,    84,   194,    -1,    90,   173,    -1,
     110,   291,   154,    -1,     1,   154,    -1,   176,   177,    56,
     188,    -1,   174,   159,   160,    -1,   174,   159,   179,   160,
      -1,   174,   159,     1,   160,    -1,    -1,    75,    -1,    72,
      -1,    -1,    74,    -1,   159,   160,    -1,   159,   179,   160,
      -1,   159,     1,   160,    -1,   171,    -1,   179,   171,    -1,
     291,    -1,   291,    11,   291,    -1,   180,   135,   291,    -1,
     180,   135,   291,    11,   291,    -1,    -1,   180,    -1,   157,
      -1,   180,    -1,   103,   267,   154,    -1,   103,   291,    33,
     182,   154,    -1,   103,   291,    63,   181,   154,    -1,    80,
     267,   154,    -1,   294,   230,   293,   154,    -1,   294,   158,
     293,   154,    -1,   294,   126,   293,   154,    -1,   294,   120,
     293,   154,    -1,   294,   121,   293,   154,    -1,    -1,     3,
      -1,     3,    -1,    92,    -1,    36,    -1,    95,    -1,   190,
      -1,     3,    -1,    92,    -1,    36,    -1,    95,    -1,   190,
      -1,     3,    -1,    92,    -1,    15,    -1,    46,    -1,    99,
      -1,    76,    -1,    41,    -1,    22,    -1,    88,    -1,    90,
      -1,    86,    -1,    67,    -1,    85,    -1,    16,    -1,   102,
      -1,    30,    -1,    43,    -1,    54,    -1,    61,    -1,   105,
      -1,    15,    -1,    46,    -1,    99,    -1,    76,    -1,    41,
      -1,    22,    -1,    88,    -1,    54,    -1,    61,    -1,   105,
      -1,    90,    -1,    86,    -1,    30,    -1,    43,    -1,    -1,
       7,    -1,    29,   173,    -1,   178,    -1,    81,   154,    -1,
      81,   293,   154,    -1,   154,    -1,   197,    -1,    75,   197,
      -1,    72,   197,    -1,   216,    -1,   250,    -1,   212,    -1,
     198,    -1,   224,    -1,   254,    -1,    39,   291,   154,    -1,
      39,   291,    33,   182,   154,    -1,    39,   291,    63,   181,
     154,    -1,    39,   254,    -1,    35,     9,    -1,    29,   173,
     108,   291,   154,    -1,   108,   291,   194,    -1,    21,   291,
      42,   291,   281,   194,    -1,    21,   291,    42,   201,   281,
     194,    -1,    21,   291,   281,   194,    -1,    37,   291,    42,
     291,   194,    -1,    37,   291,    42,   201,   194,    -1,    37,
     291,   194,    -1,    37,   201,   194,    -1,    37,    68,   188,
      42,   291,   194,    -1,    38,   291,    42,   291,   194,    -1,
      38,   291,    42,   291,   284,   194,    -1,    38,   291,    42,
     201,   194,    -1,    38,   291,    42,   201,   284,   194,    -1,
      38,   291,   194,    -1,    38,   291,   284,   194,    -1,    38,
     201,   194,    -1,    38,   201,   284,   194,    -1,   163,   267,
      42,   291,   164,   173,    -1,   163,   267,    42,   291,   284,
     164,   173,    -1,   163,   267,    42,   201,   164,   173,    -1,
     163,   267,    42,   201,   284,   164,   173,    -1,   163,   267,
     164,   173,    -1,   163,   267,   284,   164,   173,    -1,   163,
     201,   164,   173,    -1,   163,   201,   284,   164,   173,    -1,
     111,   161,   267,   162,    -1,    40,   291,    91,   173,    -1,
      40,   291,   178,    -1,    40,   291,    91,   173,    31,   173,
      -1,    40,   291,   178,    31,   173,    -1,    40,   291,   230,
     291,    91,   173,    -1,    40,   291,   230,   291,   178,    -1,
      40,   291,   230,   291,    91,   173,    31,   173,    -1,    40,
     291,   230,   291,   178,    31,   173,    -1,    26,   173,    -1,
      96,   291,   154,    -1,    97,   291,   154,    -1,    96,   185,
      -1,    97,   185,    -1,    96,   178,   205,    -1,    97,   178,
     205,    -1,    -1,   205,   206,    -1,    18,   178,    -1,    18,
     207,   178,    -1,    18,   161,   207,   162,   178,    -1,   188,
      -1,   188,   134,   291,    -1,    93,   291,   154,    -1,    83,
     291,   159,   210,   160,    -1,    83,   291,   159,     1,   160,
      -1,    -1,   210,   211,    -1,   106,   267,   194,    -1,    64,
     173,    -1,    64,    29,   173,    -1,   213,   188,   214,   159,
     215,   160,    -1,    35,   193,   213,   188,   214,   159,   215,
     160,    -1,   213,   188,   214,   159,     1,   160,    -1,    35,
     193,   213,   188,   214,   159,     1,   160,    -1,    19,    -1,
      77,    -1,   101,    -1,    -1,   134,   267,    -1,    -1,   215,
     196,    -1,   215,   172,   196,    -1,   217,   188,   159,   218,
     160,    -1,   217,   188,   159,     1,   160,    -1,    32,    -1,
     219,    -1,   218,   135,    -1,   218,   135,   219,    -1,   188,
      -1,   188,   114,   291,    -1,    -1,    -1,    50,   221,   232,
     222,   239,   262,   246,   242,    -1,    -1,    44,    -1,    34,
     292,    -1,    35,   292,    -1,    66,    -1,    -1,    -1,   223,
     238,   225,   227,   226,   239,   261,   240,   246,   241,    -1,
     237,   229,   231,    -1,   237,   230,   231,    -1,   237,   228,
     137,   229,   231,    -1,   237,   228,   137,   230,   231,    -1,
     237,     1,   231,    -1,   274,    -1,   161,   291,   162,    -1,
     187,    -1,   131,   188,    -1,   130,    -1,   132,    -1,   133,
      -1,   131,    -1,   140,    -1,   150,    -1,   147,    -1,   143,
      -1,   146,    -1,   142,    -1,   152,    -1,   148,    -1,   157,
      -1,   136,    -1,   155,    -1,   156,    -1,   149,    -1,   141,
      -1,   129,    -1,    17,    -1,   144,    -1,    10,    -1,   158,
      -1,   145,    -1,    48,    -1,   188,   129,    -1,   114,    -1,
     125,    -1,   122,    -1,   124,    -1,   118,    -1,   123,    -1,
     119,    -1,   115,    -1,   116,    -1,   117,    -1,   128,    -1,
     127,    -1,    -1,   161,   233,   162,    -1,   161,   233,   162,
      -1,    -1,   234,    -1,   233,   135,   234,    -1,   235,   188,
     266,   259,    -1,   172,   235,   188,   266,   259,    -1,   235,
     188,   266,   245,    -1,   235,   161,   258,   162,   266,   259,
      -1,   235,   161,   258,   162,   266,   245,    -1,    -1,   236,
      -1,    42,    -1,    45,    -1,    65,    -1,    24,    -1,    24,
      42,    -1,    24,    79,    -1,    68,    -1,    79,    -1,    98,
      -1,    -1,    68,    -1,    79,    -1,    24,    79,    -1,    24,
      -1,    98,    -1,    73,    -1,    47,    -1,    -1,    24,    -1,
      24,    79,    -1,    79,    -1,    68,    -1,    98,    -1,    -1,
      94,    -1,   154,    -1,   242,    -1,   178,    -1,   195,    -1,
     153,   188,    -1,   153,    -1,    -1,   243,    -1,   139,   291,
      -1,   139,   244,    -1,    -1,   107,   291,    -1,    52,   247,
      -1,   107,   291,    52,   247,    -1,    52,   247,   107,   291,
      -1,   248,    -1,   247,   135,   248,    -1,   249,   114,   249,
      -1,   249,   146,   249,    -1,   249,   147,   249,    -1,   249,
     140,   249,    -1,   249,   142,   249,    -1,   249,   143,   249,
      -1,    81,   249,    -1,     3,    -1,    92,    -1,    98,   251,
     154,    -1,    23,    98,   251,   154,    -1,    35,    98,   251,
     154,    -1,   188,   252,    -1,   188,   252,   135,   251,    -1,
      -1,   114,   275,    -1,   114,   263,    -1,    68,    -1,    24,
      -1,    79,    -1,    24,    79,    -1,   104,    -1,    23,   253,
     255,   154,    -1,    35,   292,   253,   255,   154,    -1,   253,
     255,   154,    -1,   256,    -1,   255,   135,   256,    -1,   188,
     262,   259,    -1,   161,   258,   162,   262,   259,    -1,   100,
      -1,   188,    -1,   161,   258,   162,    -1,   257,    -1,   257,
     135,    -1,   257,   135,   258,    -1,    -1,   114,    59,    -1,
     114,   293,    -1,   163,   164,   275,    -1,   163,   164,    -1,
     163,   267,   164,   275,    -1,   163,   267,   164,    -1,   163,
     164,   260,    -1,   163,   267,   164,   260,    -1,   163,     1,
     164,    -1,    -1,   134,   275,    -1,   134,   260,    -1,   134,
     192,    -1,     1,    -1,    -1,   134,   275,    -1,   134,   263,
      -1,   134,   192,    -1,     1,    -1,   163,   267,   164,   275,
      -1,   163,   267,   164,   263,    -1,   163,   267,    42,   291,
     164,   275,    -1,   163,     1,   164,    -1,    -1,   275,    -1,
     243,    -1,   163,   164,   264,    -1,   163,   267,   164,   264,
      -1,   163,   164,   265,    -1,   163,   267,   164,   265,    -1,
     163,   267,    42,   291,   164,   264,    -1,    -1,   134,   275,
      -1,   134,   243,    -1,   134,   192,    -1,   134,   265,    -1,
     291,    -1,   243,    -1,   267,   135,   291,    -1,   267,   135,
     243,    -1,   291,    -1,   268,   135,   291,    -1,   100,    -1,
     293,    -1,   243,    -1,   269,   135,   269,    -1,   270,   135,
     269,    -1,    -1,   272,    -1,   273,    -1,   272,   135,   273,
      -1,   189,   114,   243,    -1,   189,   114,   293,    -1,   243,
      -1,   293,    -1,   189,    -1,   191,    -1,   276,    -1,   276,
     153,    -1,   279,    -1,   294,    -1,   278,    -1,   303,    -1,
     302,    -1,    86,   291,    -1,    43,   161,   271,   162,    -1,
      30,   161,   271,   162,    -1,    89,   161,   271,   162,    -1,
      87,    89,   161,   271,   162,    -1,    12,   291,    -1,    90,
     291,    -1,    67,    -1,    67,   291,    -1,   102,    -1,   102,
     291,    -1,    85,    -1,    85,   291,    -1,    16,    -1,    16,
     291,    -1,    37,   291,    42,   291,    29,   291,    -1,    37,
     291,    42,   201,    29,   291,    -1,    37,   291,    29,   291,
      -1,    37,   291,    42,   291,    29,    40,   291,    91,   291,
      -1,    37,   291,    42,   201,    29,    40,   291,    91,   291,
      -1,    37,   291,    29,    40,   291,    91,   291,    -1,    38,
     291,    42,   291,    29,   291,    -1,    38,   291,    42,   201,
      29,   291,    -1,    38,   291,    29,   291,    -1,    38,   291,
      42,   291,    29,    40,   291,    91,   291,    -1,    38,   291,
      42,   201,    29,    40,   291,    91,   291,    -1,    38,   291,
      29,    40,   291,    91,   291,    -1,   163,   267,   164,   291,
      -1,   163,   267,    42,   291,   164,   291,    -1,   163,   267,
      42,   201,   164,   291,    -1,   163,   267,    42,   291,   164,
      40,   291,    91,   291,    -1,   163,   267,    42,   201,   164,
      40,   291,    91,   291,    -1,    40,   291,    91,   291,    31,
     291,    -1,    58,    -1,   279,    -1,   274,    -1,   297,    -1,
     296,    -1,   220,    -1,   289,    -1,   290,    -1,   288,   145,
     291,    -1,    -1,   282,    -1,   109,   161,   283,   162,    -1,
     286,    -1,   283,   135,   286,    -1,   109,   161,   285,   162,
      -1,   286,    -1,   285,   135,   286,    -1,   287,   274,   262,
     259,    -1,   306,    78,   274,    -1,   291,    78,   274,    -1,
      24,    -1,    42,    -1,    24,    42,    -1,    79,    -1,    24,
      79,    -1,   104,    -1,   294,    -1,   288,   145,   291,    -1,
      57,   291,    -1,    51,   255,    42,   291,    -1,   300,    -1,
     275,    -1,   277,    -1,   304,    -1,   305,    -1,   220,    -1,
     289,    -1,   290,    -1,   161,   139,   291,   162,    -1,   291,
     134,   291,    -1,   291,   138,   291,    -1,   291,   138,    -1,
     138,   291,    -1,   138,    -1,    -1,   291,    -1,    96,   291,
      -1,    97,   291,    -1,   291,    -1,   274,    -1,   296,    -1,
     297,    -1,   298,    -1,   294,    -1,   291,   129,    -1,   220,
      -1,   295,   161,   271,   162,    -1,   295,   163,   271,   164,
      -1,    71,   161,   271,   162,    -1,   291,   137,   189,    -1,
     291,   137,    98,    -1,   291,   137,    30,    -1,   291,   137,
      54,    -1,   161,   269,   162,    -1,   161,   269,   135,   162,
      -1,   161,   270,   162,    -1,   161,   270,   135,   162,    -1,
      36,    -1,    95,    -1,   299,    -1,     4,    -1,     5,    -1,
       6,    -1,     7,    -1,     8,    -1,    60,    -1,   159,   267,
     160,    -1,   159,   267,   135,   160,    -1,   163,   267,   164,
      -1,   163,   267,   135,   164,    -1,   163,   301,   164,    -1,
     163,   301,   135,   164,    -1,   291,   112,   291,    -1,   301,
     135,   291,   112,   291,    -1,   291,   152,   291,    -1,   291,
     148,   291,    -1,   291,   157,   291,    -1,   291,   136,   291,
      -1,   291,   155,   291,    -1,   291,   156,   291,    -1,   291,
     149,   291,    -1,   291,   140,   291,    -1,   291,   150,   291,
      -1,   291,   147,   291,    -1,   291,   143,   291,    -1,   291,
     146,   291,    -1,   291,   142,   291,    -1,   291,   130,   291,
      -1,   291,   132,   291,    -1,   291,   133,   291,    -1,   291,
     113,   291,    -1,   291,   151,   291,    -1,   291,   141,   291,
      -1,   291,    17,   291,    -1,   291,    10,   291,    -1,   291,
     144,   291,    -1,   291,    28,   291,    -1,   152,   291,    -1,
     148,   291,    -1,    55,   291,    -1,    69,   291,    -1,   129,
     291,    -1,   291,   129,    -1,   131,   291,    -1,   291,    78,
     291,    -1,   291,    78,   201,    -1,   306,    78,   291,    -1,
     306,    78,   201,    -1,   291,    82,   291,    -1,   291,    82,
     201,    -1,   306,    82,   291,    -1,   306,    82,   201,    -1,
     152,    -1,   157,    -1,   113,    -1,   151,    -1,   130,    -1,
     132,    -1,   133,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   488,   488,   493,   494,   500,   501,   506,   507,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,   545,
     558,   563,   568,   576,   577,   578,   582,   583,   596,   597,
     598,   603,   604,   609,   614,   619,   623,   630,   635,   639,
     644,   648,   649,   650,   654,   658,   660,   662,   664,   666,
     673,   674,   678,   679,   680,   681,   682,   685,   686,   687,
     688,   689,   701,   702,   713,   714,   715,   716,   717,   718,
     719,   720,   721,   722,   723,   724,   725,   726,   727,   728,
     729,   730,   734,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   750,   751,   756,   757,   762,   763,   767,   768,
     772,   773,   777,   778,   779,   780,   781,   782,   783,   784,
     788,   789,   793,   794,   795,   796,   800,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,   817,   818,   819,
     820,   821,   822,   823,   824,   825,   831,   837,   843,   849,
     856,   863,   867,   874,   878,   879,   880,   881,   882,   884,
     886,   888,   893,   896,   897,   898,   899,   900,   901,   905,
     906,   910,   911,   912,   916,   917,   921,   924,   926,   931,
     932,   936,   938,   940,   947,   957,   967,   977,   990,   995,
    1000,  1008,  1009,  1014,  1015,  1017,  1022,  1038,  1045,  1054,
    1062,  1066,  1073,  1074,  1079,  1084,  1078,  1111,  1117,  1124,
    1130,  1136,  1147,  1153,  1146,  1185,  1189,  1194,  1198,  1203,
    1210,  1211,  1215,  1216,  1217,  1218,  1219,  1220,  1221,  1222,
    1223,  1224,  1225,  1226,  1227,  1228,  1229,  1230,  1231,  1232,
    1233,  1234,  1235,  1236,  1237,  1238,  1239,  1240,  1241,  1242,
    1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,
    1256,  1257,  1261,  1262,  1266,  1270,  1271,  1272,  1276,  1278,
    1280,  1282,  1284,  1289,  1290,  1294,  1295,  1296,  1297,  1298,
    1299,  1300,  1301,  1302,  1306,  1307,  1308,  1309,  1310,  1311,
    1315,  1316,  1320,  1321,  1322,  1323,  1324,  1325,  1329,  1330,
    1333,  1334,  1338,  1339,  1343,  1345,  1350,  1351,  1355,  1356,
    1360,  1361,  1363,  1365,  1367,  1372,  1373,  1376,  1377,  1378,
    1379,  1380,  1381,  1382,  1385,  1386,  1389,  1391,  1393,  1398,
    1411,  1428,  1429,  1431,  1436,  1437,  1438,  1439,  1440,  1444,
    1450,  1456,  1464,  1465,  1473,  1475,  1480,  1482,  1484,  1489,
    1491,  1493,  1500,  1501,  1502,  1507,  1509,  1511,  1515,  1519,
    1521,  1525,  1533,  1534,  1535,  1536,  1537,  1542,  1543,  1544,
    1545,  1546,  1566,  1570,  1574,  1582,  1589,  1590,  1591,  1595,
    1597,  1603,  1605,  1607,  1612,  1613,  1614,  1615,  1616,  1622,
    1623,  1624,  1625,  1629,  1630,  1634,  1635,  1636,  1640,  1641,
    1645,  1646,  1650,  1651,  1655,  1656,  1657,  1658,  1662,  1663,
    1674,  1676,  1681,  1682,  1683,  1684,  1685,  1686,  1688,  1690,
    1692,  1694,  1696,  1698,  1701,  1703,  1705,  1707,  1709,  1711,
    1713,  1715,  1720,  1722,  1724,  1726,  1728,  1730,  1732,  1734,
    1736,  1738,  1740,  1742,  1744,  1751,  1757,  1763,  1769,  1778,
    1788,  1796,  1797,  1798,  1799,  1800,  1801,  1802,  1803,  1808,
    1809,  1813,  1817,  1818,  1822,  1826,  1827,  1831,  1835,  1839,
    1846,  1847,  1848,  1849,  1850,  1851,  1855,  1856,  1861,  1866,
    1874,  1875,  1876,  1877,  1878,  1879,  1880,  1881,  1882,  1884,
    1886,  1888,  1890,  1892,  1897,  1898,  1901,  1902,  1903,  1906,
    1907,  1908,  1909,  1920,  1921,  1922,  1926,  1927,  1928,  1932,
    1933,  1934,  1935,  1943,  1944,  1945,  1946,  1950,  1951,  1955,
    1956,  1957,  1958,  1959,  1960,  1961,  1962,  1963,  1964,  1965,
    1966,  1970,  1978,  1979,  1983,  1984,  1985,  1986,  1987,  1988,
    1989,  1990,  1991,  1992,  1993,  1994,  1995,  1996,  1997,  1998,
    1999,  2000,  2001,  2002,  2003,  2004,  2005,  2009,  2010,  2011,
    2012,  2013,  2014,  2015,  2019,  2020,  2021,  2022,  2026,  2027,
    2028,  2029,  2034,  2035,  2036,  2037,  2038,  2039,  2040
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "CSTRINGLITERAL", "EXTERNCODE", "TALIGN",
  "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBOOL", "TBORROWED", "TBY",
  "TCATCH", "TCLASS", "TCOBEGIN", "TCOFORALL", "TCOMPLEX", "TCONFIG",
  "TCONST", "TCONTINUE", "TDEFER", "TDELETE", "TDMAPPED", "TDO", "TDOMAIN",
  "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE", "TFOR",
  "TFORALL", "TFORWARDING", "TIF", "TIMAG", "TIN", "TINDEX", "TINLINE",
  "TINOUT", "TINT", "TITER", "TINITEQUALS", "TLABEL", "TLAMBDA", "TLET",
  "TLIFETIME", "TLOCAL", "TLOCALE", "TMINUSMINUS", "TMODULE", "TNEW",
  "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY", "TOTHERWISE",
  "TOUT", "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS", "TPRAGMA",
  "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC", "TREAL",
  "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT",
  "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN",
  "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS", "TTRUE", "TTRY",
  "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE", "TUNION", "TUNMANAGED",
  "TUSE", "TVAR", "TVOID", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD",
  "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR",
  "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND",
  "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY",
  "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL", "TASSIGNSR", "TBANG",
  "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TIO", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOTEQUAL",
  "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT",
  "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR",
  "TNOELSE", "TUMINUS", "TUPLUS", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "module_decl_start",
  "module_decl_stmt", "access_control", "opt_prototype", "block_stmt",
  "stmt_ls", "only_ls", "opt_only_ls", "except_ls", "use_stmt",
  "require_stmt", "assignment_stmt", "opt_label_ident", "ident_fn_def",
  "ident_def", "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "opt_string", "do_stmt", "return_stmt",
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
  "opt_query_expr", "var_arg_expr", "opt_lifetime_where",
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
  "shadow_var_prefix", "io_expr", "new_expr", "let_expr", "expr",
  "opt_expr", "opt_try_expr", "lhs_expr", "fun_expr", "call_expr",
  "dot_expr", "parenthesized_expr", "bool_literal", "literal_expr",
  "assoc_expr_ls", "binary_op_expr", "unary_op_expr", "reduce_expr",
  "scan_expr", "reduce_scan_op_expr", YY_NULL
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
     415,   416,   417,   418,   419,   420,   421,   422
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   168,   169,   170,   170,   171,   171,   172,   172,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   174,
     175,   175,   175,   176,   176,   176,   177,   177,   178,   178,
     178,   179,   179,   180,   180,   180,   180,   181,   181,   182,
     182,   183,   183,   183,   184,   185,   185,   185,   185,   185,
     186,   186,   187,   187,   187,   187,   187,   188,   188,   188,
     188,   188,   189,   189,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   192,   192,   192,   192,   193,   193,   194,   194,
     195,   195,   196,   196,   196,   196,   196,   196,   196,   196,
     197,   197,   198,   198,   198,   198,   199,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   201,   202,   202,   202,   202,   202,   202,
     202,   202,   203,   204,   204,   204,   204,   204,   204,   205,
     205,   206,   206,   206,   207,   207,   208,   209,   209,   210,
     210,   211,   211,   211,   212,   212,   212,   212,   213,   213,
     213,   214,   214,   215,   215,   215,   216,   216,   217,   218,
     218,   218,   219,   219,   221,   222,   220,   223,   223,   223,
     223,   223,   225,   226,   224,   227,   227,   227,   227,   227,
     228,   228,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   231,   231,   232,   233,   233,   233,   234,   234,
     234,   234,   234,   235,   235,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   237,   237,   237,   237,   237,   237,
     238,   238,   239,   239,   239,   239,   239,   239,   240,   240,
     241,   241,   242,   242,   243,   243,   244,   244,   245,   245,
     246,   246,   246,   246,   246,   247,   247,   248,   248,   248,
     248,   248,   248,   248,   249,   249,   250,   250,   250,   251,
     251,   252,   252,   252,   253,   253,   253,   253,   253,   254,
     254,   254,   255,   255,   256,   256,   257,   257,   257,   258,
     258,   258,   259,   259,   259,   260,   260,   260,   260,   260,
     260,   260,   261,   261,   261,   261,   261,   262,   262,   262,
     262,   262,   263,   263,   263,   263,   264,   264,   264,   265,
     265,   265,   265,   265,   266,   266,   266,   266,   266,   267,
     267,   267,   267,   268,   268,   269,   269,   269,   270,   270,
     271,   271,   272,   272,   273,   273,   273,   273,   274,   274,
     275,   275,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   278,
     279,   280,   280,   280,   280,   280,   280,   280,   280,   281,
     281,   282,   283,   283,   284,   285,   285,   286,   286,   286,
     287,   287,   287,   287,   287,   287,   288,   288,   289,   290,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   292,   292,   293,   293,   293,   294,
     294,   294,   294,   295,   295,   295,   296,   296,   296,   297,
     297,   297,   297,   298,   298,   298,   298,   299,   299,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   301,   301,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   303,   303,   303,
     303,   303,   303,   303,   304,   304,   304,   304,   305,   305,
     305,   305,   306,   306,   306,   306,   306,   306,   306
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     2,     2,     3,     2,     4,
       3,     4,     4,     0,     1,     1,     0,     1,     2,     3,
       3,     1,     2,     1,     3,     3,     5,     0,     1,     1,
       1,     3,     5,     5,     3,     4,     4,     4,     4,     4,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     2,     1,
       2,     3,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     3,     5,     5,     2,     2,     5,     3,     6,
       6,     4,     5,     5,     3,     3,     6,     5,     6,     5,
       6,     3,     4,     3,     4,     6,     7,     6,     7,     4,
       5,     4,     5,     4,     4,     3,     6,     5,     6,     5,
       8,     7,     2,     3,     3,     2,     2,     3,     3,     0,
       2,     2,     3,     5,     1,     3,     3,     5,     5,     0,
       2,     3,     2,     3,     6,     8,     6,     8,     1,     1,
       1,     0,     2,     0,     2,     3,     5,     5,     1,     1,
       2,     3,     1,     3,     0,     0,     8,     0,     1,     2,
       2,     1,     0,     0,    10,     3,     3,     5,     5,     3,
       1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     3,     0,     1,     3,     4,     5,
       4,     6,     6,     0,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     0,     1,     1,     2,     1,     1,
       1,     1,     0,     1,     2,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     2,     1,     0,     1,     2,     2,
       0,     2,     2,     4,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     3,     4,     4,     2,
       4,     0,     2,     2,     1,     1,     1,     2,     1,     4,
       5,     3,     1,     3,     3,     5,     1,     1,     3,     1,
       2,     3,     0,     2,     2,     3,     2,     4,     3,     3,
       4,     3,     0,     2,     2,     2,     1,     0,     2,     2,
       2,     1,     4,     4,     6,     3,     0,     1,     1,     3,
       4,     3,     4,     6,     0,     2,     2,     2,     2,     1,
       1,     3,     3,     1,     3,     1,     1,     1,     3,     3,
       0,     1,     1,     3,     3,     3,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     2,     4,     4,
       4,     5,     2,     2,     1,     2,     1,     2,     1,     2,
       1,     2,     6,     6,     4,     9,     9,     7,     6,     6,
       4,     9,     9,     7,     4,     6,     6,     9,     9,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     4,     1,     3,     4,     1,     3,     4,     3,     3,
       1,     1,     2,     1,     2,     1,     1,     3,     2,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     3,
       3,     2,     2,     1,     0,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     2,     1,     4,     4,     4,     3,
       3,     3,     3,     3,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     4,     3,     4,
       3,     4,     3,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   530,   531,   532,   533,
     534,     0,   469,    70,   102,   440,   198,   469,     0,   107,
       0,   345,    70,     0,     0,     0,     0,   208,   504,   504,
     527,     0,     0,     0,     0,   106,     0,   218,   103,     0,
     214,     0,     0,   109,     0,     0,   460,   535,   110,     0,
     221,   434,   344,     0,     0,     0,    45,    44,   105,   199,
     346,     0,     0,     0,     0,   438,     0,     0,   108,     0,
       0,    83,     0,   528,     0,     0,     0,   104,   200,   436,
       0,   348,   111,     0,     0,   584,     0,   586,     0,   587,
     588,   503,     0,   585,   582,   122,   583,     0,     0,     0,
       4,     0,     5,     0,     9,    46,    10,    11,    12,    14,
     418,   419,    22,    13,   123,   129,    15,    17,    16,    19,
      20,    21,    18,   128,     0,   126,     0,   495,     0,   130,
     127,     0,   131,   509,   491,   420,   492,   424,   422,     0,
       0,   496,   497,     0,   423,     0,   510,   511,   512,   529,
     490,   426,   425,   493,   494,     0,    38,    24,   432,     0,
       0,   470,    71,     0,     0,     0,     0,     0,     0,     0,
       0,   495,   509,   422,   496,   497,   441,   423,   510,   511,
       0,   469,     0,     0,   347,     0,   172,     0,   403,     0,
     410,   505,   219,   533,   136,     0,     0,   220,     0,     0,
       0,     0,     0,     0,     0,   504,   135,     0,     0,   410,
      77,    84,    95,    89,    97,    79,    88,    98,    85,    99,
     100,    93,    87,    94,    92,    90,    91,    78,    80,    86,
      96,   101,     0,    81,     0,     0,     0,     0,   352,     0,
     119,    32,     0,   569,   488,     0,   435,   570,     7,   410,
     504,   125,   124,   315,   400,     0,   399,     0,     0,   120,
     508,     0,     0,    35,     0,   439,   427,     0,   410,    36,
     433,     0,   179,   175,     0,   423,   179,   176,     0,   341,
       0,   437,     0,   399,     0,     0,   571,   573,   502,   568,
     567,     0,    48,    51,     0,     0,   405,     0,   407,     0,
       0,   406,     0,     0,   399,     0,     0,     6,     0,    47,
       0,   201,     0,   301,   300,   222,     0,   421,    23,     0,
       0,     0,     0,     0,     0,     0,   572,     0,     0,     0,
       0,     0,     0,   501,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
     267,   268,   269,   264,   266,     0,     0,   262,   265,   263,
     261,     0,   271,   270,     0,     0,   410,   410,     0,     0,
       0,    25,    26,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,    28,     0,    29,     0,   418,   416,     0,
     411,   412,   417,     0,     0,     0,     0,     0,   145,     0,
       0,   144,     0,   153,     0,     0,     0,   151,     0,     0,
       0,    57,   132,     0,   165,     0,     0,    30,   283,   215,
     356,     0,   357,   359,     0,   381,     0,   362,     0,     0,
     118,    31,    33,     0,   314,     0,    64,   506,   507,   121,
       0,    34,   410,     0,   186,   177,   173,   178,   174,     0,
     339,   336,    61,     0,    57,   138,    37,    50,    49,    52,
       0,   536,     0,     0,   523,     0,   525,     0,     0,     0,
       0,     0,     0,     0,     0,   540,     8,     0,    40,     0,
       0,     0,     0,     0,   294,   351,   468,   564,   563,   566,
     575,   574,   579,   578,   560,   557,   558,   559,   499,   547,
     521,   522,   520,   519,   500,   551,   562,   556,   554,   565,
     555,   553,   545,   550,   552,   561,   544,   548,   549,   546,
       0,     0,     0,     0,     0,     0,     0,   577,   576,   581,
     580,   480,   481,   483,   485,     0,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   538,   469,   469,   141,
     337,   349,   404,     0,     0,   429,     0,   338,   201,     0,
       0,     0,     0,   444,     0,     0,     0,   154,     0,   450,
       0,     0,   152,   583,    60,     0,    53,    58,     0,   164,
       0,     0,     0,   428,   288,   285,   286,   287,   291,   292,
     293,   283,     0,   276,     0,   284,   302,     0,   360,     0,
     114,   115,   113,   112,     0,   380,   379,   491,     0,   354,
     489,   353,   518,   402,   401,     0,     0,     0,   430,     0,
     180,   343,   491,     0,     0,     0,   537,   498,   524,   408,
     526,   409,   161,     0,     0,     0,   539,   159,   454,     0,
     542,   541,     0,    42,    41,    39,   202,     0,     0,   212,
       0,   209,   298,   295,   296,   299,   223,     0,    68,    69,
      67,    66,    65,   516,   517,   482,   484,     0,   471,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   137,   414,   415,   413,     0,   350,     0,   163,
       0,     0,   143,     0,   142,     0,   475,     0,     0,   149,
       0,     0,   147,     0,     0,   133,     0,   134,     0,     0,
     167,     0,   169,   289,   290,     0,   283,   274,     0,   394,
     303,   306,   305,   307,     0,   358,   361,   362,     0,     0,
     363,   364,   188,     0,     0,   187,   190,   431,     0,   181,
     184,     0,   340,    62,    63,   162,     0,     0,     0,     0,
     160,     0,     0,   217,   207,     0,   210,   206,   297,   302,
     272,    72,   255,    84,   253,    89,    74,    88,    85,   258,
      99,   100,    87,    90,    73,    75,    86,   101,   252,   234,
     237,   235,   236,   247,   238,   251,   243,   241,   254,   257,
     242,   240,   245,   250,   239,   244,   248,   249,   246,   256,
       0,   232,     0,    76,     0,   272,   272,   230,   473,   362,
     509,   509,     0,     0,     0,     0,     0,     0,     0,     0,
     140,   139,     0,   146,     0,     0,   443,     0,   442,     0,
     474,     0,     0,   449,   150,     0,   448,   148,    55,    54,
     166,   459,   168,     0,   394,   277,     0,     0,   362,   304,
     320,   355,   385,     0,   538,     0,   192,     0,     0,     0,
     182,     0,   157,   456,     0,     0,   155,   455,     0,   543,
     196,   504,   217,   217,   194,   217,   204,   213,   211,     0,
     283,   229,   233,     0,   259,     0,   225,   226,   477,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,   447,
       0,     0,   476,   453,     0,     0,     0,     0,   171,   362,
     394,     0,   397,   396,   398,   491,   316,   280,   278,     0,
       0,     0,     0,   383,   491,   193,   191,     0,   185,     0,
     158,     0,   156,   205,   376,     0,   308,     0,   231,    72,
      73,   272,   272,     0,     0,     0,     0,     0,     0,   197,
     195,     0,     0,     0,     0,    56,   170,   279,   362,   386,
       0,   317,   319,   318,   334,     0,   335,   322,   325,     0,
     321,   312,   313,   216,     0,   183,     0,     0,     0,   375,
     374,   491,   309,   320,   273,   227,   228,     0,     0,     0,
       0,     0,     0,   446,   445,   452,   451,   282,   281,   388,
     389,   391,   491,     0,   538,   333,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   491,   458,   457,     0,   366,
       0,     0,     0,   390,   392,   324,   326,   327,   330,   331,
     332,   328,   329,   323,   371,   369,   491,   538,   310,   224,
     311,   386,   370,   491,   393
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   293,   101,   579,   103,   104,   105,   310,
     106,   294,   574,   578,   575,   107,   108,   109,   163,   801,
     236,   110,   233,   111,   605,   196,   241,   112,   113,   114,
     115,   116,   117,   678,   118,   119,   120,   445,   620,   741,
     121,   122,   616,   736,   123,   124,   482,   753,   125,   126,
     650,   651,   171,   234,   596,   128,   129,   484,   759,   656,
     804,   805,   415,   881,   419,   592,   593,   594,   595,   657,
     315,   724,   983,  1039,   973,   254,   962,   917,   921,   967,
     968,   969,   130,   280,   450,   131,   132,   237,   238,   423,
     424,   609,   980,   936,   427,   606,  1000,   914,   848,   295,
     187,   299,   300,   389,   390,   391,   172,   134,   135,   136,
     137,   173,   139,   160,   161,   535,   404,   695,   536,   537,
     140,   174,   175,   143,   197,   392,   177,   145,   178,   179,
     148,   149,   150,   305,   151,   152,   153,   154,   155
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -878
static const yytype_int16 yypact[] =
{
    -878,    99,  2830,  -878,   -54,  -878,  -878,  -878,  -878,  -878,
    -878,  4282,    64,   106,  -878, 12168,  -878,    64,  9720,  -878,
     186,   218,   106,  4282,  9720,  4282,    58,  -878,  9720,  6822,
    -878,  8202,  8800,  6539,  9720,  -878,    70,  -878,  -878, 16348,
    -878,  1223,  8938,  -878,  9720,  9720,  -878,  -878,  -878,  9720,
    -878, 12168,  -878,  9720,   342,   191,  1178,  1178,  -878,  -878,
    -878,  9076,  7742,  9720,  8938, 12168,  9720,   280,  -878,   200,
    4282,  -878,  9720,  -878,  9858,  9858, 16348,  -878,  -878, 12168,
    9076,  -878,  -878,  9720,  9720,  -878,  9720,  -878,  9720,  -878,
    -878, 11790,  9720,  -878,  9720,  -878,  -878,  3316,  6960,  8340,
    -878,  4121,  -878,   239,  -878,   310,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878, 16348,  -878, 16348,   -53,   214,  -878,
    -878,  1223,  -878,   251,  -878,   270,  -878,  -878,   328,   329,
     343,   335,   341, 16016,  1985,   222,   344,   345,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,   337,  -878,  -878, 16016,   339,
    4282,  -878,  -878,   347,  9720,  9720,  9720,  9720,  9720,  9076,
    9076,   285,  -878,  -878,  -878,  -878,   365,   290,  -878,  -878,
     346, 14214, 16348,  1223,  -878,   350,  -878,   141, 16016,   398,
    7903, 16016,  -878,   116,  -878, 16348,   122,   192, 16348,   349,
      38, 13733,     8, 13692,   192,  9720,  -878, 14117, 12941,  7903,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  4282,  -878,   351,  1050,    47,    -6,  -878,  4282,
    -878,  -878, 13876,   516,   365, 13876,   365,   516,  -878,  7903,
    9720,  -878,  -878, 16348,  -878,   144, 16016,  9720,  9720,  -878,
   16016,   364, 14064,  -878, 13876,   365, 16016,   358,  7903,  -878,
   16016, 14450,  -878,  -878, 14507, 11787,  -878,  -878, 14593,   406,
     368,   365,   208, 14267, 13876, 14650,   445,   445,  1520,   516,
     516,   258,  -878,  -878,  3477,   140,  -878,  9720,  -878,   -59,
     -51,  -878,   -52,    16, 14736,    76,   518,  -878,  3638,  -878,
     468,   392,   369,  -878,  -878,  -878,   237,  -878,  -878,  9720,
    9720,  9720,  9720,  8800,  8800,  9720,   313,  9720,  9720,  9720,
    9720,  9720,   417, 11790,  9720,  9720,  9720,  9720,  9720,  9720,
    9720,  9720,  9720,  9720,  9720,  9720,  9720,  9720,  9720,  -878,
    -878,  -878,  -878,  -878,  -878,  8501,  8501,  -878,  -878,  -878,
    -878,  8501,  -878,  -878,  8501,  8501,  7903,  7903,  8800,  8800,
    7443,  -878,  -878, 14364, 14410, 14690,     4,  3799,  -878,  8800,
      38,   382,   242,  -878,  9720,  -878,  9720,   423,  -878,   378,
     407,  -878,  -878,   393, 16348,  1223,   504,  9076,  -878,  4443,
    8800,  -878,   391,  -878,    38,  4604,  8800,  -878,    38,   192,
    9996,  9720,  -878,  4282,   522,  9720,   396,  -878,   535,  -878,
    -878,  1050,  -878,   419,   399,  -878, 10134,   446,  9720,  1223,
    -878,  -878,  -878,   401,  -878,  9076,  -878, 16016, 16016,  -878,
      31,  -878,  7903,   404,  -878,   549,  -878,   549,  -878, 10272,
     433,  -878,  -878,  9996,  9720,  -878,  -878,  -878,  -878,  -878,
    8639,  -878, 12798,  7121,  -878,  7282,  -878,  4282,   408,  8800,
    6053,  3155,   409,  9720,  6377,  -878,  -878,   307,  -878,  3960,
   16348,  9076,   411, 16291,    25,  -878, 14833, 13831, 13831,   272,
    -878,   272,  -878,   272, 14547,    96,  1476,   547,   365,   445,
    -878,  -878,  -878,  -878,  1520,  1389,   272, 12892, 12892, 13831,
   12892, 12892,  1232,   445,  1389, 13966,  1232,   516,   516,   445,
     422,   424,   429,   431,   434,   425,   426,  -878,   272,  -878,
     272,    55,  -878,  -878,  -878,   -29,  -878,  1980, 16065,   386,
   10410,  8800, 10548,  8800,  9720,  8800, 12396,    64, 14886,  -878,
    -878,  -878, 16016, 14926,  7903,  -878,  7903,  -878,   392,   262,
    9720,   101,  9720, 16016,    45, 13921,  7443,  -878,  9720, 16016,
      18, 13778,  -878,   441,   461,   447, 14983,   461,   448,   568,
   15069,  4282, 14019,  -878,   170,  -878,  -878,  -878,  -878,  -878,
    -878,   725,   145,  -878, 12477,  -878,   324,   444,  1050,    47,
      58,    70,  9720,  9720,  5892,  -878,  -878,   620,  8064,  -878,
   16016,  -878,  -878,  -878, 16016,   450,    32,   449,  -878,  2465,
    -878,  -878,   291, 16348,   454,   458,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  4282,   -38,  2627,  -878,  -878, 16016,  4282,
   16016,  -878, 15126,  -878,  -878,  -878,   481,  2273,   459,   506,
     205,  -878,   542,  -878,  -878,  -878,  -878,  6663,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  7443,  -878,    28,
    8800,  8800,  9720,   589, 15158,  9720,   593, 15301,   460, 12614,
      38,    38,  -878,  -878,  -878,  -878,   464,  -878, 13876,  -878,
   13017,  4765,  -878,  4926,  -878,   212,  -878, 13093,  5087,  -878,
      38,  5248,  -878,    38,  9720,  -878,  9720,  -878,  4282,  9720,
    -878,  4282,   596,  -878,  -878, 16348,   959,  -878,  1050,   498,
     558,  -878,  -878,  -878,    69,  -878,  -878,   446,   474,    36,
    -878,  -878,  -878,  5409,  9076,  -878,  -878,  -878, 16348,  -878,
     507,   346,  -878,  -878,  -878,  -878,  5570,   478,  5731,   479,
    -878,  9720,   484,  1893,  -878,  9720, 16348,  -878,  -878,   324,
     485,   -35,  -878,   510,  -878,   511,   525,   514,   521,  -878,
     526,   529,   531,   537,   156,   540,   538,   548,  -878,  -878,
   16348,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    9720,  -878,   550,   560,   555,   485,   485,  -878,  -878,   446,
     216,   225, 15269, 10686, 10824, 15412, 10962, 11100, 11238, 11376,
    -878,  -878,  2568,  -878,  4282,  9720, 16016,  9720, 16016,  7443,
    -878,  4282,  9720, 16016,  -878,  9720, 16016,  -878, 15444, 16016,
    -878, 16016,   624,  4282,   498,  -878,   543,  9237,   207,  -878,
      78,  -878,  -878,  8800, 12282,  4282,  -878,    10,   545,  9720,
    -878,  9720,  -878, 16016,  4282,  9720,  -878, 16016,  4282, 16016,
    -878,  7604,   867,   867,  -878, 16214,  -878, 16016,  -878,   114,
     535,  -878,  -878, 12851,  -878, 12558,  -878,  -878,  -878,  9720,
    9720,  9720,  9720,  9720,  9720,  9720,  9720,   534, 16200, 15069,
   13169, 13245,  -878, 15069, 13321, 13397,  9720,  4282,  -878,   446,
     498,  6215,  -878,  -878,  -878,   127,  9076,  -878,  -878,   128,
    9720,    11, 12701,  -878,   908,   398,  -878,   346, 16016, 13473,
    -878, 13549,  -878,  -878,  -878, 11514,   606,   228,  -878,   579,
     580,   485,   485, 15555, 15587, 15698, 15730, 15841, 15873,  -878,
    -878,  4282,  4282,  4282,  4282, 16016,  -878,  -878,   207,  9398,
      80,  -878,  -878, 16016,  -878,    57,  -878,   171,  -878,   224,
   15984,  -878,  -878,  -878, 11376,  -878,  4282,  4282,  2991,  -878,
    -878,   327,  -878,    78,  -878,  -878,  -878,  9720,  9720,  9720,
    9720,  9720,  9720, 15069, 15069, 15069, 15069,  -878,  -878,  -878,
    -878,  -878,   153,  8800, 11924,  -878,  9720,   128,    57,    57,
      57,    57,    57,    57,   128,   944, 15069, 15069,   551, 11652,
      94,    59, 12758,  -878,  -878, 16016,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,   576,  -878,  -878,   532, 12038,  -878,  -878,
    -878,  9559,  -878,   658,  -878
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -878,  -878,  -878,     0,  -377,  2115,  -878,  -878,  -878,  -878,
     227,   405,  -177,   260,   259,  -878,  -878,   168,   694,  -878,
    1642,   -62,  -630,  -878,  -819,  -878,   517,  -877,  -723,   -42,
    -878,  -878,  -878,    87,  -878,  -878,  -878,   452,  -878,   -16,
    -878,  -878,  -878,  -878,  -878,   528,   163,   -97,  -878,  -878,
    -878,   -30,   947,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -156,  -143,  -715,  -878,  -150,    17,   147,  -878,  -878,
    -878,   -28,  -878,  -878,  -289,    15,  -878,  -222,  -242,  -271,
    -262,  -572,  -878,  -179,  -878,    -8,   713,  -121,   318,  -878,
    -376,  -702,  -738,  -878,  -564,  -440,  -769,  -861,  -762,   -56,
    -878,    27,  -878,  -183,  -878,   194,   539,  -370,  -878,  -878,
    -878,  1098,  -878,    -9,  -878,  -878,  -138,  -878,  -547,  -878,
    -878,  1334,  1459,   -11,   -17,   152,   752,  -878,  1750,  1851,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -364
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -539
static const yytype_int16 yytable[] =
{
     158,   365,   100,   381,   176,   255,   539,   181,   180,   621,
     316,   192,   183,   188,   251,   252,   393,   191,   191,   696,
     201,   203,   207,   208,   282,   851,   416,   803,   912,   425,
     876,   242,   615,   243,   244,   727,   428,   239,   245,   239,
     246,   591,   247,   303,   972,   597,   545,   698,   425,   652,
     256,   260,   262,   264,   265,   266,   607,   402,   469,   270,
     964,   271,   382,   274,   278,   408,   433,   239,   281,   283,
     425,   402,   284,   285,   691,   286,   463,   287,   853,   622,
     288,   289,   909,   290,   465,   443,   256,   260,   304,  -377,
     886,   887,    62,   653,   -77,  -189,   733,   665,  1001,     3,
     156,  -465,   -82,   464,   654,   809,   667,   888,  -515,   162,
    -515,   466,   467,   298,   376,   934,   979,   402,   200,   202,
     808,  -377,  1003,   655,   322,   402,   746,   402,   387,   429,
     919,   964,   365,   668,   666,  -117,   545,  -189,   734,   470,
      62,    16,  -377,  1024,   972,   435,   918,   387,   958,   966,
    -377,   470,   933,   158,   373,   374,   375,   270,   256,   304,
     850,  -377,   426,  -377,   468,   472,  -372,   377,   546,   377,
     377,   470,   380,   159,   323,   876,  -377,   377,   324,   260,
     471,   426,  -377,   525,   526,   920,   302,   387,   409,   395,
    -377,  -189,   735,  -117,   191,  -372,   183,   377,   260,    59,
     854,  -377,   539,   426,   377,   388,   387,   957,  -372,   965,
      21,   474,   713,  1038,   261,   470,    21,  -117,   377,   190,
     966,  -372,   726,    78,   388,   326,   985,   986,  -377,   470,
     330,   209,   331,   332,   577,  1023,   435,   335,   260,   191,
     475,  -395,   273,   277,  1004,   342,   437,   438,   935,   714,
     301,   346,   347,   348,    52,   803,   998,   260,  1037,   617,
      52,   313,  -395,   689,   388,    60,  -395,  -387,  -372,   240,
     503,    60,  1044,  -372,   559,   460,   384,   577,  1006,   435,
     716,  1035,   902,   388,   182,   -78,   462,   314,  -387,  -395,
      81,   240,  -387,   -83,   459,   385,    81,   184,   436,  1042,
     461,   272,   276,   539,   387,   387,  1007,   717,   486,   487,
     488,   489,   491,   493,   494,  -387,   495,   496,   497,   498,
     499,   608,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   518,   519,  1008,   591,
     756,   561,   846,   435,   260,   260,   916,   829,   720,   248,
     260,  -479,   249,   260,   260,   260,   260,   528,   530,   538,
    -478,   268,   452,   716,  1009,   757,  1010,  1011,   548,   267,
    1012,  1013,   429,   552,   830,   553,   875,   429,  -479,  -373,
     387,   388,   388,   366,   309,   367,   256,  -478,   563,   565,
     984,   485,   721,  1005,   569,   571,   551,   429,   308,   576,
     576,   395,   580,   722,   582,  -462,   330,   378,  -373,   332,
     490,   492,   156,   335,   923,   368,   687,   610,   457,   369,
       5,  -373,   723,   317,   614,   646,  -342,   240,   240,   240,
     240,   260,   700,   703,  -373,   414,  1027,  1028,  1029,  1030,
    1031,  1032,   576,   576,   742,  -342,  -515,   500,  -515,   614,
     613,  -513,   260,  -513,   260,   527,   529,   388,   635,   614,
     638,   156,   640,   642,   671,   539,   547,   643,   369,   240,
     256,   501,   240,   322,  -514,   613,  -514,   915,   298,   459,
     298,  -373,  -461,   318,   924,   613,  -373,   564,   319,  -466,
     629,   240,   631,   570,   387,  -467,   747,   749,  -464,  -463,
     370,   372,   332,   591,   383,   377,   386,   520,   521,    71,
     397,   240,   418,   522,   806,   502,   523,   524,   439,   442,
     449,   875,   451,   323,   480,   476,   481,   324,   483,   563,
     674,   569,   677,   580,   679,   638,   550,   554,   680,   681,
     555,   133,   556,   260,   322,   260,   560,   557,   729,   688,
     133,   690,   566,   581,   598,   538,   634,   697,   583,   584,
     608,   599,   133,   612,   133,   981,   618,   619,   623,   683,
     647,   388,   633,   639,   326,   322,   658,   585,   659,   330,
     586,   263,   332,   660,  -365,   661,   335,   663,   662,  1002,
     664,   266,   270,   304,   323,   -59,   704,   260,   324,   708,
     587,   705,   707,   588,  1015,    54,   725,   240,   743,   133,
     732,   737,   744,  -365,   589,   301,   435,   301,   813,   754,
     755,   758,   816,   822,   818,   323,  -365,   843,   673,   324,
     676,   240,   847,   590,  1002,   240,   133,   849,   852,  -365,
     133,   859,   864,   868,   870,   326,   880,  -102,  -107,  1036,
     330,  -106,   331,   332,   -79,   907,   538,   335,  -103,   491,
     528,   812,  -378,  -109,   815,   342,  -110,  1043,  -105,   -80,
    -275,  1002,  -378,   348,  -108,  -104,   326,   327,   857,   884,
     826,   330,   828,   331,   332,  -111,  -365,   833,   335,   -81,
     836,  -365,   885,   838,   949,   839,   342,  -275,   841,   133,
     982,  -378,   346,   347,   348,   910,   684,   927,   -77,   -78,
    -367,  1007,   624,   479,   625,  1034,   185,   398,   401,   403,
     407,   686,   858,   256,   394,   898,   878,  -378,   447,   941,
     937,   879,  1040,   845,  -378,   863,   997,   867,   715,  -367,
     869,  1021,   942,  1033,   877,  1026,   206,   611,     0,   584,
     685,     0,  -367,     0,   144,  -378,     0,   490,   527,   431,
     731,     0,   432,   144,     0,  -367,     0,   585,     0,     0,
     586,   133,     0,     0,  -378,   144,     0,   144,   133,  -378,
       0,   441,  -378,     0,     0,     0,     0,     0,     0,   883,
     587,   240,   240,   588,     0,   306,     0,   240,   240,     0,
       0,   455,   826,   828,   589,   833,   836,   863,   867,   712,
       0,     0,  -367,   899,   900,     0,   901,  -367,   538,     0,
     903,   904,   144,   590,   905,     0,   275,   275,     0,     0,
     251,   252,     0,   133,     0,     0,     0,     0,     0,     0,
       0,     0,   922,   638,     0,     0,   739,   133,   928,   144,
     929,     0,     0,   144,   931,   960,     0,     0,     0,     0,
     191,     0,   913,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   899,   943,
     944,   903,   945,   946,   947,   948,     0,     0,     0,     0,
     204,    21,     0,     0,     0,   955,     0,   549,     0,     0,
     304,    28,   250,     0,     0,   963,     0,   240,   240,   970,
       0,    37,   144,     0,     0,   240,   133,   414,     0,     0,
       0,   567,  1020,     0,   414,   572,     0,   240,     0,     0,
     240,   961,     0,    50,     0,    52,     0,     0,   133,     0,
     993,   994,   995,   996,   133,     0,    60,     0,     0,   127,
    -382,     0,   133,     0,     0,     0,     0,     0,   127,     0,
    -382,     0,     0,   867,     0,  1016,  1017,   304,   860,     0,
     127,    81,   127,     0,   999,     0,   993,   994,   995,   996,
    1016,  1017,     0,   584,   144,     0,  -384,     0,     0,  -382,
       0,   144,  1022,   638,     0,  1025,  -384,     0,     0,     0,
       0,   585,     0,     0,   586,     0,   133,     0,     0,     0,
     133,     0,     0,     0,     0,  -382,     0,   127,   133,   999,
       0,     0,  -382,     0,   587,  -384,   638,   588,     0,    54,
     867,     0,     0,     0,     0,     0,     0,     0,   589,     0,
       0,     0,     0,  -382,   127,     0,   144,     0,   127,     0,
       0,  -384,     0,   210,     0,     0,   999,   590,  -384,     0,
     144,     0,  -382,     0,     0,   211,   212,  -382,     0,     0,
    -382,     0,   213,     0,     0,     0,   669,     0,     0,  -384,
     214,   692,   694,     0,   240,     0,   215,   699,   702,     0,
       0,   216,     0,   217,     0,     0,   218,     0,  -384,     0,
     138,     0,     0,  -384,   219,     0,  -384,   127,     0,   138,
       0,   220,     0,     0,     0,     0,     0,   221,     0,     0,
     133,   138,     0,   138,     0,     0,   222,   414,   414,   144,
       0,   414,   414,     0,     0,   223,   224,     0,   225,     0,
     226,     0,   227,     0,     0,   228,     0,     0,   971,   229,
     420,   144,   230,     0,   975,   231,   414,   144,   414,     0,
       0,     0,     0,     0,     0,   144,     0,     0,   138,     0,
       0,     0,   133,     0,     0,     0,     0,     0,   133,   127,
       0,     0,     0,     0,     0,     0,   127,     0,     0,     0,
       0,     0,     0,     0,     0,   138,   807,   820,   821,   138,
       0,   204,    21,     0,     0,   823,     0,     0,     0,   810,
     811,   421,    28,   250,     0,     0,     0,   834,     0,   144,
     837,     0,    37,   144,     0,  -217,   210,     0,     0,     0,
     133,   144,   133,     0,     0,     0,     0,   133,   211,   212,
     133,   127,     0,     0,    50,   213,    52,   133,   971,     0,
     133,  -217,     0,   214,     0,   127,     0,    60,   138,   215,
     322,     0,     0,     0,   216,     0,   217,     0,     0,   218,
       0,     0,   133,     0,     0,     0,     0,   219,     0,     0,
       0,     0,    81,     0,   220,   133,     0,   133,     0,     0,
     221,     0,     0,     0,     0,     0,     0,     0,     0,   222,
       0,     0,     0,     0,     0,     0,     0,     0,   223,   224,
     323,   225,     0,   226,   324,   227,     0,     0,   228,     0,
       0,     0,   229,     0,   127,   230,     0,     0,   231,     0,
     138,     0,     0,   144,     0,     0,   141,   138,     0,     0,
       0,     0,     0,     0,     0,   141,   127,     0,     0,     0,
       0,     0,   127,     0,     0,     0,     0,   141,     0,   141,
     127,   326,   327,   133,   328,   329,   330,     0,   331,   332,
     133,     0,     0,   335,   926,     0,     0,     0,     0,     0,
       0,   342,   133,     0,   235,   144,     0,   346,   347,   348,
       0,   144,   138,     0,   133,     0,     0,     0,     0,     0,
       0,     0,     0,   133,   141,     0,   138,   133,     0,     0,
       0,     0,     0,     0,   127,     0,     0,   322,   127,     0,
       0,     0,     0,     0,     0,     0,   127,     0,     0,     0,
       0,   141,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,   144,     0,   144,   133,     0,     0,     0,
     144,     0,     0,   144,     0,     0,     0,     0,     0,     0,
     144,   142,     0,   144,     0,     0,     0,   323,     0,     0,
     142,   324,     0,     0,     0,   138,     0,     0,     0,     0,
       0,     0,   142,     0,   142,   144,     0,     0,     0,     0,
     133,   133,   133,   133,   141,     0,     0,   138,   144,     0,
     144,     0,     0,   138,   322,     0,     0,     0,     0,     0,
       0,   138,     0,     0,     0,   133,   133,     0,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,   127,   142,
     335,   336,   337,     0,     0,   339,   340,   341,   342,     0,
       0,   345,     0,     0,   346,   347,   348,     0,   322,     0,
       0,     0,     0,     0,   323,     0,   142,     0,   324,     0,
     142,     0,     0,     0,     0,   138,   141,     0,     0,   138,
       0,     0,     0,   141,     0,     0,   144,   138,     0,     0,
     127,     0,     0,   144,     0,     0,   127,     0,     0,     0,
       0,     0,     0,     0,     0,   144,     0,     0,   323,     0,
       0,     0,   324,     0,     0,   326,   327,   144,     0,   329,
     330,     0,   331,   332,     0,     0,   144,   335,     0,   142,
     144,     0,     0,     0,     0,   342,     0,     0,   141,     0,
       0,   346,   347,   348,     0,     0,     0,     0,   127,     0,
     127,     0,   141,     0,     0,   127,     0,     0,   127,   326,
     327,     0,   328,   329,   330,   127,   331,   332,   127,   144,
       0,   335,     0,     0,     0,     0,     0,     0,   341,   342,
       0,     0,   345,     0,     0,   346,   347,   348,     0,   138,
     127,   232,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   142,     0,   127,     0,   127,     0,     0,   142,     0,
       0,     0,     0,   144,   144,   144,   144,     0,     0,     0,
       0,   141,     0,     0,     0,     0,     0,     0,   279,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   144,   144,
       0,   138,     0,   141,     0,     0,     0,   138,     0,   141,
       0,     0,     0,     0,     0,     0,     0,   141,     0,     0,
       0,     0,   146,   142,     0,     0,     0,     0,     0,     0,
       0,   146,     0,     0,     0,     0,   311,   142,   312,     0,
       0,   127,     0,   146,     0,   146,     0,     0,   127,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   138,
     127,   138,     0,     0,     0,     0,   138,     0,     0,   138,
       0,   141,   127,     0,     0,   141,   138,     0,     0,   138,
       0,   127,     0,   141,     0,   127,     0,     0,     0,     0,
     146,     0,     0,     0,   279,     0,     0,     0,     0,     0,
       0,   138,     0,     0,     0,     0,   142,   279,     0,     0,
     396,     0,     0,     0,   138,     0,   138,   146,     0,     0,
       0,   146,     0,   147,   127,     0,     0,     0,   142,     0,
       0,     0,   147,     0,   142,     0,     0,     0,     0,     0,
       0,     0,   142,     0,   147,     0,   147,   422,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,   127,   127,
     127,   127,     0,     0,     0,     0,     0,     0,     0,     0,
     146,     0,    16,     0,     0,   141,    20,    21,     0,     0,
       0,   147,   138,   127,   127,    27,   142,    28,   871,   138,
     142,     0,    33,     0,     0,     0,     0,    37,   142,     0,
       0,   138,     0,     0,     0,     0,     0,     0,   147,     0,
       0,     0,   147,   138,     0,     0,     0,     0,     0,    50,
       0,    52,   138,    54,     0,   872,   138,   141,   873,     0,
      59,     0,    60,   141,     0,     0,     0,     0,     0,     0,
       0,     0,   146,     5,     0,     0,     0,     0,     0,   146,
       0,    76,     0,     0,    78,    14,     0,    81,     0,     0,
       0,     0,    19,     0,     0,   138,     0,     0,     0,     0,
       0,   147,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    35,     0,     0,     0,   141,    38,   141,     0,     0,
       0,     0,   141,     0,    43,   141,   558,     0,     0,     0,
     142,    48,   141,     0,   146,   141,     0,    95,     0,   138,
     138,   138,   138,   874,     0,     0,    58,     0,   146,     0,
       0,     0,     0,   422,     0,     0,     0,   141,    68,     0,
       0,     0,    71,     0,   138,   138,     0,     0,     0,    77,
     141,     0,   141,   147,     0,    82,     0,     0,     0,     0,
     147,     0,   142,     0,     0,     0,     0,     0,   142,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,     0,     0,     0,   102,     0,     0,
       0,     0,   645,     0,     0,   649,   157,   146,     0,     0,
    -486,     0,     0,     0,     0,     0,     0,     0,   186,     0,
     189,     0,     0,   364,     0,   147,  -513,     0,  -513,   146,
     142,     0,   142,     0,     0,   146,     0,   142,   141,   147,
     142,     0,     0,   146,     0,   141,     0,   142,     0,     0,
     142,     0,     0,     0,     0,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,   269,     0,     0,     0,   141,
       0,     0,   142,     0,     0,     0,     0,     0,   141,     0,
       0,     0,   141,     0,     0,   142,     0,   142,     0,     0,
       0,     0,   102,     0,     0,     0,   307,   146,     0,     0,
       0,   146,     0,     0,     0,     0,     0,     0,   147,   146,
       0,     0,     0,     0,     0,     0,   719,     0,     0,     0,
     422,   141,     0,     0,     0,     0,     0,     0,     0,     0,
     147,     0,     0,     0,     0,     0,   147,     0,     0,     0,
       0,   740,     0,     0,   147,   279,     0,     0,     0,     0,
       0,     0,     0,     0,   752,   371,     0,     0,     0,     0,
       0,     0,     0,   142,     0,   141,   141,   141,   141,     0,
     142,     0,  -203,     0,     0,     0,  -203,  -203,     0,   802,
       0,     0,   142,     0,     0,  -203,     0,  -203,  -203,     0,
     141,   141,  -203,     0,   142,     0,     0,  -203,   147,     0,
    -203,     0,   147,   142,     0,     0,     0,   142,     0,     0,
     147,   146,     0,     0,     0,     0,     0,     0,     0,  -203,
       0,  -203,     0,  -203,     0,  -203,  -203,   417,  -203,     0,
    -203,     0,  -203,     0,   430,     0,     0,   844,     0,     0,
     422,     0,     0,     0,     0,     0,   142,     0,     0,     0,
       0,  -203,     0,     0,  -203,     0,     0,  -203,     0,     0,
     740,     0,     0,   146,     0,     0,     0,     0,     0,   146,
       0,     0,     0,     0,     0,     0,     0,     0,   649,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
     142,   142,   142,   142,     0,     0,     0,     0,     0,     0,
       0,     0,   882,   102,     0,     0,     0,  -203,     0,     0,
       0,     0,   147,  -203,     0,   142,   142,     0,     0,     0,
       0,   146,     0,   146,     0,     0,     0,     0,   146,     0,
       0,   146,     0,     0,     0,     0,     0,     0,   146,     0,
       0,   146,     0,     0,     0,     0,     0,     0,   210,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     211,   212,     0,   146,   147,     0,     0,   213,     0,     0,
     147,     0,   102,     0,     0,   214,   146,     0,   146,     0,
       0,   215,     0,     0,     0,     0,   216,     0,   217,     0,
       0,   218,     0,     0,   430,     0,     0,     0,     0,   219,
     430,     0,     0,     0,     0,     0,   220,   802,     0,     0,
       0,     0,   221,     0,     0,     0,     0,     0,     0,     0,
       0,   222,   147,     0,   147,     0,     0,     0,     0,   147,
     223,   224,   147,   225,     0,   226,     0,   227,     0,   147,
     228,     0,   147,     0,   229,     0,     0,   230,     0,   897,
     231,     0,     0,     0,   146,     0,     0,     0,     0,     0,
       0,   146,   632,     0,   147,     0,   637,  -203,     0,     0,
       0,  -203,  -203,   146,   102,     0,     0,   147,     0,   147,
    -203,     0,  -203,  -203,     0,   146,     0,  -203,     0,     0,
       0,     0,  -203,     0,   146,  -203,     0,     0,   146,     0,
       0,     0,     0,     0,   377,     0,   738,     0,     0,     0,
       0,     0,     0,     0,  -203,     0,  -203,   320,  -203,     0,
    -203,  -203,     0,  -203,   321,  -203,     0,  -203,     0,     0,
       0,     0,     0,     0,     0,   322,     0,   146,     0,     0,
       0,     0,     0,     0,     0,     0,  -203,     0,     0,  -203,
       0,     0,  -203,     0,     0,   147,     0,     0,     0,     0,
       0,     0,   147,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   147,     0,   710,     0,     0,     0,
       0,   146,   146,   146,   146,   323,   147,     0,     0,   324,
       0,     0,     0,     0,     0,   147,     0,     0,     0,   147,
       0,     0,  -203,     0,     0,     0,   146,   146,  -203,     0,
       0,     0,     0,     0,     0,     0,   402,     0,     0,     0,
     325,     0,     0,     0,     0,     0,     0,     0,   745,     0,
       0,     0,     0,     0,   750,     0,   326,   327,   147,   328,
     329,   330,     0,   331,   332,   333,     0,   334,   335,   336,
     337,   338,     0,   339,   340,   341,   342,   343,   344,   345,
       0,     0,   346,   347,   348,     0,     0,     0,     0,     0,
       0,   748,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   147,   147,   147,   147,   430,     0,   430,     0,
       0,     0,     0,   430,     0,     0,   430,     0,     0,     0,
       0,     0,     0,   840,     0,     0,   842,   147,   147,     0,
      -2,     4,     0,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,   856,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,   862,    27,   866,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,    37,     0,    38,  -217,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,    48,    49,     0,     0,     0,    50,    51,    52,    53,
      54,    55,    56,  -217,   -43,    57,    58,    59,     0,    60,
      61,    62,     0,    63,    64,    65,    66,    67,    68,    69,
      70,     0,    71,    72,     0,    73,    74,    75,    76,    77,
       0,    78,    79,    80,    81,    82,     0,     0,    83,     0,
      84,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   908,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
     925,     0,     0,     0,     0,     0,     0,     0,    92,   930,
       0,    93,    94,   932,    95,     0,     0,    96,     0,    97,
       0,    98,  1018,    99,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   164,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,   956,     0,     0,     0,     0,    30,   165,   166,
       0,   167,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   168,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,   253,     0,     0,     0,    96,     0,
     169,     0,    98,     0,   170,  1019,     4,     0,     5,     6,
       7,     8,     9,    10,     0,  -538,     0,    11,    12,    13,
      14,    15,  -538,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,  -538,    25,    26,  -538,    27,     0,    28,
      29,    30,    31,    32,    33,    34,    35,     0,    36,    37,
       0,    38,  -217,     0,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,    48,    49,     0,     0,
       0,    50,    51,    52,    53,     0,    55,    56,  -217,   -43,
      57,    58,    59,  -538,    60,    61,    62,  -538,    63,    64,
      65,    66,    67,    68,    69,    70,     0,    71,    72,     0,
      73,    74,    75,    76,    77,     0,    78,    79,    80,    81,
      82,     0,     0,    83,     0,    84,     0,     0,  -538,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -538,  -538,    88,  -538,  -538,  -538,
    -538,  -538,  -538,  -538,     0,  -538,  -538,  -538,  -538,  -538,
       0,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,    95,
    -538,  -538,  -538,     0,    97,  -538,    98,   291,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -217,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,    48,    49,     0,
       0,     0,    50,    51,    52,    53,    54,    55,    56,  -217,
     -43,    57,    58,    59,     0,    60,    61,    62,     0,    63,
      64,    65,    66,    67,    68,    69,    70,     0,    71,    72,
       0,    73,    74,    75,    76,    77,     0,    78,    79,    80,
      81,    82,     0,     0,    83,     0,    84,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,   253,
      95,     0,     0,    96,     0,    97,   292,    98,     4,    99,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,    37,     0,    38,  -217,     0,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,    48,    49,
       0,     0,     0,    50,    51,    52,    53,    54,    55,    56,
    -217,   -43,    57,    58,    59,     0,    60,    61,    62,     0,
      63,    64,    65,    66,    67,    68,    69,    70,     0,    71,
      72,     0,    73,    74,    75,    76,    77,     0,    78,    79,
      80,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,    95,     0,     0,    96,     0,    97,   458,    98,   477,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -217,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,    48,
      49,     0,     0,     0,    50,    51,    52,    53,    54,    55,
      56,  -217,   -43,    57,    58,    59,     0,    60,    61,    62,
       0,    63,    64,    65,    66,    67,    68,    69,    70,     0,
      71,    72,     0,    73,    74,    75,    76,    77,     0,    78,
      79,    80,    81,    82,     0,     0,    83,     0,    84,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,     0,    95,     0,     0,    96,     0,    97,   478,    98,
     291,    99,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -217,     0,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
      48,    49,     0,     0,     0,    50,    51,    52,    53,    54,
      55,    56,  -217,   -43,    57,    58,    59,     0,    60,    61,
      62,     0,    63,    64,    65,    66,    67,    68,    69,    70,
       0,    71,    72,     0,    73,    74,    75,    76,    77,     0,
      78,    79,    80,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,    95,     0,     0,    96,     0,    97,   292,
      98,     4,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,    37,     0,    38,  -217,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,    48,    49,     0,     0,     0,    50,    51,    52,    53,
      54,    55,    56,  -217,   -43,    57,    58,    59,     0,    60,
      61,    62,     0,    63,    64,    65,    66,    67,    68,    69,
      70,     0,    71,    72,     0,    73,    74,    75,    76,    77,
       0,    78,    79,    80,    81,    82,     0,     0,    83,     0,
      84,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,     0,    95,     0,     0,    96,     0,    97,
     644,    98,     4,    99,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,    37,     0,    38,  -217,     0,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,    47,    48,    49,     0,     0,     0,    50,    51,    52,
      53,   306,    55,    56,  -217,   -43,    57,    58,    59,     0,
      60,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,     0,    71,    72,     0,    73,    74,    75,    76,
      77,     0,    78,    79,    80,    81,    82,     0,     0,    83,
       0,    84,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,    95,     0,     0,    96,     0,
      97,     0,    98,     4,    99,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,    37,     0,    38,  -217,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,    48,    49,     0,     0,     0,    50,    51,
      52,    53,     0,    55,    56,  -217,   -43,    57,    58,    59,
       0,    60,    61,    62,     0,    63,    64,    65,    66,    67,
      68,    69,    70,     0,    71,    72,     0,    73,    74,    75,
      76,    77,     0,    78,    79,    80,    81,    82,     0,     0,
      83,     0,    84,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,     0,    95,     0,     0,    96,
       0,    97,     0,    98,     4,    99,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,   562,    35,     0,    36,    37,     0,    38,
    -217,     0,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,    47,    48,    49,     0,     0,     0,    50,
      51,    52,    53,     0,    55,    56,  -217,   -43,    57,    58,
      59,     0,    60,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,     0,    71,    72,     0,    73,    74,
      75,    76,    77,     0,    78,    79,    80,    81,    82,     0,
       0,    83,     0,    84,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,    95,     0,     0,
      96,     0,    97,     0,    98,     4,    99,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,   568,    35,     0,    36,    37,     0,
      38,  -217,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,    48,    49,     0,     0,     0,
      50,    51,    52,    53,     0,    55,    56,  -217,   -43,    57,
      58,    59,     0,    60,    61,    62,     0,    63,    64,    65,
      66,    67,    68,    69,    70,     0,    71,    72,     0,    73,
      74,    75,    76,    77,     0,    78,    79,    80,    81,    82,
       0,     0,    83,     0,    84,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,     0,    95,     0,
       0,    96,     0,    97,     0,    98,     4,    99,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,   825,    35,     0,    36,    37,
       0,    38,  -217,     0,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,    48,    49,     0,     0,
       0,    50,    51,    52,    53,     0,    55,    56,  -217,   -43,
      57,    58,    59,     0,    60,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,     0,    71,    72,     0,
      73,    74,    75,    76,    77,     0,    78,    79,    80,    81,
      82,     0,     0,    83,     0,    84,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,    95,
       0,     0,    96,     0,    97,     0,    98,     4,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,   827,    35,     0,    36,
      37,     0,    38,  -217,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,    48,    49,     0,
       0,     0,    50,    51,    52,    53,     0,    55,    56,  -217,
     -43,    57,    58,    59,     0,    60,    61,    62,     0,    63,
      64,    65,    66,    67,    68,    69,    70,     0,    71,    72,
       0,    73,    74,    75,    76,    77,     0,    78,    79,    80,
      81,    82,     0,     0,    83,     0,    84,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,     0,
      95,     0,     0,    96,     0,    97,     0,    98,     4,    99,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,   832,    35,     0,
      36,    37,     0,    38,  -217,     0,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,    48,    49,
       0,     0,     0,    50,    51,    52,    53,     0,    55,    56,
    -217,   -43,    57,    58,    59,     0,    60,    61,    62,     0,
      63,    64,    65,    66,    67,    68,    69,    70,     0,    71,
      72,     0,    73,    74,    75,    76,    77,     0,    78,    79,
      80,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,    95,     0,     0,    96,     0,    97,     0,    98,     4,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,   835,    35,
       0,    36,    37,     0,    38,  -217,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,    48,
      49,     0,     0,     0,    50,    51,    52,    53,     0,    55,
      56,  -217,   -43,    57,    58,    59,     0,    60,    61,    62,
       0,    63,    64,    65,    66,    67,    68,    69,    70,     0,
      71,    72,     0,    73,    74,    75,    76,    77,     0,    78,
      79,    80,    81,    82,     0,     0,    83,     0,    84,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,     0,    95,     0,     0,    96,     0,    97,     0,    98,
       4,    99,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,   855,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -217,     0,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
      48,    49,     0,     0,     0,    50,    51,    52,    53,     0,
      55,    56,  -217,   -43,    57,    58,    59,     0,    60,    61,
      62,     0,    63,    64,    65,    66,    67,    68,    69,    70,
       0,    71,    72,     0,    73,    74,    75,    76,    77,     0,
      78,    79,    80,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,    95,     0,     0,    96,     0,    97,     0,
      98,     4,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
     861,    35,     0,    36,    37,     0,    38,  -217,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,    48,    49,     0,     0,     0,    50,    51,    52,    53,
       0,    55,    56,  -217,   -43,    57,    58,    59,     0,    60,
      61,    62,     0,    63,    64,    65,    66,    67,    68,    69,
      70,     0,    71,    72,     0,    73,    74,    75,    76,    77,
       0,    78,    79,    80,    81,    82,     0,     0,    83,     0,
      84,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,     0,    95,     0,     0,    96,     0,    97,
       0,    98,     4,    99,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,   865,    35,     0,    36,    37,     0,    38,  -217,     0,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,    47,    48,    49,     0,     0,     0,    50,    51,    52,
      53,     0,    55,    56,  -217,   -43,    57,    58,    59,     0,
      60,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,     0,    71,    72,     0,    73,    74,    75,    76,
      77,     0,    78,    79,    80,    81,    82,     0,     0,    83,
       0,    84,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,    95,     0,     0,    96,     0,
      97,     0,    98,   728,    99,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   164,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,   253,     0,     0,     0,    96,
       0,   169,     0,    98,     0,   170,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   164,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     165,   166,     0,   167,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   168,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,   253,     0,     0,     0,
      96,     0,   169,     0,    98,     0,   170,   636,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   164,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   165,   166,     0,   167,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   168,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,   253,     0,
       0,     0,    96,     0,   169,     0,    98,     0,   170,   959,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   164,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   165,   166,     0,   167,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   168,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,     0,     0,     0,    96,     0,   169,     0,    98,     0,
     170,   641,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   164,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,   204,    21,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,   205,    30,   165,   166,     0,   167,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,    52,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,    60,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   168,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,    81,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   760,     0,   761,     0,    86,    87,
      88,    89,    90,   762,     0,     0,     0,    91,   763,   212,
     764,     0,     0,     0,     0,   765,     0,    92,     0,     0,
      93,    94,     0,   214,     0,     0,    96,     0,   169,   766,
      98,     0,   170,     0,   767,     0,   217,     0,     0,   768,
       0,   769,     0,     0,     0,     0,     0,   770,     0,     0,
       0,     0,     0,     0,   771,     0,     0,     0,     0,     0,
     221,     0,     0,     0,     0,     0,     0,     0,     0,   772,
       0,     0,     0,     0,     0,     0,     0,     0,   223,   224,
       0,   773,     0,   226,     0,   774,     0,     0,   775,     0,
       0,     0,   776,     0,     0,   230,     0,     0,   777,     0,
       0,     0,     0,     0,     0,     0,     0,   349,   350,   351,
     352,   353,   354,     0,     0,   357,   358,   359,   360,     0,
     362,   363,   778,   779,   780,   781,   782,     0,     0,   783,
       0,     0,     0,   784,   785,   786,   787,   788,   789,   790,
     791,   792,   793,   794,     0,   795,     0,     0,   796,   797,
     798,   799,     0,     0,   800,     5,     6,     7,     8,   193,
      10,   194,     0,     0,   164,     0,     0,    14,    15,     0,
       0,  -116,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,  -116,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
     195,    77,     0,  -116,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     5,     6,     7,     8,     9,    10,     0,
      92,     0,   164,    93,    94,    14,    15,     0,     0,    96,
       0,   169,    19,    98,     0,   170,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     167,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     168,     0,    71,     0,     0,    73,   257,   258,     0,    77,
     296,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,   297,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,   253,     0,     0,     0,    96,     0,   169,
       0,    98,     0,   170,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   164,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   165,   166,
       0,   167,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   168,     0,    71,     0,     0,    73,   257,   258,     0,
      77,   296,     0,    79,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,   253,     0,     0,     0,    96,     0,
     169,     0,    98,   628,   170,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   164,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,   257,   258,
       0,    77,   296,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,   253,     0,     0,     0,    96,
       0,   169,     0,    98,   630,   170,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   164,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,   531,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     165,   166,     0,   167,    35,   532,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,   533,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   168,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,   534,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,     0,     0,     0,
      96,     0,   169,     0,    98,     0,   170,     5,     6,     7,
       8,   193,    10,     0,     0,     0,   164,     0,     0,    14,
      15,     0,     0,  -116,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   167,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,  -116,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
       0,     0,   195,    77,     0,  -116,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   164,    93,    94,    14,    15,     0,
       0,    96,     0,   169,    19,    98,     0,   170,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,   257,   258,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,     0,   259,     0,     0,    96,
       0,   169,     0,    98,     0,   170,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   164,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     165,   166,     0,   167,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   168,     0,    71,     0,     0,    73,   257,
     258,     0,    77,     0,     0,    79,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,   253,     0,     0,     0,
      96,     0,   169,     0,    98,     0,   170,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   164,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   167,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,   730,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
     257,   258,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   164,    93,    94,    14,    15,     0,
       0,    96,     0,   169,    19,    98,     0,   170,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
     198,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,   199,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     5,     6,     7,     8,     9,    10,     0,
      92,     0,   164,    93,    94,    14,    15,     0,     0,    96,
       0,   169,    19,    98,     0,   170,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     167,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     168,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,   199,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,   253,     0,     0,     0,    96,     0,   169,
       0,    98,     0,   170,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   164,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   165,   166,
       0,   167,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   168,     0,    71,     0,     0,    73,   257,   258,     0,
      77,     0,     0,    79,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    92,
       0,   164,    93,    94,    14,    15,     0,     0,    96,     0,
     169,    19,    98,     0,   170,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   165,   166,     0,   167,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   168,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,   253,     0,     0,     0,    96,     0,   169,   626,
      98,     0,   170,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   164,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     167,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     168,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,   199,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    92,     0,
     164,    93,    94,    14,    15,     0,     0,    96,     0,   169,
      19,    98,     0,   170,     0,     0,     0,   239,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   167,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   164,    93,
      94,    14,    15,     0,     0,    96,     0,    97,    19,    98,
       0,   170,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   167,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,   253,
       0,     0,     0,    96,     0,   169,     0,    98,     0,   170,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   164,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,   600,     0,     0,
       0,     0,     0,    30,   165,   166,     0,   167,    35,     0,
     601,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,   602,    67,    68,    69,   603,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
     253,     0,     0,     0,    96,     0,   169,     0,    98,     0,
     911,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     164,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   167,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,   253,     0,     0,     0,    96,     0,   169,     0,    98,
       0,   911,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   164,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   165,   166,     0,   896,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   168,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,   253,     0,     0,     0,    96,     0,   169,     0,
      98,     0,   170,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   164,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     167,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     168,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    92,     0,
     164,    93,    94,    14,    15,     0,     0,    96,     0,   169,
      19,    98,     0,   170,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   167,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   164,    93,
      94,    14,    15,     0,     0,    96,     0,    97,    19,    98,
       0,   170,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   167,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    92,     0,   164,    93,    94,    14,
      15,     0,     0,   573,     0,   169,    19,    98,     0,   170,
       0,     0,     0,     0,   600,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   167,    35,     0,   601,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
     602,    67,    68,    69,   603,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   164,    93,    94,    14,    15,     0,
       0,    96,     0,   169,    19,    98,     0,   604,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     5,     6,     7,     8,     9,    10,     0,
      92,     0,   164,    93,    94,    14,    15,     0,     0,    96,
       0,   169,    19,    98,     0,   604,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     672,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     168,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    92,     0,
     164,    93,    94,    14,    15,     0,     0,    96,     0,   169,
      19,    98,     0,   170,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   675,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   164,    93,
      94,    14,    15,     0,     0,    96,     0,   169,    19,    98,
       0,   170,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   890,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    92,     0,   164,    93,    94,    14,
      15,     0,     0,    96,     0,   169,    19,    98,     0,   170,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   891,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   164,    93,    94,    14,    15,     0,
       0,    96,     0,   169,    19,    98,     0,   170,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   893,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     5,     6,     7,     8,     9,    10,     0,
      92,     0,   164,    93,    94,    14,    15,     0,     0,    96,
       0,   169,    19,    98,     0,   170,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     894,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     168,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    92,     0,
     164,    93,    94,    14,    15,     0,     0,    96,     0,   169,
      19,    98,     0,   170,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   895,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   164,    93,
      94,    14,    15,     0,     0,    96,     0,   169,    19,    98,
       0,   170,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   896,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    92,     0,   164,    93,    94,    14,
      15,     0,     0,    96,     0,   169,    19,    98,     0,   170,
       0,     0,     0,     0,   600,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   167,    35,     0,   601,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
     602,    67,    68,    69,   603,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   164,    93,    94,    14,    15,     0,
       0,    96,     0,   169,    19,    98,     0,   978,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     5,     6,     7,     8,     9,    10,     0,
      92,     0,   164,    93,    94,    14,    15,     0,     0,    96,
       0,     0,    19,    98,     0,   978,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     167,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     168,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   164,     0,    92,    14,
      15,     0,    94,     0,     0,   364,    19,    96,  -513,   169,
    -513,    98,     0,   170,    26,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   167,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,  -386,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     164,     0,     0,    14,    15,    88,     0,     0,     0,  -386,
      19,     0,     0,  -386,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,   253,   167,    35,
       0,    36,     0,   169,    38,    98,  -386,   911,    40,    41,
    -368,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,  -368,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,  -368,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,  -368,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     164,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,  -368,     0,     0,     0,     0,  -368,    26,    98,
       0,   978,     0,     0,    30,   165,   166,     0,   167,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   164,     0,     0,    14,    15,    88,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,   169,    38,    98,
       0,   170,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   164,     0,
       0,    14,    15,    88,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   167,    35,     0,    36,
       0,   169,    38,    98,     0,   604,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     210,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,   211,   212,     0,    77,     0,     0,    79,   213,
       0,    82,     0,     0,     0,     0,     0,   214,     0,     0,
       0,     0,     0,   215,     0,     0,     0,     0,   216,     0,
     217,     0,     0,   218,     0,     0,     0,    88,     0,     0,
       0,   219,     0,     0,     0,     0,     0,     0,   220,     0,
       0,     0,     0,     0,   221,     0,     0,     0,     0,     0,
       0,     0,     0,   222,     0,     0,     0,    98,     0,   170,
       0,   939,   223,   224,     0,   225,     0,   226,   762,   227,
       0,     0,   228,   211,   212,   764,   229,     0,     0,   230,
     213,     0,   231,     0,     0,     0,     0,     0,   214,     0,
       0,     0,     0,     0,   766,     0,     0,     0,     0,   216,
       0,   217,     0,     0,   218,     0,   769,     0,     0,     0,
       0,     0,   219,     0,     0,     0,     0,     0,     0,   220,
       0,     0,     0,     0,   320,   221,     0,     0,     0,     0,
       0,   321,     0,     0,   222,     0,     0,     0,   718,     0,
       0,     0,   322,   223,   224,     0,   225,     0,   226,     0,
     940,     0,     0,   775,     0,     0,     0,   229,     0,     0,
     230,     0,     0,   231,     0,     0,     0,     0,     0,     0,
       0,     0,   349,   350,   351,   352,   353,   354,     0,     0,
     357,   358,   359,   360,     0,   362,   363,   778,   779,   780,
     781,   782,   323,     0,   783,     0,   324,     0,   784,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,     0,
     795,   320,     0,   796,   797,   798,   799,     0,   321,     0,
       0,     0,     0,     0,     0,     0,     0,   325,     0,   322,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,     0,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,     0,   320,   346,
     347,   348,     0,     0,     0,   321,     0,     0,   819,   323,
       0,     0,     0,   324,     0,     0,   322,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   320,     0,
       0,     0,     0,     0,   325,   321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,     0,     0,     0,
     326,   327,     0,   328,   329,   330,   323,   331,   332,   333,
     324,   334,   335,   336,   337,   338,     0,   339,   340,   341,
     342,   343,   344,   345,     0,     0,   346,   347,   348,     0,
       0,   320,     0,     0,     0,   974,     0,     0,   321,     0,
       0,   325,     0,     0,     0,     0,   323,     0,     0,   322,
     324,     0,     0,     0,     0,     0,     0,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,     0,   334,   335,
     336,   337,   338,     0,   339,   340,   341,   342,   343,   344,
     345,   325,     0,   346,   347,   348,     0,     0,     0,     0,
     322,     0,  1041,     0,     0,     0,     0,   326,   327,   323,
     328,   329,   330,   324,   331,   332,   333,     0,   334,   335,
     336,   337,   338,     0,   339,   340,   341,   342,   343,   344,
     345,   320,     0,   346,   347,   348,     0,     0,   321,     0,
     627,     0,     0,     0,   325,     0,     0,     0,     0,   322,
     323,     0,     0,     0,   324,     0,     0,     0,     0,     0,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
       0,   334,   335,   336,   337,   338,     0,   339,   340,   341,
     342,   343,   344,   345,     0,     0,   346,   347,   348,     0,
       0,     0,     0,   938,     0,     0,     0,     0,     0,   323,
       0,   326,   327,   324,   328,   329,   330,   320,   331,   332,
     333,     0,   413,   335,   321,     0,     0,     0,     0,     0,
     341,   342,     0,     0,   345,   322,     0,   346,   347,   348,
       0,     0,     0,     0,   325,   349,   350,   351,   352,   353,
     354,     0,     0,   357,   358,   359,   360,     0,   362,   363,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
       0,   334,   335,   336,   337,   338,     0,   339,   340,   341,
     342,   343,   344,   345,     0,   323,   346,   347,   348,   324,
     377,     0,     0,   320,     0,     0,     0,     0,   824,     0,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   322,     0,     0,     0,     0,     0,     0,     0,     0,
     325,   349,   350,   351,   352,   353,   354,     0,     0,   357,
     358,   359,   360,     0,   362,   363,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,     0,   334,   335,   336,
     337,   338,     0,   339,   340,   341,   342,   343,   344,   345,
       0,   323,   346,   347,   348,   324,   377,     0,     0,   320,
       0,     0,     0,     0,   831,     0,   321,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   322,     0,     0,
       0,     0,     0,     0,     0,     0,   325,   349,   350,   351,
     352,   353,   354,     0,     0,   357,   358,   359,   360,     0,
     362,   363,   326,   327,     0,   328,   329,   330,     0,   331,
     332,   333,     0,   334,   335,   336,   337,   338,     0,   339,
     340,   341,   342,   343,   344,   345,     0,   323,   346,   347,
     348,   324,   377,     0,     0,   320,     0,     0,     0,     0,
     951,     0,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   322,     0,     0,     0,     0,     0,     0,
       0,     0,   325,   349,   350,   351,   352,   353,   354,     0,
       0,   357,   358,   359,   360,     0,   362,   363,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,     0,   334,
     335,   336,   337,   338,     0,   339,   340,   341,   342,   343,
     344,   345,     0,   323,   346,   347,   348,   324,   377,     0,
       0,   320,     0,     0,     0,     0,   952,     0,   321,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   322,
       0,     0,     0,     0,     0,     0,     0,     0,   325,   349,
     350,   351,   352,   353,   354,     0,     0,   357,   358,   359,
     360,     0,   362,   363,   326,   327,     0,   328,   329,   330,
       0,   331,   332,   333,     0,   334,   335,   336,   337,   338,
       0,   339,   340,   341,   342,   343,   344,   345,     0,   323,
     346,   347,   348,   324,   377,     0,     0,   320,     0,     0,
       0,     0,   953,     0,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   322,     0,     0,     0,     0,
       0,     0,     0,     0,   325,   349,   350,   351,   352,   353,
     354,     0,     0,   357,   358,   359,   360,     0,   362,   363,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
       0,   334,   335,   336,   337,   338,     0,   339,   340,   341,
     342,   343,   344,   345,     0,   323,   346,   347,   348,   324,
     377,     0,     0,   320,     0,     0,     0,     0,   954,     0,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   322,     0,     0,     0,     0,     0,     0,     0,     0,
     325,   349,   350,   351,   352,   353,   354,     0,     0,   357,
     358,   359,   360,     0,   362,   363,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,     0,   334,   335,   336,
     337,   338,     0,   339,   340,   341,   342,   343,   344,   345,
       0,   323,   346,   347,   348,   324,   377,     0,     0,   320,
       0,     0,     0,     0,   976,     0,   321,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   322,     0,     0,
       0,     0,     0,     0,     0,     0,   325,   349,   350,   351,
     352,   353,   354,     0,     0,   357,   358,   359,   360,     0,
     362,   363,   326,   327,     0,   328,   329,   330,     0,   331,
     332,   333,     0,   334,   335,   336,   337,   338,     0,   339,
     340,   341,   342,   343,   344,   345,     0,   323,   346,   347,
     348,   324,   377,     0,     0,     0,     0,     0,     0,     0,
     977,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   325,   349,   350,   351,   352,   353,   354,     0,
       0,   357,   358,   359,   360,     0,   362,   363,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,     0,   334,
     335,   336,   337,   338,     0,   339,   340,   341,   342,   343,
     344,   345,   320,     0,   346,   347,   348,     0,   377,   321,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     322,   405,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   406,     0,     0,     0,     0,     0,
       0,     0,     0,   320,     0,     0,     0,     0,     0,     0,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   322,   399,     0,     0,     0,     0,     0,     0,     0,
     323,     0,     0,     0,   324,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   320,     0,
       0,     0,     0,     0,     0,   321,     0,     0,     0,     0,
       0,   402,     0,     0,     0,   325,   322,   701,     0,     0,
       0,   323,     0,     0,     0,   324,     0,     0,     0,     0,
       0,   326,   327,     0,   328,   329,   330,     0,   331,   332,
     333,     0,   334,   335,   336,   337,   338,     0,   339,   340,
     341,   342,   343,   344,   345,     0,   325,   346,   347,   348,
       0,   377,     0,     0,     0,     0,   323,     0,     0,   322,
     324,     0,   326,   327,     0,   328,   329,   330,     0,   331,
     332,   333,     0,   334,   335,   336,   337,   338,     0,   339,
     340,   341,   342,   343,   344,   345,   320,   402,   346,   347,
     348,   325,   377,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   322,   239,     0,   326,   327,   323,
     328,   329,   330,   324,   331,   332,   333,     0,   334,   335,
     336,   337,   338,     0,   339,   340,   341,   342,   343,   344,
     345,   320,     0,   346,   347,   348,     0,   377,   321,     0,
       0,     0,     0,     0,   325,     0,     0,     0,     0,   322,
     693,     0,     0,     0,   323,     0,     0,     0,   324,     0,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
       0,   334,   335,   336,   337,     0,     0,   339,   340,   341,
     342,   343,   344,   345,     0,     0,   346,   347,   348,   325,
       0,     0,     0,     0,   322,     0,     0,     0,     0,   323,
       0,     0,     0,   324,     0,   326,   327,     0,   328,   329,
     330,     0,   331,   332,   333,     0,   334,   335,   336,   337,
     338,     0,   339,   340,   341,   342,   343,   344,   345,   320,
       0,   346,   347,   348,   325,   377,   321,     0,     0,     0,
       0,     0,     0,     0,   323,     0,     0,   322,   324,     0,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
       0,   334,   335,   336,   337,   338,     0,   339,   340,   341,
     342,   343,   344,   345,   320,     0,   346,   347,   348,   325,
     377,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,     0,     0,   326,   327,   323,   328,   329,
     330,   324,   331,   332,   333,     0,   334,   335,   336,   337,
     711,     0,   339,   340,   341,   342,   343,     0,   345,     0,
       0,   346,   347,   348,     0,     0,     0,   320,     0,     0,
       0,     0,   325,     0,   321,     0,     0,     0,     0,     0,
       0,     0,   323,     0,     0,   322,   324,     0,   326,   327,
     410,   328,   329,   330,     0,   331,   332,   333,     0,   334,
     335,   336,   337,   338,     0,   339,   340,   341,   342,   343,
     344,   345,     0,     0,   346,   347,   348,   325,   377,     0,
     411,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   326,   327,   323,   328,   329,   330,   324,
     331,   332,   333,     0,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,     0,     0,   346,
     347,   348,     0,   440,   320,     0,     0,     0,     0,     0,
     325,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,     0,     0,     0,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,   379,   334,   335,   336,
     337,   338,     0,   339,   340,   341,   342,   343,   344,   345,
       0,   412,   346,   347,   348,     0,     0,   320,     0,     0,
       0,     0,     0,     0,   321,     0,     0,     0,     0,     0,
       0,     0,   323,     0,     0,   322,   324,     0,     0,     0,
     453,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   159,     0,     0,     0,   325,     0,     0,
     454,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   326,   327,   323,   328,   329,   330,   324,
     331,   332,   333,     0,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,     0,     0,   346,
     347,   348,     0,     0,   320,     0,     0,     0,     0,     0,
     325,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,   540,     0,     0,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,   541,   334,   335,   336,
     337,   338,     0,   339,   340,   341,   342,   343,   344,   345,
     320,     0,   346,   347,   348,     0,     0,   321,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   322,   542,
       0,     0,   323,     0,     0,     0,   324,     0,     0,     0,
       0,     0,   543,     0,     0,     0,     0,     0,     0,     0,
     320,     0,     0,     0,     0,     0,     0,   321,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   323,     0,
       0,     0,   324,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,     0,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,   320,     0,   346,
     347,   348,     0,   325,   321,     0,     0,     0,   323,     0,
       0,     0,   324,     0,     0,   322,     0,     0,     0,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,     0,
     334,   335,   336,   337,   338,     0,   339,   340,   341,   342,
     343,   344,   345,   325,     0,   346,   347,   348,     0,     0,
       0,     0,     0,     0,     0,   322,     0,     0,     0,   326,
     327,     0,   328,   329,   330,   323,   331,   332,   333,   324,
     334,   335,   336,   337,   338,     0,   339,   340,   341,   342,
     343,   344,   345,   320,   444,   346,   347,   348,     0,     0,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     325,   322,     0,     0,     0,   323,     0,     0,     0,   324,
       0,     0,     0,     0,     0,     0,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,     0,   334,   335,   336,
     337,   338,     0,   339,   340,   341,   342,   343,   344,   345,
     320,   446,   346,   347,   348,     0,     0,   321,     0,     0,
       0,   323,     0,     0,     0,   324,   326,   327,   322,   328,
     329,   330,     0,   331,   332,   333,     0,   334,   335,   336,
     337,     0,     0,   339,   340,   341,   342,   343,     0,   345,
     320,     0,   346,   347,   348,     0,   325,   321,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,   326,   327,     0,   328,   329,   330,   323,   331,
     332,   333,   324,   334,   335,   336,   337,   338,     0,   339,
     340,   341,   342,   343,   344,   345,   320,   448,   346,   347,
     348,     0,     0,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   325,   322,     0,     0,     0,   323,     0,
       0,     0,   324,     0,     0,     0,     0,     0,     0,   326,
     327,   544,   328,   329,   330,     0,   331,   332,   333,     0,
     334,   335,   336,   337,   338,     0,   339,   340,   341,   342,
     343,   344,   345,   325,   456,   346,   347,   348,     0,     0,
       0,     0,     0,     0,   323,     0,     0,     0,   324,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,     0,
     334,   335,   336,   337,   338,     0,   339,   340,   341,   342,
     343,   344,   345,   320,     0,   346,   347,   348,   473,   325,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   322,     0,     0,     0,   326,   327,     0,   328,   329,
     330,     0,   331,   332,   333,     0,   334,   335,   336,   337,
     338,     0,   339,   340,   341,   342,   343,   344,   345,     0,
       0,   346,   347,   348,     0,     0,   320,     0,     0,     0,
       0,     0,     0,   321,     0,     0,     0,     0,     0,     0,
       0,   323,     0,     0,   322,   324,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,     0,     0,     0,
       0,     0,     0,   321,     0,     0,   325,     0,     0,     0,
       0,     0,     0,     0,   322,     0,     0,     0,     0,     0,
       0,     0,   326,   327,   323,   328,   329,   330,   324,   331,
     332,   333,     0,   334,   335,   336,   337,   338,  -487,   339,
     340,   341,   342,   343,   344,   345,     0,     0,   346,   347,
     348,     0,     0,   320,   706,   159,     0,     0,     0,   325,
     321,     0,     0,     0,   323,     0,     0,     0,   324,     0,
       0,   322,     0,     0,     0,   326,   327,     0,   328,   329,
     330,     0,   331,   332,   333,     0,   334,   335,   336,   337,
     338,     0,   339,   340,   341,   342,   343,   344,   345,   325,
       0,   346,   347,   348,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   326,   327,     0,   328,   329,
     330,   323,   331,   332,   333,   324,   334,   335,   336,   337,
     338,     0,   339,   340,   341,   342,   343,   344,   345,   320,
     682,   346,   347,   348,     0,     0,   321,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   325,   322,     0,     0,
     709,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   326,   327,     0,   328,   329,   330,     0,   331,
     332,   333,     0,   334,   335,   336,   337,   338,     0,   339,
     340,   341,   342,   343,   344,   345,   320,     0,   346,   347,
     348,     0,     0,   321,     0,     0,     0,   323,     0,     0,
       0,   324,     0,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   320,     0,
       0,     0,     0,     0,     0,   321,     0,     0,     0,     0,
       0,     0,   325,     0,     0,     0,   322,   814,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   326,   327,
       0,   328,   329,   330,   323,   331,   332,   333,   324,   334,
     335,   336,   337,   338,     0,   339,   340,   341,   342,   343,
     344,   345,     0,     0,   346,   347,   348,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,   751,   325,
     324,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   326,   327,     0,   328,   329,
     330,     0,   331,   332,   333,     0,   334,   335,   336,   337,
     338,   325,   339,   340,   341,   342,   343,   344,   345,   320,
       0,   346,   347,   348,     0,     0,   321,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,   322,   334,   335,
     336,   337,   338,     0,   339,   340,   341,   342,   343,   344,
     345,   320,     0,   346,   347,   348,     0,     0,   321,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   322,
     817,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   323,     0,     0,
       0,   324,     0,     0,     0,     0,     0,     0,     0,     0,
     889,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
       0,     0,   325,   324,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,     0,   334,
     335,   336,   337,   338,   325,   339,   340,   341,   342,   343,
     344,   345,   320,     0,   346,   347,   348,     0,     0,   321,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
     322,   334,   335,   336,   337,   338,     0,   339,   340,   341,
     342,   343,   344,   345,   320,   906,   346,   347,   348,     0,
       0,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     323,     0,     0,     0,   324,     0,     0,     0,     0,     0,
       0,     0,     0,   892,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   323,     0,     0,   325,   324,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   326,   327,     0,   328,   329,   330,     0,   331,   332,
     333,     0,   334,   335,   336,   337,   338,   325,   339,   340,
     341,   342,   343,   344,   345,   320,     0,   346,   347,   348,
       0,     0,   321,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,   322,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,   320,     0,   346,
     347,   348,     0,     0,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,     0,     0,     0,   324,     0,     0,
       0,     0,     0,     0,     0,     0,   987,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   323,     0,     0,   325,   324,
       0,     0,     0,     0,     0,     0,     0,     0,   988,     0,
       0,     0,     0,     0,   326,   327,     0,   328,   329,   330,
       0,   331,   332,   333,     0,   334,   335,   336,   337,   338,
     325,   339,   340,   341,   342,   343,   344,   345,   320,     0,
     346,   347,   348,     0,     0,   321,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,   322,   334,   335,   336,
     337,   338,     0,   339,   340,   341,   342,   343,   344,   345,
     320,     0,   346,   347,   348,     0,     0,   321,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,     0,     0,
     324,     0,     0,     0,     0,     0,     0,     0,     0,   989,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   323,     0,
       0,   325,   324,     0,     0,     0,     0,     0,     0,     0,
       0,   990,     0,     0,     0,     0,     0,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,     0,   334,   335,
     336,   337,   338,   325,   339,   340,   341,   342,   343,   344,
     345,   320,     0,   346,   347,   348,     0,     0,   321,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,   322,
     334,   335,   336,   337,   338,     0,   339,   340,   341,   342,
     343,   344,   345,   320,     0,   346,   347,   348,     0,     0,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   322,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
       0,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,     0,   991,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,     0,     0,   325,   324,     0,     0,     0,     0,
       0,     0,     0,     0,   992,     0,     0,     0,     0,     0,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
       0,   334,   335,   336,   337,   338,   325,   339,   340,   341,
     342,   343,   344,   345,   320,     0,   346,   347,   348,     0,
       0,   321,   326,   327,     0,   328,   329,   330,     0,   331,
     332,   333,   322,   334,   335,   336,   337,   338,     0,   339,
     340,   341,   342,   343,   344,   345,   320,     0,   346,   347,
     348,     0,     0,   321,     0,     0,  1014,     0,     0,     0,
       0,     0,     0,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   323,     0,     0,     0,   324,     0,     0,     0,
       0,     0,     0,     0,     0,   320,     0,     0,     0,     0,
       0,     0,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   322,   323,     0,     0,   325,   324,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,     0,   334,   335,   336,   337,   338,   325,
     339,   340,   341,   342,   343,   344,   345,     0,     0,   346,
     347,   348,     0,   670,     0,   326,   327,   324,   328,   329,
     330,     0,   331,   332,   333,     0,   334,   335,   336,   337,
     338,     0,   339,   340,   341,   342,   343,   344,   345,     0,
       0,   346,   347,   348,     0,     0,     0,     0,   325,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   326,   327,     0,   328,   329,   330,
       0,   331,   332,   333,     0,   334,   335,   336,   337,   338,
       0,   339,   340,   341,   342,   343,   344,   345,     0,    16,
     346,   347,   348,    20,    21,     0,     0,     0,     0,     0,
       0,     0,    27,    16,    28,   871,     0,    20,    21,    33,
       0,     0,     0,     0,    37,     0,    27,     0,    28,   871,
       0,     0,     0,    33,     0,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,     0,    50,     0,    52,     0,
      54,     0,   872,     0,     0,   873,     0,    59,     0,    60,
      50,     0,    52,     0,   306,     0,   872,     0,     0,   873,
       0,    59,   648,    60,   210,     0,     0,     0,    76,     0,
       0,    78,     0,     0,    81,     0,   211,   212,     0,     0,
       0,     0,    76,   213,     0,    78,     0,     0,    81,     0,
       0,   214,     0,     0,     0,     0,     0,   215,     0,     0,
       0,     0,   216,     0,   217,     0,     0,   218,     0,     0,
       0,     0,     0,     0,     0,   219,     0,     0,     0,     0,
       0,   210,   220,     0,    95,     0,     0,     0,   221,     0,
     950,     0,     0,   211,   212,     0,     0,   222,    95,     0,
     213,     0,     0,     0,     0,     0,   223,   224,   214,   225,
       0,   226,     0,   227,   215,     0,   228,     0,     0,   216,
     229,   217,     0,   230,   218,     0,   231,     0,     0,     0,
       0,     0,   219,     0,     0,     0,     0,     0,     0,   220,
       0,     0,     0,     0,     0,   221,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,     0,     0,     0,     0,
       0,     0,     0,   223,   224,     0,   225,     0,   226,     0,
     227,     0,     0,   228,     0,     0,     0,   229,     0,     0,
     230,     0,     0,   231
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-878)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,   144,     2,   182,    15,    61,   370,    18,    17,   449,
     131,    28,    20,    24,    56,    57,   195,    28,    29,   566,
      31,    32,    33,    34,    80,   727,   209,   657,   847,     1,
     753,    42,     1,    44,    45,   599,    42,    29,    49,    29,
      51,   418,    53,    99,   921,   421,    42,    29,     1,    24,
      61,    62,    63,    64,    65,    66,   426,   109,    42,    70,
       3,    72,   183,    74,    75,   203,   249,    29,    79,    80,
       1,   109,    83,    84,    29,    86,   135,    88,    42,   449,
      91,    92,   844,    94,   135,   268,    97,    98,    99,    42,
     805,   806,    81,    68,   129,    64,    64,    42,   959,     0,
     154,   154,   137,   162,    79,   669,   135,   809,   161,     3,
     163,   162,   164,    98,   170,     1,   935,   109,    31,    32,
     667,    52,    42,    98,    28,   109,   164,   109,   190,   135,
      52,     3,   275,   162,    79,    19,    42,   106,   106,   135,
      81,    19,   114,  1004,  1021,   135,   848,   209,   910,    92,
      81,   135,   875,   164,   165,   166,   167,   168,   169,   170,
     724,   114,   134,   135,   302,   303,    52,   159,   164,   159,
     159,   135,   181,   109,    78,   898,   107,   159,    82,   190,
     164,   134,   135,   366,   367,   107,    99,   249,   205,   197,
     162,   160,   160,    77,   205,    81,   204,   159,   209,    77,
     164,   154,   566,   134,   159,   190,   268,   909,    94,    81,
      24,   135,    42,   154,    62,   135,    24,   101,   159,   161,
      92,   107,   598,   101,   209,   129,   941,   942,   159,   135,
     134,   161,   136,   137,   411,  1004,   135,   141,   249,   250,
     164,   114,    74,    75,   164,   149,   257,   258,   134,    79,
      98,   155,   156,   157,    68,   885,   958,   268,   164,   442,
      68,    47,   135,   162,   249,    79,   139,   114,   154,    42,
     332,    79,  1041,   159,   395,   135,   135,   454,   107,   135,
     135,  1019,   829,   268,    98,   129,   297,    73,   135,   162,
     104,    64,   139,   137,   294,   154,   104,    79,   154,  1037,
     160,    74,    75,   667,   366,   367,   135,   162,   319,   320,
     321,   322,   323,   324,   325,   162,   327,   328,   329,   330,
     331,   114,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   114,   716,
     135,   397,   718,   135,   355,   356,   139,   135,    24,     7,
     361,   135,   161,   364,   365,   366,   367,   368,   369,   370,
     135,   161,   154,   135,   140,   160,   142,   143,   379,    89,
     146,   147,   135,   384,   162,   386,   753,   135,   162,    52,
     442,   366,   367,   161,    74,   163,   397,   162,   399,   400,
     162,   154,    68,   965,   405,   406,   154,   135,   159,   410,
     411,   409,   413,    79,   415,   154,   134,   180,    81,   137,
     323,   324,   154,   141,   854,    78,   154,   428,   160,    82,
       3,    94,    98,   153,   435,   481,   135,   200,   201,   202,
     203,   442,   570,   571,   107,   208,  1008,  1009,  1010,  1011,
    1012,  1013,   453,   454,   623,   154,   161,    30,   163,   460,
     435,   161,   463,   163,   465,   368,   369,   442,   469,   470,
     471,   154,   473,   474,    78,   829,   379,   160,    82,   242,
     481,    54,   245,    28,   161,   460,   163,   847,   463,   479,
     465,   154,   154,   154,   854,   470,   159,   400,   145,   154,
     463,   264,   465,   406,   556,   154,   634,   635,   154,   154,
     161,   154,   137,   880,   154,   159,   108,   355,   356,    92,
     161,   284,   161,   361,   657,    98,   364,   365,   154,   161,
     114,   898,   154,    78,    56,     7,   134,    82,   159,   540,
     541,   542,   543,   544,   545,   546,   154,   114,   547,   548,
     162,     2,   135,   554,    28,   556,    42,   154,   604,   560,
      11,   562,   161,    31,   135,   566,   469,   568,   162,    24,
     114,   162,    23,   162,    25,   935,   162,    18,   135,   554,
     159,   556,   164,   164,   129,    28,   154,    42,   154,   134,
      45,    64,   137,   154,    52,   154,   141,   162,   154,   959,
     164,   602,   603,   604,    78,   154,   135,   608,    82,    31,
      65,   154,   154,    68,   974,    70,   162,   380,   154,    70,
     160,   162,   154,    81,    79,   463,   135,   465,    29,   160,
     114,    79,    29,   159,   164,    78,    94,    31,   541,    82,
     543,   404,   134,    98,  1004,   408,    97,    79,   164,   107,
     101,   134,   164,   164,   160,   129,   161,   137,   137,  1019,
     134,   137,   136,   137,   129,    31,   667,   141,   137,   670,
     671,   672,    42,   137,   675,   149,   137,  1037,   137,   129,
     135,  1041,    52,   157,   137,   137,   129,   130,   734,   129,
     691,   134,   693,   136,   137,   137,   154,   698,   141,   129,
     701,   159,   137,   704,   160,   706,   149,   162,   709,   160,
      94,    81,   155,   156,   157,   162,   554,   162,   129,   129,
      52,   135,   453,   308,   454,   164,    22,   200,   201,   202,
     203,   558,   738,   734,   196,   822,   756,   107,   276,   885,
     880,   759,  1021,   716,   114,   746,   958,   748,   591,    81,
     751,   983,   885,  1014,   755,  1007,    33,   429,    -1,    24,
     556,    -1,    94,    -1,     2,   135,    -1,   670,   671,   242,
     608,    -1,   245,    11,    -1,   107,    -1,    42,    -1,    -1,
      45,   232,    -1,    -1,   154,    23,    -1,    25,   239,   159,
      -1,   264,   162,    -1,    -1,    -1,    -1,    -1,    -1,   800,
      65,   564,   565,    68,    -1,    70,    -1,   570,   571,    -1,
      -1,   284,   813,   814,    79,   816,   817,   818,   819,   582,
      -1,    -1,   154,   824,   825,    -1,   827,   159,   829,    -1,
     831,   832,    70,    98,   835,    -1,    74,    75,    -1,    -1,
     872,   873,    -1,   294,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   853,   854,    -1,    -1,   619,   308,   859,    97,
     861,    -1,    -1,   101,   865,   911,    -1,    -1,    -1,    -1,
     871,    -1,   847,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   889,   890,
     891,   892,   893,   894,   895,   896,    -1,    -1,    -1,    -1,
      23,    24,    -1,    -1,    -1,   906,    -1,   380,    -1,    -1,
     911,    34,    35,    -1,    -1,   916,    -1,   680,   681,   920,
      -1,    44,   160,    -1,    -1,   688,   377,   690,    -1,    -1,
      -1,   404,   978,    -1,   697,   408,    -1,   700,    -1,    -1,
     703,   916,    -1,    66,    -1,    68,    -1,    -1,   399,    -1,
     951,   952,   953,   954,   405,    -1,    79,    -1,    -1,     2,
      42,    -1,   413,    -1,    -1,    -1,    -1,    -1,    11,    -1,
      52,    -1,    -1,   974,    -1,   976,   977,   978,   741,    -1,
      23,   104,    25,    -1,   959,    -1,   987,   988,   989,   990,
     991,   992,    -1,    24,   232,    -1,    42,    -1,    -1,    81,
      -1,   239,  1003,  1004,    -1,  1006,    52,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    45,    -1,   467,    -1,    -1,    -1,
     471,    -1,    -1,    -1,    -1,   107,    -1,    70,   479,  1004,
      -1,    -1,   114,    -1,    65,    81,  1037,    68,    -1,    70,
    1041,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,   135,    97,    -1,   294,    -1,   101,    -1,
      -1,   107,    -1,     3,    -1,    -1,  1041,    98,   114,    -1,
     308,    -1,   154,    -1,    -1,    15,    16,   159,    -1,    -1,
     162,    -1,    22,    -1,    -1,    -1,   537,    -1,    -1,   135,
      30,   564,   565,    -1,   857,    -1,    36,   570,   571,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,   154,    -1,
       2,    -1,    -1,   159,    54,    -1,   162,   160,    -1,    11,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
     581,    23,    -1,    25,    -1,    -1,    76,   900,   901,   377,
      -1,   904,   905,    -1,    -1,    85,    86,    -1,    88,    -1,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,   921,    99,
     100,   399,   102,    -1,   927,   105,   929,   405,   931,    -1,
      -1,    -1,    -1,    -1,    -1,   413,    -1,    -1,    70,    -1,
      -1,    -1,   633,    -1,    -1,    -1,    -1,    -1,   639,   232,
      -1,    -1,    -1,    -1,    -1,    -1,   239,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,   657,   680,   681,   101,
      -1,    23,    24,    -1,    -1,   688,    -1,    -1,    -1,   670,
     671,   161,    34,    35,    -1,    -1,    -1,   700,    -1,   467,
     703,    -1,    44,   471,    -1,    47,     3,    -1,    -1,    -1,
     691,   479,   693,    -1,    -1,    -1,    -1,   698,    15,    16,
     701,   294,    -1,    -1,    66,    22,    68,   708,  1021,    -1,
     711,    73,    -1,    30,    -1,   308,    -1,    79,   160,    36,
      28,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,   733,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,   104,    -1,    61,   746,    -1,   748,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      78,    88,    -1,    90,    82,    92,    -1,    -1,    95,    -1,
      -1,    -1,    99,    -1,   377,   102,    -1,    -1,   105,    -1,
     232,    -1,    -1,   581,    -1,    -1,     2,   239,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,   399,    -1,    -1,    -1,
      -1,    -1,   405,    -1,    -1,    -1,    -1,    23,    -1,    25,
     413,   129,   130,   824,   132,   133,   134,    -1,   136,   137,
     831,    -1,    -1,   141,   857,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   843,    -1,   161,   633,    -1,   155,   156,   157,
      -1,   639,   294,    -1,   855,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   864,    70,    -1,   308,   868,    -1,    -1,
      -1,    -1,    -1,    -1,   467,    -1,    -1,    28,   471,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   479,    -1,    -1,    -1,
      -1,    97,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   691,    -1,   693,   907,    -1,    -1,    -1,
     698,    -1,    -1,   701,    -1,    -1,    -1,    -1,    -1,    -1,
     708,     2,    -1,   711,    -1,    -1,    -1,    78,    -1,    -1,
      11,    82,    -1,    -1,    -1,   377,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    25,   733,    -1,    -1,    -1,    -1,
     951,   952,   953,   954,   160,    -1,    -1,   399,   746,    -1,
     748,    -1,    -1,   405,    28,    -1,    -1,    -1,    -1,    -1,
      -1,   413,    -1,    -1,    -1,   976,   977,    -1,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,   581,    70,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    97,    -1,    82,    -1,
     101,    -1,    -1,    -1,    -1,   467,   232,    -1,    -1,   471,
      -1,    -1,    -1,   239,    -1,    -1,   824,   479,    -1,    -1,
     633,    -1,    -1,   831,    -1,    -1,   639,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   843,    -1,    -1,    78,    -1,
      -1,    -1,    82,    -1,    -1,   129,   130,   855,    -1,   133,
     134,    -1,   136,   137,    -1,    -1,   864,   141,    -1,   160,
     868,    -1,    -1,    -1,    -1,   149,    -1,    -1,   294,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,    -1,   691,    -1,
     693,    -1,   308,    -1,    -1,   698,    -1,    -1,   701,   129,
     130,    -1,   132,   133,   134,   708,   136,   137,   711,   907,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,   581,
     733,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   232,    -1,   746,    -1,   748,    -1,    -1,   239,    -1,
      -1,    -1,    -1,   951,   952,   953,   954,    -1,    -1,    -1,
      -1,   377,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   976,   977,
      -1,   633,    -1,   399,    -1,    -1,    -1,   639,    -1,   405,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   413,    -1,    -1,
      -1,    -1,     2,   294,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    11,    -1,    -1,    -1,    -1,   124,   308,   126,    -1,
      -1,   824,    -1,    23,    -1,    25,    -1,    -1,   831,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   691,
     843,   693,    -1,    -1,    -1,    -1,   698,    -1,    -1,   701,
      -1,   467,   855,    -1,    -1,   471,   708,    -1,    -1,   711,
      -1,   864,    -1,   479,    -1,   868,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,
      -1,   733,    -1,    -1,    -1,    -1,   377,   195,    -1,    -1,
     198,    -1,    -1,    -1,   746,    -1,   748,    97,    -1,    -1,
      -1,   101,    -1,     2,   907,    -1,    -1,    -1,   399,    -1,
      -1,    -1,    11,    -1,   405,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   413,    -1,    23,    -1,    25,   235,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   253,    -1,    -1,   951,   952,
     953,   954,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    19,    -1,    -1,   581,    23,    24,    -1,    -1,
      -1,    70,   824,   976,   977,    32,   467,    34,    35,   831,
     471,    -1,    39,    -1,    -1,    -1,    -1,    44,   479,    -1,
      -1,   843,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,   101,   855,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    68,   864,    70,    -1,    72,   868,   633,    75,    -1,
      77,    -1,    79,   639,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   232,     3,    -1,    -1,    -1,    -1,    -1,   239,
      -1,    98,    -1,    -1,   101,    15,    -1,   104,    -1,    -1,
      -1,    -1,    22,    -1,    -1,   907,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,   691,    46,   693,    -1,    -1,
      -1,    -1,   698,    -1,    54,   701,   394,    -1,    -1,    -1,
     581,    61,   708,    -1,   294,   711,    -1,   154,    -1,   951,
     952,   953,   954,   160,    -1,    -1,    76,    -1,   308,    -1,
      -1,    -1,    -1,   421,    -1,    -1,    -1,   733,    88,    -1,
      -1,    -1,    92,    -1,   976,   977,    -1,    -1,    -1,    99,
     746,    -1,   748,   232,    -1,   105,    -1,    -1,    -1,    -1,
     239,    -1,   633,    -1,    -1,    -1,    -1,    -1,   639,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,     2,    -1,    -1,
      -1,    -1,   480,    -1,    -1,   483,    11,   377,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      25,    -1,    -1,   158,    -1,   294,   161,    -1,   163,   399,
     691,    -1,   693,    -1,    -1,   405,    -1,   698,   824,   308,
     701,    -1,    -1,   413,    -1,   831,    -1,   708,    -1,    -1,
     711,    -1,    -1,    -1,    -1,    -1,    -1,   843,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,   855,
      -1,    -1,   733,    -1,    -1,    -1,    -1,    -1,   864,    -1,
      -1,    -1,   868,    -1,    -1,   746,    -1,   748,    -1,    -1,
      -1,    -1,    97,    -1,    -1,    -1,   101,   467,    -1,    -1,
      -1,   471,    -1,    -1,    -1,    -1,    -1,    -1,   377,   479,
      -1,    -1,    -1,    -1,    -1,    -1,   594,    -1,    -1,    -1,
     598,   907,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     399,    -1,    -1,    -1,    -1,    -1,   405,    -1,    -1,    -1,
      -1,   619,    -1,    -1,   413,   623,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   824,    -1,   951,   952,   953,   954,    -1,
     831,    -1,    19,    -1,    -1,    -1,    23,    24,    -1,   657,
      -1,    -1,   843,    -1,    -1,    32,    -1,    34,    35,    -1,
     976,   977,    39,    -1,   855,    -1,    -1,    44,   467,    -1,
      47,    -1,   471,   864,    -1,    -1,    -1,   868,    -1,    -1,
     479,   581,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    68,    -1,    70,    -1,    72,    73,   232,    75,    -1,
      77,    -1,    79,    -1,   239,    -1,    -1,   715,    -1,    -1,
     718,    -1,    -1,    -1,    -1,    -1,   907,    -1,    -1,    -1,
      -1,    98,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     738,    -1,    -1,   633,    -1,    -1,    -1,    -1,    -1,   639,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   756,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,
     951,   952,   953,   954,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   780,   308,    -1,    -1,    -1,   154,    -1,    -1,
      -1,    -1,   581,   160,    -1,   976,   977,    -1,    -1,    -1,
      -1,   691,    -1,   693,    -1,    -1,    -1,    -1,   698,    -1,
      -1,   701,    -1,    -1,    -1,    -1,    -1,    -1,   708,    -1,
      -1,   711,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    16,    -1,   733,   633,    -1,    -1,    22,    -1,    -1,
     639,    -1,   377,    -1,    -1,    30,   746,    -1,   748,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,   399,    -1,    -1,    -1,    -1,    54,
     405,    -1,    -1,    -1,    -1,    -1,    61,   885,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,   691,    -1,   693,    -1,    -1,    -1,    -1,   698,
      85,    86,   701,    88,    -1,    90,    -1,    92,    -1,   708,
      95,    -1,   711,    -1,    99,    -1,    -1,   102,    -1,     1,
     105,    -1,    -1,    -1,   824,    -1,    -1,    -1,    -1,    -1,
      -1,   831,   467,    -1,   733,    -1,   471,    19,    -1,    -1,
      -1,    23,    24,   843,   479,    -1,    -1,   746,    -1,   748,
      32,    -1,    34,    35,    -1,   855,    -1,    39,    -1,    -1,
      -1,    -1,    44,    -1,   864,    47,    -1,    -1,   868,    -1,
      -1,    -1,    -1,    -1,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    68,    10,    70,    -1,
      72,    73,    -1,    75,    17,    77,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,   907,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,
      -1,    -1,   104,    -1,    -1,   824,    -1,    -1,    -1,    -1,
      -1,    -1,   831,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   843,    -1,   581,    -1,    -1,    -1,
      -1,   951,   952,   953,   954,    78,   855,    -1,    -1,    82,
      -1,    -1,    -1,    -1,    -1,   864,    -1,    -1,    -1,   868,
      -1,    -1,   154,    -1,    -1,    -1,   976,   977,   160,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   633,    -1,
      -1,    -1,    -1,    -1,   639,    -1,   129,   130,   907,   132,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   951,   952,   953,   954,   691,    -1,   693,    -1,
      -1,    -1,    -1,   698,    -1,    -1,   701,    -1,    -1,    -1,
      -1,    -1,    -1,   708,    -1,    -1,   711,   976,   977,    -1,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,   733,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,   746,    32,   748,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    -1,    95,    96,    97,    98,    99,
      -1,   101,   102,   103,   104,   105,    -1,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   843,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
     855,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   864,
      -1,   151,   152,   868,   154,    -1,    -1,   157,    -1,   159,
      -1,   161,     1,   163,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,   907,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,
      99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,    -1,   157,    -1,
     159,    -1,   161,    -1,   163,   164,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    10,    -1,    12,    13,    14,
      15,    16,    17,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    -1,    -1,
      -1,    66,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    -1,
      95,    96,    97,    98,    99,    -1,   101,   102,   103,   104,
     105,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    -1,   159,   160,   161,     1,   163,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    -1,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      -1,    95,    96,    97,    98,    99,    -1,   101,   102,   103,
     104,   105,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,   153,
     154,    -1,    -1,   157,    -1,   159,   160,   161,     1,   163,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    -1,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    -1,    95,    96,    97,    98,    99,    -1,   101,   102,
     103,   104,   105,    -1,    -1,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
      -1,   154,    -1,    -1,   157,    -1,   159,   160,   161,     1,
     163,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    -1,    60,    61,
      62,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      -1,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    -1,    95,    96,    97,    98,    99,    -1,   101,
     102,   103,   104,   105,    -1,    -1,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,
     152,    -1,   154,    -1,    -1,   157,    -1,   159,   160,   161,
       1,   163,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    -1,    95,    96,    97,    98,    99,    -1,
     101,   102,   103,   104,   105,    -1,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,
     151,   152,    -1,   154,    -1,    -1,   157,    -1,   159,   160,
     161,     1,   163,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    -1,    95,    96,    97,    98,    99,
      -1,   101,   102,   103,   104,   105,    -1,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,
      -1,   151,   152,    -1,   154,    -1,    -1,   157,    -1,   159,
     160,   161,     1,   163,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    80,    81,    -1,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    -1,    95,    96,    97,    98,
      99,    -1,   101,   102,   103,   104,   105,    -1,    -1,   108,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
      -1,    -1,   151,   152,    -1,   154,    -1,    -1,   157,    -1,
     159,    -1,   161,     1,   163,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    -1,    95,    96,    97,
      98,    99,    -1,   101,   102,   103,   104,   105,    -1,    -1,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,    -1,    -1,   151,   152,    -1,   154,    -1,    -1,   157,
      -1,   159,    -1,   161,     1,   163,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    -1,    -1,    -1,    66,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    80,    81,    -1,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    -1,    95,    96,
      97,    98,    99,    -1,   101,   102,   103,   104,   105,    -1,
      -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,    -1,    -1,   151,   152,    -1,   154,    -1,    -1,
     157,    -1,   159,    -1,   161,     1,   163,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    -1,    95,
      96,    97,    98,    99,    -1,   101,   102,   103,   104,   105,
      -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,    -1,    -1,   151,   152,    -1,   154,    -1,
      -1,   157,    -1,   159,    -1,   161,     1,   163,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    -1,    -1,
      -1,    66,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    80,    81,    -1,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    -1,
      95,    96,    97,    98,    99,    -1,   101,   102,   103,   104,
     105,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,    -1,    -1,   151,   152,    -1,   154,
      -1,    -1,   157,    -1,   159,    -1,   161,     1,   163,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    -1,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      -1,    95,    96,    97,    98,    99,    -1,   101,   102,   103,
     104,   105,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,    -1,
     154,    -1,    -1,   157,    -1,   159,    -1,   161,     1,   163,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      -1,    -1,    -1,    66,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    -1,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    -1,    95,    96,    97,    98,    99,    -1,   101,   102,
     103,   104,   105,    -1,    -1,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
      -1,   154,    -1,    -1,   157,    -1,   159,    -1,   161,     1,
     163,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    -1,    60,    61,
      62,    -1,    -1,    -1,    66,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      -1,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    -1,    95,    96,    97,    98,    99,    -1,   101,
     102,   103,   104,   105,    -1,    -1,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,
     152,    -1,   154,    -1,    -1,   157,    -1,   159,    -1,   161,
       1,   163,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    -1,    95,    96,    97,    98,    99,    -1,
     101,   102,   103,   104,   105,    -1,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,
     151,   152,    -1,   154,    -1,    -1,   157,    -1,   159,    -1,
     161,     1,   163,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    -1,    -1,    -1,    66,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    -1,    95,    96,    97,    98,    99,
      -1,   101,   102,   103,   104,   105,    -1,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,
      -1,   151,   152,    -1,   154,    -1,    -1,   157,    -1,   159,
      -1,   161,     1,   163,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    -1,    -1,    -1,    66,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    80,    81,    -1,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    -1,    95,    96,    97,    98,
      99,    -1,   101,   102,   103,   104,   105,    -1,    -1,   108,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
      -1,    -1,   151,   152,    -1,   154,    -1,    -1,   157,    -1,
     159,    -1,   161,     1,   163,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
      -1,   159,    -1,   161,    -1,   163,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,    -1,   159,    -1,   161,    -1,   163,   164,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,
      -1,    -1,   157,    -1,   159,    -1,   161,    -1,   163,   164,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    87,    88,    89,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
      -1,    -1,    -1,    -1,   157,    -1,   159,    -1,   161,    -1,
     163,   164,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    -1,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    90,
      -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,
      -1,   102,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,    -1,   129,   130,
     131,   132,   133,    10,    -1,    -1,    -1,   138,    15,    16,
      17,    -1,    -1,    -1,    -1,    22,    -1,   148,    -1,    -1,
     151,   152,    -1,    30,    -1,    -1,   157,    -1,   159,    36,
     161,    -1,   163,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      -1,    88,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,   132,   133,    -1,    -1,   136,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    -1,   152,    -1,    -1,   155,   156,
     157,   158,    -1,    -1,   161,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    19,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      98,    99,    -1,   101,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
     148,    -1,    12,   151,   152,    15,    16,    -1,    -1,   157,
      -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    96,    97,    -1,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,   161,    -1,   163,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    -1,    95,    96,    97,    -1,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,    -1,   157,    -1,
     159,    -1,   161,   162,   163,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    96,    97,
      -1,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
      -1,   159,    -1,   161,   162,   163,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    42,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    99,    -1,    -1,   102,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,    -1,    -1,   151,   152,    -1,    -1,    -1,    -1,
     157,    -1,   159,    -1,   161,    -1,   163,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    19,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    98,    99,    -1,   101,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,   148,    -1,    12,   151,   152,    15,    16,    -1,
      -1,   157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    96,    97,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,    -1,    -1,   151,   152,    -1,   154,    -1,    -1,   157,
      -1,   159,    -1,   161,    -1,   163,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,    -1,   159,    -1,   161,    -1,   163,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    59,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,   148,    -1,    12,   151,   152,    15,    16,    -1,
      -1,   157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,   111,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
     148,    -1,    12,   151,   152,    15,    16,    -1,    -1,   157,
      -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,   111,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,   161,    -1,   163,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    -1,    95,    96,    97,    -1,
      99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   148,
      -1,    12,   151,   152,    15,    16,    -1,    -1,   157,    -1,
     159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    90,
      -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,    -1,   157,    -1,   159,   160,
     161,    -1,   163,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,   111,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,   148,    -1,
      12,   151,   152,    15,    16,    -1,    -1,   157,    -1,   159,
      22,   161,    -1,   163,    -1,    -1,    -1,    29,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,   148,    -1,    12,   151,
     152,    15,    16,    -1,    -1,   157,    -1,   159,    22,   161,
      -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,   153,
      -1,    -1,    -1,   157,    -1,   159,    -1,   161,    -1,   163,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    87,    88,    89,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,    -1,   157,    -1,   159,    -1,   161,    -1,
     163,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,   161,
      -1,   163,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    90,
      -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,
     161,    -1,   163,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,   148,    -1,
      12,   151,   152,    15,    16,    -1,    -1,   157,    -1,   159,
      22,   161,    -1,   163,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,   148,    -1,    12,   151,
     152,    15,    16,    -1,    -1,   157,    -1,   159,    22,   161,
      -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   148,    -1,    12,   151,   152,    15,
      16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,   163,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,   148,    -1,    12,   151,   152,    15,    16,    -1,
      -1,   157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
     148,    -1,    12,   151,   152,    15,    16,    -1,    -1,   157,
      -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,   148,    -1,
      12,   151,   152,    15,    16,    -1,    -1,   157,    -1,   159,
      22,   161,    -1,   163,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,   148,    -1,    12,   151,
     152,    15,    16,    -1,    -1,   157,    -1,   159,    22,   161,
      -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   148,    -1,    12,   151,   152,    15,
      16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,   163,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,   148,    -1,    12,   151,   152,    15,    16,    -1,
      -1,   157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
     148,    -1,    12,   151,   152,    15,    16,    -1,    -1,   157,
      -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,   148,    -1,
      12,   151,   152,    15,    16,    -1,    -1,   157,    -1,   159,
      22,   161,    -1,   163,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,   148,    -1,    12,   151,
     152,    15,    16,    -1,    -1,   157,    -1,   159,    22,   161,
      -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   148,    -1,    12,   151,   152,    15,
      16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,   163,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,   148,    -1,    12,   151,   152,    15,    16,    -1,
      -1,   157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
     148,    -1,    12,   151,   152,    15,    16,    -1,    -1,   157,
      -1,    -1,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,   148,    15,
      16,    -1,   152,    -1,    -1,   158,    22,   157,   161,   159,
     163,   161,    -1,   163,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,   131,    -1,    -1,    -1,   135,
      22,    -1,    -1,   139,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,   153,    40,    41,
      -1,    43,    -1,   159,    46,   161,   162,   163,    50,    51,
      52,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    94,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,   154,    -1,    -1,    -1,    -1,   159,    30,   161,
      -1,   163,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,   131,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,   159,    46,   161,
      -1,   163,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,   131,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,   159,    46,   161,    -1,   163,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    15,    16,    -1,    99,    -1,    -1,   102,    22,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,   161,    -1,   163,
      -1,     3,    85,    86,    -1,    88,    -1,    90,    10,    92,
      -1,    -1,    95,    15,    16,    17,    99,    -1,    -1,   102,
      22,    -1,   105,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    10,    67,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    76,    -1,    -1,    -1,   161,    -1,
      -1,    -1,    28,    85,    86,    -1,    88,    -1,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
     132,   133,    78,    -1,   136,    -1,    82,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,    -1,
     152,    10,    -1,   155,   156,   157,   158,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,    -1,    10,   155,
     156,   157,    -1,    -1,    -1,    17,    -1,    -1,   164,    78,
      -1,    -1,    -1,    82,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,   113,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
     129,   130,    -1,   132,   133,   134,    78,   136,   137,   138,
      82,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    10,    -1,    -1,    -1,   164,    -1,    -1,    17,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    78,    -1,    -1,    28,
      82,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,   113,    -1,   155,   156,   157,    -1,    -1,    -1,    -1,
      28,    -1,   164,    -1,    -1,    -1,    -1,   129,   130,    78,
     132,   133,   134,    82,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,    10,    -1,   155,   156,   157,    -1,    -1,    17,    -1,
     162,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    28,
      78,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,    78,
      -1,   129,   130,    82,   132,   133,   134,    10,   136,   137,
     138,    -1,    91,   141,    17,    -1,    -1,    -1,    -1,    -1,
     148,   149,    -1,    -1,   152,    28,    -1,   155,   156,   157,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    -1,    78,   155,   156,   157,    82,
     159,    -1,    -1,    10,    -1,    -1,    -1,    -1,    91,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      -1,    78,   155,   156,   157,    82,   159,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    91,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,    -1,    78,   155,   156,
     157,    82,   159,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      91,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,    -1,    78,   155,   156,   157,    82,   159,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    91,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    -1,    78,
     155,   156,   157,    82,   159,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    91,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    -1,    78,   155,   156,   157,    82,
     159,    -1,    -1,    10,    -1,    -1,    -1,    -1,    91,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      -1,    78,   155,   156,   157,    82,   159,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    91,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,    -1,    78,   155,   156,
     157,    82,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,    10,    -1,   155,   156,   157,    -1,   159,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    82,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,   113,    28,    29,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,
      -1,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,    -1,   113,   155,   156,   157,
      -1,   159,    -1,    -1,    -1,    -1,    78,    -1,    -1,    28,
      82,    -1,   129,   130,    -1,   132,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,    10,   109,   155,   156,
     157,   113,   159,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    29,    -1,   129,   130,    78,
     132,   133,   134,    82,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,    10,    -1,   155,   156,   157,    -1,   159,    17,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    28,
      29,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,   150,   151,   152,    -1,    -1,   155,   156,   157,   113,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    82,    -1,   129,   130,    -1,   132,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,   143,
     144,    -1,   146,   147,   148,   149,   150,   151,   152,    10,
      -1,   155,   156,   157,   113,   159,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    28,    82,    -1,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    10,    -1,   155,   156,   157,   113,
     159,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,   129,   130,    78,   132,   133,
     134,    82,   136,   137,   138,    -1,   140,   141,   142,   143,
      91,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,   113,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    28,    82,    -1,   129,   130,
      33,   132,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,    -1,    -1,   155,   156,   157,   113,   159,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,    78,   132,   133,   134,    82,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,    -1,    -1,   155,
     156,   157,    -1,   159,    10,    -1,    -1,    -1,    -1,    -1,
     113,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,    42,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    28,    82,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,   113,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,    78,   132,   133,   134,    82,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
     113,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    -1,    -1,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,    42,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      10,    -1,   155,   156,   157,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,    78,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    82,   129,   130,    -1,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,    10,    -1,   155,
     156,   157,    -1,   113,    17,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    82,    -1,    -1,    28,    -1,    -1,    -1,   129,
     130,    -1,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,   113,    -1,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,   129,
     130,    -1,   132,   133,   134,    78,   136,   137,   138,    82,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,    10,   154,   155,   156,   157,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    28,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      10,   154,   155,   156,   157,    -1,    -1,    17,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    82,   129,   130,    28,   132,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,   150,    -1,   152,
      10,    -1,   155,   156,   157,    -1,   113,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,   129,   130,    -1,   132,   133,   134,    78,   136,
     137,   138,    82,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,    10,   154,   155,   156,
     157,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    28,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,    91,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,   113,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,   129,
     130,    -1,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,    10,    -1,   155,   156,   157,   112,   113,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,   129,   130,    -1,   132,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,   143,
     144,    -1,   146,   147,   148,   149,   150,   151,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    28,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,    78,   132,   133,   134,    82,   136,
     137,   138,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,    -1,   155,   156,
     157,    -1,    -1,    10,    11,   109,    -1,    -1,    -1,   113,
      17,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,
      -1,    28,    -1,    -1,    -1,   129,   130,    -1,   132,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,   143,
     144,    -1,   146,   147,   148,   149,   150,   151,   152,   113,
      -1,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,
     134,    78,   136,   137,   138,    82,   140,   141,   142,   143,
     144,    -1,   146,   147,   148,   149,   150,   151,   152,    10,
     154,   155,   156,   157,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    28,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,    -1,   132,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,    10,    -1,   155,   156,
     157,    -1,    -1,    17,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    82,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
      -1,   132,   133,   134,    78,   136,   137,   138,    82,   140,
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,   112,   113,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,   143,
     144,   113,   146,   147,   148,   149,   150,   151,   152,    10,
      -1,   155,   156,   157,    -1,    -1,    17,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,    28,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,    10,    -1,   155,   156,   157,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,   113,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,   143,   144,   113,   146,   147,   148,   149,   150,
     151,   152,    10,    -1,   155,   156,   157,    -1,    -1,    17,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      28,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    10,    11,   155,   156,   157,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,   113,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,   113,   146,   147,
     148,   149,   150,   151,   152,    10,    -1,   155,   156,   157,
      -1,    -1,    17,   129,   130,    -1,   132,   133,   134,    -1,
     136,   137,   138,    28,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,    10,    -1,   155,
     156,   157,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,   113,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
     113,   146,   147,   148,   149,   150,   151,   152,    10,    -1,
     155,   156,   157,    -1,    -1,    17,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,    28,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      10,    -1,   155,   156,   157,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,   113,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,   113,   146,   147,   148,   149,   150,   151,
     152,    10,    -1,   155,   156,   157,    -1,    -1,    17,   129,
     130,    -1,   132,   133,   134,    -1,   136,   137,   138,    28,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,    10,    -1,   155,   156,   157,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,   113,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,   143,   144,   113,   146,   147,   148,
     149,   150,   151,   152,    10,    -1,   155,   156,   157,    -1,
      -1,    17,   129,   130,    -1,   132,   133,   134,    -1,   136,
     137,   138,    28,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,    10,    -1,   155,   156,
     157,    -1,    -1,    17,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    78,    -1,    -1,   113,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,   113,
     146,   147,   148,   149,   150,   151,   152,    -1,    -1,   155,
     156,   157,    -1,    78,    -1,   129,   130,    82,   132,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,   143,
     144,    -1,   146,   147,   148,   149,   150,   151,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    -1,    19,
     155,   156,   157,    23,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    19,    34,    35,    -1,    23,    24,    39,
      -1,    -1,    -1,    -1,    44,    -1,    32,    -1,    34,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,
      70,    -1,    72,    -1,    -1,    75,    -1,    77,    -1,    79,
      66,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    75,
      -1,    77,     1,    79,     3,    -1,    -1,    -1,    98,    -1,
      -1,   101,    -1,    -1,   104,    -1,    15,    16,    -1,    -1,
      -1,    -1,    98,    22,    -1,   101,    -1,    -1,   104,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,     3,    61,    -1,   154,    -1,    -1,    -1,    67,    -1,
     160,    -1,    -1,    15,    16,    -1,    -1,    76,   154,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    85,    86,    30,    88,
      -1,    90,    -1,    92,    36,    -1,    95,    -1,    -1,    41,
      99,    43,    -1,   102,    46,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    88,    -1,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   169,   170,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    29,    30,    32,    34,    35,
      36,    37,    38,    39,    40,    41,    43,    44,    46,    49,
      50,    51,    53,    54,    55,    57,    58,    60,    61,    62,
      66,    67,    68,    69,    70,    71,    72,    75,    76,    77,
      79,    80,    81,    83,    84,    85,    86,    87,    88,    89,
      90,    92,    93,    95,    96,    97,    98,    99,   101,   102,
     103,   104,   105,   108,   110,   113,   129,   130,   131,   132,
     133,   138,   148,   151,   152,   154,   157,   159,   161,   163,
     171,   172,   173,   174,   175,   176,   178,   183,   184,   185,
     189,   191,   195,   196,   197,   198,   199,   200,   202,   203,
     204,   208,   209,   212,   213,   216,   217,   220,   223,   224,
     250,   253,   254,   274,   275,   276,   277,   278,   279,   280,
     288,   289,   290,   291,   294,   295,   296,   297,   298,   299,
     300,   302,   303,   304,   305,   306,   154,   173,   291,   109,
     281,   282,     3,   186,    12,    37,    38,    40,    90,   159,
     163,   220,   274,   279,   289,   290,   291,   294,   296,   297,
     281,   291,    98,   253,    79,   186,   173,   268,   291,   173,
     161,   291,   292,     7,     9,    98,   193,   292,    68,   111,
     201,   291,   201,   291,    23,    35,   254,   291,   291,   161,
       3,    15,    16,    22,    30,    36,    41,    43,    46,    54,
      61,    67,    76,    85,    86,    88,    90,    92,    95,    99,
     102,   105,   188,   190,   221,   161,   188,   255,   256,    29,
     178,   194,   291,   291,   291,   291,   291,   291,     7,   161,
      35,   197,   197,   153,   243,   267,   291,    96,    97,   154,
     291,   293,   291,   194,   291,   291,   291,    89,   161,   173,
     291,   291,   178,   185,   291,   294,   178,   185,   291,   188,
     251,   291,   267,   291,   291,   291,   291,   291,   291,   291,
     291,     1,   160,   171,   179,   267,   100,   139,   243,   269,
     270,   293,   201,   267,   291,   301,    70,   173,   159,    74,
     177,   188,   188,    47,    73,   238,   255,   153,   154,   145,
      10,    17,    28,    78,    82,   113,   129,   130,   132,   133,
     134,   136,   137,   138,   140,   141,   142,   143,   144,   146,
     147,   148,   149,   150,   151,   152,   155,   156,   157,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   158,   230,   161,   163,    78,    82,
     161,   173,   154,   291,   291,   291,   267,   159,   178,    42,
     281,   251,   255,   154,   135,   154,   108,   189,   243,   271,
     272,   273,   293,   251,   213,   253,   188,   161,   194,    29,
      42,   194,   109,   194,   284,    29,    42,   194,   284,   292,
      33,    63,   154,    91,   178,   230,   271,   173,   161,   232,
     100,   161,   188,   257,   258,     1,   134,   262,    42,   135,
     173,   194,   194,   271,   188,   135,   154,   291,   291,   154,
     159,   194,   161,   271,   154,   205,   154,   205,   154,   114,
     252,   154,   154,    33,    63,   194,   154,   160,   160,   171,
     135,   160,   291,   135,   162,   135,   162,   164,   284,    42,
     135,   164,   284,   112,   135,   164,     7,     1,   160,   179,
      56,   134,   214,   159,   225,   154,   291,   291,   291,   291,
     201,   291,   201,   291,   291,   291,   291,   291,   291,   291,
      30,    54,    98,   189,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     293,   293,   293,   293,   293,   271,   271,   201,   291,   201,
     291,    24,    42,    79,   104,   283,   286,   287,   291,   306,
      29,    42,    29,    42,    91,    42,   164,   201,   291,   194,
     154,   154,   291,   291,   114,   162,   135,   154,   188,   255,
      42,   267,    40,   291,   201,   291,   161,   194,    40,   291,
     201,   291,   194,   157,   180,   182,   291,   180,   181,   173,
     291,    31,   291,   162,    24,    42,    45,    65,    68,    79,
      98,   172,   233,   234,   235,   236,   222,   258,   135,   162,
      30,    43,    86,    90,   163,   192,   263,   275,   114,   259,
     291,   256,   162,   243,   291,     1,   210,   271,   162,    18,
     206,   263,   275,   135,   182,   181,   160,   162,   162,   269,
     162,   269,   173,   164,   201,   291,   164,   173,   291,   164,
     291,   164,   291,   160,   160,   188,   267,   159,     1,   188,
     218,   219,    24,    68,    79,    98,   227,   237,   154,   154,
     154,   154,   154,   162,   164,    42,    79,   135,   162,   274,
      78,    78,    40,   201,   291,    40,   201,   291,   201,   291,
     281,   281,   154,   243,   293,   273,   214,   154,   291,   162,
     291,    29,   194,    29,   194,   285,   286,   291,    29,   194,
     284,    29,   194,   284,   135,   154,    11,   154,    31,    31,
     173,    91,   178,    42,    79,   235,   135,   162,   161,   188,
      24,    68,    79,    98,   239,   162,   258,   262,     1,   267,
      59,   293,   160,    64,   106,   160,   211,   162,   161,   178,
     188,   207,   251,   154,   154,   173,   164,   284,   164,   284,
     173,   112,     1,   215,   160,   114,   135,   160,    79,   226,
       1,     3,    10,    15,    17,    22,    36,    41,    46,    48,
      54,    61,    76,    88,    92,    95,    99,   105,   129,   130,
     131,   132,   133,   136,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   152,   155,   156,   157,   158,
     161,   187,   188,   190,   228,   229,   230,   274,   286,   262,
     274,   274,   291,    29,    29,   291,    29,    29,   164,   164,
     194,   194,   159,   194,    91,    40,   291,    40,   291,   135,
     162,    91,    40,   291,   194,    40,   291,   194,   291,   291,
     173,   291,   173,    31,   188,   234,   258,   134,   266,    79,
     262,   259,   164,    42,   164,    29,   173,   267,   207,   134,
     178,    40,   173,   291,   164,    40,   173,   291,   164,   291,
     160,    35,    72,    75,   160,   172,   196,   291,   219,   239,
     161,   231,   188,   291,   129,   137,   231,   231,   259,    91,
      40,    40,    91,    40,    40,    40,    40,     1,   215,   291,
     291,   291,   286,   291,   291,   291,    11,    31,   173,   266,
     162,   163,   192,   243,   265,   275,   139,   245,   259,    52,
     107,   246,   291,   263,   275,   173,   194,   162,   291,   291,
     173,   291,   173,   196,     1,   134,   261,   233,   162,     3,
      92,   229,   230,   291,   291,   291,   291,   291,   291,   160,
     160,    91,    91,    91,    91,   291,   173,   259,   266,   164,
     267,   243,   244,   291,     3,    81,    92,   247,   248,   249,
     291,   178,   195,   242,   164,   178,    91,    91,   163,   192,
     260,   275,    94,   240,   162,   231,   231,    91,    91,    91,
      91,    91,    91,   291,   291,   291,   291,   245,   259,   243,
     264,   265,   275,    42,   164,   249,   107,   135,   114,   140,
     142,   143,   146,   147,    52,   275,   291,   291,     1,   164,
     267,   246,   291,   264,   265,   291,   248,   249,   249,   249,
     249,   249,   249,   247,   164,   260,   275,   164,   154,   241,
     242,   164,   260,   275,   264
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
#line 488 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 494 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 526 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 528 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 529 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 530 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 546 "chapel.ypp"
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

  case 40:
/* Line 1792 of yacc.c  */
#line 559 "chapel.ypp"
    { (yyvsp[(1) - (3)].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (3)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (3)]).prevModule;  // restore previous module name
    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 564 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = (yyvsp[(3) - (4)].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 569 "chapel.ypp"
    { (yyvsp[(1) - (4)].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[(1) - (4)].pmodsymbol)));
      currentModuleName = (yylsp[(1) - (4)]).prevModule;  // restore previous module name
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 596 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 597 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 598 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 603 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 609 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 619 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 623 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 649 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 650 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 654 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(2) - (4)].pch));   }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 661 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 663 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 665 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 667 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 673 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 678 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 679 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 680 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 681 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 682 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 686 "chapel.ypp"
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); redefiningReservedTypeError((yyvsp[(1) - (1)].pch)); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pch) = "this"; }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 713 "chapel.ypp"
    { (yyval.pch) = "bool"; }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 714 "chapel.ypp"
    { (yyval.pch) = "int"; }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 715 "chapel.ypp"
    { (yyval.pch) = "uint"; }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 716 "chapel.ypp"
    { (yyval.pch) = "real"; }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 717 "chapel.ypp"
    { (yyval.pch) = "imag"; }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 718 "chapel.ypp"
    { (yyval.pch) = "complex"; }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 719 "chapel.ypp"
    { (yyval.pch) = "string"; }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 720 "chapel.ypp"
    { (yyval.pch) = "sync"; }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 721 "chapel.ypp"
    { (yyval.pch) = "single"; }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 722 "chapel.ypp"
    { (yyval.pch) = "owned"; }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 723 "chapel.ypp"
    { (yyval.pch) = "shared"; }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 724 "chapel.ypp"
    { (yyval.pch) = "borrowed"; }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 725 "chapel.ypp"
    { (yyval.pch) = "unmanaged"; }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 726 "chapel.ypp"
    { (yyval.pch) = "domain"; }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.pch) = "index"; }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 728 "chapel.ypp"
    { (yyval.pch) = "locale"; }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.pch) = "nothing"; }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pch) = "void"; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 743 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 750 "chapel.ypp"
    { (yyval.pch) = "_syncvar"; }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pch) = "_singlevar"; }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 756 "chapel.ypp"
    { (yyval.pch) = "_domain"; }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pch) = "_index"; }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 762 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 763 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 768 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 777 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 779 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 780 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 793 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 795 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 796 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 801 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
    }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 807 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 808 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 809 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt));       }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 810 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 811 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt));     }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 812 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 813 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 814 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 815 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 816 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 817 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 818 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), false, false); }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 819 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr),   (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true,  false); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 820 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr),   (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true,  false); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 821 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 822 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), false, false); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 823 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pcallexpr), NULL, (yyvsp[(3) - (3)].pblockstmt), true,  false); }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 824 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt), true,  false); }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 826 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), false, true);
    }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 832 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), false, true);
    }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 838 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true,  true);
    }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 844 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true,  true);
    }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 850 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), false, true);
    }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 857 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(),                       (yyvsp[(3) - (5)].pcallexpr), new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), false, true);
    }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 864 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)), true,  true);
    }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 868 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)), true,  true);
    }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 880 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 882 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 884 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 886 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 888 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 896 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 897 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 898 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 899 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 900 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 901 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 905 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 910 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 911 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pdefexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 912 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pdefexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 916 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 917 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 921 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 925 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 927 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 939 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 941 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 948 "chapel.ypp"
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

  case 195:
/* Line 1792 of yacc.c  */
#line 958 "chapel.ypp"
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

  case 196:
/* Line 1792 of yacc.c  */
#line 968 "chapel.ypp"
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

  case 197:
/* Line 1792 of yacc.c  */
#line 978 "chapel.ypp"
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

  case 198:
/* Line 1792 of yacc.c  */
#line 990 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 995 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1000 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1008 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1023 "chapel.ypp"
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

  case 207:
/* Line 1792 of yacc.c  */
#line 1039 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1046 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1055 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1063 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1067 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1073 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1074 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1079 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1084 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1089 "chapel.ypp"
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

  case 217:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1117 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1124 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1130 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[(2) - (2)].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1136 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1147 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1153 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1160 "chapel.ypp"
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

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[(4) - (10)].pfnsymbol), (yyvsp[(6) - (10)].retTag), (yyvsp[(7) - (10)].pexpr), (yyvsp[(8) - (10)].b), (yyvsp[(9) - (10)].lifetimeAndWhere).where, (yyvsp[(9) - (10)].lifetimeAndWhere).lifetime, (yyvsp[(10) - (10)].pblockstmt), (yylsp[(1) - (10)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1186 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1190 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1195 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1199 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1204 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1219 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1221 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1226 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1227 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1228 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1231 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1232 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1235 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1236 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1237 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1241 "chapel.ypp"
    { (yyval.pch) = "init="; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1242 "chapel.ypp"
    { (yyval.pch) = astr((yyvsp[(1) - (2)].pch), "!"); }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1247 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1248 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1249 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1250 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1251 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1252 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1253 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1254 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 1255 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 1256 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1257 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1261 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1262 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1266 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 1270 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1272 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[(1) - (5)].vpch), buildArgDefExpr((yyvsp[(2) - (5)].pt), (yyvsp[(3) - (5)].pch), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr), NULL)); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1285 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 1308 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1320 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1321 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1322 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1329 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1330 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1333 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1350 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (2)].pexpr), NULL); }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1376 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[(1) - (1)].pch))); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1386 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1390 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1392 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1394 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1399 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1412 "chapel.ypp"
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

  case 341:
/* Line 1792 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1432 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1436 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1437 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyval.pflagset) = buildVarDeclFlags(); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1445 "chapel.ypp"
    {
      (yyvsp[(2) - (4)].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), context->latestComment, (yyvsp[(2) - (4)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1451 "chapel.ypp"
    {
      (yyvsp[(3) - (5)].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), context->latestComment, (yyvsp[(3) - (5)].pflagset), (yyvsp[(2) - (5)].pexpr));
      context->latestComment = NULL;
    }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(2) - (3)].pblockstmt), context->latestComment, (yyvsp[(1) - (3)].pflagset));
      context->latestComment = NULL;
    }
    break;

  case 353:
/* Line 1792 of yacc.c  */
#line 1466 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1492 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 361:
/* Line 1792 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 362:
/* Line 1792 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 363:
/* Line 1792 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 364:
/* Line 1792 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 365:
/* Line 1792 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (3)].pcallexpr)), NULL);
    }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1526 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1575 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1583 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1612 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 395:
/* Line 1792 of yacc.c  */
#line 1613 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 396:
/* Line 1792 of yacc.c  */
#line 1614 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 397:
/* Line 1792 of yacc.c  */
#line 1615 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(2) - (2)].pch)); }
    break;

  case 398:
/* Line 1792 of yacc.c  */
#line 1616 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 399:
/* Line 1792 of yacc.c  */
#line 1622 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 400:
/* Line 1792 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 401:
/* Line 1792 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1625 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 404:
/* Line 1792 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1635 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1636 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1641 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 414:
/* Line 1792 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1658 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 418:
/* Line 1792 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 419:
/* Line 1792 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 420:
/* Line 1792 of yacc.c  */
#line 1675 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 421:
/* Line 1792 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS, (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1687 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1691 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1697 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1702 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
    break;

  case 435:
/* Line 1792 of yacc.c  */
#line 1704 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 436:
/* Line 1792 of yacc.c  */
#line 1706 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_unmanaged"); }
    break;

  case 437:
/* Line 1792 of yacc.c  */
#line 1708 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 438:
/* Line 1792 of yacc.c  */
#line 1710 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
    break;

  case 439:
/* Line 1792 of yacc.c  */
#line 1712 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 440:
/* Line 1792 of yacc.c  */
#line 1714 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_borrowed"); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1716 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1721 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1725 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1727 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1731 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1733 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1735 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1737 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1743 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1745 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1752 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1758 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1764 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1770 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pcallexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1779 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1804 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1813 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1818 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1822 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1826 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1827 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1832 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[(1) - (4)].pShadowVarPref), (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1836 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1840 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1846 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST;     }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1847 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_IN;        }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1848 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1849 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_REF;       }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1850 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1851 "chapel.ypp"
    { (yyval.pShadowVarPref) = SVP_VAR;       }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1857 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1862 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1867 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1883 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1885 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1887 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1889 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1891 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1893 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1897 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1898 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1901 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1902 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 508:
/* Line 1792 of yacc.c  */
#line 1903 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 513:
/* Line 1792 of yacc.c  */
#line 1920 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 514:
/* Line 1792 of yacc.c  */
#line 1921 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 515:
/* Line 1792 of yacc.c  */
#line 1922 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 516:
/* Line 1792 of yacc.c  */
#line 1926 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 517:
/* Line 1792 of yacc.c  */
#line 1927 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 518:
/* Line 1792 of yacc.c  */
#line 1928 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 519:
/* Line 1792 of yacc.c  */
#line 1932 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 520:
/* Line 1792 of yacc.c  */
#line 1933 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 521:
/* Line 1792 of yacc.c  */
#line 1934 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 522:
/* Line 1792 of yacc.c  */
#line 1935 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "locale"); }
    break;

  case 523:
/* Line 1792 of yacc.c  */
#line 1943 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 524:
/* Line 1792 of yacc.c  */
#line 1944 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 525:
/* Line 1792 of yacc.c  */
#line 1945 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 526:
/* Line 1792 of yacc.c  */
#line 1946 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 527:
/* Line 1792 of yacc.c  */
#line 1950 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gFalse); }
    break;

  case 528:
/* Line 1792 of yacc.c  */
#line 1951 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gTrue); }
    break;

  case 530:
/* Line 1792 of yacc.c  */
#line 1956 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch), yyfilename, chplLineno);    }
    break;

  case 531:
/* Line 1792 of yacc.c  */
#line 1957 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 532:
/* Line 1792 of yacc.c  */
#line 1958 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 533:
/* Line 1792 of yacc.c  */
#line 1959 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 534:
/* Line 1792 of yacc.c  */
#line 1960 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 535:
/* Line 1792 of yacc.c  */
#line 1961 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNone); }
    break;

  case 536:
/* Line 1792 of yacc.c  */
#line 1962 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 537:
/* Line 1792 of yacc.c  */
#line 1963 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 538:
/* Line 1792 of yacc.c  */
#line 1964 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 539:
/* Line 1792 of yacc.c  */
#line 1965 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 540:
/* Line 1792 of yacc.c  */
#line 1967 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 541:
/* Line 1792 of yacc.c  */
#line 1971 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 542:
/* Line 1792 of yacc.c  */
#line 1978 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 543:
/* Line 1792 of yacc.c  */
#line 1979 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 544:
/* Line 1792 of yacc.c  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 545:
/* Line 1792 of yacc.c  */
#line 1984 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 546:
/* Line 1792 of yacc.c  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 547:
/* Line 1792 of yacc.c  */
#line 1986 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 548:
/* Line 1792 of yacc.c  */
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 549:
/* Line 1792 of yacc.c  */
#line 1988 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 550:
/* Line 1792 of yacc.c  */
#line 1989 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 551:
/* Line 1792 of yacc.c  */
#line 1990 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 552:
/* Line 1792 of yacc.c  */
#line 1991 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 553:
/* Line 1792 of yacc.c  */
#line 1992 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 554:
/* Line 1792 of yacc.c  */
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 555:
/* Line 1792 of yacc.c  */
#line 1994 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 556:
/* Line 1792 of yacc.c  */
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 557:
/* Line 1792 of yacc.c  */
#line 1996 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 558:
/* Line 1792 of yacc.c  */
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 559:
/* Line 1792 of yacc.c  */
#line 1998 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 560:
/* Line 1792 of yacc.c  */
#line 1999 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 561:
/* Line 1792 of yacc.c  */
#line 2000 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 562:
/* Line 1792 of yacc.c  */
#line 2001 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 563:
/* Line 1792 of yacc.c  */
#line 2002 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 564:
/* Line 1792 of yacc.c  */
#line 2003 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 565:
/* Line 1792 of yacc.c  */
#line 2004 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 566:
/* Line 1792 of yacc.c  */
#line 2005 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 567:
/* Line 1792 of yacc.c  */
#line 2009 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 568:
/* Line 1792 of yacc.c  */
#line 2010 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 569:
/* Line 1792 of yacc.c  */
#line 2011 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 570:
/* Line 1792 of yacc.c  */
#line 2012 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 571:
/* Line 1792 of yacc.c  */
#line 2013 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 572:
/* Line 1792 of yacc.c  */
#line 2014 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 573:
/* Line 1792 of yacc.c  */
#line 2015 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 574:
/* Line 1792 of yacc.c  */
#line 2019 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 575:
/* Line 1792 of yacc.c  */
#line 2020 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 576:
/* Line 1792 of yacc.c  */
#line 2021 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 577:
/* Line 1792 of yacc.c  */
#line 2022 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 578:
/* Line 1792 of yacc.c  */
#line 2026 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 579:
/* Line 1792 of yacc.c  */
#line 2027 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 580:
/* Line 1792 of yacc.c  */
#line 2028 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 581:
/* Line 1792 of yacc.c  */
#line 2029 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pcallexpr), true); }
    break;

  case 582:
/* Line 1792 of yacc.c  */
#line 2034 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 583:
/* Line 1792 of yacc.c  */
#line 2035 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 584:
/* Line 1792 of yacc.c  */
#line 2036 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 585:
/* Line 1792 of yacc.c  */
#line 2037 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 586:
/* Line 1792 of yacc.c  */
#line 2038 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 587:
/* Line 1792 of yacc.c  */
#line 2039 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 588:
/* Line 1792 of yacc.c  */
#line 2040 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 9572 "bison-chapel.cpp"
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


